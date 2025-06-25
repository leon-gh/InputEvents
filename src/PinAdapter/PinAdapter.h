#ifndef PinAdapter_h
#define PinAdapter_h

/**
 * @brief The interface specification for button, encoder button and switch pins.
 * 
 */
class PinAdapter {
    public:
    /**
     * @brief Initialise the pin adapter. Must be safe for repeated calls (Idempotent)
     * 
     */
    virtual void begin() = 0;
    /**
     * @brief Read the current state of the pin
     * 
     * @return true/HIGH
     * @return false/LOW
     */
    virtual bool read() = 0;

    virtual ~PinAdapter() = default;
};

#endif