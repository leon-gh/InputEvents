/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */


#include "EventJoystick.h"


EventJoystick::EventJoystick(byte analogX, byte analogY)
    : x(analogX), y(analogY) {
        #ifdef FUNCTIONAL_SUPPORTED
        x.setCallback([&](InputEventType et, EventAnalog &enc) { onInputXCallback(et, enc); });
        y.setCallback([&](InputEventType et, EventAnalog &enc) { onInputYCallback(et, enc); });
        #else
        x.setOwner(this);
        x.setCallback(EventJoystick::analogXCallback);
        y.setOwner(this);
        y.setCallback(EventJoystick::analogYCallback);
        #endif
        setCentreBoundary(200);
        /*
         * readAnalog() will not work if object is contructed as global (like most sketches
         * in the Arduino examples) so call setStartValues() in setup() if required.
        */
        setStartValues();
}




void EventJoystick::onInputCallback(InputEventType et, EventInputBase & ie) {
    //Only fire ENABLED and DISABLED events from EventJoystick
    if ( et == InputEventType::ENABLED || et == InputEventType::DISABLED) return;
    //Only fire IDLE if both the EventAnalogs are idle
    if ( et == InputEventType::IDLE && (!x.isIdle() || !y.isIdle()  ) ) return;
    //Translate the callback
    invoke(et);    
}


void EventJoystick::onInputXCallback(InputEventType et, EventInputBase & ie) {
    if ( et == InputEventType::CHANGED ) {
        onInputCallback(InputEventType::CHANGED_X, ie);
    } else {
        onInputCallback(et, ie);
    }
}

void EventJoystick::onInputYCallback(InputEventType et, EventInputBase & ie) {
    if ( et == InputEventType::CHANGED ) {
        onInputCallback(InputEventType::CHANGED_Y, ie);
    } else {
        onInputCallback(et, ie);
    }
}


void EventJoystick::update() {
    x.update();
    y.update();
}


void EventJoystick::setStartValues() {
    x.setStartValue();
    y.setStartValue();
}


void EventJoystick::setNumIncrements(uint8_t numIncr /*=10*/) {
    x.setNumIncrements(numIncr);
    y.setNumIncrements(numIncr);
}

void EventJoystick::setNumNegativeIncrements(uint8_t numIncr /*=10*/) {
    x.setNumNegativeIncrements(numIncr);
    y.setNumNegativeIncrements(numIncr);
}

void EventJoystick::setNumPositiveIncrements(uint8_t numIncr /*=10*/) {
    x.setNumPositiveIncrements(numIncr);
    y.setNumPositiveIncrements(numIncr);
}

void EventJoystick::setCentreBoundary(uint16_t width /*=200*/) {
    x.setStartBoundary(width);
    y.setStartBoundary(width);
}

void EventJoystick::setOuterBoundary(uint16_t width /*=100*/) {
    x.setEndBoundary(width);
    y.setEndBoundary(width);
}

bool EventJoystick::hasChanged() {
    return x.hasChanged() || y.hasChanged();
}


bool EventJoystick::isEnabled() { return x.isEnabled() && y.isEnabled(); }

void EventJoystick::allowRead(bool allow) { 
    x.allowRead(allow);
    y.allowRead(allow);
}


bool EventJoystick::isIdle() {
    //Return true if idle (whether idle callback defined or not)
    return x.isIdle() && y.isIdle();
}

void EventJoystick::setRateLimit(uint16_t ms) {
    x.setRateLimit(ms);
    y.setRateLimit(ms);
}


void EventJoystick::invoke(InputEventType et) {
    if (isEventAllowed(et) && callbackFunction != nullptr) {
        callbackFunction(et, *this);
    }    
}

void EventJoystick::onEnabled() {
    x.enable(true);
    y.enable(true);
    invoke(InputEventType::ENABLED);
}

void EventJoystick::onDisabled() {
    x.enable(false);
    y.enable(false);
    invoke(InputEventType::DISABLED);
}


