/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "InputEvents.h"


#ifndef EXCLUDE_EVENT_ENCODER

#ifndef EVENT_ENCODER_BUTTON_H
#define EVENT_ENCODER_BUTTON_H


#include "Arduino.h"
#include "EventEncoder.h"
#include "EventButton.h"


class EventEncoderButton : public EventInputBase {

    protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        typedef std::function<void(InputEventType et, EventEncoderButton &ie)> CallbackFunction;
    #else
        typedef void (*CallbackFunction)(InputEventType et, EventEncoderButton &);
    #endif

    CallbackFunction callbackFunction = nullptr;

public:

    /**
     * Construct a rotary encoder button
     */
    EventEncoderButton(byte encoderPin1, byte encoderPin2, byte buttonPin);


    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventEncoderButton&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventEncoderButton &ie) {
            (instance->*method)(et, ie); // Call the member function on the instance
        };
        callbackIsSet = true;
    }
    #endif

    void unsetCallback() override;

    void update();


    //************ Encoder Methods ************************ */
    void setRateLimit(long ms);

    /**
     * Quadrature encoders usually have four states for each 'click' of the 
     * rotary control, although some have just 2. 
     * Normally we only want to fire an event once per click so the default
     * positionDivider is 4.
     * You can set this to any positive integer eg 8 would increment the
     * position every 2 clicks. 
     * Affects pressed+turning too.
     */
    void setPositionDivider(uint8_t divider=4);

    uint8_t getPositionDivider();


    /**
     * Reset the counted position of the encoder. 
     */
    void resetPosition(long pos = 0);

    /**
     * Returns a positive (CW) or negative (CCW) integer. Is normally 1 or -1 but if your 
     * loop() has lots of processing, your Arduino is slow or you setRateLimit()
     * this will report the actual number of increments made by the encoder since
     * the last encoder handler event.
     */
    int16_t increment();

    /**
     * The current position of the encoder. Can be reset by resetPosition()
     */
    long position();

    void resetPressedPosition(long pos);

    long pressedPosition();

    //************ Button Methods ************************ */
    /**
     * Default is set in the Bounce2 library (currently 10ms)
     */
    void setDebounceInterval(unsigned int intervalMs=10);

    /**
     * Set the interval in ms between double, triple or
     * multi clicks
     */
    void setMultiClickInterval(unsigned int intervalMs=250);

    /**
     * Set the ms that defines a lonf click. Long pressed callback
     * will be fired at this interval if repeat is set to true via the
     * setLongPressHandler()
     */
    void setLongClickDuration(unsigned int longDurationMs=750);

    /**
     * Choose whether to repeat the long press callback
     * (default is 'false')
     */
    void enableLongPressRepeat(bool repeat=true);

    /**
     * Set the interval in ms between long press repeats
     * if enabled
     */
    void setLongPressInterval(unsigned int intervalMs=500);

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
    unsigned char clickCount();


    /**
     * The number of times the long press handler has  been fired in the 
     * button pressed event
     */
    uint8_t longPressCount();

    /**
     * Returns true if pressed
     */
    bool isPressed();


protected:
    void invoke(InputEventType et) override;
    void onEnabled() override;
    void onDisabled() override;
    void onIdle() override {/* Do nothing. Fire idle callback from either encoder or button but only if both are idle.*/ }

    EventEncoder encoder;
    EventButton button;
    protected:
    /**
     * Convert the encoder & button callbacks to a EventEncoderButton callbacks
     */
    void onInputCallback(InputEventType et, EventInputBase & ie);

private:
    int32_t currentPressedPosition  = 0;
    bool encodingPressed = false;

#ifndef FUNCTIONAL_SUPPORTED
private:
    static void encoderCallback(InputEventType et, EventEncoder &enc) {
        EventEncoderButton *instance = static_cast<EventEncoderButton *>(enc.getOwner());
        if (instance) instance->onInputCallback(et, enc);
    }

    static void buttonCallback(InputEventType et, EventButton &btn) {
        EventEncoderButton *instance = static_cast<EventEncoderButton *>(btn.getOwner());
        if (instance) instance->onInputCallback(et, btn);
    }
#endif

};


#endif
#endif

