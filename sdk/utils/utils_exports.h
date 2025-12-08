#pragma once

#ifdef _WIN32
  #ifdef YUTILS_UTILS_EXPORTS
    #define YUTILS_UTILS_API __declspec(dllexport)
  #else
    #define YUTILS_UTILS_API __declspec(dllimport)
  #endif
#else
  #define YUTILS_UTILS_API
#endif