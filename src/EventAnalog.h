/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */


#ifndef EVENT_ANALOG_H
#define EVENT_ANALOG_H

#include "Arduino.h"
#include "EventInputBase.h"

class EventAnalog : public EventInputBase {

protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        typedef std::function<void(InputEventType et, EventAnalog &ie)> CallbackFunction;
    #else
        typedef void (*CallbackFunction)(InputEventType et, EventAnalog &);
    #endif

    CallbackFunction callbackFunction = nullptr;

    void invoke(InputEventType et) override;

public:

    /**
     * Construct a button
     */
    EventAnalog(byte analogPin);

    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventAnalog&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventAnalog &ie) {
            (instance->*method)(et, ie); // Call the member function on the instance
        };
        callbackIsSet = true;
    }
    #endif

    void unsetCallback() override;

    void update();

    /**
     * @brief Read the current position - this is not the analog value
     * but the mapped position of the increments you have defined.
     * 
     * @return int16_t 
     */
    int16_t position() { return _reversePosition ? currentPos*-1 : currentPos; }
    /**
     * @brief Read the previous position - this is not the analog value
     * but the mapped position of the increments you have defined.
     * 
     * @return int16_t 
     */
    int16_t previousPosition() { return _reversePosition ? previousPos*-1 : previousPos; }


    /**
     * @brief Returns true of the position of either EventAnalog
     * axis has changed since previous update()
     * 
     * @return true 
     * @return false 
     */
    bool hasChanged() { return _hasChanged; }



    /**
     * @brief Split the analog range into this number of slices.
     * A changed callback will be fire each time the increment changes. 
     * 
     * @param numIncr 
     */
    void setNumIncrements(uint8_t numIncr=10);

    /**
     * @brief Set the underlying analog value that equates to the zero position
     * 
     * @param val 
     */
    void setStartValue(uint16_t val);
    
    /**
     * @brief Set the underlying analog value that equates to the zero position - read from current position
     * 
     * @param val 
     */
    void setStartValue();
    
    /**
     * @brief Used primarily for joysticks - it is very difficult to press the 
     * joystick button without moving the joystick so with this we can
     * create a central 'deadzone'.
     * @param width - the analog value 
     * 
     */
    void setStartBoundary(uint16_t width=200);
    
    /**
     * Again, used primarily for joysticks to create an outer 'deadzone'
     * where joysticks are notoriously inconsistent.
     * @param width - the analog value 
     */
    void setEndBoundary(uint16_t width=100);

    /**
     * Set the ADC resolution for your board. Only use if your board
     * is not the default 1023 resolution or you are testing a 5V UNO 
     * board with a 3V3 input (675)!
     * Note: All resolutionas are internally mapped to 0-1023.
     */
    void setAdcResolution(uint16_t res=1023) { adcResolution = res; }

    /**
     * @brief Normally increments are set with setNumIncrements but
     * you can also set the negative and positive sides individually.
     * 
     * @param numIncr 
     */
    void setNumNegativeIncrements(uint8_t numIncr=10);

    /**
     * @brief Normally increments are set with setNumIncrements but
     * you can also set the negative and positive sides individually.
     * 
     * @param numIncr 
     */
    void setNumPositiveIncrements(uint8_t numIncr=10);


    void setRateLimit(uint16_t ms) { rateLimit = ms; }

    /**
     * If allowRead is set to true, will still do analogRead
     * to allow manual setting of max negative/positive values
     * even when input is disabled (ie no callbacks fired)
     */
    void allowRead(bool allow=true) { _allowRead = allow; }


    void reversePosition(bool rev=true) { _reversePosition = rev; }

    bool isPositionReversed() { return _reversePosition; }

    protected:

    void setSliceNeg() { sliceNeg = max(((startVal-startBoundary-minVal-endBoundary)/negativeIncrements),1); }//Never allow 0

    void setSlicePos() { slicePos = max(((maxVal-endBoundary-startBoundary-startVal)/positiveIncrements),1);  }//Never allow 0

private:
    byte analogPin = 0;
    int16_t startVal = 100;
    int16_t readVal = startVal;
    int16_t currentVal = startVal;
    int16_t previousVal = currentVal;
    int16_t minVal = 100;
    int16_t maxVal = 980;
    int16_t startBoundary = 50;
    int16_t endBoundary = 50;
    int16_t adcResolution = 1023;


    int16_t negativeIncrements = 25;
    int16_t positiveIncrements = 25;
    int16_t sliceNeg = 20;
    int16_t slicePos = 20;

    int16_t readPos = 0;
    int16_t currentPos = 0;
    int16_t previousPos = 0;
    bool _reversePosition = false;

    bool _allowRead = true;
    bool _hasChanged = false;
    bool _started = false;

    uint16_t rateLimit = 0;
    unsigned long rateLimitCounter = 0;   

    void setReadPos(int16_t offset);




};

#endif




