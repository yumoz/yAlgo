# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 定义日志SDK库名称和版本
set(LOG_SDK_NAME "yutils_log")
set(YUTILS_LOG_VERSION_MAJOR 1)
set(YUTILS_LOG_VERSION_MINOR 0)
set(YUTILS_LOG_VERSION_PATCH 0)
set(YUTILS_LOG_VERSION "${YUTILS_LOG_VERSION_MAJOR}.${YUTILS_LOG_VERSION_MINOR}.${YUTILS_LOG_VERSION_PATCH}")

# 设置当前日期和时间
string(TIMESTAMP YUTILS_LOG_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP YUTILS_LOG_BUILD_TIME "%H:%M:%S")

# 添加库的头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# sdk目录作为父目录，不再创建重复的库
# 库的实际实现在sdk/log和sdk/utils子目录中
# 此目录主要用于提供统一的头文件路径和依赖管理

# 设置资源目录，确保版本资源文件生成路径存在
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resources)

# 设置输出目录变量供子目录使用
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE PATH "Archive output directory")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE PATH "Library output directory")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Runtime output directory")

# 设置目标属性（版本信息）
set_target_properties(${LOG_SDK_NAME} PROPERTIES
    VERSION "${YUTILS_LOG_VERSION}"
    SOVERSION "${YUTILS_LOG_VERSION_MAJOR}"
    
    # 产品信息
    PROJECT_VERSION "${YUTILS_LOG_VERSION}"
    PROJECT_VERSION_MAJOR "${YUTILS_LOG_VERSION_MAJOR}"
    PROJECT_VERSION_MINOR "${YUTILS_LOG_VERSION_MINOR}"
    PROJECT_VERSION_PATCH "${YUTILS_LOG_VERSION_PATCH}"
)

# Windows平台特定的版本信息设置
if(WIN32)
    # 为Windows DLL添加版本信息
    set_target_properties(${LOG_SDK_NAME} PROPERTIES
        # DLL版本信息
        VERSION "${YUTILS_LOG_VERSION}"
        # 文件描述
        OUTPUT_NAME "${LOG_SDK_NAME}"
        # 设置DLL资源编译器的定义
        DEFINE_SYMBOL "YUTILS_LOG_EXPORTS"
    )
endif()

# 配置版本信息文件（供子目录使用的通用配置）
if(EXISTS "${CMAKE_SOURCE_DIR}/resources/yutils_log_version.in")
    configure_file(
        "${CMAKE_SOURCE_DIR}/resources/yutils_log_version.in"
        "${CMAKE_BINARY_DIR}/etc/yutils_log_version.conf"
        @ONLY
    )
endif()

# sdk目录不再直接安装目标，由子目录负责各自的安装规则

# 安装头文件（仅作为备份路径，主要安装在子目录中处理）
if(EXISTS "${CMAKE_SOURCE_DIR}/include/yutils/log/")
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/yutils/log/
        DESTINATION include/yutils/log
        FILES_MATCHING PATTERN "*.h"
        OPTIONAL
    )
endif()

# 安装配置文件
if(EXISTS "${CMAKE_BINARY_DIR}/etc/yutils_log_version.conf")
    install(FILES "${CMAKE_BINARY_DIR}/etc/yutils_log_version.conf"
        DESTINATION etc
        OPTIONAL
    )
endif()
