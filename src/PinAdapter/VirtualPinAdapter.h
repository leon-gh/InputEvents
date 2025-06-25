#ifndef VirtualPinAdapter_h
#define VirtualPinAdapter_h

#include <Arduino.h>
#include "PinAdapter.h"

/**
 * @brief A PinAdapter that can set state programtically.
 * 
 */
class VirtualPinAdapter : public PinAdapter {

    public:
    /**
     * @brief Construct a VirtualPinAdapter.
     * 
     * @param pressedState The state that represents 'pressed'. Default is LOW.
     */
    VirtualPinAdapter(bool pressedState = LOW)
    : pressedState(pressedState)
    { }

    /**
     * @brief Sets the iniial state
     */
    void begin() { 
        state = !pressedState;
    }

    /**
     * @brief Returns the current state
     * 
     * @return true 
     * @return false 
     */
    bool read() {
        return state;
    }

    /**
     * @brief Set the state to pressedState
     */
    void press() {
        state = pressedState;
    }

    /**
     * @brief Set the state to 'not pressedState'
     */
    void release() {
        state = !pressedState;
    }

    /**
     * @brief Directly set the state.
     * 
     * @param newState 
     */
    void setState(bool newState) {
        state = newState;
    }

    private:
    bool pressedState = LOW;
    bool state = HIGH;

};

#endif