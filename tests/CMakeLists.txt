cmake_minimum_required(VERSION 3.10)
project(yAlgoTests LANGUAGES CXX)

# 启用测试
enable_testing()

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含 SDK 头文件路径
include_directories(${CMAKE_SOURCE_DIR}/../sdk)
include_directories(${CMAKE_SOURCE_DIR}/../sdk/log)
include_directories(${CMAKE_SOURCE_DIR}/../sdk/utils)
include_directories(${CMAKE_SOURCE_DIR}/../sdk/math)
include_directories(${CMAKE_SOURCE_DIR}/../sdk/earth)
include_directories(${CMAKE_SOURCE_DIR}/../sdk/rpc)

# 添加子目录
add_subdirectory(unit)
add_subdirectory(integration)

# 输出测试信息
message(STATUS "yAlgo Tests configured successfully")
message(STATUS "  - Unit tests: enabled")
message(STATUS "  - Integration tests: enabled")
