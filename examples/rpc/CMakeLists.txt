# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 示例程序名称
set(EXAMPLE_NAME "rpc_example")

# 添加库的头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/sdk)

# --- gRPC/Protobuf 依赖（统一查找模块）---
include(FindYalgoGrpc)

# --- Proto编译（手动调用protoc + grpc_cpp_plugin）---
set(PROTO_FILE echo.proto)
get_filename_component(PROTO_ABS ${PROTO_FILE} ABSOLUTE)

set(PROTO_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(PROTO_SRCS ${PROTO_OUT_DIR}/echo.pb.cc)
set(PROTO_HDRS ${PROTO_OUT_DIR}/echo.pb.h)
set(GRPC_SRCS ${PROTO_OUT_DIR}/echo.grpc.pb.cc)
set(GRPC_HDRS ${PROTO_OUT_DIR}/echo.grpc.pb.h)

# 使用 FindYalgoGrpc 提供的工具路径
set(PROTOC_EXECUTABLE ${YALGO_PROTOC_EXECUTABLE})
set(GRPC_CPP_PLUGIN_EXECUTABLE ${YALGO_GRPC_CPP_PLUGIN})

# 生成protobuf代码
add_custom_command(
    OUTPUT ${PROTO_SRCS} ${PROTO_HDRS}
    COMMAND ${PROTOC_EXECUTABLE}
        --cpp_out=${PROTO_OUT_DIR}
        --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
        ${PROTO_ABS}
    DEPENDS ${PROTO_ABS}
    COMMENT "Generating protobuf code from ${PROTO_FILE}"
)

# 生成gRPC代码
add_custom_command(
    OUTPUT ${GRPC_SRCS} ${GRPC_HDRS}
    COMMAND ${PROTOC_EXECUTABLE}
        --grpc_out=${PROTO_OUT_DIR}
        --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE}
        --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
        ${PROTO_ABS}
    DEPENDS ${PROTO_SRCS} ${PROTO_ABS}
    COMMENT "Generating gRPC code from ${PROTO_FILE}"
)

# 添加可执行文件
add_executable(${EXAMPLE_NAME}
    rpc_example.cpp
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    ${GRPC_SRCS}
    ${GRPC_HDRS}
)

# 包含生成的proto头文件目录
target_include_directories(${EXAMPLE_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# 链接依赖
target_link_libraries(${EXAMPLE_NAME} PRIVATE
    yalgo_rpc
    yalgo_log
    ${YALGO_GRPC_TARGETS}
)

# --- 独立服务端 ---
add_executable(rpc_server
    rpc_server.cpp
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    ${GRPC_SRCS}
    ${GRPC_HDRS}
)
target_include_directories(rpc_server PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(rpc_server PRIVATE
    yalgo_rpc
    yalgo_log
    ${YALGO_GRPC_TARGETS}
)

# --- 独立客户端 ---
add_executable(rpc_client
    rpc_client.cpp
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    ${GRPC_SRCS}
    ${GRPC_HDRS}
)
target_include_directories(rpc_client PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(rpc_client PRIVATE
    yalgo_rpc
    yalgo_log
    ${YALGO_GRPC_TARGETS}
)

# Windows平台特定的版本信息设置
if(WIN32)
    set(EXAMPLE_VERSION_MAJOR 1)
    set(EXAMPLE_VERSION_MINOR 0)
    set(EXAMPLE_VERSION_PATCH 0)
    set(EXAMPLE_VERSION "${EXAMPLE_VERSION_MAJOR}.${EXAMPLE_VERSION_MINOR}.${EXAMPLE_VERSION_PATCH}")
    
    string(TIMESTAMP EXAMPLE_BUILD_DATE "%Y-%m-%d")
    string(TIMESTAMP EXAMPLE_BUILD_TIME "%H:%M:%S")
    
    set(PROJECT_VERSION_MAJOR ${EXAMPLE_VERSION_MAJOR})
    set(PROJECT_VERSION_MINOR ${EXAMPLE_VERSION_MINOR})
    set(PROJECT_VERSION_PATCH ${EXAMPLE_VERSION_PATCH})
    set(PROJECT_VERSION ${EXAMPLE_VERSION})
    set(PROJECT_NAME ${EXAMPLE_NAME})
    set(PROJECT_DESCRIPTION "yAlgo RPC Example Application")
    set(PROJECT_COMPANY "yAlgo Project")
    set(PROJECT_COPYRIGHT "Copyright (C) ${EXAMPLE_BUILD_DATE}")
    set(BUILD_DATE ${EXAMPLE_BUILD_DATE})
    set(BUILD_TIME ${EXAMPLE_BUILD_TIME})
    
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/templates/version_app.rc.in"
        "${CMAKE_BINARY_DIR}/resources/${EXAMPLE_NAME}_version.rc"
        @ONLY
    )
    
    target_sources(${EXAMPLE_NAME} PRIVATE "${CMAKE_BINARY_DIR}/resources/${EXAMPLE_NAME}_version.rc")
    file(COPY "${CMAKE_SOURCE_DIR}/resources/logo.png" DESTINATION "${CMAKE_BINARY_DIR}/resources/")
endif()
