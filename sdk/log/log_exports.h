#pragma once

#ifdef _WIN32
  #ifdef YALGO_LOG_EXPORTS
    #define LOG_API __declspec(dllexport)
  #else
    #define LOG_API __declspec(dllimport)
  #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
  #define LOG_API __attribute__((visibility("default")))
#else
  #define LOG_API
#endif
