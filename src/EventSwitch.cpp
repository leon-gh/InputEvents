/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventSwitch.h"


EventSwitch::EventSwitch(byte buttonPin)
    : bounce(new Bounce()) {
        pinMode(buttonPin, INPUT_PULLUP); //Set pullup first
        // Top tip From PJRC's Encoder - without this delay the
        // long-press doesn't fire on first press.
        // allow time for a passive R-C filter to charge
        // through the pullup resistors, before reading
        // the initial state
        delayMicroseconds(2000); //Delay
        bounce->attach(buttonPin, INPUT_PULLUP); //then attach button
        currentState = bounce->read(); //Initialise switch state from Bounce2
    }



void EventSwitch::update() {
    if (_enabled) {
        if (bounce->update()) {
            lastEventMs = millis();
            idleFlagged = false;
            currentState = bounce->read();
            if (bounce->fell()) {
                previousState = HIGH;
                invoke(reversed ? InputEventType::OFF :InputEventType::ON);
            } else if (bounce->rose()) {
                invoke(reversed ? InputEventType::ON :InputEventType::OFF);
                previousState = LOW;
            }
        }
        EventInputBase::update();
    }
}

void EventSwitch::invoke(InputEventType et) {
    if (isEventAllowed(et) && callbackFunction != nullptr) {
        callbackFunction(et, *this);
    }    
}


void EventSwitch::setDebounceInterval(unsigned int intervalMs) { bounce->interval(intervalMs); }

unsigned long EventSwitch::currentDuration() { return bounce->currentDuration(); }

unsigned long EventSwitch::previousDuration() { return bounce->previousDuration(); }

