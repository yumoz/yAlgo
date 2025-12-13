# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# sdk目录作为父目录，不再创建重复的库
# 库的实际实现在sdk/log和sdk/utils子目录中
# 此目录主要用于提供统一的头文件路径和依赖管理

# 设置资源目录，确保版本资源文件生成路径存在
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resources)

# 设置输出目录变量供子目录使用
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE PATH "Archive output directory")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE PATH "Library output directory")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Runtime output directory")

# sdk目录不再直接安装目标，由子目录负责各自的安装规则
