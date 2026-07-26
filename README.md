# yAlgo

![CI](https://github.com/your-org/yAlgo/actions/workflows/cmake-multi-platform.yml/badge.svg)

C++17 工具库/SDK 集合，包含日志系统、数学工具、地球坐标转换和通用工具模块。

## 模块

| 模块 | 库 | 说明 |
|---|---|---|
| **log** | `yalgo_log` | 高性能异步日志系统，支持控制台/文件/Syslog 三路输出、日志轮转、ANSI 颜色 |
| **math** | `yalgo_math` | 数学工具：角度弧度转换、功率单位换算（W/kW/hp/dBW/dBm）、通用分贝计算 |
| **utils** | `yalgo_utils` | 工具函数：可执行文件路径获取、系统类型检测 |
| **earth** | `yalgo_earth` | 地球坐标：WGS84↔ECEF↔UTM↔墨卡托转换、多椭球体、多边形包含、通视判断 |
| **rpc** | `yalgo_rpc` | gRPC 通信封装：双向流式、客户端/服务端、拦截器、proto 编译工具 |

## 快速开始

### 前提条件

- CMake 3.10+（推荐 3.14+）
- C++17 编译器（MSVC、GCC、Clang）
- Ninja 或 Visual Studio 2022（可选，用于加速构建）
- gRPC/Protobuf（rpc 模块需要，可选）
  - macOS: `brew install grpc protobuf`
  - Ubuntu: `apt install libgrpc-dev libprotobuf-dev`
  - 或通过 FetchContent 自动下载（无需预装）

### 使用 CMake Presets（推荐）

```bash
# Windows（Visual Studio 17 2022）
cmake --preset windows-release
cmake --build --preset windows-release

# Linux（GCC）
cmake --preset linux-release
cmake --build --preset linux-release

# macOS（Clang）
cmake --preset macos-release
cmake --build --preset macos-release
```

支持 Debug/Release 及 Kylin ARM/x86 预设，详见 `CMakePresets.json`。

### 手动构建

```bash
# Debug
cmake -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build build

# Release
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build
```

### 安装

```bash
cmake --install build    # 安装到 <project>/install/
```

## 运行测试

```bash
# 构建完成后
ctest --build-config Release --test-dir build

# 详细输出
ctest -V --build-config Release --test-dir build
```

| 测试名 | 类型 | 说明 |
|---|---|---|
| `unit_log` | 单元测试 | 日志模块 |
| `unit_utils` | 单元测试 | 工具模块 |
| `unit_math` | 单元测试 | 数学模块 |
| `unit_earth` | 单元测试 | 地球坐标模块 |
| `unit_rpc` | 单元测试 | RPC 模块（gRPC） |
| `integration_modules` | 集成测试 | 多模块联合测试 |

## 项目结构

```
yAlgo/
├── CMakeLists.txt          # 主构建配置
├── CMakePresets.json       # 跨平台预设
├── .github/workflows/      # CI/CD（MSVC、GCC、Clang）
├── sdk/                    # SDK 源码
│   ├── log/               # yalgo_log（SHARED）
│   ├── math/              # yalgo_math（SHARED）
│   ├── utils/             # yalgo_utils（SHARED）
│   ├── earth/             # yalgo_earth（SHARED）
│   └── rpc/               # yalgo_rpc（SHARED，gRPC 通信）
├── examples/               # 示例程序
│   ├── log/               # log_example
│   ├── math/              # math_example
│   ├── utils/             # utils_example
│   ├── earth/             # earth_example
│   └── rpc/               # rpc_example + rpc_server + rpc_client
├── tests/                  # 测试套件
│   ├── unit/              # 单元测试
│   └── integration/       # 集成测试
├── cmake/                  # 自定义 CMake 模块
├── bin/                    # 编译输出（exe/dll）
├── lib/                    # 编译输出（静态导入库）
└── install/                # 安装目录
```

### 输出目录

构建产物按 `<bin|lib>/<os>/<build_type>/` 组织：

| 平台 | 路径 |
|---|---|
| Windows | `bin/windows/release/`、`lib/windows/release/` |
| Ubuntu | `bin/ubuntu/release/`、`lib/ubuntu/release/` |
| Linux | `bin/linux/release/`、`lib/linux/release/` |
| macOS | `bin/macos/release/`、`lib/macos/release/` |
| Kylin ARM | `bin/kylin_arm/release/`、`lib/kylin_arm/release/` |
| Kylin x86 | `bin/kylin_x86/release/`、`lib/kylin_x86/release/` |

Debug 构建输出到 `<os>/debug/`，库文件名附加 `d` 后缀（如 `yalgo_logd.dll`）。

## 使用示例

### 日志模块

```cpp
#include "log/async_logger.h"

yalgo::log::LogConfig config;
config.runtime_level = yalgo::log::LogLevel::INFO;
config.enable_console = true;
config.enable_file = true;
config.log_file = "app.log";
yalgo::log::AsyncLogger::getInstance().init(config);

YLOG_INFO("应用启动");
YLOG_WARN("磁盘空间不足: %zu MB", free_space);
YLOG_ERROR("连接失败: %s", err.what());
```

### 数学模块

```cpp
#include "math/math_utils.h"

double rad = yalgo::math::MathUtils::degreesToRadians(180.0);
double kw  = yalgo::math::MathUtils::wattsToKilowatts(1000.0);
double dbm = yalgo::math::MathUtils::wattsToDbm(1.0);  // 30.0
double db  = yalgo::math::MathUtils::linearToDb(100.0); // 20.0
```

### 工具模块

```cpp
#include "utils/exe_path.h"

std::string exe  = yalgo::utils::ExePath::getExecutablePath();
std::string dir  = yalgo::utils::ExePath::getExecutableDir();
std::string cfg  = yalgo::utils::ExePath::getFileInExeDir("config.json");
std::string sys  = yalgo::utils::ExePath::getSystemType(); // "windows" / "linux" / "macos"
```

### 地球坐标模块

```cpp
#include "earth/earth_point.h"
#include "earth/earth_converter.h"
#include "earth/earth_geometry.h"

// WGS84 ↔ UTM
yalgo::earth::EarthPoint beijing(116.4, 39.9);
yalgo::earth::EarthConverter conv;
auto utm = conv.wgs84ToUTM(beijing);
auto back = conv.utmToWGS84(utm);

// 墨卡托投影
auto merc = conv.wgs84ToMercator(beijing);
auto recovered = conv.mercatorToWGS84(merc);

// 多边形包含判断
yalgo::earth::EarthGeometry geo;
std::vector<yalgo::earth::EarthPoint> polygon = { ... };
bool inside = geo.isPointInPolygon(beijing, polygon, ProjectionType::UTM);
```

### RPC 模块

```cpp
#include "rpc/grpc_server.h"
#include "rpc/grpc_client.h"

// 服务端
yalgo::rpc::GrpcServer server("0.0.0.0:50051");
server.registerService(&myServiceImpl);
server.startAsync();
server.waitForShutdown();

// 客户端
yalgo::rpc::GrpcClient client("localhost:50051");
client.waitForReady(5);
auto stub = MyService::NewStub(client.getChannel());
```

## 自定义 CMake 模块

`cmake/` 目录提供可复用的 CMake 模块：

| 模块 | 说明 |
|---|---|
| `FindYalgoGrpc.cmake` | 统一查找 gRPC/Protobuf 依赖（系统包优先，FetchContent 降级） |
| `YuMakeSDK.cmake` | 构建 SHARED 库目标，自动收集源码、配置 C++17、设置输出目录 |
| `YuMakeApp.cmake` | 构建可执行目标，配置链接和输出目录 |
| `YuPath.cmake` | 路径工具函数（获取当前/父目录、规范化、相对路径） |
| `YuLibInstall.cmake` | 统一安装函数：`yutils_install_lib()` 安装库+头文件，`yutils_install_app()` 安装可执行文件+配置文件 |
| `yutils.cmake` | 主入口，聚合以上所有模块 |

## CI/CD

项目使用 GitHub Actions，配置见 `.github/workflows/cmake-multi-platform.yml`。

| 运行环境 | 编译器 | 构建类型 |
|---|---|---|
| `windows-latest` | MSVC cl | Release |
| `ubuntu-latest` | GCC g++ | Release |
| `ubuntu-latest` | Clang clang++ | Release |

流水线：Checkout → CMake Configure → Build → CTest。

## 许可证

MIT License
