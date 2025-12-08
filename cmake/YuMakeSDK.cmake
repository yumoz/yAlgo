# yUtils SDK构建脚本
# 用于创建共享库模块，提供统一的构建配置

# 参数检查
if(NOT DEFINED TARGET)
    message(FATAL_ERROR "必须定义TARGET变量")
endif()

# 可选参数默认值
if(NOT DEFINED parent_dir_name)
    set(parent_dir_name "SDK")
endif()

# 全局属性设置
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_DEPFILE_FLAGS_C   "")
set(CMAKE_DEPFILE_FLAGS_CXX "")

# C++标准配置 (使用现代C++标准)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # 不使用编译器特定扩展

# 平台特定配置
function(setup_platform_configuration)
    if (WIN32)
        if (MSVC)
            # Windows/MSVC特定编译选项
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8" PARENT_SCOPE)
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO" PARENT_SCOPE)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO" PARENT_SCOPE)
            # 可选的管理员权限设置，默认不启用
            # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"")
        endif()
        
        # Windows输出目录配置
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/bin/debug/        PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/export/lib/debug  PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/export/lib/debug  PARENT_SCOPE)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/bin/release/     PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/export/lib/release PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/export/lib/release PARENT_SCOPE)
    else()
        # Unix/Linux特定编译选项
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,:'$$ORIGIN' -Wl,-unresolved-symbols=ignore-in-shared-libs" PARENT_SCOPE)
        
        # Unix输出目录配置
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/bin/debug/tools  PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/bin/debug/tools  PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${SRC_ROOT_DIR}/bin/debug/tools  PARENT_SCOPE)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/bin/release/tools PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/bin/release/tools PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${SRC_ROOT_DIR}/bin/release/tools PARENT_SCOPE)
    endif()
endfunction()

# 调用平台配置函数
setup_platform_configuration()

# 源文件收集和处理
function(collect_source_files)
    # 转换为CMake路径格式
    file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" SOURCEPATH)
    
    # 收集源文件和头文件
    file(GLOB_RECURSE SOURCES_FILES 
        "${SOURCEPATH}/*.cpp" 
        "${SOURCEPATH}/*.c" 
    )
    
    file(GLOB_RECURSE HEADER_FILES 
        "${SOURCEPATH}/*.h" 
        "${SOURCEPATH}/*.hpp" 
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
collect_source_files()

# 创建共享库目标
if(SOURCES_FILES OR HEADER_FILES)
    add_library(${TARGET} SHARED ${HEADER_FILES} ${SOURCES_FILES})
    
    # 设置目标属性
    set_target_properties(${TARGET} PROPERTIES 
        FOLDER ${parent_dir_name}
        DEBUG_POSTFIX "d"           # Debug版本添加'd'后缀
        RELEASE_POSTFIX ""          # Release版本无后缀
    )
    
    # 添加公共头文件目录
    if(DEFINED SRC_ROOT_DIR)
        target_include_directories(${TARGET} PRIVATE ${SRC_ROOT_DIR}/export)
    endif()
    
    message(STATUS "模块配置完成: ${parent_dir_name} --> ${TARGET}")
else()
    message(WARNING "未找到任何源文件或头文件: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# 注释掉的示例代码，保留作为参考
# set_property(TARGET ${TARGET} PROPERTY FOLDER "tools/${TARGET}")
# set_property(SOURCE ${CPP_SOURCE} APPEND PROPERTY OBJECT_DEPENDS "${FORMS_SOURCE}")
# set_property(TARGET ${TARGET} PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=${_qt5_root_dir}/../../bin;%PATH%")
# set_property(TARGET ${TARGET} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG},${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>")

