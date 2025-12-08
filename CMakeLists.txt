# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目版本策略
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()

# 项目名称和版本
project(yUtils VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# yUtils 项目不依赖Qt，使用纯C++实现

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# 包含头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# 添加自定义cmake模块
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    # 暂时注释掉，因为相关模块可能不存在
    # include(YuMakeApp)
endif()

# 添加子目录
add_subdirectory(sdk/log)
add_subdirectory(sdk/utils)
add_subdirectory(examples/log_example)

# 查找Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # 配置Doxygen
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_IN ${CMAKE_CURRENT_BINARY_DIR}/CMakeDoxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # 配置Doxyfile
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # 创建文档目标
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "正在生成Doxygen文档..."
        VERBATIM
    )
endif(DOXYGEN_FOUND)