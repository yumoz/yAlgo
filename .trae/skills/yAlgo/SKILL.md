---
name: "yAlgo"
description: "提供 yAlgo 项目的代码分析、编译和使用指南。当用户需要了解项目结构、编译项目或使用其中的功能时调用。"
---

# yAlgo 项目技能

## 项目概述

yAlgo 是一个 C++17 算法/SDK 库集合，包含以下模块：
- **log**: 高性能异步日志系统
- **utils**: 工具函数（如获取可执行文件路径、系统类型检测）
- **math**: 数学工具函数（角度弧度转换、功率单位换算、分贝计算）
- **earth**: 地球坐标转换（WGS84/UTM/墨卡托/ECEF）和几何计算（多边形包含、通视分析）

## 项目结构

```
yAlgo/
├── CMakeLists.txt          # 主 CMake 配置文件
├── CMakePresets.json       # CMake 预设（VS 2022 / Unix Makefiles）
├── sdk/                    # SDK 核心源码
│   ├── log/               # 日志模块（SHARED 库 yalgo_log）
│   ├── utils/             # 工具模块（SHARED 库 yalgo_utils）
│   ├── math/              # 数学模块（SHARED 库 yalgo_math）
│   └── earth/             # 地球坐标模块（SHARED 库 yalgo_earth）
├── examples/              # 示例可执行程序
├── tests/                 # 测试套件
│   ├── unit/              # 单元测试（test_log, test_utils, test_math, test_earth）
│   └── integration/       # 集成测试（test_integration）
├── cmake/                 # 自定义 CMake 模块（YuMakeSDK, YuMakeApp, YuLibInstall 等）
├── build/                 # CMake 构建输出目录
├── bin/                   # 编译产物：可执行文件 / DLL（按 OS/构建类型分目录）
├── lib/                   # 编译产物：静态导入库（按 OS/构建类型分目录）
└── .github/workflows/     # CI/CD 配置
```

**输出目录结构**（自动按 OS 分类）：
- `bin/<os>/release/` — Release 可执行文件 & DLL
- `lib/<os>/release/` — Release 静态导入库
- Debug 版本添加 `d` 后缀（如 `yalgo_logd.dll`）并输出到 `debug/` 子目录

## 编译项目

### 方式一：使用 CMake Presets（推荐）

项目预定义了跨平台 CMake Presets，自动选择合适的 Generator：

```bash
# Windows（Visual Studio 17 2022）
cmake --preset windows-release
cmake --build --preset windows-release

cmake --preset windows-debug
cmake --build --preset windows-debug

# Linux / macOS（Unix Makefiles）
cmake --preset linux-release
cmake --build --preset linux-release

cmake --preset linux-debug
cmake --build --preset linux-debug

# macOS
cmake --preset macos-release
cmake --build --preset macos-release
```

### 方式二：手动 CMake（Ninja 推荐）

```bash
# Debug 构建
cmake -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build build

# Release 构建
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build
```

### 安装

```bash
cmake --install build    # 安装到 <project>/install/
```

### 跨平台注意事项

- **Windows（MSVC）**：CMake 自动添加 `/utf-8` 编译选项；`getenv` 在 MSVC 下会触发 C4996 警告
- **Linux（GCC/Clang）**：自动检测 Ubuntu 或 Kylin 系统，选择合适的 OS_NAME
- **编码**：全项目使用 UTF-8 编码，Linux 下需确保 locale 支持 UTF-8
- **平台差异代码**：`<windows.h>` 相关 API（SetConsoleOutputCP 等）已用 `#ifdef _WIN32` 保护

## 运行测试

项目使用 CTest，测试注册名称如下：

```bash
# 运行所有测试
ctest --build-config Release

# 详细输出
ctest -V --build-config Release

# 仅运行单元测试
ctest -R unit --build-config Release

# 仅运行集成测试
ctest -R integration --build-config Release
```

**已注册的测试**：

| 测试名称 | 类型 | 说明 |
|---|---|---|
| `unit_log` | 单元测试 | 日志模块 |
| `unit_utils` | 单元测试 | 工具模块 |
| `unit_math` | 单元测试 | 数学模块 |
| `unit_earth` | 单元测试 | 地球坐标模块 |
| `integration_modules` | 集成测试 | 多模块联合测试 |

## CI/CD

项目使用 GitHub Actions 自动构建和测试，配置位于 `.github/workflows/cmake-multi-platform.yml`。

**矩阵策略**（fail-fast: false，各配置独立报告）：

| 运行环境 | 编译器 | 构建类型 |
|---|---|---|
| `windows-latest` | MSVC cl | Release |
| `ubuntu-latest` | GCC g++ | Release |
| `ubuntu-latest` | Clang clang++ | Release |

**流水线步骤**：
1. `actions/checkout@v4`
2. CMake 配置（设置 C/C++ 编译器）
3. `cmake --build` 构建
4. `ctest` 运行全部测试

