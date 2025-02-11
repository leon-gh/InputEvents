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

#ifndef Bounce2_h
    #include <Bounce2.h>
#endif

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
     * @name Constructor 
     */
    /**
     * @brief Construct an EventSwitch input
     * 
     * @param switchPin A pin that connects to GNG via the switch
     */
    EventSwitch(byte switchPin);
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
     * @brief Return true if switch is on. Takes reverseOnOff() into account.
     * 
     * @return true  Switch is deemed ON.
     * @return false  Switch is deemed OFF.
     */
    bool isOn() { return ((currentState == LOW) ^ reversed); }

    /**
     * @brief Return true if switch is off. Takes reverseOnOff() into account.
     * 
     * @return true Switch is deemed OFF.
     * @return false  Switch is deemed ON.
     */
    bool isOff() { return ((currentState == HIGH) ^ reversed); }

    /**
     * @brief Directly get the duration of the switch current state from Bounce2
     */
    unsigned long currentDuration();

    /**
     * @brief Directly get the duration of the switch previous state from Bounce2
     */
    unsigned long previousDuration();
    ///@}


    ///@{
    /**
     * @name Other Configuration Settings
     */

    /**
     * @brief Reverse the On/OFF behaviour
     * 
     * @param rev pass true to reverse ON/OFF and false to return to default behaviour.
     */
    void reverseOnOff(bool rev=true) { reversed = rev; }

    /**
     * @brief Returns true if reverseOnOff() has bee set to true.
     * 
     */
    bool isOnOffReversed() { return reversed; }


    /**
     * @brief Set the Bounce2 debounce interval.
     * 
     * @details Default in the Bounce2 library is 10ms
     */
    void setDebounceInterval(unsigned int intervalMs);
    ///@}

private:

    byte switchPin;
    Bounce* bounce;
    unsigned char currentState = HIGH;
    bool previousState = LOW;
    bool reversed = false;
};

#endif