# FindYalgoGrpc.cmake — 统一查找 gRPC/Protobuf 依赖
#
# 提供的变量：
#   YALGO_GRPC_FOUND        - 是否成功找到依赖
#   YALGO_GRPC_TARGETS      - 可链接的 target 列表
#   YALGO_PROTOC_EXECUTABLE - protoc 可执行文件路径
#   YALGO_GRPC_CPP_PLUGIN   - grpc_cpp_plugin 可执行文件路径
#
# 提供的宏：
#   yalgo_fix_macos_includes() - 修复 macOS 上 Homebrew 头文件与系统 libc++ 冲突
#
# 搜索策略：
#   1. 检查 CMake 变量/环境变量指定的路径 (GRPC_ROOT / PROTOBUF_ROOT)
#   2. 尝试 find_package(Protobuf) + find_package(gRPC) 查找系统包
#   3. 系统包找不到时，使用 FetchContent 从 GitHub 下载
#
# 使用示例：
#   include(FindYalgoGrpc)
#   target_link_libraries(my_target PRIVATE ${YALGO_GRPC_TARGETS})

# 防止重复包含
if(_FIND_YALGO_GRPC_INCLUDED)
    return()
endif()
set(_FIND_YALGO_GRPC_INCLUDED TRUE)

include(CMakePrintHelpers)

