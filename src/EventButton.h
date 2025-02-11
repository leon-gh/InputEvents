/*
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef EVENT_BUTTON_H
#define EVENT_BUTTON_H

#include "Arduino.h"
#include "EventInputBase.h"

#ifndef Bounce2_h
    #include <Bounce2.h>
#endif

/**
 * @brief The EventButton class is for momentary inputs. The momentary switch (button) must be wired between the pin and GND.

   @details EventButton is probably the easiest way to implement a button - a single function and four lines of code gives you information on all types of user inputs.


The following InputEventTypes are fired by EventButton:
  - InputEventType::ENABLED - fired when the input is enabled.
  - InputEventType::DISABLED - fired when the input is disabled.
  - InputEventType::IDLE - fired after no other event (except <code>ENABLED</code> & <code>DISABLED</code>) has been fired for a specified time. Each input can define its own idle timeout. Default is 10 seconds.
  - InputEventType::PRESSED - fired after (as) a button is pressed
  - InputEventType::RELEASED - fired after a button is released.
  - InputEventType::CLICKED - fired after <code>RELEASED</code> if not <code>LONG_CLICKED</code> and button is pressed and released once.
  - InputEventType::DOUBLE_CLICKED - fired after <code>RELEASED</code> if not <code>LONG_CLICKED</code> and button is pressed and released twice.
  - InputEventType::MULTI_CLICKED - fired after <code>RELEASED</code> if not <code>LONG_CLICKED</code> and button is pressed and released more than twice (no limit!). The method clickCount() returns the number of clicks.
  - InputEventType::LONG_PRESS - fired *during* a long press (hence change of tense). Will repeat by default but this can be turned off.
  - InputEventType::LONG_CLICKED - fired *after* a long press.
 * 
 */
class EventButton : public EventInputBase {

    protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        /**
         * @brief If <code>std::function</code> is supported, this creates the callback type.
         */
        typedef std::function<void(InputEventType et, EventButton &ie)> CallbackFunction;
    #else
        /**
         * @brief Used to create the callback type as pointer if <code>std::function</code> is not supported.
         */
        typedef void (*CallbackFunction)(InputEventType et, EventButton &);
    #endif

    /**
     * @brief The callback function member.
     */
    CallbackFunction callbackFunction = nullptr;


    public:

    ///@{ 
    /** 
     * @name Constructor 
     */
    /**
     * @brief Construct an EventButton
     * 
     * @param buttonPin Any type of pin. Button contact must pull down to to GND when pressed.
     */
    EventButton(byte buttonPin);
    ///@}


    ///@{ 
    /** 
     * @name Common Methods
     * @details These methods are common to all InputEvent classes.
     * 
     * Additional methods for input enable, timeout, event blocking and user ID/value are also inherited from the EventInputBase class.
     */

    /**
     * @brief Initialise the EventButton
     * 
     * @details *Must* be called from within <code>setup()</code>
     */
    void begin();

    /**
     * @brief Set the Callback function.
     * 
     * @param f A function of type <code>EventButton::CallbackFunction</code> type.
     */
    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    /**
     * @brief Set the Callback function to a class method.
     * 
     * @details Note: This method is only available if <code>std:function</code> is supported.
     * 
     * 
     * @param instance The instance of a class implementing a CallbackFunction method.
     * @param method The class method of type <code>EventButton::CallbackFunction</code> type.
     */
    #if defined(FUNCTIONAL_SUPPORTED)
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventButton&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventButton& ie) {
            (instance->*method)(et, ie); // Call the member function on the instance
        };
        callbackIsSet = true;
    }
    #endif

    /**
     * @brief Unset a previously set callback function or method.
     * 
     * @details Must be called before the set function or method is destoyed.
     */
    void unsetCallback() override;

    /**
     * @brief Update the state from the pin input.
     * 
     * @details *Must* be called from within <code>loop()</code>
     */
    void update();
    ///@}

    ///@{
    /**
     * @name Getting the State
     * @details These methods return the current state of the input (as set during the last update() )
     */
    /**
     * @brief The number of clicks that have been fired in the MULTI_CLICKED event. 
     * @details This is also set do CLICK and DOUBLE_CLICKED and is reset to zero after any CLICKED event is fired.
     * 
     * @return uint8_t Number of clicks
     */
    uint8_t clickCount() { return prevClickCount; }


    /**
     * @brief The number of times the long press has occurred during a button press. 
     * @details This is oncremented even if the setLongPressRepeat is false, so can be read in the LONC_CLICKED event. This is reset to zero after `LONG_CLICKED` is fired.

     * @return uint8_t A count of long press occurences
     */
    uint8_t longPressCount() { return longPressCounter; }

    /**
     * @brief Returns true if pressed
     */
    bool isPressed();

    /**
     * @brief Directly get the duration of the button current state from Bounce2
     * 
     * @return Current state duration in milliseconds
     */
    uint32_t currentDuration();

    /**
     * @brief Directly get the duration of the button previous state from Bounce2
     * 
     * @return Previous state duration in milliseconds
     */
    uint32_t previousDuration();

    /**
     * @brief Get the button state as reported by Bounce2
     * 
     * @return HIGH or LOW
     */
    bool buttonState();

    ///@}

    ///@{
    /**
     * @name Other Configuration Settings
     */
    /**
     * @brief Choose whether to repeat the long press callback. (true by default)
     * 
     * @param repeat Pass true to repeat, false to not repeat.
     */
    void enableLongPressRepeat(bool repeat=true) { repeatLongPress = repeat; }

    /**
     * @brief Set the number of milliseconds that define the *first* long click duration.
     * @details The LONG_PRESS event will fire after this duration, then repeat at the interval setLongPressInterval(ms) (default 500ms) if enableLongPressRepeat() is true. After the button is released, a LONG_CLICK event will fire.
     * 
     * @param longDurationMs Default 750ms
     */
    void setLongClickDuration(unsigned int longDurationMs=750) { longClickDuration = longDurationMs; }

    /**
     * @brief Set the number of milliseconds that define the *subbsequent* long click intervals.
     * @details If enableLongPressRepeat() is true a LONG_PRESS event will fire every intervalMs after the first LONG_PRESS has fired.
    * 
     * @param intervalMs The interval in milliseconds (default is 500ms).
     */
    void setLongPressInterval(unsigned int intervalMs=500) { longPressInterval = intervalMs; }

    /**
     * @brief Set the multi click interval.
     * 
     * @param intervalMs The interval in milliseconds between double, triple or multi clicks
     */
    void setMultiClickInterval(unsigned int intervalMs=250) { multiClickInterval = intervalMs; }

    /**
     * @brief Set the Bounce2 debounce interval.
     * 
     * @details Default in the Bounce2 library is 10ms
     */
    void setDebounceInterval(unsigned int intervalMs=10);
    ///@}


    protected:

    /**
     * @brief Override of the <code>EventInputBase::invoke()</code> virtual method.
     * 
     * @param et Enum of type <code>InputEventType</code>
     */
    void invoke(InputEventType et) override;


    /**
     * @brief Override base method to reset click and pressed counts.
     * 
     */
    void onDisabled() override;

    private:

    byte buttonPin;
    Bounce* bounce;

    //state
    uint8_t _buttonState = HIGH;
    bool previousState = LOW;
    uint8_t clickCounter = 0;
    uint8_t prevClickCount = 0;
    bool clickFired = true;

    //setup
    uint16_t multiClickInterval = 250;
    uint16_t longClickDuration = 750;
    bool repeatLongPress = true;
    uint16_t longPressInterval = 500;
    uint16_t longPressCounter = 0;



};


#endif










