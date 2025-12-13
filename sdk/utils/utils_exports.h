#pragma once

#ifdef _WIN32
  #ifdef YALGO_UTILS_EXPORTS
    #define YALGO_UTILS_API __declspec(dllexport)
  #else
    #define YALGO_UTILS_API __declspec(dllimport)
  #endif
#else
  #define YALGO_UTILS_API
#endif