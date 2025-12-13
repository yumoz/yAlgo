#pragma once

#ifdef _WIN32
  #ifdef YALGO_EARTH_EXPORTS
    #define EARTH_API __declspec(dllexport)
  #else
    #define EARTH_API __declspec(dllimport)
  #endif
#else
  #define EARTH_API
#endif