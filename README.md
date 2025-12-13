# yAlgo 项目

## 项目简介

yAlgo是一个C++工具库集合，提供了日志记录和通用工具函数，帮助开发者快速构建C++应用程序。

## 主要功能模块

### yalgo_log
- 高性能日志记录库
- 支持异步日志
- 提供灵活的日志配置选项

### yalgo_utils
- 实用工具函数库
- 包含执行路径相关的实用工具

## 项目结构

```
yAlgo/
├── CMakeLists.txt        # 主构建配置文件
├── cmake/                # 自定义CMake模块
│   ├── YuLibInstall.cmake # 统一安装模块
│   └── YuMakeApp.cmake   # 应用程序构建模块
├── examples/             # 示例程序
│   ├── log/              # 日志库示例
│   └── utils/            # 工具库示例
├── resources/            # 资源文件
├── sdk/                  # SDK源代码
│   ├── log/              # 日志模块
│   └── utils/            # 工具模块
├── bin/                  # 编译输出的可执行文件和动态库
├── lib/                  # 编译输出的静态库文件
└── install/              # 安装目录（cmake --install生成）
```

## 构建和安装

### 前提条件
- CMake 3.14+
- 支持C++17的编译器

### 构建步骤

1. 创建构建目录
```bash
mkdir build
cd build
```

2. 配置项目
```bash
cmake .. -G Ninja  # 或使用其他生成器如 "Visual Studio 17 2022"
```

3. 编译项目
```bash
cmake --build .
```

4. 安装项目
```bash
cmake --install .
```

安装后，文件将被安装到项目根目录下的`install`文件夹：
- `install/bin/` - 可执行文件和动态库
- `install/lib/` - 静态库文件
- `install/include/yAlgo/` - 头文件
- `install/etc/` - 配置文件

## 使用示例

### 日志库使用示例
可以参考 `examples/log` 目录下的示例程序，展示了如何使用yalgo_log库进行日志记录。

### 工具库使用示例
可以参考 `examples/utils` 目录下的示例程序，展示了如何使用yalgo_utils库进行工具函数调用。

## 自定义CMake模块

### YuLibInstall.cmake
统一的安装模块，提供了两个主要函数：
- `yalgo_install_lib` - 用于安装库文件和头文件
- `yalgo_install_app` - 用于安装应用程序和配置文件

### YuMakeApp.cmake
yAlgo应用程序构建脚本，用于创建可执行应用程序，提供统一的构建配置

## 许可证

[在此添加许可证信息]
