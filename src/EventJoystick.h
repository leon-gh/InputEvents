/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */


#ifndef EVENT_JOYSTICK_H
#define EVENT_JOYSTICK_H

#include "Arduino.h"
#include "EventAnalog.h"


class EventJoystick : public EventInputBase {

protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        typedef std::function<void(InputEventType et, EventJoystick &ie)> CallbackFunction;
    #else
        typedef void (*CallbackFunction)(InputEventType et, EventJoystick &);
    #endif

    CallbackFunction callbackFunction = nullptr;

    void invoke(InputEventType et) override;

    void onEnabled() override;
    void onDisabled() override;
    void onIdle() override { /* Do nothing. Fire idle callback from either x or y but only if both are idle from onInputCallback() */ }

public:

    void begin();

    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventJoystick&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventJoystick &ie) {
            (instance->*method)(et, ie); // Call the member function on the instance
        };
        callbackIsSet = true;
    }
    #endif

    EventAnalog x;
    EventAnalog y;

    /**
     * Construct a joystick
     */
    EventJoystick(byte analogX, byte analogY, uint8_t adcBits=10);

    void unsetCallback() override;

    void update();



    /** ***************************************
     *  joystick setup
     */


    /**
     * @brief Set the underlying analog values for x & y that equates to the zero position
     *  - read from current position
     * 
     * @param val 
     */
    void setStartValues();

    /**
     * @brief Split the analog range of both axis into this number of slices.
     * A changed callback will be fire each time the increment changes. 
     * You can also set increments on individial axis after this has 
     * been called.
     * @param numIncr 
     */
    void setNumIncrements(uint8_t numIncr=10);

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

    void setCentreBoundary(uint16_t width=200);
    void setOuterBoundary(uint16_t width=100);


    /**
     * @brief Returns true of the position of either EventAnalog
     * axis has changed since previous update()
     * 
     * @return true 
     * @return false 
     */
    bool hasChanged();

    /**
     * Returns true if enabled or if either of the EventAnalg axis are enabled
     */
    bool isEnabled();

    /**
     * @brief Return true if no activity on both EventAnalog
     * axis for  longer than setIdleTimeout - irrespective of 
     * whether the idle (or changed) callback has been fired
     * 
     * @return true 
     * @return false 
     */
    bool isIdle();

    void setRateLimit(uint16_t ms);

    /**
     * If enableAutoCalibrate is set to true (the default), will
     * to auto calibrate setting of minValue and maxValue.
     * This will be done even when input is disabled (ie no callbacks fired)
     * Applied to both x and y axis.
     */
    void enableAutoCalibrate(bool allow=true);



protected:
    /**
     * Convert the EventAnalog callbacks to EventJoystic callbacks
     */
    void onInputCallback(InputEventType et, EventInputBase & ie);
    /**
     * Convert the EventAnalog X CHANGED callbacks to CHANGED_X
     */
    void onInputXCallback(InputEventType et, EventInputBase & ie);
    /**
     * Convert the EventAnalog Y CHANGED callbacks to CHANGED_Y
     */
    void onInputYCallback(InputEventType et, EventInputBase & ie);

private:


#ifndef FUNCTIONAL_SUPPORTED
private:
    static void analogXCallback(InputEventType et, EventAnalog &enc) {
        EventJoystick *instance = static_cast<EventJoystick *>(enc.getOwner());
        if (instance) instance->onInputXCallback(et, enc);
    }

    static void analogYCallback(InputEventType et, EventAnalog &enc) {
        EventJoystick *instance = static_cast<EventJoystick *>(enc.getOwner());
        if (instance) instance->onInputYCallback(et, enc);
    }
#endif


};


#endif

