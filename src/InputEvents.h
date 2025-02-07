/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_H
#define INPUT_EVENTS_H


#ifndef FUNCTIONAL_SUPPORTED
    #if defined(__has_include) // Check if __has_include is supported
        #if __has_include(<functional>)
            #define FUNCTIONAL_SUPPORTED
        #endif
    #endif
#endif

#if defined(__has_include) // Check if __has_include is supported
    #if !__has_include(<Encoder.h>)
        #ifndef EXCLUDE_EVENT_ENCODER //might be defined by build_flags
            #define EXCLUDE_EVENT_ENCODER
        #endif
    #endif
#endif


// Some platforms stupidly use #define for DISABLED (ahem, espressif), so replace it with a constexpr variable
// Many thanks to @kfoltman https://github.com/kfoltman for the workaround.
#ifdef DISABLED
constexpr auto ACTUAL_DISABLED_VALUE=DISABLED;
#undef DISABLED
constexpr auto DISABLED=ACTUAL_DISABLED_VALUE;
#endif

#include <Arduino.h>

constexpr size_t NUM_EVENT_TYPE_ENUMS = 19; 
enum class InputEventType : uint8_t {
    //Common
    ENABLED,
    DISABLED,
    IDLE,
    //Button, Encoder Button
    PRESSED,
    RELEASED,
    CLICKED,
    DOUBLE_CLICKED,
    MULTI_CLICKED,
    LONG_CLICKED,
    LONG_PRESS,
    //Encoder, Analog
    CHANGED,
    //Encoder Button (maybe joystick button?)
    CHANGED_PRESSED,
    CHANGED_RELEASED,
    //Joystick
    CHANGED_X,
    CHANGED_Y,
    //Switch
    ON,
    OFF,
    //TouchScreen
    DRAGGED,
    DRAGGED_RELEASED
};

#endif