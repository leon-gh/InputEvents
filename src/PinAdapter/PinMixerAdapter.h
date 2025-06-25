#ifndef PinMixerAdapter_h
#define PinMixerAdapter_h

#include <Arduino.h>
#include "PinAdapter.h"

/**
 * @brief This is the default PinAdapter for regular GPIO pins.
 * 
 */
class PinMixerAdapter : public PinAdapter {

    public:
    /**
     * @brief Takes two PinAdapter classes and mixes their read() calls. Initial state is taken from pin1.
     * @details The mixed read() state will only change when either PinAdapter *changes* its read() state.
     * ie if pin1 state changes from HIGH to LOW the read() state will change to LOW, even if pin2 state is currently HIGH.
     * The read() state will remain LOW until either pin changes state again (eg pin1 changes to HIGH or pin2 state changes from HIGH->LOW->HIGH
     * 
     * @param pin1 A reference to a PinAdapter
     * @param pin2 A reference to a PinAdapter
     */
    PinMixerAdapter(PinAdapter* pin1, PinAdapter* pin2 )
        : pin1(pin1), pin2(pin2)
        { }

    void begin() {
        pin1->begin();
        pin2->begin();
        currentStatePin1 = previousStatePin1 = pin1->read();
        currentStatePin2 = previousStatePin2 = pin2->read();
        state = currentStatePin1; //we have to choose one...
    }

    bool read() {
        //Only update state if the either of the pixer pin states have changed
        currentStatePin1 = pin1->read();
        if ( currentStatePin1 != previousStatePin1 ) {
            previousStatePin1 = currentStatePin1;
            state = currentStatePin1;
        }
        currentStatePin2 = pin2->read();
        if ( currentStatePin2 != previousStatePin2 ) {
            previousStatePin2 = currentStatePin2;
            state = currentStatePin2;
        }
        return state;
    }

    private:
    PinAdapter* pin1;
    PinAdapter* pin2;
    bool state; //the returned state
    bool currentStatePin1;
    bool previousStatePin1;
    bool currentStatePin2;
    bool previousStatePin2;

};

#endif