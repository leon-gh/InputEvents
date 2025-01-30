

#ifndef INPUT_EVENTS_DEFINES_H
#define INPUT_EVENTS_DEFINES_H


#if defined(__has_include) // Check if __has_include is supported
    #if __has_include(<functional>)
        #define FUNCTIONAL_SUPPORTED
    #endif
#endif

#if defined(__has_include) // Check if __has_include is supported
    #if !__has_include(<EncoderAdapter.h>)
        #ifndef EXCLUDE_EVENT_ENCODER //might be defined by build_flags
            #define EXCLUDE_EVENT_ENCODER
        #endif
    #endif
#endif


#endif