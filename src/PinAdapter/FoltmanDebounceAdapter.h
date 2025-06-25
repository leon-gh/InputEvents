#ifndef FoltmanDebounceAdapter_h
#define FoltmanDebounceAdapter_h

#include "Arduino.h"
#include "DebounceAdapter.h"

/**
 * @brief This is the default InputEvents debouncer. Many thanks to @kfoltman.
 * 
 */
class FoltmanDebounceAdapter : public DebounceAdapter {
    public:
    FoltmanDebounceAdapter(PinAdapter* pinAdapter)
    : DebounceAdapter(pinAdapter)
    { }

    void begin() {
        DebounceAdapter::begin();
        lastChangeMs = millis();
        nextState = lastState = pinAdapter->read();
    }

    bool read() override {
        bool newState = pinAdapter->read();
        if (nextState == lastState) {
            // Steady state so far
            if (newState != nextState) {
                // Initiating state change
                nextState = newState;
                lastChangeMs = millis();
            }
        } else {
            // Change pending
            if (newState != nextState) {
                // Glitch: reset the counter
                nextState = lastState;
                lastChangeMs = millis();
            } else if (millis() - lastChangeMs >= debounceInterval) {
                // Got debounceInterval ms of glitchless signal
                lastState = newState;
            }
        }
        return lastState;
    }

    private:
    uint32_t lastChangeMs;
    bool lastState, nextState;
};
#endif