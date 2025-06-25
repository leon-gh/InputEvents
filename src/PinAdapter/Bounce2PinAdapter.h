#ifndef Bounce2PinAdapter_h
#define Bounce2PinAdapter_h

#include "DebounceAdapter.h"
#include "Bounce2.h"
/**
 * @brief A PinAdapter for the Bounce2 library. Note: this is *not* a DebounceAdapter as Bounce2 
 * can only read the GPIO pins directly, not via a PinAdapter.
 * 
 * If you want to use this PinAdapter, you must manually install the Bounce2 library as it is 
 * no longer a dependency for InputEvents.
 */
class Bounce2PinAdapter : public PinAdapter {

    public:

    Bounce2PinAdapter(byte pin)
    : buttonPin(pin)
    , bounce(new Bounce())
    { }


    void begin() {
        delayMicroseconds(2000); //Delay
        bounce->attach(buttonPin, INPUT_PULLUP);
    }

    bool read() {
        bounce->update();
        return bounce->read();
    }

    private:
    byte buttonPin;
    Bounce* bounce;


};

#endif