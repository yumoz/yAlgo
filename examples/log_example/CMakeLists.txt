# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 示例程序名称和版本
set(EXAMPLE_NAME "log_example")
set(LOG_EXAMPLE_VERSION_MAJOR 1)
set(LOG_EXAMPLE_VERSION_MINOR 0)
set(LOG_EXAMPLE_VERSION_PATCH 0)
set(LOG_EXAMPLE_VERSION "${LOG_EXAMPLE_VERSION_MAJOR}.${LOG_EXAMPLE_VERSION_MINOR}.${LOG_EXAMPLE_VERSION_PATCH}")

# 设置当前日期和时间
string(TIMESTAMP LOG_EXAMPLE_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP LOG_EXAMPLE_BUILD_TIME "%H:%M:%S")

# 添加可执行文件
add_executable(${EXAMPLE_NAME} log_example.cpp)

# 包含头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# 链接日志SDK动态库
target_link_libraries(${EXAMPLE_NAME} PRIVATE
    yutils_log
)

# 设置输出目录
set_target_properties(${EXAMPLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 配置版本信息文件
if(EXISTS "${CMAKE_SOURCE_DIR}/resources/log_example_version.in")
    configure_file(
        "${CMAKE_SOURCE_DIR}/resources/log_example_version.in"
        "${CMAKE_BINARY_DIR}/etc/log_example_version.conf"
        @ONLY
    )
endif()

# 安装规则
install(TARGETS ${EXAMPLE_NAME}
    RUNTIME DESTINATION bin
)

# 安装配置文件
if(EXISTS "${CMAKE_BINARY_DIR}/etc/log_example_version.conf")
    install(FILES "${CMAKE_BINARY_DIR}/etc/log_example_version.conf"
        DESTINATION etc
    )
endif()
