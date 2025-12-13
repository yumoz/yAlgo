#pragma once

#ifdef _WIN32
  #ifdef YALGO_MATH_EXPORTS
    #define MATH_API __declspec(dllexport)
  #else
    #define MATH_API __declspec(dllimport)
  #endif
#else
  #define MATH_API
#endif
