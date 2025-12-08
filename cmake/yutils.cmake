# yUtils CMake 主入口模块
# 一次性包含所有 yUtils CMake 功能

# 检查必要的变量
if(NOT DEFINED SRC_ROOT_DIR)
    set(SRC_ROOT_DIR "${CMAKE_SOURCE_DIR}")
endif()

# 定义版本信息
set(YUTILS_CMAKE_VERSION "1.0.0")
message(STATUS "yUtils CMake 模块 v${YUTILS_CMAKE_VERSION}")

# 包含所有工具模块
include(${CMAKE_CURRENT_LIST_DIR}/YuPath.cmake)

# 定义模块导入函数
function(yutils_add_library target_name)
    set(TARGET ${target_name})
    include(${CMAKE_CURRENT_LIST_DIR}/YuMakeSDK.cmake)
endfunction()

function(yutils_add_executable target_name)
    set(TARGET ${target_name})
    include(${CMAKE_CURRENT_LIST_DIR}/YuMakeApp.cmake)
endfunction()

# 定义构建类型检查函数
function(check_build_type)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
    endif()
    message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
endfunction()

# 定义平台检查函数
function(print_platform_info)
    if(WIN32)
        message(STATUS "平台: Windows")
    elseif(UNIX AND NOT APPLE)
        message(STATUS "平台: Linux")
    elseif(APPLE)
        message(STATUS "平台: macOS")
    else()
        message(STATUS "平台: 未知")
    endif()
    
    if(MSVC)
        message(STATUS "编译器: MSVC ${MSVC_VERSION}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "编译器: GCC ${CMAKE_CXX_COMPILER_VERSION}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "编译器: Clang ${CMAKE_CXX_COMPILER_VERSION}")
    else()
        message(STATUS "编译器: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
endfunction()

# 默认调用一些工具函数
check_build_type()
print_platform_info()
