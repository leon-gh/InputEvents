/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventEncoder.h"

#ifndef EXCLUDE_EVENT_ENCODER

#ifdef FUNCTIONAL_SUPPORTED
    #include <functional>
#endif

/**
 * Construct a rotary encoder
 */
EventEncoder::EventEncoder(uint8_t pin1, uint8_t pin2) 
    : encoderPin1(pin1), encoderPin2(pin2) { }

void EventEncoder::begin() {
    encoder = new Encoder(encoderPin1, encoderPin2); 
}

void EventEncoder::invoke(InputEventType et) {
    if (isEventAllowed(et) && callbackFunction != nullptr) {
        callbackFunction(et, *this);
    }    
}


void EventEncoder::onEnabled() {
    //Reset the encoder so we don't trigger other events
    //idleFlagged = true;
    //encoder->write(encoderPosition*positionDivider);
    encoder->write(currentPosition*positionDivider);
    invoke(InputEventType::ENABLED);
}

void EventEncoder::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventEncoder::update() {
    // @TODO Do we store the current position when disabled and update if re-enabled?
    if ( _enabled ) {
        //encoder udate (fires encoder rotation callbacks)
        if ( millis() > (rateLimitCounter + rateLimit) ) { 
            readIncrement();
            if ( encoderIncrement !=0 ) {
                idleFlagged = false;    
                lastEventMs = millis();
                currentPosition += encoderIncrement;
                invoke(InputEventType::CHANGED);
            }
            rateLimitCounter = millis();
        }
        EventInputBase::update();
    }
}

void EventEncoder::readIncrement() {
    long newPosition = floor(encoder->read()/positionDivider);
    encoderIncrement = newPosition - oldPosition;
    oldPosition = newPosition;
}

#else 

#pragma message("Info: EventEncoder and EventEncoderButton excluded from your build. Install PJRC's Encoder library and #include <Encoder.h> to resolve.")

#endif