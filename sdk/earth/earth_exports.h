#pragma once

#ifdef _WIN32
  #ifdef YALGO_EARTH_EXPORTS
    #define EARTH_API __declspec(dllexport)
  #else
    #define EARTH_API __declspec(dllimport)
  #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
  #define EARTH_API __attribute__((visibility("default")))
#else
  #define EARTH_API
#endif