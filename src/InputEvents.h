

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


#endif