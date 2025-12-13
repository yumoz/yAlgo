# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 示例程序名称和版本
set(EXAMPLE_NAME "math_example")
set(MATH_EXAMPLE_VERSION_MAJOR 1)
set(MATH_EXAMPLE_VERSION_MINOR 0)
set(MATH_EXAMPLE_VERSION_PATCH 0)
set(MATH_EXAMPLE_VERSION "${MATH_EXAMPLE_VERSION_MAJOR}.${MATH_EXAMPLE_VERSION_MINOR}.${MATH_EXAMPLE_VERSION_PATCH}")

# 设置当前日期和时间
string(TIMESTAMP MATH_EXAMPLE_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP MATH_EXAMPLE_BUILD_TIME "%H:%M:%S")

# 添加可执行文件
add_executable(${EXAMPLE_NAME} 
    math_test.cpp
    math_example.cpp
)

# 包含头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# 链接SDK动态库
target_link_libraries(${EXAMPLE_NAME} PRIVATE
    yalgo_math
)

# 使用通用安装模块
include(YuLibInstall)

# 定义配置文件列表
set(EXAMPLE_CONFIG_FILES "")

# 调用应用程序安装函数
yutils_install_app(
    TARGET_NAME ${EXAMPLE_NAME}
    CONFIG_FILES ${EXAMPLE_CONFIG_FILES}
)
