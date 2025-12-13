# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 示例程序名称和版本
set(EXAMPLE_NAME "utils_example")
set(UTILS_EXAMPLE_VERSION_MAJOR 1)
set(UTILS_EXAMPLE_VERSION_MINOR 0)
set(UTILS_EXAMPLE_VERSION_PATCH 0)
set(UTILS_EXAMPLE_VERSION "${UTILS_EXAMPLE_VERSION_MAJOR}.${UTILS_EXAMPLE_VERSION_MINOR}.${UTILS_EXAMPLE_VERSION_PATCH}")

# 设置当前日期和时间
string(TIMESTAMP UTILS_EXAMPLE_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP UTILS_EXAMPLE_BUILD_TIME "%H:%M:%S")

# 添加可执行文件
add_executable(${EXAMPLE_NAME} 
    utils_test.cpp
)

# 包含头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# 链接SDK动态库
target_link_libraries(${EXAMPLE_NAME} PRIVATE
    yalgo_utils
)

# 设置输出目录
set_target_properties(${EXAMPLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 配置版本信息文件
if(EXISTS "${CMAKE_SOURCE_DIR}/resources/utils_example_version.in")
    configure_file(
        "${CMAKE_SOURCE_DIR}/resources/utils_example_version.in"
        "${CMAKE_BINARY_DIR}/etc/utils_example_version.conf"
        @ONLY
    )
endif()

# 使用通用安装模块
include(YuLibInstall)

# 定义配置文件列表
set(EXAMPLE_CONFIG_FILES "")
if(EXISTS "${CMAKE_BINARY_DIR}/etc/utils_example_version.conf")
    set(EXAMPLE_CONFIG_FILES "${CMAKE_BINARY_DIR}/etc/utils_example_version.conf")
endif()

# 调用应用程序安装函数
yutils_install_app(
    TARGET_NAME ${EXAMPLE_NAME}
    CONFIG_FILES ${EXAMPLE_CONFIG_FILES}
)