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

### yalgo_math
- 基础数学函数库
- 支持角度/弧度互相转换
- 支持传统功率单位转换（瓦特、千瓦、马力）
- 支持分贝功率转换（dBW、dBm与瓦特之间的转换）
- 支持线性值与分贝值之间的通用转换

## 项目结构

```
yAlgo/
├── CMakeLists.txt        # 主构建配置文件
├── cmake/                # 自定义CMake模块
│   ├── YuLibInstall.cmake # 统一安装模块
│   └── YuMakeApp.cmake   # 应用程序构建模块
├── examples/             # 示例程序
│   ├── log/              # 日志库示例
│   ├── math/             # 数学库示例
│   └── utils/            # 工具库示例
├── resources/            # 资源文件
├── sdk/                  # SDK源代码
│   ├── log/              # 日志模块
│   ├── math/             # 数学模块
│   └── utils/            # 工具模块
├── bin/                  # 编译输出的可执行文件和动态库
│   └── windows/          # Windows平台输出
│       ├── debug/        # Debug版本输出
│       └── release/      # Release版本输出
├── lib/                  # 编译输出的静态库文件
│   └── windows/          # Windows平台库文件
│       ├── debug/        # Debug版本库文件
│       └── release/      # Release版本库文件
└── install/              # 安装目录（cmake --install生成）
```

## 构建和安装

### 前提条件
- CMake 3.14+
- 支持C++17的编译器
- 建议使用Ninja生成器以获得更快的构建速度

### 构建步骤

#### Debug版本构建

1. 创建构建目录并进入
```bash
mkdir build
cd build
```

2. 配置Debug版本
```bash
cmake -DCMAKE_BUILD_TYPE=Debug .. -G Ninja  # 或使用其他生成器如 "Visual Studio 17 2022"
```

3. 编译Debug版本
```bash
cmake --build .
```

#### Release版本构建

1. 清理现有构建（可选，如果已经有build目录）
```bash
# 方法1：使用clean-all目标清理所有输出文件
cmake --build . --target clean-all

# 方法2：手动删除构建目录和输出目录
rm -rf ../bin ../lib ../install
cd ..
rm -rf build
```

2. 重新创建构建目录并进入
```bash
mkdir build
cd build
```

3. 配置Release版本
```bash
cmake -DCMAKE_BUILD_TYPE=Release .. -G Ninja  # 或使用其他生成器如 "Visual Studio 17 2022"
```

4. 编译Release版本
```bash
cmake --build .
```

#### 安装项目

无论是Debug还是Release版本，都可以使用以下命令安装：

```bash
cmake --install .
```

### 输出目录结构

构建完成后，输出文件将按照以下结构组织：

#### Debug版本
- **可执行文件和动态库**：`bin/windows/debug/`
  - libyalgo_log.dll (日志库)
  - libyalgo_math.dll (数学库)
  - libyalgo_utils.dll (工具库)
  - log_example.exe (日志测试示例)
  - math_example.exe (数学测试示例)
  - utils_example.exe (工具测试示例)

- **静态库文件**：`lib/windows/debug/`
  - libyalgo_log.dll.a
  - libyalgo_math.dll.a
  - libyalgo_utils.dll.a

#### Release版本
- **可执行文件和动态库**：`bin/windows/release/`
  - libyalgo_log.dll (日志库)
  - libyalgo_math.dll (数学库)
  - libyalgo_utils.dll (工具库)
  - log_example.exe (日志测试示例)
  - math_example.exe (数学测试示例)
  - utils_example.exe (工具测试示例)

- **静态库文件**：`lib/windows/release/`
  - libyalgo_log.dll.a
  - libyalgo_math.dll.a
  - libyalgo_utils.dll.a

#### 安装目录结构

安装后，文件将被安装到项目根目录下的`install`文件夹：
- `install/bin/` - 可执行文件和动态库
- `install/lib/` - 静态库文件
- `install/include/yAlgo/` - 头文件
- `install/etc/` - 配置文件

## 使用示例

### 日志库使用示例
可以参考 `examples/log` 目录下的示例程序，展示了如何使用yalgo_log库进行日志记录。

### 数学库使用示例
可以参考 `examples/math` 目录下的示例程序，展示了如何使用yalgo_math库进行角度/弧度转换和功率单位转换。

#### 角度/弧度转换
```cpp
#include "yalgo/math/math_utils.h"

// 角度转弧度
double radians = degreesToRadians(90.0);  // 返回 π/2

// 弧度转角度
double degrees = radiansToDegrees(M_PI);  // 返回 180.0
```

#### 传统功率单位转换
```cpp
// 瓦特转千瓦
double kw = wattsToKilowatts(1000.0);  // 返回 1.0

// 千瓦转瓦特
double w = kilowattsToWatts(5.0);  // 返回 5000.0

// 瓦特转马力
double hp = wattsToHorsepower(735.5);  // 返回约 1.0
```

#### 分贝功率转换
```cpp
// 瓦特转dBW
double dbw = wattsToDbW(10.0);  // 返回 10.0

// dBW转瓦特
double w = dbWToWatts(20.0);  // 返回 100.0

// 瓦特转dBm
double dbm = wattsToDbm(1.0);  // 返回 30.0

// dBm转瓦特
double w = dbmToWatts(0.0);  // 返回 0.001

// dBW与dBm之间转换
double dbm = dbWToDbm(0.0);  // 返回 30.0
double dbw = dbmToDbW(30.0);  // 返回 0.0
```

#### 通用线性值与dB值转换
```cpp
// 线性值转dB值（默认参考值为1.0）
double db = linearToDb(100.0);  // 返回 20.0

// dB值转线性值（默认参考值为1.0）
double linear = dbToLinear(10.0);  // 返回 10.0

// 使用自定义参考值的转换
double db = linearToDb(8.0, 2.0);  // 返回 6.0206（参考值为2.0）
double linear = dbToLinear(6.0206, 2.0);  // 返回 8.0（参考值为2.0）
```

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
