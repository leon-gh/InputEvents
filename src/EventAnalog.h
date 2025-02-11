/*
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

/**
 * @brief The EventAnalog class is for analog inputs - slice an analog range into configurable number of increments.
 * @details  For many uses of an analog input, the 1024 'slices' in the standard 10 bit analog range are more than is necessary. For 12 bit (4096), 14 bit (16384) and 16 bit (65536!)), even more so and with those higher numbers comes greater issues with noise, often resulting in a continuous fluctuation of values. 
 * 
 * This library allows you to reduce those 1024 (or up to 65536!) slices to a more managable number, calling a handler function or method each time a defined slice increments up or down.
 * 
 * This approach also provides very effective noise reduction.

The following InputEventTypes are fired by EventAnalog:
  - InputEventType::ENABLED - fired when the input is enabled.
  - InputEventType::DISABLED - fired when the input is disabled.
  - InputEventType::IDLE - fired after no other event (except <code>ENABLED</code> & <code>DISABLED</code>) has been fired for a specified time. Each input can define its own idle timeout. Default is 10 seconds.
  - InputEventType::CHANGED - fired on each change of increment
 */
class EventAnalog : public EventInputBase {

protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        /**
         * @brief If <code>std::function</code> is supported, this creates the callback type.
         */
        typedef std::function<void(InputEventType et, EventAnalog &ie)> CallbackFunction;
    #else
        /**
         * @brief Used to create the callback type as pointer if <code>std::function</code> is not supported.
         */
        typedef void (*CallbackFunction)(InputEventType et, EventAnalog &);
    #endif

    /**
     * @brief The callback function member.
     */
    CallbackFunction callbackFunction = nullptr;

    /**
     * @brief Override of the <code>EventInputBase::invoke()</code> virtual method.
     * 
     * @param et Enum of type <code>InputEventType</code>
     */
    void invoke(InputEventType et) override;

public:

    ///@{
    /** 
     * @name Constructor
     */
    /**
     * @brief Construct an EventAnalog input

     * @param analogPin  *Must* be an analog pin. For ESP32 avoid using pins attached to ADC2 (GPIO 0, 2, 4, 12-15, 25-27) as these are shared by the WiFi module.
     * @param adcBits For most boards the default 10 (bits) will work fine but if your board has an ADC (analog to digital converter) resolution that is higher, pass the resolution (in bits) of your board. For most ESP32s this is 12.
     */
    EventAnalog(byte analogPin, uint8_t adcBits=10);
    ///@}


    //@{
    /**
     * @name Common Methods
     * @details These methods are common to all InputEvent classes. 
     * 
     * Additional methods for input enable, timeout, event blocking and user ID/value are also inherited from the EventInputBase class.
     */

    /**
     * @brief Initialise the EventAnalog. *Must* be called from within <code>setup()</code>
     */
    void begin();

    /**
     * @brief Set the Callback function.
     * 
     * @param f A function of type <code>EventAnalog::CallbackFunction</code> type.
     */
    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    /**
     * @brief Set the Callback function to a class method.
     * 
     * @details Note: This method is only available if <code>std:function</code> is supported.
     * 
     * @param instance The instance of a class implementing a CallbackFunction method.
     * @param method The class method of type <code>EventAnalog::CallbackFunction</code> type.
     */
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

    /**
     * @brief Unset a previously set callback function or method.
     * @details Must be called before the set function or method is destoyed.
     */
    void unsetCallback() override;

    /**
     * @brief Update the state from the analog input. Must be called from within <code>loop()</code> in order to update state from the pin.
     */
    void update();
    /*@}*/

    ///@{
    /**
     * @name Getting the State
     * @details These methods return the current state of the input (as set during the last update() )
     */

    /**
     * @brief Returns the current position - this is not the analog value but the mapped position of the increments you have defined. 
     * @details Can be negative if setStartValue() is greater than the minimum position (eg for joysticks).
     * 
     * @return int16_t 
     */
    int16_t position() { return _reversePosition ? currentPos*-1 : currentPos; }

    /**
     * @brief Returns the previous position - this is not the analog value but the mapped position of the increments you have defined.
     * 
     * @return int16_t 
     */
    int16_t previousPosition() { return _reversePosition ? previousPos*-1 : previousPos; }


    /**
     * @brief Returns true if the position has changed since previous update()
     * 
     * @return true Changed since previous update()
     * @return false No change since previous update()
     */
    bool hasChanged() { return _hasChanged; }
    ///@}



    ///@{
    /**
     * @name Setting Required Increments
     * @details These methods are used to set the the number of increments to be 'sliced'. 
     */

    /**
     * @brief Split the analog range into this number of slices. The default is 25. 
     * @details  Note: This library is intended to reduce the usual 0-1024 range of analogRead() to a much smaller, more manageable number of 'slices'. 
     * Much higher numbers may produce variable results as a slice width is the ADC resolution (1024) divided by the number of increments. 
     * 
     * An InputEventType::CHANGED event will be fired each time the increment changes. 
     * 
     * @param numIncr The number of desired increments. Should not be more than 20% of the ADC max value.
     */
    void setNumIncrements(uint8_t numIncr=10);

