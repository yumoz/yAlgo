# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目版本策略
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()

# 项目名称和版本
project(yAlgo VERSION 1.0.0 LANGUAGES CXX)

# 设置安装前缀为sdk同级目录的install文件夹
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "安装目录" FORCE)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# yUtils 项目不依赖Qt，使用纯C++实现

# 根据操作系统和构建类型设置输出目录
set(OS_NAME "unknown")
if(WIN32)
    set(OS_NAME "windows")
elseif(APPLE)
    set(OS_NAME "macos")
elseif(UNIX)
    set(OS_NAME "linux")
endif()

# 获取构建类型，如果没有设置默认使用Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "构建类型" FORCE)
endif()
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_LOWER)

# 设置输出目录 - 按照操作系统和构建类型区分
set(OUTPUT_BIN_DIR "${CMAKE_SOURCE_DIR}/bin/${OS_NAME}/${BUILD_TYPE_LOWER}")
set(OUTPUT_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/${OS_NAME}/${BUILD_TYPE_LOWER}")

# 确保输出目录存在
file(MAKE_DIRECTORY ${OUTPUT_BIN_DIR})
file(MAKE_DIRECTORY ${OUTPUT_LIB_DIR})

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_LIB_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_LIB_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BIN_DIR}")

# 包含头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# 添加自定义cmake模块
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    # 暂时注释掉，因为相关模块可能不存在
    # include(YuMakeApp)
    # 包含安装模块但不执行任何操作，确保路径设置正确
    include(YuLibInstall OPTIONAL)
endif()

# 添加子目录
add_subdirectory(sdk/log)
add_subdirectory(sdk/utils)
add_subdirectory(sdk/math)
add_subdirectory(sdk/earth)
add_subdirectory(examples/log)
add_subdirectory(examples/utils)
add_subdirectory(examples/math)
add_subdirectory(examples/earth)

# 创建复制文件的自定义目标已不再需要，因为文件已直接生成在最终位置
# 保留此目标但不执行任何操作，以保持向后兼容性
add_custom_target(copy_binaries ALL
    DEPENDS yalgo_log yalgo_utils yalgo_math yalgo_earth log_example utils_example math_example earth_example
    COMMENT "文件已直接生成在最终输出目录，无需复制"
)

# 项目清理目标
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_SOURCE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_SOURCE_DIR}/lib"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_SOURCE_DIR}/install"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}"
    COMMENT "清理所有生成的文件和目录"
)

# 注意：各个模块的安装规则已由YuLibInstall.cmake模块处理
# 此处不再需要全局的头文件安装规则，避免重复安装