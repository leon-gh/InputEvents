#ifndef DebounceAdapter_h
#define DebounceAdapter_h

#include "Arduino.h"
#include "PinAdapter.h"

/**
 * @brief This is the interface/base class for debounce adapters
 * 
 */
class DebounceAdapter : public PinAdapter {
    public:
    DebounceAdapter(uint16_t debounceInterval = 10)
    : debounceInterval(debounceInterval)
    { }

    DebounceAdapter(PinAdapter* pinAdapter, uint16_t debounceInterval = 10)
    : pinAdapter(pinAdapter),
      debounceInterval(debounceInterval)
    { }


    /**
     * @brief Initialise the debouncer and pin adapter. Must be safe for repeated calls (Idempotent)
     * 
     */
    void begin() {
        pinAdapter->begin();
    }

    /**
     * @brief Return the debounced state of the pin adapter
     * 
     * @return true/HIGH
     * @return false/LOW
     */
    virtual bool read() = 0;

    /**
     * @brief The pinAdapter is usually passed via the constructor. 
     * If it is not, it must be set before begin() is called.
     * 
     * @param adapter 
     */
    void setPinAdapter(PinAdapter* adapter ) {
        pinAdapter = adapter;
    }

    /**
     * @brief Set the debounce interval. Default is 10ms
     * 
     * @param interval 
     */
    virtual void setDebounceInterval(uint16_t interval) {
        debounceInterval = interval;
    }

    protected:
    PinAdapter* pinAdapter;
    uint16_t debounceInterval = 10;   
};

#endif



