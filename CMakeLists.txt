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

# 确保输出目录存在
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# 设置输出目录 - 统一放在项目根目录下的bin和lib文件夹
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

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
add_subdirectory(examples/log)
add_subdirectory(examples/utils)

# 创建复制文件到根目录的自定义目标
add_custom_target(copy_binaries ALL
    DEPENDS yalgo_log yalgo_utils log_example utils_example
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/bin" "${CMAKE_SOURCE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/lib" "${CMAKE_SOURCE_DIR}/lib"
    COMMENT "复制生成的文件到项目根目录的bin和lib文件夹"
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