    /**
     * @brief Normally increments are set with setNumIncrements() but you can also set the negative and positive sides of 'centre normal' individually.
     * 
     * @param numIncr The number of increments required on the negative side.
     */
    void setNumNegativeIncrements(uint8_t numIncr=10);

    /**
     * @brief Normally increments are set with setNumIncrements() but you can also set the negative and positive sides of 'centre normal' individually.
     * 
     * @param numIncr The number of increments required on the positive side.
     */
    void setNumPositiveIncrements(uint8_t numIncr=10);
    ///@}


    ///@{
    /**
     * @name Setting Analog Min, Max and Start values
     * @details These methods are used to set the min, max and start analog values of the input.
     */

    /**
     * @brief Set the *analog* value that represents the 'starting' position. 
     * @details For normal potentiometers this would be 0 (the default) but for joysticks it would be the centre position of the joystick's potentionmeter (normally 512).
     * 
     * @param val This is the analog (ADC) value
     */
    void setStartValue(uint16_t val);
    
    /**
     * @brief Will set the start value as the current position. 
     * @details Useful for joysticks that 'rest' in the centre but can also be called from a button push to change the behaviour of an analog input.
     */
    void setStartValue();
    
    /**
     * @brief The minimum ADC value that can be read by the analog pin.
     * @details By default the minValue is set to 5% of the ADC range and auto calibrated  as the input is used. 
     * You can manually set it here if you have confidence in your potentiometer range.
     * 
     * @param val This is the minimum readable analog (ADC) value
     */
    void setMinValue(uint16_t val);

    /**
     * @brief The maximmum ADC value that can be read by the analog pin.
     * @details By default the maxValue is set to 95% of the ADC range and auto calibrated as the input is used. 
     * You can manually set it here if you have confidence in your potentiometer range.

     * @param val This is the maximum readable analog (ADC) value
     */
    void setMaxValue(uint16_t val);
    ///@}

    ///@{
    /**
     * @name Setting Analog Start and End Boundaries
     * @details Boundaries set an area at the start or end of travel that is ignored. Some potentiometers are very inconsistent at either end and for joysticks it is impossible to reach min/max 'on the diagonal'.
     */

    /**
     * @brief Used primarily for joysticks.
     * @details It is very difficult to press the joystick button without moving the joystick so with this we can
     * create a central 'deadzone'.
     * 
     * @param width - the analog value 
     */
    void setStartBoundary(uint16_t width=200);
    
    /**
     * @brief Used primarily for joysticks.
     * @details Again, used primarily for joysticks to create an outer 'deadzone'
     * where joysticks are notoriously inconsistent or cannot be reach on a diagonal.
     * 
     * @param width - the analog value 
     */
    void setEndBoundary(uint16_t width=100);
    ///@}
    
    ///@{
    /**
     * @name Other Configuration Settings
     */
    /**
     * @brief Limit the rate at which events are fired.
     * @details When moving an analog fast it can generate a lot of events, so you can rate limit.
     * 
     * @param ms Number of milliseconds between events
     */
    void setRateLimit(uint16_t ms) { rateLimit = ms; }

    /**
     * @brief If enableAutoCalibrate is set to true (the default), will
     * do auto calibration, setting the minValue and maxValue.
     * 
     * @details This will be done even when input is disabled (ie no callbacks fired)
     */
    void enableAutoCalibrate(bool enable=true) { autoCalibrate = enable; }

    /**
     * @brief Reverse the increments
     * @details If your position is coming out backawards (negative), you can set this rather that rewire your input.
     * 
     * @param rev Default true to reverse, pass false to restore default behaviour.
     */
    void reversePosition(bool rev=true) { _reversePosition = rev; }

    /**
     * @brief Returns true if position is reversed.
     * 
     * @return true Position is reverse.
     * @return false Position is not reversed (default behaviours)
     */
    bool isPositionReversed() { return _reversePosition; }
    ///@}

    protected:

    /**
     * @brief Set the size of each negative slice/increment
     */
    void setSliceNeg() { sliceNeg = max(((startVal-startBoundary-minVal-endBoundary)/negativeIncrements),1); }//Never allow 0

    /**
     * @brief Set the size of each positive slice/increment
     */
    void setSlicePos() { slicePos = max(((maxVal-endBoundary-startBoundary-startVal)/positiveIncrements),1);  }//Never allow 0

private:
    byte analogPin = 0;
    int16_t startVal = 0;
    int16_t readVal = startVal;
    int16_t currentVal = startVal;
    int16_t previousVal = currentVal;
    int16_t minVal = 100;
    int16_t maxVal = 980;
    int16_t startBoundary = 0;
    int16_t endBoundary = 0;
    int16_t adcMax = 1023;


    int16_t negativeIncrements = 25;
    int16_t positiveIncrements = 25;
    int16_t sliceNeg = 20;
    int16_t slicePos = 20;

    int16_t readPos = 0;
    int16_t currentPos = 0;
    int16_t previousPos = 0;
    bool _reversePosition = false;

    bool autoCalibrate = true;
    bool _hasChanged = false;
    bool _started = false;

    uint16_t rateLimit = 0;
    unsigned long rateLimitCounter = 0;   

    void setReadPos(int16_t offset);
    void setInitialReadPos();




};

#endif




