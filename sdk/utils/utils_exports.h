#pragma once

#ifdef _WIN32
  #ifdef YALGO_UTILS_EXPORTS
    #define YALGO_UTILS_API __declspec(dllexport)
  #else
    #define YALGO_UTILS_API __declspec(dllimport)
  #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
  #define YALGO_UTILS_API __attribute__((visibility("default")))
#else
  #define YALGO_UTILS_API
#endif