#pragma once

#ifdef _WIN32
    #ifdef _WIN64
        #define NICKEL2_PLATFORM_WINDOWS
    #else
        #error "x86 is not supported."
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported."
    #elif TARGET_OS_IPHONE == 1
        #define NICKEL2_PLATFORM_IOS
        #error "IOS is not supported."
    #elif TARGET_OS_MAC == 1
        #define NICKEL2_PLATFORM_MACOS
        #error "macOS is not supported."
    #else
        #error "unknown apple platform."
    #endif
#elif defined(__ANDROID__)
    #define NICKEL2_PLATFORM_ANDROID
    #error "android is not supported."
#elif defined(__linux__)
    #define NICKEL2_PLATFORM_LINUX
    #error "linux is not supported."
#else
    #error "unknown platform."
#endif