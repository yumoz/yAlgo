# 日志模块单元测试
add_executable(test_log test_log.cpp)
target_link_libraries(test_log PRIVATE yalgo_log)
add_test(NAME unit_log COMMAND test_log)

# 工具模块单元测试
add_executable(test_utils test_utils.cpp)
target_link_libraries(test_utils PRIVATE yalgo_utils)
add_test(NAME unit_utils COMMAND test_utils)

# 数学模块单元测试
add_executable(test_math test_math.cpp)
target_link_libraries(test_math PRIVATE yalgo_math)
add_test(NAME unit_math COMMAND test_math)

# 地球坐标模块单元测试
add_executable(test_earth test_earth.cpp)
target_link_libraries(test_earth PRIVATE yalgo_earth)
add_test(NAME unit_earth COMMAND test_earth)

# RPC模块单元测试
include(FindYalgoGrpc)

set(TEST_PROTOC_EXECUTABLE ${YALGO_PROTOC_EXECUTABLE})
set(TEST_GRPC_CPP_PLUGIN_EXECUTABLE ${YALGO_GRPC_CPP_PLUGIN})

set(RPC_TEST_PROTO ${CMAKE_CURRENT_LIST_DIR}/../../examples/rpc/echo.proto)
set(RPC_TEST_PROTO_DIR ${CMAKE_CURRENT_BINARY_DIR}/rpc_test_proto)
file(MAKE_DIRECTORY ${RPC_TEST_PROTO_DIR})

set(RPC_TEST_PROTO_SRCS ${RPC_TEST_PROTO_DIR}/echo.pb.cc)
set(RPC_TEST_PROTO_HDRS ${RPC_TEST_PROTO_DIR}/echo.pb.h)
set(RPC_TEST_GRPC_SRCS ${RPC_TEST_PROTO_DIR}/echo.grpc.pb.cc)
set(RPC_TEST_GRPC_HDRS ${RPC_TEST_PROTO_DIR}/echo.grpc.pb.h)

add_custom_command(
    OUTPUT ${RPC_TEST_PROTO_SRCS} ${RPC_TEST_PROTO_HDRS}
    COMMAND ${TEST_PROTOC_EXECUTABLE}
        --cpp_out=${RPC_TEST_PROTO_DIR}
        --proto_path=${CMAKE_CURRENT_LIST_DIR}/../../examples/rpc
        ${RPC_TEST_PROTO}
    DEPENDS ${RPC_TEST_PROTO}
    COMMENT "Generating protobuf code for rpc test"
)

add_custom_command(
    OUTPUT ${RPC_TEST_GRPC_SRCS} ${RPC_TEST_GRPC_HDRS}
    COMMAND ${TEST_PROTOC_EXECUTABLE}
        --grpc_out=${RPC_TEST_PROTO_DIR}
        --plugin=protoc-gen-grpc=${TEST_GRPC_CPP_PLUGIN_EXECUTABLE}
        --proto_path=${CMAKE_CURRENT_LIST_DIR}/../../examples/rpc
        ${RPC_TEST_PROTO}
    DEPENDS ${RPC_TEST_PROTO_SRCS} ${RPC_TEST_PROTO}
    COMMENT "Generating gRPC code for rpc test"
)

add_executable(test_rpc test_rpc.cpp
    ${RPC_TEST_PROTO_SRCS} ${RPC_TEST_PROTO_HDRS}
    ${RPC_TEST_GRPC_SRCS} ${RPC_TEST_GRPC_HDRS}
)
target_include_directories(test_rpc PRIVATE ${RPC_TEST_PROTO_DIR} ${CMAKE_SOURCE_DIR}/examples/rpc)
target_link_libraries(test_rpc PRIVATE yalgo_rpc yalgo_log ${YALGO_GRPC_TARGETS})
add_test(NAME unit_rpc COMMAND test_rpc)

message(STATUS "  Unit tests configured: log, utils, math, earth, rpc")
