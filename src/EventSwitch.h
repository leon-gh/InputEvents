/*
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef EVENT_SWITCH_H
#define EVENT_SWITCH_H

#include "Arduino.h"
#include "EventInputBase.h"
#include "PinAdapter/FoltmanDebounceAdapter.h"
#include "PinAdapter/GpioPinAdapter.h"

/**
 * @brief The EventSwitch class is for standard on/off inputs. Like the EventButton the switch must be wired between the pin and GND. 
 * 
 * @details  When the switch is closed (LOW) its state will be ON and when open (HIGH) it will be OFF although this behaviour can be reversed with the reverseOnOff() method.
 *
 * A quick explainer: The switch pin is configured with the microcontroller's internal [INPUT_PULLUP resistor](https://en.wikipedia.org/wiki/Pull-up_resistor). In this configuration, without anything attached, the pin will read as HIGH because it has been 'pulled up' to the voltage of the board (either 3.3V or 5V). When you attach a switch (or button) that is wired from the pin to GND, if the switch is closed (or the button pressed), the weak pullup resistor is overcome by the closed switch 'pulling' the pin to GND (0V) or LOW.
 * 
 * 
The following InputEventTypes are fired by EventSwitch:
  - InputEventType::ENABLED - fired when the input is enabled.
  - InputEventType::DISABLED - fired when the input is disabled.
  - InputEventType::IDLE - fired after no other event (except <code>ENABLED</code> & <code>DISABLED</code>) has been fired for a specified time. Each input can define its own idle timeout. Default is 10 seconds.
  - InputEventType::ON - fired after switch is turned on.
  - InputEventType::OFF - fired after switch is turned off.
 * 
 */
class EventSwitch : public EventInputBase {

protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        /**
         * @brief If <code>std::function</code> is supported, this creates the callback type.
         */
        typedef std::function<void(InputEventType et, EventSwitch &ie)> CallbackFunction;
    #else
        /**
         * @brief Used to create the callback type as pointer if <code>std::function</code> is not supported.
         */
        typedef void (*CallbackFunction)(InputEventType et, EventSwitch &);
    #endif

    /**
     * @brief The callback function member.
     */
    CallbackFunction callbackFunction = nullptr;

    /**
     * @brief Override of the <code>EventInputBase::invoke()</code> virtual method.
     * 
     * @param et Enum of type <code>InputEventType</code>
     */
    void invoke(InputEventType et) override;

public:

    ///@{ 
    /** 
     * @name Constructors 
     */
    /**
     * @brief Construct an EventSwitch input
     * 
     * @param switchPin A pin that connects to GNG via the switch
     */
    EventSwitch(byte switchPin, bool useDefaultDebouncer=true);

    /**
     * @brief Construct a new EventSwitch with a PinAdapter and optionally use the default debouncer
     * 
     * @param pinAdapter 
     */
    EventSwitch(PinAdapter* _pinAdapter, bool useDefaultDebouncer=true);

    /**
     * @brief Construct a new EventSwitch with a PinAdapter and a DebounceAdapter
     * 
     * @param pinAdapter 
     * @param debounceAdapter 
     */
    EventSwitch(PinAdapter* _pinAdapter, DebounceAdapter* debounceAdapter);


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
     * @param f A function of type <code>EventSwitch::CallbackFunction</code> type.
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
     * @param method The class method of type <code>EventSwitch::CallbackFunction</code> type.
     */
    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventSwitch&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventSwitch &ie) {
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
     * @brief Update the state from the switch pin input.
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
     * @brief Return true if switch is on.
     * 
     * @return true  Switch is deemed ON.
     * @return false  Switch is deemed OFF.
     */
    bool isOn() { return currentState == onState; }

    /**
     * @brief Return true if switch is off.
     * 
     * @return true Switch is deemed OFF.
     * @return false  Switch is deemed ON.
     */
    bool isOff() { return currentState != onState; }

    /**
     * @brief Duration in milliseconds of the current state
     */
    unsigned long currentDuration();

    /**
     * @brief Duration in milliseconds of the previous state
     */
    unsigned long previousDuration() { return durationOfPreviousState; }
    ///@}


    ///@{
    /**
     * @name Other Configuration Settings
     */
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
    bool setDebounceInterval(unsigned int intervalMs=10);

    /**
     * @brief Set the pin state that represents 'on'. By default this is `LOW` (ie pulled down).
     * 
     * @details If you set this value to `HIGH`, you must also pass `INPUT_PULLDOWN` to the GpioPinAdapter constructor. 
     * 
     * If your board does not support `INPUT_PULLDOWN`, pass `INPUT` and use an external resistor.
     * 
     * @param state Either LOW (default) or HIGH
     */
    void setOnState(bool state = LOW) { onState = state; }
    ///@}

    protected:
    /**
     * @brief Returns true if pinAdapter changed the switch state
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
     * @brief Change the switch state and flag as changed
     * 
     * @param newState 
     */
    void changeState(bool newState);

    /**
     * @brief Returns true if state has changed and previous state is onState
     * 
     * @return true 
     * @return false 
     */
    bool turningOff() { return stateChanged && previousState == onState; }
    
    /**
     * @brief Returns true if state has changed and previous state is not onState
     * 
     * @return true 
     * @return false 
     */
    bool turningOn() { return stateChanged && previousState != onState; }


private:

    PinAdapter* pinAdapter;
    DebounceAdapter* debouncer;

    bool onState = LOW; //The state that represents 'pressed'

    unsigned char currentState = HIGH;
    bool previousState = HIGH;

    bool currentPinState = HIGH;
    bool previousPinState = HIGH;

    bool stateChanged = false;
    uint32_t stateChangeLastTime;
    uint32_t durationOfPreviousState;
    


};

#endif