## 外部依赖

项目为**纯 C++17 实现，无外部运行时依赖**。所有功能均基于 C++ 标准库实现，无需安装额外的 SDK 或第三方库。

## 模块功能

### 1. 日志模块 (log)
- 高性能异步日志系统（独立工作线程 + 消息队列）
- 支持控制台（ANSI 颜色）、文件、系统日志（Windows Event Log / Linux syslog）三路输出
- 支持流式日志宏（`YLOG_INFO`、`YLOG_WARN`、`YLOG_ERROR`、`YLOG_DEBUG`）
- 支持模块名称、文件名、行号、函数名自动记录
- 支持按天/按文件大小自动轮转
- 支持从配置文件（INI 风格）和环境变量加载配置
- 支持运行时动态调整日志级别
- 内置性能统计（日志吞吐量、写入耗时、丢弃数、队列长度）

### 2. 工具模块 (utils)
- 获取可执行文件完整路径
- 获取可执行文件所在目录
- 拼接可执行文件目录下的文件路径
- 检查可执行文件目录下的文件是否存在
- 获取当前系统类型（`"windows"` / `"linux"` / `"macos"`）

### 3. 数学模块 (math)
- 角度 ↔ 弧度转换
- 功率单位转换：W ↔ kW、W ↔ hp（马力）、W ↔ dBW、W ↔ dBm、dBW ↔ dBm
- 通用分贝计算

### 4. 地球坐标模块 (earth)
- 多坐标系支持：WGS84、ECEF、UTM、墨卡托
- 坐标双向转换：WGS84 ↔ ECEF、WGS84 ↔ UTM、WGS84 ↔ 墨卡托
- 多椭球体模型：WGS84、GRS80、Clarke1866、Airy、Bessel1841
- 几何计算：Haversine 距离、Vincenty 距离、方位角、向量运算、直线距离
- 通视判断（考虑地球曲率）
- 射线法判断点是否在多边形内（支持 UTM/墨卡托投影坐标系）
- GEO 卫星中国区域覆盖模拟测试
- 投影精度统计分析

## 使用示例

### 日志模块示例
```cpp
yalgo::log::LogConfig config;
config.runtime_level = yalgo::log::LogLevel::INFO;
config.enable_console = true;
config.enable_file = true;
yalgo::log::AsyncLogger::getInstance().init(config);

YLOG_INFO("这是一条信息日志");
YLOG_WARN("这是一条警告日志");
YLOG_ERROR("这是一条错误日志");
YLOG_DEBUG("这是一条调试日志");
```

### 工具模块示例
```cpp
std::string exe_path = yalgo::utils::ExecPathUtils::getExecutablePath();
std::string exe_dir = yalgo::utils::ExecPathUtils::getExecutableDir();
std::string config_path = yalgo::utils::ExecPathUtils::getFileInExeDir("config.json");
std::string system_type = yalgo::utils::ExecPathUtils::getSystemType();
```

### 地球坐标模块示例
```cpp
EarthPoint point(116.4, 39.9);
EarthConverter converter;
EarthConverter::UTMCoordinate utm = converter.wgs84ToUTM(point);
EarthConverter::MercatorCoordinate merc = converter.wgs84ToMercator(point);

std::vector<EarthPoint> polygon = { /* 多边形顶点 */ };
EarthGeometry geometry;
bool isInside = geometry.isPointInPolygon(point, polygon, ProjectionType::UTM);
```

### 数学模块示例
```cpp
double rad = yalgo::math::MathUtils::degreesToRadians(180.0);
double dbm = yalgo::math::MathUtils::wattsToDbm(1.0);
```

## 注意事项

1. **C++ 标准**：要求 C++17 或更高版本
2. **编码**：全项目 UTF-8，MSVC 已配置 `/utf-8`
3. **线程安全**：日志模块是线程安全的；EarthPoint 等值类型非线程安全
4. **墨卡托投影范围**：仅支持纬度 -85.05° 到 85.05°
5. **平台兼容**：已在 Windows（MSVC）、Ubuntu（GCC/Clang）上验证；macOS 支持尚未完整测试

## Doxygen 注释风格

### 文件注释
```cpp
/**
 * @file filename.h
 * @brief 文件描述
 * @author yAlgo Team
 * @date 2026-01-01
 */
```

### 类注释
```cpp
/**
 * @class ClassName
 * @brief 类描述
 */
class ClassName { };
```

### 函数注释
```cpp
/**
 * @brief 函数描述
 * @param param1 参数1描述
 * @param param2 参数2描述
 * @return 返回值描述
 */
return_type function_name(param1_type param1, param2_type param2);
```

### 成员变量注释
```cpp
variable_type variable_name; ///< 变量描述
```

### 枚举注释
```cpp
/**
 * @enum EnumName
 * @brief 枚举描述
 */
enum class EnumName {
    VALUE1, ///< 值1描述
    VALUE2  ///< 值2描述
};
```
