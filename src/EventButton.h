/**
 *
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


class EventButton : public EventInputBase {

    protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        typedef std::function<void(InputEventType et, EventButton &ie)> CallbackFunction;
    #else
        typedef void (*CallbackFunction)(InputEventType et, EventButton &);
    #endif

    CallbackFunction callbackFunction = nullptr;


    public:

    /**
     * Construct a button
     */
    EventButton(byte buttonPin);

    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventButton&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventButton& ie) {
            (instance->*method)(et, ie); // Call the member function on the instance
        };
        callbackIsSet = true;
    }
    #endif

    void unsetCallback() override;

    void update();

    /**
     * Default is set in the Bounce2 library (currently 10ms)
     */
    void setDebounceInterval(unsigned int intervalMs=10);

    /**
     * Set the interval in ms between double, triple or
     * multi clicks
     */
    void setMultiClickInterval(unsigned int intervalMs=250) { multiClickInterval = intervalMs; }

    /**
     * Set the ms that defines a lonf click. Long pressed callback
     * will be fired at this interval if repeat is set to true via the
     * setLongPressHandler()
     */
    void setLongClickDuration(unsigned int longDurationMs=750) { longClickDuration = longDurationMs; }

    /**
     * Choose whether to repeat the long press callback
     * (default is 'false')
     */
    void enableLongPressRepeat(bool repeat=true) { repeatLongPress = repeat; }

    /**
     * Set the interval in ms between long press repeats
     * if enabled
     */
    void setLongPressInterval(unsigned int intervalMs=500) { longPressInterval = intervalMs; }

    /**
     * Directly get the current button state from Bounce2
     */
    bool buttonState();

    /**
     * Directly get the duration of the button current state from Bounce2
     */
    unsigned long currentDuration();

    /**
     * Directly get the duration of the button previous state from Bounce2
     */
    unsigned long previousDuration();

    /**
     * The number of multi-clicks that have been fired in the clicked event
     */
    unsigned char clickCount() { return prevClickCount; }


    /**
     * The number of times the long press handler has  been fired in the 
     * button pressed event
     */
    uint8_t longPressCount() { return longPressCounter; }

    /**
     * Returns true if pressed
     */
    bool isPressed();

    protected:

    void invoke(InputEventType et) override;

    void onDisabled() override;

    private:

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
    uint32_t idleTimeout = 10000;



};


#endif










