---
name: "yAlgo"
description: "提供 yAlgo 项目的代码分析、编译和使用指南。当用户需要了解项目结构、编译项目或使用其中的功能时调用。"
---

# yAlgo 项目技能

## 项目概述

yAlgo 是一个 C++ 算法库，包含以下模块：
- **log**: 异步日志系统
- **utils**: 工具函数（如获取可执行文件路径）
- **math**: 数学工具函数
- **earth**: 地球坐标转换和几何计算

## 项目结构

```
yAlgo/
├── CMakeLists.txt        # 主 CMake 配置文件
├── CMakePresets.json     # CMake 预设配置
├── sdk/                  # SDK 目录
│   ├── log/             # 日志模块
│   ├── utils/           # 工具模块
│   ├── math/            # 数学模块
│   └── earth/           # 地球坐标模块
├── examples/            # 示例代码
│   ├── log/             # 日志模块示例
│   ├── utils/           # 工具模块示例
│   ├── math/            # 数学模块示例
│   └── earth/           # 地球坐标模块示例
├── tests/               # 测试套件
│   ├── unit/            # 单元测试
│   │   ├── test_log.cpp
│   │   ├── test_utils.cpp
│   │   ├── test_math.cpp
│   │   └── test_earth.cpp
│   └── integration/     # 集成测试
│       └── test_integration.cpp
├── build/               # 构建目录
├── bin/                 # 可执行文件输出目录
└── lib/                 # 库文件输出目录
```

## 编译项目

### 步骤
1. **创建构建目录**：`mkdir build`
2. **生成构建系统**：`cd build && cmake ..`
3. **编译 Release 版本**：`cmake --build . --config Release`
4. **编译 Debug 版本**：`cmake --build . --config Debug`

### 运行测试
```bash
# 运行所有测试
ctest

# 详细输出
ctest -V

# 运行单元测试
ctest -R unit

# 运行集成测试
ctest -R integration
```

### 编译结果

- **Windows 系统**：
  - Release 版本：`bin/windows/release/` 和 `lib/windows/release/`
  - Debug 版本：`bin/windows/debug/` 和 `lib/windows/debug/`（带有 "d" 后缀，如 `yalgo_earthd.lib`）
- **macOS 系统**：
  - Release 版本：`bin/macos/release/` 和 `lib/macos/release/`
  - Debug 版本：`bin/macos/debug/` 和 `lib/macos/debug/`（带有 "d" 后缀，如 `yalgo_earthd.lib`）
- **Linux 系统**：
  - Release 版本：`bin/linux/release/` 和 `lib/linux/release/`
  - Debug 版本：`bin/linux/debug/` 和 `lib/linux/debug/`（带有 "d" 后缀，如 `yalgo_earthd.lib`）
- **Ubuntu 系统**：
  - Release 版本：`bin/ubuntu/release/` 和 `lib/ubuntu/release/`
  - Debug 版本：`bin/ubuntu/debug/` 和 `lib/ubuntu/debug/`（带有 "d" 后缀，如 `yalgo_earthd.lib`）
- **麒麟系统**：
  - ARM 架构：`bin/kylin_arm/release/` 和 `lib/kylin_arm/release/`
  - x86 架构：`bin/kylin_x86/release/` 和 `lib/kylin_x86/release/`
  - Debug 版本：相应目录下带有 "d" 后缀

## 模块功能

### 1. 日志模块 (log)
- 异步日志系统
- 支持控制台和文件输出
- 支持模块日志
- 支持流式日志
- 支持动态日志级别调整
- 支持多线程日志

### 2. 工具模块 (utils)
- 获取可执行文件路径
- 获取可执行文件目录
- 拼接可执行文件目录下的文件路径
- 检查可执行文件目录下的文件是否存在
- 获取当前系统类型（"windows"、"linux" 或 "macos"）

### 3. 数学模块 (math)
- 数学工具函数

### 4. 地球坐标模块 (earth)
- WGS84 到 UTM 坐标转换
- WGS84 到墨卡托坐标转换
- 点是否在多边形内判断
- 投影精度统计
- GEO 卫星中国区域覆盖测试

## 使用示例

### 日志模块示例
```cpp
// 初始化日志系统
yalgo::log::LogConfig config;
config.runtime_level = yalgo::log::LogLevel::INFO;
config.enable_console = true;
config.enable_file = true;
yalgo::log::AsyncLogger::getInstance().init(config);

// 基本日志输出
YLOG_INFO("这是一条信息日志");
YLOG_WARN("这是一条警告日志");
YLOG_ERROR("这是一条错误日志");
YLOG_DEBUG("这是一条调试日志");
```

### 工具模块示例
```cpp
// 获取可执行文件路径
std::string exe_path = yalgo::utils::ExecPathUtils::getExecutablePath();

// 获取可执行文件目录
std::string exe_dir = yalgo::utils::ExecPathUtils::getExecutableDir();

// 拼接文件路径
std::string config_path = yalgo::utils::ExecPathUtils::getFileInExeDir("config.json");

// 获取当前系统类型
std::string system_type = yalgo::utils::ExecPathUtils::getSystemType();
std::cout << "当前系统类型: " << system_type << std::endl;

// 根据系统类型执行不同的命令
if (system_type == "windows") {
    // Windows 命令
    system("dir");
} else {
    // Linux/macOS 命令
    system("ls -la");
}
```

### 地球坐标模块示例
```cpp
// 创建地球点
EarthPoint point(116.4, 39.9); // 北京

// 坐标转换
EarthConverter converter;
EarthConverter::UTMCoordinate utm = converter.wgs84ToUTM(point);
EarthConverter::MercatorCoordinate merc = converter.wgs84ToMercator(point);

// 判断点是否在多边形内
std::vector<EarthPoint> polygon = { /* 多边形顶点 */ };
EarthGeometry geometry;
bool isInside = geometry.isPointInPolygon(point, polygon, ProjectionType::UTM);
```

## 注意事项

1. **编码问题**：项目使用 UTF-8 编码，已在 CMakeLists.txt 中配置 MSVC 使用 `/utf-8` 选项
2. **依赖**：项目使用纯 C++ 实现，不依赖外部库
3. **平台**：支持 Windows、macOS 和 Linux

## 常见问题

### 编译错误
- **编码错误**：确保使用 UTF-8 编码，MSVC 已配置 `/utf-8` 选项
- **缺少依赖**：项目使用纯 C++，无需额外依赖

### 运行错误
- **日志文件路径**：确保有写入权限
- **坐标范围**：墨卡托投影仅支持纬度 -85.05 到 85.05 度

## Doxygen 注释风格

### 文件注释
```cpp
/**
 * @file filename.h
 * @brief 文件描述
 * @author 作者
 * @date 日期
 */
```

### 类注释
```cpp
/**
 * @class ClassName
 * @brief 类描述
 * 
 * 详细描述
 */
class ClassName {
    // 类成员
};
```

### 函数注释
```cpp
/**
 * @brief 函数描述
 * @param param1 参数1描述
 * @param param2 参数2描述
 * @return 返回值描述
 * @throws 异常描述
 */
return_type function_name(param1_type param1, param2_type param2);
```

### 变量注释
```cpp
/**
 * @brief 变量描述
 */
variable_type variable_name;

// 或者使用行内注释
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

### 结构体注释
```cpp
/**
 * @struct StructName
 * @brief 结构体描述
 */
struct StructName {
    /**
     * @brief 成员描述
     */
    member_type member_name;
};
```