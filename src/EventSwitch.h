/**
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

class EventSwitch : public EventInputBase {

protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        typedef std::function<void(InputEventType et, EventSwitch &ie)> CallbackFunction;
    #else
        typedef void (*CallbackFunction)(InputEventType et, EventSwitch &);
    #endif

    CallbackFunction callbackFunction = nullptr;

    void invoke(InputEventType et) override;

public:

    /**
     * Construct a button
     */
    EventSwitch(byte switchPin);

    void begin();

    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

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

    void unsetCallback() override;

    void update();

    void reverseOnOff(bool rev=true) { reversed = rev; }

    bool isOnOffReversed() { return reversed; }


    /**
     * Default is set in the Bounce2 library (currently 10ms)
     */
    void setDebounceInterval(unsigned int intervalMs);

    /**
     * Directly get the duration of the switch current state from Bounce2
     */
    unsigned long currentDuration();

    /**
     * Directly get the duration of the switch previous state from Bounce2
     */
    unsigned long previousDuration();

    bool isOn() { return ((currentState == LOW) ^ reversed); }
    bool isOff() { return ((currentState == HIGH) ^ reversed); }

private:

    byte switchPin;
    Bounce* bounce;
    unsigned char currentState = HIGH;
    bool previousState = LOW;
    bool reversed = false;
};

#endif