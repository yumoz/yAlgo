# yUtils 应用程序构建脚本
# 用于创建可执行应用程序，提供统一的构建配置

# 检查必要变量
if(NOT DEFINED SRC_ROOT_DIR)
    message(FATAL_ERROR "必须定义SRC_ROOT_DIR变量")
endif()

# 可选参数默认值
if(NOT DEFINED parent_dir_name)
    set(parent_dir_name "Applications")
endif()

# 自动设置目标名称（如果未指定）
if(NOT DEFINED TARGET)
    get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)
endif()

# 包含路径工具
include(${SRC_ROOT_DIR}/cmake/YuPath.cmake)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 平台特定配置
function(setup_app_platform_config)
    if(WIN32)
        if(MSVC)
            # Windows/MSVC特定编译选项
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8" PARENT_SCOPE)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO" PARENT_SCOPE)
        endif()
        
        # 输出目录配置
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/bin/debug         PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/export/lib/debug  PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/export/lib/debug  PARENT_SCOPE)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/bin/release       PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/export/lib/release PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/export/lib/release PARENT_SCOPE)
    else()
        # Unix/Linux特定编译选项（修复了$ORIGIN拼写错误）
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,:'$$ORIGIN' -Wl,--unresolved-symbols=ignore-in-shared-libs" PARENT_SCOPE)
        
        # 输出目录配置
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/bin/debug         PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/export/lib/debug  PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/export/lib/debug  PARENT_SCOPE)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/bin/release       PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/export/lib/release PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/export/lib/release PARENT_SCOPE)
    endif()
endfunction()

# 调用平台配置函数
setup_app_platform_config()

# 源文件收集和处理
function(collect_app_source_files)
    # 转换为CMake路径格式
    file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" SOURCEPATH)
    
    # 收集源文件和头文件
    file(GLOB_RECURSE SOURCES_FILES 
        "${SOURCEPATH}/*.cpp" 
        "${SOURCEPATH}/*.c" 
        "${SOURCEPATH}/*.cc" 
        "${SOURCEPATH}/*.cxx" 
    )
    
    file(GLOB_RECURSE HEADER_FILES 
        "${SOURCEPATH}/*.h" 
        "${SOURCEPATH}/*.hpp" 
        "${SOURCEPATH}/*.hh" 
    )
    
    # 分组源文件以便在IDE中更好地组织
    if(SOURCES_FILES)
        source_group(TREE "${SOURCEPATH}" PREFIX "Source Files" FILES ${SOURCES_FILES})
    endif()
    
    if(HEADER_FILES)
        source_group(TREE "${SOURCEPATH}" PREFIX "Header Files" FILES ${HEADER_FILES})
    endif()
    
    # 将结果返回给父作用域
    set(SOURCES_FILES ${SOURCES_FILES} PARENT_SCOPE)
    set(HEADER_FILES ${HEADER_FILES} PARENT_SCOPE)
endfunction()

# 调用源文件收集函数
collect_app_source_files()

# 创建可执行目标
if(SOURCES_FILES OR HEADER_FILES)
    add_executable(${TARGET} ${SOURCES_FILES} ${HEADER_FILES})
    
    # 设置目标属性
    set_target_properties(${TARGET} PROPERTIES 
        FOLDER ${parent_dir_name}
        DEBUG_POSTFIX "d"           # Debug版本添加'd'后缀
        RELEASE_POSTFIX ""          # Release版本无后缀
    )
    
    # 添加公共头文件目录
    target_include_directories(${TARGET} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SRC_ROOT_DIR}/export
    )
    
    # 添加调试器工作目录设置（适用于Visual Studio）
    if(MSVC)
        set_property(TARGET ${TARGET} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG},${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>")
    endif()
    
    message(STATUS "应用程序配置完成: ${parent_dir_name} --> ${TARGET}")
    
    # 可选功能：自动链接SDK
    # if(EXISTS ${SRC_ROOT_DIR}/sdk)
    #     # 可以在这里添加自动查找和链接SDK的逻辑
    # endif()
    
else()
    message(WARNING "未找到任何源文件或头文件: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()