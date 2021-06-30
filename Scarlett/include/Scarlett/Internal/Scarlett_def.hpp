#pragma once


#ifdef _WIN32
#define __SCARLETT_USE_WIDE_STRINGS
#endif

#ifdef __SCARLETT_USE_WIDE_STRINGS
#define SCARLETT_WIDEN(x) _XPLATSTR(x)
#else
#define SCARLETT_WIDEN(x) x
#endif


#if defined(BUILD_SHARED_LIBS)
    #if defined(WIN32)
        #if defined(SCARLETT_EXPORT_LIB)
            #define SCDLL __declspec(dllexport)
        #else
            #define SDCDLL __declspec(dllimport)
        #endif
    #elif defined(__GNUC__)
        #define SCDLL __attribute__ ((__visibility__("default")))
    #endif
#else
#define SCDLL
#endif
