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
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# 收集源文件
set(SOURCES
    async_logger.cpp
)

# 创建动态库
add_library(${LOG_SDK_NAME} SHARED ${SOURCES})

# 设置目标属性（输出目录和版本信息）
set_target_properties(${LOG_SDK_NAME} PROPERTIES
    # 输出目录设置
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    
    # 版本信息
    VERSION "${YUTILS_LOG_VERSION}"
    SOVERSION "${YUTILS_LOG_VERSION_MAJOR}"
    
    # 产品信息
    PROJECT_VERSION "${YUTILS_LOG_VERSION}"
    PROJECT_VERSION_MAJOR "${YUTILS_LOG_VERSION_MAJOR}"
    PROJECT_VERSION_MINOR "${YUTILS_LOG_VERSION_MINOR}"
    PROJECT_VERSION_PATCH "${YUTILS_LOG_VERSION_PATCH}"
)

# 使用通用安装模块
include(YuLibInstall)

# 设置头文件列表
set(LOG_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/async_logger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/log_exports.h
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
)

# 调用安装函数
yutils_install_lib(
    TARGET_NAME ${LOG_SDK_NAME}
    HEADER_FILES ${LOG_HEADER_FILES}
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
    
    # 创建Windows版本资源文件的内容
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/templates/log_version.rc.in"
        "${CMAKE_BINARY_DIR}/resources/${LOG_SDK_NAME}_version.rc"
        @ONLY
    )
    
    # 添加版本资源文件到库
    target_sources(${LOG_SDK_NAME} PRIVATE "${CMAKE_BINARY_DIR}/resources/${LOG_SDK_NAME}_version.rc")
endif()

# 添加编译定义
target_compile_definitions(${LOG_SDK_NAME} PRIVATE YUTILS_LOG_EXPORTS)

# 安装规则
install(TARGETS ${LOG_SDK_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY ${CMAKE_SOURCE_DIR}/sdk/log/
    DESTINATION include/yutils/log
    FILES_MATCHING PATTERN "*.h"
)
