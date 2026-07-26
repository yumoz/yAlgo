#pragma once

#ifdef _WIN32
  #ifdef YALGO_MATH_EXPORTS
    #define MATH_API __declspec(dllexport)
  #else
    #define MATH_API __declspec(dllimport)
  #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
  #define MATH_API __attribute__((visibility("default")))
#else
  #define MATH_API
#endif
