/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventButton.h"


EventButton::EventButton(byte pin, bool useDefaultDebouncer /*=true*/)
    : pinAdapter(new GpioPinAdapter(pin))
    { 
        if ( useDefaultDebouncer ) {
            debouncer = new FoltmanDebounceAdapter(pinAdapter);
        }
    }

EventButton::EventButton(PinAdapter* _pinAdapter, bool useDefaultDebouncer /*=true*/)
    : pinAdapter(_pinAdapter) 
    { 
        if ( useDefaultDebouncer ) {
            debouncer = new FoltmanDebounceAdapter(pinAdapter);
        }
    }

EventButton::EventButton(PinAdapter* _pinAdapter, DebounceAdapter* debounceAdapter) 
    : pinAdapter(_pinAdapter),
      debouncer(debounceAdapter)
    { 
        debouncer->setPinAdapter(pinAdapter);
    }

void EventButton::begin() {
    pinAdapter->begin();
    if ( debouncer ) {
        debouncer->begin();
    }
    changedState(); //Use to read/set inital state
    stateChanged = false;
}

void EventButton::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventButton::update() {
    if (_enabled) {
        //button update (fires pressed/released callbacks)
        if ( changedState() ) {
            if (pressing()) {
                invoke(InputEventType::PRESSED);
            } else if (releasing()) {
                clickFired = false;
                clickCounter++;
                prevClickCount = clickCounter;
                invoke(InputEventType::RELEASED);
            }
            stateChanged = false;
        }
        //fire long press callbacks
        if (currentState == pressedState) {
            resetIdleTimer();
            if (currentDuration() > (uint16_t)(longClickDuration + (longPressCounter * longPressInterval ))) {
                longPressCounter++;
                if ((repeatLongPress || longPressCounter == 1) ) {
                    invoke(InputEventType::LONG_PRESS);
                }
            }
        }
        //fire button click callbacks
        if (!clickFired && currentState != pressedState && currentDuration() > multiClickInterval) {
            clickFired = true;
            if (previousDuration() > longClickDuration) {
                clickCounter = 0;
                prevClickCount = 1;
                invoke(InputEventType::LONG_CLICKED);
                longPressCounter = 0;
            } else {
                if ( clickCounter == 1 ) {
                    invoke(InputEventType::CLICKED);
                } else if (clickCounter == 2 ) {
                    invoke(InputEventType::DOUBLE_CLICKED);
                } else {
                    invoke(InputEventType::MULTI_CLICKED);
                }
                clickCounter = 0;
            }
        }
        EventInputBase::update();
    }
}


void EventButton::invoke(InputEventType et) {
    if ( isInvokable(et) ) {
        callbackFunction(et, *this);
    }    
}

void EventButton::onDisabled() {
    //Reset button state
    clickCounter = 0;
    longPressCounter = 0;
    invoke(InputEventType::DISABLED);
}

bool EventButton::changedState() {
    if ( debouncer ) {
        currentPinState = debouncer->read();
    } else {
        currentPinState = pinAdapter->read();
    }
    if ( changedPinState() && currentPinState != currentState ) {
            changeState(currentPinState);
    }
    return stateChanged;
}

bool EventButton::changedPinState() {
    if ( currentPinState == previousPinState ) return false;
    previousPinState = currentPinState;
    return true;
}


void EventButton::changeState(bool newState) {
    previousState = currentState;
    currentState = newState;
    stateChanged = true;
    durationOfPreviousState = millis() - stateChangeLastTime;
    stateChangeLastTime = millis();
}

void EventButton::setDebouncer(DebounceAdapter* debounceAdapter) {
    debouncer = debounceAdapter;
    if (debouncer) { //Can pass nullptr to unset?
        debouncer->setPinAdapter(pinAdapter);
        debouncer->begin();
    }
}

bool EventButton::setDebounceInterval(uint16_t intervalMs) { 
    if ( debouncer ) {
        debouncer->setDebounceInterval(intervalMs);
        return true;
    }
    return false;
}

uint32_t EventButton::currentDuration() { return (millis() - stateChangeLastTime); }
