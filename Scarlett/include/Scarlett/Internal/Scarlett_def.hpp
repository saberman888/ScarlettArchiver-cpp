#pragma once


#ifdef _WIN32
#define __SCARLETT_USE_WIDE_STRINGS
#endif

#ifdef __SCARLETT_USE_WIDE_STRINGS
#define SCARLETT_WIDEN(x) _XPLATSTR(x)
#else
#define SCARLETT_WIDEN(x) x
#endif

#ifdef SCARLETT_STATIC_LIB
#define SCDLL
#elif defined(SCARLETT_EXPORT_LIB)
#if defined(WIN32)
#define SCDLL __declspec(dllexport)
#else
#define SCDLL __attribute__ ((__visibility__("default")))
#endif
#else
#define SCDLL __declspec(dllimport)
#endif
