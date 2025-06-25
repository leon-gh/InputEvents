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
#include "PinAdapter/FoltmanDebounceAdapter.h"
#include "PinAdapter/GpioPinAdapter.h"

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
     * @name Constructors
     */
    /**
     * @brief Construct an EventButton with a GPIO pin
     * 
     * @param buttonPin Any type of pin and optionally use the default debouncer (default true). By default button contact should pull down to to GND when pressed. This behaviour can be reversed with setPressedState()
     */
    EventButton(byte buttonPin, bool useDefaultDebouncer=true);

    /**
     * @brief Construct a new EventButton with a PinAdapter and optionally use the default debouncer
     * 
     * @param pinAdapter 
     */
    EventButton(PinAdapter* _pinAdapter, bool useDefaultDebouncer=true);

    /**
     * @brief Construct a new EventButton with a PinAdapter and a DebounceAdapter
     * 
     * @param pinAdapter 
     * @param debounceAdapter 
     */
    EventButton(PinAdapter* _pinAdapter, DebounceAdapter* debounceAdapter);

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
     * @details This is incremented even if the setLongPressRepeat is false, so can be read in the LONC_CLICKED event. This is reset to zero after `LONG_CLICKED` is fired.

     * @return uint8_t A count of long press occurences
     */
    uint8_t longPressCount() { return longPressCounter; }

    /**
     * @brief Returns true if button is currently pressed
     */
    bool isPressed() { return currentState == pressedState; }

    /**
     * @brief Duration in milliseconds of the current state
     * 
     * @return Current state duration in milliseconds
     */
    uint32_t currentDuration();

    /**
     * @brief Duration in milliseconds of the previous state
     * 
     * @return Previous state duration in milliseconds
     */
    uint32_t previousDuration()  { return durationOfPreviousState; }

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
    void setLongClickDuration(uint16_t longDurationMs=750) { longClickDuration = longDurationMs; }

    /**
     * @brief Set the number of milliseconds that define the *subbsequent* long click intervals.
     * @details If enableLongPressRepeat() is true a LONG_PRESS event will fire every intervalMs after the first LONG_PRESS has fired.
    * 
     * @param intervalMs The interval in milliseconds (default is 500ms).
     */
    void setLongPressInterval(uint16_t intervalMs=500) { longPressInterval = intervalMs; }

    /**
     * @brief Set the multi click interval.
     * 
     * @param intervalMs The interval in milliseconds between double, triple or multi clicks
     */
    void setMultiClickInterval(uint16_t intervalMs=250) { multiClickInterval = intervalMs; }

    /**
     * @brief Set the debouncer.
     * **Note:** When planning to use `setDebouncer()` you must ensure `useDefaultDebouncer` is set to `false` in the button or switch constructor. *Previously set debouncers are not deleted*.
     * 
     * @param debounceAdapter 
     */
    void setDebouncer(DebounceAdapter* debounceAdapter);

    /**
     * @brief Set the DebounceAdapter debounce interval. Default is 10ms
     * 
     * @return true If the debounce interval has been updated
     * @return false If the debouncer interval has not been updated (ie no debouncer set)
     */
    bool setDebounceInterval(uint16_t intervalMs=10);

    /**
     * @brief Set the pin state that represents 'pressed'. By default this is `LOW` (ie pulled down for pressed).
     * 
     * @details If you set this value to `HIGH`, you must also pass `INPUT_PULLDOWN` to the GpioPinAdapter constructor. 
     * 
     * If your board does not support `INPUT_PULLDOWN`, pass `INPUT` and use an external resistor.
     * 
     * @param state Either LOW (default) or HIGH
     */
    void setPressedState(bool state = LOW){ pressedState = state; }

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

    /**
     * @brief Change the button state and flag as changed
     * 
     * @param newState 
     */
    void changeState(bool newState);

    /**
     * @brief Returns true if either pinAdapter, press() or release() changed the button state
     * 
     * @return true 
     * @return false 
     */
    bool changedState();

    /**
     * @brief Returns true if pinAdapter read() has changed since last call
     * 
     * @return true 
     * @return false 
     */
    bool changedPinState();

    /**
     * @brief Returns true if state has changed and previous state is pressedState
     * 
     * @return true 
     * @return false 
     */
    bool releasing() { return stateChanged && previousState == pressedState; }
    
    /**
     * @brief Returns true if state has changed and previous state is not pressedState
     * 
     * @return true 
     * @return false 
     */
    bool pressing() { return stateChanged && previousState != pressedState; }

    private:

    PinAdapter* pinAdapter;
    DebounceAdapter* debouncer;

    bool pressedState = LOW; //The state that represents 'pressed'

    //state

    bool currentPinState = HIGH;
    bool previousPinState = HIGH;

    bool currentState = HIGH; //set via PinAdapter->read()
    bool previousState = HIGH;
    bool stateChanged = false;
    uint32_t stateChangeLastTime;
    uint32_t durationOfPreviousState;

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