# ============================================================
# macOS 头文件路径修复宏
# ============================================================
macro(yalgo_fix_macos_includes)
    if(APPLE AND NOT _YALGO_MACOS_INCLUDE_FIXED)
        set(_YALGO_MACOS_INCLUDE_FIXED TRUE)
        execute_process(
            COMMAND xcrun --show-sdk-path
            OUTPUT_VARIABLE _YALGO_MACOS_SDK_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        set(_YALGO_SYSTEM_CXX_INCLUDE "${_YALGO_MACOS_SDK_PATH}/usr/include/c++/v1")
        if(EXISTS "${_YALGO_SYSTEM_CXX_INCLUDE}")
            include_directories(BEFORE SYSTEM "${_YALGO_SYSTEM_CXX_INCLUDE}")
        endif()
    endif()
endmacro()

# ============================================================
# Step 1: 尝试查找系统包
# ============================================================
set(YALGO_GRPC_FOUND FALSE)
set(YALGO_GRPC_TARGETS "")
set(YALGO_PROTOC_EXECUTABLE "")
set(YALGO_GRPC_CPP_PLUGIN "")

# 允许用户通过 CMake 变量或环境变量指定路径
if(DEFINED GRPC_ROOT)
    list(APPEND CMAKE_PREFIX_PATH "${GRPC_ROOT}")
endif()
if(DEFINED PROTOBUF_ROOT)
    list(APPEND CMAKE_PREFIX_PATH "${PROTOBUF_ROOT}")
endif()
if(DEFINED ENV{GRPC_ROOT})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{GRPC_ROOT}")
endif()
if(DEFINED ENV{PROTOBUF_ROOT})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{PROTOBUF_ROOT}")
endif()

# 查找 Protobuf
find_package(Protobuf QUIET)
find_package(gRPC QUIET)

if(Protobuf_FOUND AND gRPC_FOUND)
    message(STATUS "Found system Protobuf: ${Protobuf_VERSION}")
    message(STATUS "Found system gRPC: ${gRPC_VERSION}")
    set(YALGO_GRPC_TARGETS gRPC::grpc++ protobuf::libprotobuf)

    # 查找与库版本匹配的 protoc（避免 PATH 中旧版本干扰）
    # 直接检查已知的正确路径，避免 find_program 被 PATH 中旧版本干扰
    set(_YALGO_PROTOC "")
    foreach(_PROTOC_SEARCH_PATH
        "$ENV{PROTOBUF_ROOT}/bin/protoc"
        "${PROTOBUF_ROOT}/bin/protoc"
        "/opt/homebrew/opt/protobuf/bin/protoc"
        "/opt/homebrew/bin/protoc"
        "/usr/local/bin/protoc"
        "/usr/bin/protoc"
    )
        if(EXISTS "${_PROTOC_SEARCH_PATH}" AND NOT _YALGO_PROTOC)
            set(_YALGO_PROTOC "${_PROTOC_SEARCH_PATH}")
        endif()
    endforeach()
    # 如果指定路径都找不到，回退到 find_program
    if(NOT _YALGO_PROTOC)
        find_program(_YALGO_PROTOC protoc)
    endif()
    if(_YALGO_PROTOC)
        set(YALGO_PROTOC_EXECUTABLE "${_YALGO_PROTOC}")
    else()
        set(YALGO_PROTOC_EXECUTABLE "${Protobuf_PROTOC_EXECUTABLE}")
    endif()

    # 查找 grpc_cpp_plugin
    find_program(_YALGO_GRPC_CPP_PLUGIN grpc_cpp_plugin
        HINTS
            "$ENV{GRPC_ROOT}/bin"
            "${GRPC_ROOT}/bin"
        PATHS
            /opt/homebrew/opt/grpc/bin
            /opt/homebrew/bin
            /usr/local/bin
            /usr/bin
    )
    if(_YALGO_GRPC_CPP_PLUGIN)
        set(YALGO_GRPC_CPP_PLUGIN "${_YALGO_GRPC_CPP_PLUGIN}")
    else()
        message(WARNING "grpc_cpp_plugin not found, proto gRPC code generation may fail")
    endif()

    set(YALGO_GRPC_FOUND TRUE)
    yalgo_fix_macos_includes()
endif()

# ============================================================
# Step 2: FetchContent 降级
# ============================================================
if(NOT YALGO_GRPC_FOUND)
    message(STATUS "System gRPC/Protobuf not found, fetching via FetchContent...")

    include(FetchContent)

    # gRPC 版本
    set(YALGO_GRPC_VERSION "1.68.2" CACHE STRING "gRPC version to fetch")

    # 禁用 gRPC 测试和共享库构建
    set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
    set(gRPC_BUILD_TESTS_SAVED "${gRPC_BUILD_TESTS}")
    set(protobuf_BUILD_TESTS_SAVED "${protobuf_BUILD_TESTS}")
    set(gRPC_BUILD_GRPC_CPP_PLUGIN_SAVED "${gRPC_BUILD_GRPC_CPP_PLUGIN}")
    set(gRPC_BUILD_CSHARP_EXT_SAVED "${gRPC_BUILD_CSHARP_EXT}")
    set(gRPC_BUILD_GRPC_NODE_PLUGIN_SAVED "${gRPC_BUILD_GRPC_NODE_PLUGIN}")
    set(gRPC_BUILD_GRPC_PYTHON_PLUGIN_SAVED "${gRPC_BUILD_GRPC_PYTHON_PLUGIN}")
    set(gRPC_BUILD_GRPC_RUBY_PLUGIN_SAVED "${gRPC_BUILD_GRPC_RUBY_PLUGIN}")

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(gRPC_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(protobuf_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(gRPC_BUILD_GRPC_CPP_PLUGIN ON CACHE BOOL "" FORCE)
    set(gRPC_BUILD_CSHARP_EXT OFF CACHE BOOL "" FORCE)
    set(gRPC_BUILD_GRPC_NODE_PLUGIN OFF CACHE BOOL "" FORCE)
    set(gRPC_BUILD_GRPC_PYTHON_PLUGIN OFF CACHE BOOL "" FORCE)
    set(gRPC_BUILD_GRPC_RUBY_PLUGIN OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        gRPC
        GIT_REPOSITORY https://github.com/grpc/grpc.git
        GIT_TAG        v${YALGO_GRPC_VERSION}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )

    FetchContent_MakeAvailable(gRPC)

    # 恢复原始值
    set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}" CACHE BOOL "" FORCE)
    set(gRPC_BUILD_TESTS "${gRPC_BUILD_TESTS_SAVED}" CACHE BOOL "" FORCE)
    set(protobuf_BUILD_TESTS "${protobuf_BUILD_TESTS_SAVED}" CACHE BOOL "" FORCE)

    set(YALGO_GRPC_TARGETS grpc++ protobuf::libprotobuf)
    set(YALGO_PROTOC_EXECUTABLE "$<TARGET_FILE:protoc>")
    set(YALGO_GRPC_CPP_PLUGIN "$<TARGET_FILE:grpc_cpp_plugin>")
    set(YALGO_GRPC_FOUND TRUE)

    message(STATUS "gRPC ${YALGO_GRPC_VERSION} fetched and configured")
    yalgo_fix_macos_includes()
endif()

# ============================================================
# 验证结果
# ============================================================
if(YALGO_GRPC_FOUND)
    message(STATUS "FindYalgoGrpc: targets=${YALGO_GRPC_TARGETS}")
    message(STATUS "FindYalgoGrpc: protoc=${YALGO_PROTOC_EXECUTABLE}")
    message(STATUS "FindYalgoGrpc: grpc_plugin=${YALGO_GRPC_CPP_PLUGIN}")
else()
    message(FATAL_ERROR
        "Could not find gRPC/Protobuf.\n"
        "Options:\n"
        "  1. Install via package manager: brew install grpc protobuf (macOS)\n"
        "                               apt install libgrpc-dev libprotobuf-dev (Ubuntu)\n"
        "  2. Set CMake variables: -DGRPC_ROOT=/path/to/grpc -DPROTOBUF_ROOT=/path/to/protobuf\n"
        "  3. Set environment variables: export GRPC_ROOT=/path/to/grpc\n"
    )
endif()
