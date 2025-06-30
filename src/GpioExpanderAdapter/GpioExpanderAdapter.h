#ifndef GpioExpanderAdapter_h
#define GpioExpanderAdapter_h

#include "Arduino.h"

/**
 * @brief The base class/interface specification for GPIO expanders.
 *
 */
class GpioExpanderAdapter {

    public:
    /**
     * @brief Initialize the expander. (Idempotent)
     *
     */
    virtual void begin() = 0;

    /**
     * @brief Read the state of all pins on the expander. 
     * 
     * @details Expander pins generally cannot be read like regular GPIO pins, they have to be 'scanned' for their current state before reading individual pins.
     *
     */
    virtual void update() = 0;

    /**
     * @brief Returns the state of a pin on the expander
     *
     * @return true aka HIGH
     * @return false aka LOW
     */
    virtual bool read(byte pin) = 0;

    /** @brief Use it to configure individual pin mode, if expander allows it.
     * Not all of them do.
     */
    virtual void attachPin(byte pin, int mode = INPUT_PULLUP) = 0;

    virtual ~GpioExpanderAdapter() = default;

};

#endif
