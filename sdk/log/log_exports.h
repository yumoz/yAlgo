#pragma once

#ifdef _WIN32
  #ifdef YUTILS_LOG_EXPORTS
    #define LOG_API __declspec(dllexport)
  #else
    #define LOG_API __declspec(dllimport)
  #endif
#else
  #define LOG_API
#endif
