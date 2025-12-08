# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 定义工具SDK库名称和版本
set(UTILS_SDK_NAME "yutils_utils")
set(YUTILS_UTILS_VERSION_MAJOR 1)
set(YUTILS_UTILS_VERSION_MINOR 0)
set(YUTILS_UTILS_VERSION_PATCH 0)
set(YUTILS_UTILS_VERSION "${YUTILS_UTILS_VERSION_MAJOR}.${YUTILS_UTILS_VERSION_MINOR}.${YUTILS_UTILS_VERSION_PATCH}")

# 设置当前日期和时间
string(TIMESTAMP YUTILS_UTILS_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP YUTILS_UTILS_BUILD_TIME "%H:%M:%S")

# 添加库的头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# 收集源文件
set(SOURCES
    exec_path_utils.cpp
)

# 创建动态库
add_library(${UTILS_SDK_NAME} SHARED ${SOURCES})

# 设置目标属性（输出目录和版本信息）
set_target_properties(${UTILS_SDK_NAME} PROPERTIES
    # 输出目录设置
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    
    # 版本信息
    VERSION "${YUTILS_UTILS_VERSION}"
    SOVERSION "${YUTILS_UTILS_VERSION_MAJOR}"
    
    # 产品信息
    PROJECT_VERSION "${YUTILS_UTILS_VERSION}"
    PROJECT_VERSION_MAJOR "${YUTILS_UTILS_VERSION_MAJOR}"
    PROJECT_VERSION_MINOR "${YUTILS_UTILS_VERSION_MINOR}"
    PROJECT_VERSION_PATCH "${YUTILS_UTILS_VERSION_PATCH}"
)

# Windows平台特定的版本信息设置
if(WIN32)
    # 为Windows DLL添加版本信息
    set_target_properties(${UTILS_SDK_NAME} PROPERTIES
        # DLL版本信息
        VERSION "${YUTILS_UTILS_VERSION}"
        # 文件描述
        OUTPUT_NAME "${UTILS_SDK_NAME}"
        # 设置DLL资源编译器的定义
        DEFINE_SYMBOL "YUTILS_UTILS_EXPORTS"
    )
    
    # 创建Windows版本资源文件的内容
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/templates/version.rc.in"
        "${CMAKE_BINARY_DIR}/resources/${UTILS_SDK_NAME}_version.rc"
        @ONLY
    )
    
    # 添加版本资源文件到库
    target_sources(${UTILS_SDK_NAME} PRIVATE "${CMAKE_BINARY_DIR}/resources/${UTILS_SDK_NAME}_version.rc")
endif()

# 添加编译定义
target_compile_definitions(${UTILS_SDK_NAME} PRIVATE YUTILS_UTILS_EXPORTS)

# 安装规则
install(TARGETS ${UTILS_SDK_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY ${CMAKE_SOURCE_DIR}/sdk/utils/
    DESTINATION include/yutils/utils
    FILES_MATCHING PATTERN "*.h"
)
