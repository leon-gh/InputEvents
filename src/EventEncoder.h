/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "InputEvents.h"

#ifndef EXCLUDE_EVENT_ENCODER

#ifndef EVENT_ENCODER_H
#define EVENT_ENCODER_H

#include "Arduino.h"
#include "EventInputBase.h"
#include <EncoderAdapter.h>

class EventEncoder : public EventInputBase {

    protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        typedef std::function<void(InputEventType et, EventEncoder &ie)> CallbackFunction;
    #else
        typedef void (*CallbackFunction)(InputEventType et, EventEncoder &);
    #endif

    CallbackFunction callbackFunction = NULL;

    void readIncrement();

public:

    /**
     * Construct a rotary encoder
     */
    //EventEncoder(uint8_t encoderPin1, uint8_t encoderPin2);
    EventEncoder(EncoderAdapter *encoderAdapter);

    ~EventEncoder();

    void begin();

    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventEncoder&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventEncoder &ie) {
            (instance->*method)(et, ie); // Call the member function on the instance
        };
        callbackIsSet = true;
    }
    #endif

    void unsetCallback() override;

    void update();


    /** ***************************************
     *  encoder setup
     */

    /**
     * Encoder callbacks are normally fired on every loop() but for MPG
     * style encoders this can fire a huge number of events (that may 
     * swamp a serial connection).
     * The encoder interupts will sitll be called but this will limit 
     * the call back firing to every set ms - read the 
     * EncoderButton.increment() for lossless counting of encoder.
     * Set to zero (default) for no rate limit.
     */
    void setRateLimit(long ms=0) { rateLimit = ms; }

    /**
     * Quadrature encoders usually have four states for each 'click' of the 
     * rotary control, although some have just 2. 
     * Normally we only want to fire an event once per click so the default
     * positionDivider is 4.
     * You can set this to any positive integer eg 8 would increment the
     * position every 2 clicks. 
     * Affects pressed+turning too.
     */
    void setPositionDivider(uint8_t divider=4) {
        if ( divider > 0 ) {
                positionDivider = divider;
            }
        }

    uint8_t getPositionDivider() { return positionDivider; }


    /**
     * Reset the counted position of the encoder. 
     */
    void resetPosition(long pos = 0) { currentPosition = pos; }

    /**
     * Returns a positive (CW) or negative (CCW) integer. Is normally 1 or -1 but if your 
     * loop() has lots of processing, your Arduino is slow or you setRateLimit()
     * this will report the actual number of increments made by the encoder since
     * the last encoder handler event.
     */
    int16_t increment() { return encoderIncrement; }

    /**
     * The current position of the encoder. Can be reset by resetPosition()
     */
    long position() { return currentPosition; }

protected:

    void invoke(InputEventType et) override;
    void onEnabled() override;

private:

    EncoderAdapter *encoder;

    uint8_t positionDivider = 4;
    int32_t currentPosition  = 0;
    int32_t oldPosition  = 0;
    unsigned int rateLimit = 0;
    unsigned long rateLimitCounter = 0;   
    int encoderIncrement  = 0;

};

#endif

#endif