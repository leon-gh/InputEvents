/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */


#include "EventAnalog.h"

EventAnalog::EventAnalog(byte pin, uint8_t adcBits /*=10*/) {
    analogPin = pin;
    adcMax = (1U << adcBits) - 1;
    minVal = adcMax/10;
    maxVal = adcMax - minVal;
    adcResolution = adcMax;
}

void EventAnalog::begin() {
    pinMode(analogPin, INPUT);
    delayMicroseconds(2000); // Allow pin to settle
    setSliceNeg();
    setSlicePos();
    // Some boards change the ADC value between begin() and first update())
    // so this is re-called in update(). Required here so position() can be used
    // before first update();
    setInitialReadPos();
}

void EventAnalog::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventAnalog::invoke(InputEventType et) {
    if ( isInvokable(et) ) {
        callbackFunction(et, *this);
    }    
}

void EventAnalog::update() {
    if (!_started) {
        // This should only be required in begin() method but on some boards (ESP32s mainly) 
        // the analog output will change between begin() and the first update()
        // triggering a CHANGED event.
        // Set the start position so we don't trigger an event before moving
        setInitialReadPos();
        _started = true;
    }

    if ( _enabled || autoCalibrate ) {
        _hasChanged = false;
        readVal =  constrain(map(analogRead(analogPin), 0, adcResolution, 0, adcMax ), 0, adcMax);
        // For joysticks, resistance either side of centre can be quite 
        // different ranges so we need to slice both sides
        if ( autoCalibrate ) {
            if ( readVal < minVal ) {
                minVal = readVal;
                setSliceNeg();
            } else if ( readVal > maxVal ) {
                maxVal = readVal;
                setSlicePos();
            }
        }
        if ( _enabled ) {
            if( millis() > (rateLimitCounter + rateLimit) ) { 
                setReadPos(readVal - startVal);
                if ( currentPos != readPos ) {
                    previousPos = currentPos;
                    currentPos = readPos;
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

void EventAnalog::setInitialReadPos() {
    // Set the start position so we don't trigger an event before moving
    readVal =  constrain(map(analogRead(analogPin), 0, adcResolution, 0, adcMax ), 0, adcMax);
    setReadPos(readVal - startVal);
    currentPos = readPos;
    previousPos = currentPos;
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


