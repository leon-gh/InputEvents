
/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventEncoderButton.h"

#ifndef EXCLUDE_EVENT_ENCODER


EventEncoderButton::EventEncoderButton(EncoderAdapter *encoderAdapter, byte buttonPin)
    : encoder(encoderAdapter), button(buttonPin) {
        #ifdef FUNCTIONAL_SUPPORTED
        encoder.setCallback([&](InputEventType et, EventEncoder &enc) { onInputCallback(et, enc); });
        button.setCallback([&](InputEventType et, EventButton &btn) { onInputCallback(et, btn); });
        #else
        encoder.setOwner(this);
        encoder.setCallback(EventEncoderButton::encoderCallback);
        button.setOwner(this);
        button.setCallback(EventEncoderButton::buttonCallback);
        #endif

    }

void EventEncoderButton::begin() {
    encoder.begin();
    button.begin();
}

void EventEncoderButton::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventEncoderButton::update() {
    encoder.update();
    button.update();
}


void EventEncoderButton::invoke(InputEventType et) {
    if ( isInvokable(et) ) {
        callbackFunction(et, *this);
    }    
}

void EventEncoderButton::onEnabled() {
    encoder.enable();
    button.enable();
    invoke(InputEventType::ENABLED);
}

void EventEncoderButton::onDisabled() {
    encoder.enable(false);
    button.enable(false);
    invoke(InputEventType::DISABLED);
}


void EventEncoderButton::onInputCallback(InputEventType et, EventInputBase & ie) {
    //Only fire ENABLED and DISABLED events from EventEncoderButton
    if ( et == InputEventType::ENABLED || et == InputEventType::DISABLED) return;

    //Only fire IDLE if both the encoder and button are idle
    if ( et == InputEventType::IDLE && !(encoder.isIdle() && button.isIdle()  ) ) return;

    // Convert CHANGED to CHANGED_PRESSED if button is pressed
    if ( et == InputEventType::CHANGED && button.isPressed() ) {
        encodingPressed = true;
        currentPressedPosition += encoder.increment();
        //Reverse out the unpressed position
        encoder.resetPosition((encoder.position() - encoder.increment()));
        et = InputEventType::CHANGED_PRESSED;
    }
    if ( encodingPressed ) {
        //Stop LONG_PRESS    
        if (et == InputEventType::LONG_PRESS) {
            return;
        }
        // Convert RELEASED to CHANGED_RELEASED
        if ( et == InputEventType::RELEASED) {
            et = InputEventType::CHANGED_RELEASED;
        }
        //Stop CLICKED and LONG_CLICKED (it will only be one of them)
        if (  et == InputEventType::CLICKED || et == InputEventType::LONG_CLICKED )  {
            encodingPressed = false;
            return;
        }
    }

    //Translate the callback
    invoke(et);    
}


void EventEncoderButton::setRateLimit(long ms) { encoder.setRateLimit(ms); }

void EventEncoderButton::setPositionDivider(uint8_t divider /*=4*/) { encoder.setPositionDivider(divider); }

uint8_t EventEncoderButton::getPositionDivider() {return encoder.getPositionDivider(); }

void EventEncoderButton::resetPosition(long pos) { encoder.resetPosition(pos);}

int16_t EventEncoderButton::increment() { return encoder.increment(); }

long EventEncoderButton::position() { return encoder.position(); }

void EventEncoderButton::resetPressedPosition(long pos) { currentPressedPosition = pos; }

long EventEncoderButton::pressedPosition() { return currentPressedPosition; }

bool EventEncoderButton::buttonState() { return  button.buttonState(); }

bool EventEncoderButton::isPressed() { return button.isPressed(); }

void EventEncoderButton::setDebounceInterval(unsigned int intervalMs) { button.setDebounceInterval(intervalMs); }

void EventEncoderButton::setMultiClickInterval(unsigned int intervalMs) { button.setMultiClickInterval(intervalMs); }

void EventEncoderButton::setLongClickDuration(unsigned int longDurationMs) { button.setLongClickDuration(longDurationMs); }

void EventEncoderButton::enableLongPressRepeat(bool repeat /*=true*/) { button.enableLongPressRepeat(repeat); }

void EventEncoderButton::setLongPressInterval(unsigned int intervalMs) { button.setLongPressInterval(intervalMs); }

unsigned char EventEncoderButton::clickCount() { return button.clickCount(); }

uint32_t EventEncoderButton::currentDuration() { return button.currentDuration(); }

uint32_t EventEncoderButton::previousDuration() { return button.previousDuration(); }

uint8_t EventEncoderButton::longPressCount() { return button.longPressCount(); }


#endif