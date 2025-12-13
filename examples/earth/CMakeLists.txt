# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 示例程序名称和版本
set(EXAMPLE_NAME "earth_example")
set(EARTH_EXAMPLE_VERSION_MAJOR 1)
set(EARTH_EXAMPLE_VERSION_MINOR 0)
set(EARTH_EXAMPLE_VERSION_PATCH 0)
set(EARTH_EXAMPLE_VERSION "${EARTH_EXAMPLE_VERSION_MAJOR}.${EARTH_EXAMPLE_VERSION_MINOR}.${EARTH_EXAMPLE_VERSION_PATCH}")

# 设置当前日期和时间
string(TIMESTAMP EARTH_EXAMPLE_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP EARTH_EXAMPLE_BUILD_TIME "%H:%M:%S")

# 添加可执行文件
add_executable(${EXAMPLE_NAME} 
    earth_test.cpp
    earth_example.cpp
)

# 包含头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# 链接SDK动态库
target_link_libraries(${EXAMPLE_NAME} PRIVATE
    yalgo_earth
)

# 使用通用安装模块
include(YuLibInstall)

# 定义配置文件列表
set(EXAMPLE_CONFIG_FILES "")

# Windows平台特定的版本信息设置
if(WIN32)
    # 为Windows可执行文件添加版本信息和图标
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/templates/earth_app_version.rc.in"
        "${CMAKE_BINARY_DIR}/resources/${EXAMPLE_NAME}_version.rc"
        @ONLY
    )
    
    # 添加版本资源文件到可执行文件
    target_sources(${EXAMPLE_NAME} PRIVATE "${CMAKE_BINARY_DIR}/resources/${EXAMPLE_NAME}_version.rc")
    
    # 复制logo.png到构建目录，以便资源编译器能找到它
    file(COPY "${CMAKE_SOURCE_DIR}/cmake/templates/logo.png" DESTINATION "${CMAKE_BINARY_DIR}/resources/")
endif()

# 调用应用程序安装函数
yutils_install_app(
    TARGET_NAME ${EXAMPLE_NAME}
    CONFIG_FILES ${EXAMPLE_CONFIG_FILES}
)
