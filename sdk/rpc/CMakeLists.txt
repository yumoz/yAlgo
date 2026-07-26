# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 定义RPC SDK库名称和版本
set(RPC_SDK_NAME "yalgo_rpc")
set(RPC_SDK_VERSION_MAJOR 1)
set(RPC_SDK_VERSION_MINOR 0)
set(RPC_SDK_VERSION_PATCH 0)
set(RPC_SDK_VERSION "${RPC_SDK_VERSION_MAJOR}.${RPC_SDK_VERSION_MINOR}.${RPC_SDK_VERSION_PATCH}")

# 设置当前日期和时间
string(TIMESTAMP RPC_SDK_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP RPC_SDK_BUILD_TIME "%H:%M:%S")

# 添加库的头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# --- gRPC/Protobuf 依赖（统一查找模块）---
include(FindYalgoGrpc)

# 收集源文件
set(SOURCES
    grpc_server.cpp
    grpc_client.cpp
    rpc_utils.cpp
)

# 创建动态库
add_library(${RPC_SDK_NAME} SHARED ${SOURCES})

# 链接依赖
target_link_libraries(${RPC_SDK_NAME}
    PRIVATE
        yalgo_log
        yalgo_utils
        ${YALGO_GRPC_TARGETS}
)

# 设置目标属性（版本信息）
set_target_properties(${RPC_SDK_NAME} PROPERTIES
    # 版本信息
    VERSION "${RPC_SDK_VERSION}"
    SOVERSION "${RPC_SDK_VERSION_MAJOR}"
    
    # 产品信息
    PROJECT_VERSION "${RPC_SDK_VERSION}"
    PROJECT_VERSION_MAJOR "${RPC_SDK_VERSION_MAJOR}"
    PROJECT_VERSION_MINOR "${RPC_SDK_VERSION_MINOR}"
    PROJECT_VERSION_PATCH "${RPC_SDK_VERSION_PATCH}"
)

# 使用通用安装模块
include(YuLibInstall)

# 设置头文件列表
set(RPC_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/grpc_server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/grpc_client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rpc_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rpc_exports.h
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
)

# 调用安装函数
yutils_install_lib(
    TARGET_NAME ${RPC_SDK_NAME}
    HEADER_FILES ${RPC_HEADER_FILES}
)

# Windows平台特定的版本信息设置
if(WIN32)
    # 为Windows DLL添加版本信息
    set_target_properties(${RPC_SDK_NAME} PROPERTIES
        # DLL版本信息
        VERSION "${RPC_SDK_VERSION}"
        # 文件描述
        OUTPUT_NAME "${RPC_SDK_NAME}"
        # 设置DLL资源编译器的定义
        DEFINE_SYMBOL "YALGO_RPC_EXPORTS"
    )
    
    # 设置通用版本资源文件变量
    set(PROJECT_VERSION_MAJOR ${RPC_SDK_VERSION_MAJOR})
    set(PROJECT_VERSION_MINOR ${RPC_SDK_VERSION_MINOR})
    set(PROJECT_VERSION_PATCH ${RPC_SDK_VERSION_PATCH})
    set(PROJECT_VERSION ${RPC_SDK_VERSION})
    set(PROJECT_NAME ${RPC_SDK_NAME})
    set(PROJECT_DESCRIPTION "yAlgo RPC SDK Library")
    set(PROJECT_COMPANY "yAlgo Project")
    set(PROJECT_COPYRIGHT "Copyright (C) ${RPC_SDK_BUILD_DATE}")
    set(BUILD_DATE ${RPC_SDK_BUILD_DATE})
    set(BUILD_TIME ${RPC_SDK_BUILD_TIME})
    
    # 创建Windows版本资源文件的内容
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/templates/version.rc.in"
        "${CMAKE_BINARY_DIR}/resources/${RPC_SDK_NAME}_version.rc"
        @ONLY
    )
    
    # 添加版本资源文件到库
    target_sources(${RPC_SDK_NAME} PRIVATE "${CMAKE_BINARY_DIR}/resources/${RPC_SDK_NAME}_version.rc")
    
    # 复制logo.png到构建目录，以便资源编译器能找到它
    file(COPY "${CMAKE_SOURCE_DIR}/resources/logo.png" DESTINATION "${CMAKE_BINARY_DIR}/resources/")
endif()

# 添加编译定义
target_compile_definitions(${RPC_SDK_NAME} PRIVATE YALGO_RPC_EXPORTS)

# 安装规则
install(TARGETS ${RPC_SDK_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY ${CMAKE_SOURCE_DIR}/sdk/rpc/
    DESTINATION include/yAlgo/rpc
    FILES_MATCHING PATTERN "*.h"
)
