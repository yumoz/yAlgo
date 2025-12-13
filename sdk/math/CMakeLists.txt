# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 定义数学SDK库名称和版本
set(MATH_SDK_NAME "yalgo_math")
set(MATH_SDK_VERSION_MAJOR 1)
set(MATH_SDK_VERSION_MINOR 0)
set(MATH_SDK_VERSION_PATCH 0)
set(MATH_SDK_VERSION "${MATH_SDK_VERSION_MAJOR}.${MATH_SDK_VERSION_MINOR}.${MATH_SDK_VERSION_PATCH}")

# 设置当前日期和时间
string(TIMESTAMP MATH_SDK_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP MATH_SDK_BUILD_TIME "%H:%M:%S")

# 添加库的头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# 收集源文件
set(SOURCES
    math_utils.cpp
)

# 创建动态库
add_library(${MATH_SDK_NAME} SHARED ${SOURCES})

# 设置目标属性（版本信息）
set_target_properties(${MATH_SDK_NAME} PROPERTIES
    # 版本信息
    VERSION "${MATH_SDK_VERSION}"
    SOVERSION "${MATH_SDK_VERSION_MAJOR}"
    
    # 产品信息
    PROJECT_VERSION "${MATH_SDK_VERSION}"
    PROJECT_VERSION_MAJOR "${MATH_SDK_VERSION_MAJOR}"
    PROJECT_VERSION_MINOR "${MATH_SDK_VERSION_MINOR}"
    PROJECT_VERSION_PATCH "${MATH_SDK_VERSION_PATCH}"
)

# 使用通用安装模块
include(YuLibInstall)

# 设置头文件列表
set(MATH_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/math_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/math_exports.h
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
)

# 调用安装函数
yutils_install_lib(
    TARGET_NAME ${MATH_SDK_NAME}
    HEADER_FILES ${MATH_HEADER_FILES}
)

# Windows平台特定的版本信息设置
if(WIN32)
    # 为Windows DLL添加版本信息
    set_target_properties(${MATH_SDK_NAME} PROPERTIES
        # DLL版本信息
        VERSION "${MATH_SDK_VERSION}"
        # 文件描述
        OUTPUT_NAME "${MATH_SDK_NAME}"
        # 设置DLL资源编译器的定义
        DEFINE_SYMBOL "YALGO_MATH_EXPORTS"
    )
    
    # 创建Windows版本资源文件的内容
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/templates/math_sdk_version.rc.in"
        "${CMAKE_BINARY_DIR}/resources/${MATH_SDK_NAME}_version.rc"
        @ONLY
    )
    
    # 添加版本资源文件到库
    target_sources(${MATH_SDK_NAME} PRIVATE "${CMAKE_BINARY_DIR}/resources/${MATH_SDK_NAME}_version.rc")
    
    # 复制logo.png到构建目录，以便资源编译器能找到它
    file(COPY "${CMAKE_SOURCE_DIR}/cmake/templates/logo.png" DESTINATION "${CMAKE_BINARY_DIR}/resources/")
endif()

# 添加编译定义
target_compile_definitions(${MATH_SDK_NAME} PRIVATE YALGO_MATH_EXPORTS)

# 安装规则
install(TARGETS ${MATH_SDK_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY ${CMAKE_SOURCE_DIR}/sdk/math/
    DESTINATION include/yAlgo/math
    FILES_MATCHING PATTERN "*.h"
)
