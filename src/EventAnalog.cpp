/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */


#include "EventAnalog.h"

EventAnalog::EventAnalog(byte pin) {
    analogPin = pin;
    setSliceNeg();
    setSlicePos();
}

void EventAnalog::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventAnalog::invoke(InputEventType et) {
    if (isEventAllowed(et) && callbackFunction != nullptr) {
        callbackFunction(et, *this);
    }    
}

void EventAnalog::update() {
    if (!_started) {
        // Set the start position so we don't trigger an event before moving
        readVal =  constrain(map(analogRead(analogPin), 0, adcResolution, 0, 1023 ), 0, 1023);
        setReadPos(readVal - startVal);
        currentPos = readPos;
        previousPos = currentPos;
        _started = true;
    }


    if ( _enabled || _allowRead ) {
        _hasChanged = false;
        readVal =  constrain(map(analogRead(analogPin), 0, adcResolution, 0, 1023 ), 0, 1023);
        // For joysticks, resistance either side of centre can be quite 
        // different ranges so we need to slice both sides
        if ( readVal < minVal ) {
            minVal = readVal;
            setSliceNeg();
        } else if ( readVal > maxVal ) {
            maxVal = readVal;
            setSlicePos();
        }

        if ( _enabled ) {
            if( millis() > (rateLimitCounter + rateLimit) ) { 
                setReadPos(readVal - startVal);
                if ( currentPos != readPos ) {
                    previousPos = currentPos;
                    currentPos = readPos;
                    lastEventMs = millis();
                    idleFlagged = false;
                    _hasChanged = true;
                    invoke(InputEventType::CHANGED);
                }
                rateLimitCounter = millis();
            }
            EventInputBase::update();
        }
    }
}

void EventAnalog::setReadPos(int16_t offset) {
    if ( offset > startBoundary) { //Going up!
        if ( abs(readVal - previousVal) > slicePos ) {
            previousVal = readVal;
            int16_t rawReadPos = ((readVal-startBoundary-startVal)/slicePos);
            readPos = min(rawReadPos, (positiveIncrements));
        }
    } else if (abs(offset) > startBoundary) { //Going down
        if ( abs(readVal - previousVal) > sliceNeg ) {
            previousVal = readVal;
            int16_t rawReadPos = ((startVal-startBoundary-readVal)/sliceNeg) * -1;
            readPos = max(rawReadPos, (int16_t)(negativeIncrements*-1));
        }
    } else {
        previousVal = readVal;
        readPos = 0;
    }
}


void EventAnalog::setStartValue(uint16_t val) {
    startVal = max(val, (uint16_t)0);
    setSliceNeg();
    setSlicePos();
}

void EventAnalog::setStartValue() {
    setStartValue(analogRead(analogPin));
}


void EventAnalog::setStartBoundary(uint16_t width /*=200*/) {
    startBoundary = max(width, (uint16_t)1);
    setSliceNeg();
    setSlicePos();
}
void EventAnalog::setEndBoundary(uint16_t width /*=100*/) {
    endBoundary = max(width, (uint16_t)1);
    setSliceNeg();
    setSlicePos();
}
void EventAnalog::setNumIncrements(uint8_t numIncr /*=10*/) {
    negativeIncrements=max(numIncr, (uint8_t)1);
    positiveIncrements=max(numIncr, (uint8_t)1);
    setSliceNeg();
    setSlicePos();
}

void EventAnalog::setNumNegativeIncrements(uint8_t numIncr /*=10*/) {
    negativeIncrements=max(numIncr, (uint8_t)1);
    setSliceNeg();
}

void EventAnalog::setNumPositiveIncrements(uint8_t numIncr /*=10*/) {
    positiveIncrements=max(numIncr, (uint8_t)1);
    setSlicePos();
}


