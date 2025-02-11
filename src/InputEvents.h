/** 
 * @file
 * @brief Contains InputEventType enums and some defines.
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_H
#define INPUT_EVENTS_H

#include <Arduino.h>

#ifndef FUNCTIONAL_SUPPORTED
    #if defined(__has_include) // Check if __has_include is supported
        #if __has_include(<functional>)
            #define FUNCTIONAL_SUPPORTED
        #endif
    #endif
#endif

#if defined(__has_include) // Check if __has_include is supported
    #if !__has_include(<EncoderAdapter.h>)
        #ifndef EXCLUDE_EVENT_ENCODER //might be defined by build_flags
            #define EXCLUDE_EVENT_ENCODER
        #endif
    #endif
#endif


/// \cond DO_NOT_DOCUMENT
/*
 * Some platforms stupidly use #define for DISABLED (ahem, espressif), so replace it with a constexpr variable
 * Many thanks to @kfoltman https://github.com/kfoltman for the workaround.
 */
#ifdef DISABLED
constexpr auto ACTUAL_DISABLED_VALUE=DISABLED;
#undef DISABLED
constexpr auto DISABLED=ACTUAL_DISABLED_VALUE;
#endif
/// \endcond


/**
 * @brief The size of the InputEventType enum
 * 
 */
constexpr size_t NUM_EVENT_TYPE_ENUMS = 19;

/**
 * @brief A list of all events that can be fired by InputEvents classes.
 */
enum class InputEventType : uint8_t {
    ENABLED,            ///< Fired by all imputs
    DISABLED,           ///< Fired by all imputs
    IDLE,               ///< Fired by all imputs
    PRESSED,            ///< Fired by EventButton, EventEncoderButton and EventTouchScreen
    RELEASED,           ///< Fired by EventButton, EventEncoderButton and EventTouchScreen
    CLICKED,            ///< Fired by EventButton, EventEncoderButton and EventTouchScreen
    DOUBLE_CLICKED,     ///< Fired by EventButton, EventEncoderButton and EventTouchScreen
    MULTI_CLICKED,      ///< Fired by EventButton, EventEncoderButton and EventTouchScreen
    LONG_CLICKED,       ///< Fired by EventButton, EventEncoderButton and EventTouchScreen
    LONG_PRESS,         ///< Fired by EventButton, EventEncoderButton and EventTouchScreen
    CHANGED,            ///< Fired by EventEncoder and EventAnalog
    CHANGED_PRESSED,    ///< Fired by EventEncoderButton
    CHANGED_RELEASED,   ///< Fired by EventEncoderButton
    CHANGED_X,          ///< Fired Bby EventJoystick
    CHANGED_Y,          ///< Fired Bby EventJoystick
    ON,                 ///< Fired by EventSwitch
    OFF,                ///< Fired by EventSwitch
    DRAGGED,            ///< Fired by [EventTouchScreen]() (experimental)
    DRAGGED_RELEASED    ///< Fired by EventTouchScreen (experimental)
};

#endif