# yUtils CMake 模块

本目录包含 yUtils 项目使用的 CMake 模块，提供统一的构建配置和工具函数。

## 目录结构

```
cmake/
├── README.md           # 说明文档
├── YuMakeSDK.cmake     # SDK/库构建模块
├── YuMakeApp.cmake     # 应用程序构建模块  
├── YuPath.cmake        # 路径处理工具模块
└── yutils.cmake        # 主入口模块（包含所有功能）
```

## 使用方法

### 构建SDK/库
在库项目的CMakeLists.txt中：

```cmake
set(SRC_ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(TARGET "your_library_name")  # 可选，如果不指定会使用目录名
set(parent_dir_name "SDK")       # 可选，IDE中的文件夹名称

include(${SRC_ROOT_DIR}/cmake/YuMakeSDK.cmake)
```

### 构建应用程序
在应用程序项目的CMakeLists.txt中：

```cmake
set(SRC_ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(TARGET "your_app_name")      # 可选，如果不指定会使用目录名
set(parent_dir_name "Applications") # 可选，IDE中的文件夹名称

include(${SRC_ROOT_DIR}/cmake/YuMakeApp.cmake)
```

### 使用路径工具
```cmake
include(${SRC_ROOT_DIR}/cmake/YuPath.cmake)

# 自动设置的变量
echo("当前目录路径: ${current_dir_path}")
echo("当前目录名称: ${current_dir_name}")
echo("父目录路径: ${parent_dir_path}")
echo("父目录名称: ${parent_dir_name}")

# 使用函数
ensure_directory_exists("${CMAKE_BINARY_DIR}/output")
get_relative_path("${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" relative)
echo("相对路径: ${relative}")
```

## 注意事项

1. 确保正确设置 `SRC_ROOT_DIR` 变量指向项目根目录
2. 支持 Visual Studio、GCC、Clang 等主流编译器
3. 自动处理 Windows 和 Unix 平台的差异
4. 提供统一的输出目录结构
