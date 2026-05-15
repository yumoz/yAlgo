# yAlgo CMake 模块

本目录包含 yAlgo 项目使用的 CMake 模块，提供统一的构建配置和工具函数。

## 目录结构

```
cmake/
├── README.md              # 说明文档
├── yutils.cmake           # 主入口模块（聚合所有模块）
├── YuMakeSDK.cmake        # SDK/库构建模块
├── YuMakeApp.cmake        # 应用程序构建模块
├── YuPath.cmake           # 路径处理工具模块
└── YuLibInstall.cmake     # 统一安装模块
```

## 模块说明

### yutils.cmake（主入口）

```cmake
include(yutils)
```

聚合所有子模块，同时提供以下函数：
- `yutils_add_library()` — 自动添加 SDK 库
- `yutils_add_executable()` — 自动添加可执行文件
- `check_build_type()` — 检查构建类型
- `print_platform_info()` — 打印平台信息

### YuMakeSDK.cmake

构建 SHARED 库目标，自动收集源码、配置 C++17、设置输出目录。

```cmake
include(${SRC_ROOT_DIR}/cmake/YuMakeSDK.cmake)
```

需预先设置：
- `SRC_ROOT_DIR` — 项目根目录
- `TARGET`（可选）— 目标名，默认使用目录名
- `parent_dir_name`（可选）— IDE 文件夹分组名

### YuMakeApp.cmake

构建可执行目标，配置链接和输出目录。

```cmake
include(${SRC_ROOT_DIR}/cmake/YuMakeApp.cmake)
```

### YuPath.cmake

路径工具函数：

```cmake
include(${SRC_ROOT_DIR}/cmake/YuPath.cmake)

echo("当前目录: ${current_dir_path}")
echo("当前目录名: ${current_dir_name}")
echo("父目录: ${parent_dir_path}")
echo("父目录名: ${parent_dir_name}")

ensure_directory_exists("${CMAKE_BINARY_DIR}/output")
get_relative_path("${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" relative_path)
join_paths("${CMAKE_SOURCE_DIR}" "subdir" joined_path)
normalize_path("${CMAKE_BINARY_DIR}/../src" normalized_path)
```

### YuLibInstall.cmake

统一安装函数，需配合 `include(YuLibInstall OPTIONAL)` 使用。

#### yutils_install_lib

安装库文件和头文件：

```cmake
yutils_install_lib(
    TARGET_NAME  my_target    # 必需：目标名称
    HEADER_FILES              # 必需：头文件列表
        ${CMAKE_CURRENT_SOURCE_DIR}/header1.h
        ${CMAKE_CURRENT_SOURCE_DIR}/header2.h
    INCLUDE_DIR               # 可选：头文件目录
    INCLUDE_DEST              # 可选：安装路径，默认 include/yAlgo/<组件名>
)
```

安装位置：
- `bin/` — 共享库（DLL / .so）
- `lib/` — 导入库（.lib / .a）
- `include/yAlgo/<组件名>/` — 头文件

#### yutils_install_app

安装可执行文件和配置文件：

```cmake
yutils_install_app(
    TARGET_NAME  my_app       # 必需：目标名称
    CONFIG_FILES              # 可选：配置文件列表
        ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
    CONFIG_DEST               # 可选：安装路径，默认 etc
)
```

安装位置：
- `bin/` — 可执行文件
- `etc/` — 配置文件

## 注意事项

1. 使用前确保正确设置 `SRC_ROOT_DIR` 指向项目根目录
2. 支持 Visual Studio、GCC、Clang 等主流编译器
3. 自动处理 Windows 和 Unix 平台差异（路径分隔符、DLL/so 后缀等）
4. 提供统一的输出目录结构 `bin/<os>/<build_type>/` 和 `lib/<os>/<build_type>/`
