#pragma once

#ifdef _WIN32
  #ifdef YALGO_RPC_EXPORTS
    #define RPC_API __declspec(dllexport)
  #else
    #define RPC_API __declspec(dllimport)
  #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
  #define RPC_API __attribute__((visibility("default")))
#else
  #define RPC_API
#endif
