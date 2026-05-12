cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(opengl_example)

# 设置Vulkan路径
set(Vulkan_INCLUDE_DIRS "C:/VulkanSDK/1.4.304.0/Include")
set(Vulkan_LIBRARIES "C:/VulkanSDK/1.4.304.0/Lib/vulkan-1.lib")

# 包含头文件目录
include_directories(
    ${Vulkan_INCLUDE_DIRS}
    ../../sdk/earth
)

# 添加源文件
set(SOURCE_FILES
    opengl_example.cpp
    vulkan_test.cpp
    vulkan_test.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 链接库
target_link_libraries(${PROJECT_NAME}
    ${Vulkan_LIBRARIES}
    ${GLFW_LIBRARIES}
    yalgo_earth
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 添加版本资源文件
if(WIN32)
    # 确保版本变量可用
    if(NOT DEFINED PROJECT_VERSION_MAJOR)
        set(PROJECT_VERSION_MAJOR 1)
    endif()
    if(NOT DEFINED PROJECT_VERSION_MINOR)
        set(PROJECT_VERSION_MINOR 0)
    endif()
    if(NOT DEFINED PROJECT_VERSION_PATCH)
        set(PROJECT_VERSION_PATCH 0)
    endif()
    if(NOT DEFINED PROJECT_VERSION)
        set(PROJECT_VERSION "1.0.0")
    endif()
    
    configure_file(${CMAKE_SOURCE_DIR}/cmake/templates/opengl_app_version.rc.in
        ${CMAKE_BINARY_DIR}/resources/opengl_example_version.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_BINARY_DIR}/resources/opengl_example_version.rc
    )
endif()
