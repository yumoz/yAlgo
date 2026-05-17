# yAlgo CLI & MCP 使用指南

## 目录

1. [概述](#1-概述)
2. [CLI 命令行工具](#2-cli-命令行工具)
   - [2.1 编译](#21-编译)
   - [2.2 使用方式](#22-使用方式)
   - [2.3 子命令参考](#23-子命令参考)
3. [MCP Server](#3-mcp-server)
   - [3.1 环境准备](#31-环境准备)
   - [3.2 启动方式](#32-启动方式)
   - [3.3 可用工具](#33-可用工具)
4. [常见问题](#4-常见问题)

---

## 1. 概述

yAlgo SDK 提供了两种方式将核心算法能力暴露给外部使用：

| 方式 | 可执行文件 | 适用场景 |
|------|-----------|---------|
| **CLI** | `yalk.exe` | 命令行调用、脚本集成、手动调试、CI/CD |
| **MCP** | `mcp_server.py` | AI 助手集成（Claude Desktop、Cursor 等） |

两种方式的关系：

```
┌─────────────────────────────────────────────────┐
│                MCP Client (AI 助手)               │
└──────────────────┬──────────────────────────────┘
                   │ JSON-RPC (stdio)
┌──────────────────▼──────────────────────────────┐
│           MCP Server (Python)                    │
│        tools/mcp_server.py                      │
│        通过 subprocess 调用 CLI                  │
└──────────────────┬──────────────────────────────┘
                   │ stdin/stdout
┌──────────────────▼──────────────────────────────┐
│            CLI 工具 (yalk.exe)                    │
│         编译自 examples/cli/main.cpp             │
│         直接调用 SDK 动态库                       │
└──────────────────┬──────────────────────────────┘
                   │ API 调用
┌──────────────────▼──────────────────────────────┐
│         yAlgo SDK 动态库 (DLL/SO)                │
│   yalgo_earth  yalgo_math  yalgo_utils           │
└─────────────────────────────────────────────────┘
```

---

## 2. CLI 命令行工具

CLI 工具名称为 `yalk`（yAlgo Kit 的缩写），将所有 SDK 算法封装为命令行子命令，输出格式为 **JSON**，便于脚本解析和集成。

### 2.1 编译

#### 前置条件

- CMake >= 3.10
- MinGW 或 Visual Studio 编译器
- yAlgo SDK 已编译（yalgo_earth, yalgo_math, yalgo_utils 动态库）

#### 编译步骤

```bash
# 进入项目根目录
cd yAlgo

# 创建构建目录（如已存在可跳过）
mkdir build-mingw
cd build-mingw

# 配置 CMake
cmake .. -G "MinGW Makefiles"

# 仅编译 CLI 工具
mingw32-make -j4 yalk

# 或编译所有目标
mingw32-make -j4
```

编译完成后，可执行文件位于 `build-mingw/bin/yalk.exe`。

#### 依赖库

运行 `yalk.exe` 需要以下动态库在同一目录或系统 PATH 中：

| 动态库 | 对应 SDK 模块 | 查找路径 |
|--------|-------------|---------|
| `libyalgo_earth.dll` | earth（地球坐标） | `build-mingw/sdk/earth/` |
| `libyalgo_math.dll` | math（数学工具） | `build-mingw/sdk/math/` |
| `libyalgo_utils.dll` | utils（系统工具） | `build-mingw/sdk/utils/` |

**提示：** 编译后可用以下命令将动态库复制到同一目录：

```bash
cp sdk/earth/*.dll bin/
cp sdk/math/*.dll bin/
cp sdk/utils/*.dll bin/
```

### 2.2 使用方式

```bash
# 查看帮助
yalk --help
yalk -h

# 查看模块帮助
yalk earth --help
yalk math --help
yalk utils --help

# 查看子命令帮助
yalk earth distance --help
```

所有命令的输出均为 JSON 格式：

```json
{"status": "success", ...}   // 成功
{"status": "error", "message": "..."}  // 失败
```

### 2.3 子命令参考

#### earth 模块

##### `yalk earth distance`

计算两点间的地表距离。

```bash
yalk earth distance --from <经度,纬度> --to <经度,纬度> [--type <类型>]
```

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `--from` | string | 是 | 起点经纬度，格式 `116.4,39.9` |
| `--to` | string | 是 | 终点经纬度 |
| `--type` | string | 否 | 距离算法：`haversine`（默认）、`vincenty`、`straight` |

**示例：**
```bash
$ yalk earth distance --from 116.4,39.9 --to 121.5,31.2
{"distance": 1071285.78, "method": "haversine", "unit": "m"}

$ yalk earth distance --from 116.4,39.9 --to 121.5,31.2 --type vincenty
{"distance": 1070961.26, "method": "vincenty", "unit": "m"}

$ yalk earth distance --from 116.4,39.9 --to 121.5,31.2 --type straight
{"distance": 1065504.50, "method": "straight", "unit": "m"}
```

##### `yalk earth convert`

坐标系转换（WGS84 → ECEF / UTM / Mercator）。

```bash
yalk earth convert --input <经度,纬度,高度> --from wgs84 --to <目标坐标系>
```

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `--input` | string | 是 | 经纬高，格式 `116.4,39.9.9,50` |
| `--from` | string | 是 | 源坐标系（当前仅支持 `wgs84`） |
| `--to` | string | 是 | 目标坐标系：`ecef`、`utm`、`mercator` |

**示例：**
```bash
$ yalk earth convert --input 116.4,39.9,50 --from wgs84 --to utm
{"easting": 448709.38, "northing": 4416830.56, "zone": 50, "hemisphere": "N"}

$ yalk earth convert --input 116.4,39.9,50 --from wgs84 --to mercator
{"x": 12962197.46, "y": 4851743.42}

$ yalk earth convert --input 116.4,39.9,50 --from wgs84 --to ecef
{"x": -2158355.76, "y": 4384342.34, "z": 4077985.57}
```

##### `yalk earth bearing`

计算两点间的方位角。

```bash
yalk earth bearing --from <经度,纬度> --to <经度,纬度>
```

**示例：**
```bash
$ yalk earth bearing --from 116.4,39.9 --to 121.5,31.2
{"bearing": 161.45, "unit": "deg"}
```

##### `yalk earth is-visible`

判断两点是否通视（考虑地球曲率）。

```bash
yalk earth is-visible --from <经度,纬度,高度> --to <经度,纬度,高度>
```

**示例：**
```bash
$ yalk earth is-visible --from 116.4,39.9,100 --to 121.5,31.2,50
{"is_visible": false, "from": {...}, "to": {...}}
```

##### `yalk earth is-point-in-polygon`

判断点是否在地理多边形内部。

```bash
yalk earth is-point-in-polygon --point <经度,纬度> --polygon <JSON> [--projection <投影类型>]
```

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `--point` | string | 是 | 待判断点，格式 `116.4,39.9` |
| `--polygon` | string | 是 | 多边形顶点 JSON 数组：`"[[lng1,lat1],[lng2,lat2],...]"` |
| `--projection` | string | 否 | 投影方式：`utm`（默认）、`mercator` |

**示例：**
```bash
$ yalk earth is-point-in-polygon --point 116.4,39.9 --polygon "[[100,50],[130,50],[130,20],[100,20],[100,50]]"
{"is_inside": true, "projection": "utm"}

$ yalk earth is-point-in-polygon --point 140.0,35.0 --polygon "[[100,50],[130,50],[130,20],[100,20],[100,50]]"
{"is_inside": false, "projection": "utm"}
```

---

#### math 模块

##### `yalk math angle`

角度与弧度互转。

```bash
yalk math angle --value <数值> --from <单位> --to <单位>
```

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `--value` | float | 是 | 待转换数值 |
| `--from` | string | 是 | 源单位：`deg`（度）或 `rad`（弧度） |
| `--to` | string | 是 | 目标单位：`deg` 或 `rad` |

**示例：**
```bash
$ yalk math angle --value 180 --from deg --to rad
{"result": 3.141592653589793, "unit": "rad"}

$ yalk math angle --value 3.14159 --from rad --to deg
{"result": 180.0, "unit": "deg"}
```

##### `yalk math power`

功率单位转换。

```bash
yalk math power --value <数值> --from <单位> --to <单位>
```

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `--value` | float | 是 | 待转换数值 |
| `--from` | string | 是 | 源单位：`w`、`kw`、`hp`、`dbw`、`dbm` |
| `--to` | string | 是 | 目标单位：同上 |

**示例：**
```bash
$ yalk math power --value 10 --from w --to dbm
{"result": 40.0, "unit": "dbm"}

$ yalk math power --value 1000 --from w --to kw
{"result": 1.0, "unit": "kw"}

$ yalk math power --value 40 --from dbm --to w
{"result": 10.0, "unit": "w"}
```

---

#### utils 模块

##### `yalk utils exec-path`

获取可执行文件路径信息。

```bash
yalk utils exec-path
```

**示例：**
```bash
$ yalk utils exec-path
{"executable_path": "E:\\...\\yalk.exe", "executable_dir": "E:\\...\\bin"}
```

##### `yalk utils system-type`

获取当前系统类型。

```bash
yalk utils system-type
```

**示例：**
```bash
$ yalk utils system-type
{"system_type": "windows"}
```

---

## 3. MCP Server

MCP（Model Context Protocol）Server 是一个 Python 脚本，将 CLI 工具包装为 MCP 协议格式，让 AI 助手（如 Claude Desktop、Cursor 等）可以直接调用 yAlgo SDK 的功能。

### 3.1 环境准备

#### 安装 Python

需要 Python 3.8 或更高版本。

```bash
python --version
```

#### 安装 MCP Python 库

```bash
pip install mcp
```

如使用 `pip` 安装失败，可尝试：

```bash
python -m pip install mcp
```

#### 指定 yalk 可执行文件路径

MCP Server 默认查找 `yalk.exe` 的路径为 `../build-mingw/bin/yalk.exe`（相对于脚本所在目录）。

如 `yalk.exe` 在不同位置，可通过环境变量指定：

```bash
# Windows (PowerShell)
$env:YALK_PATH = "D:\path\to\yalk.exe"

# Windows (CMD)
set YALK_PATH=D:\path\to\yalk.exe

# Linux / macOS
export YALK_PATH=/path/to/yalk
```

### 3.2 启动方式

#### 直接运行

```bash
cd yAlgo
python tools/mcp_server.py
```

启动后，MCP Server 会在 stdio 上监听 JSON-RPC 请求，等待客户端连接。

#### 在 Claude Desktop 中配置

编辑 Claude Desktop 配置文件（通常位于 `%APPDATA%\Claude\claude_desktop_config.json`）：

```json
{
  "mcpServers": {
    "yAlgo": {
      "command": "python",
      "args": ["E:\\Code\\LearnCode\\cpp_prj\\yAlgo\\tools\\mcp_server.py"],
      "env": {
        "YALK_PATH": "E:\\Code\\LearnCode\\cpp_prj\\yAlgo\\build-mingw\\bin\\yalk.exe"
      }
    }
  }
}
```

#### 在 Cursor 中配置

在 Cursor 的 MCP 配置中添加：

```json
{
  "mcpServers": {
    "yAlgo": {
      "command": "python",
      "args": ["E:/Code/LearnCode/cpp_prj/yAlgo/tools/mcp_server.py"]
    }
  }
}
```

### 3.3 可用工具

MCP Server 暴露了 **9 个工具**，对应 CLI 的子命令：

#### 地球坐标工具

| 工具名称 | 功能描述 | 参数 | 对应 CLI 命令 |
|---------|---------|------|-------------|
| `earth_distance` | 计算两点地面距离 | `from_lon`, `from_lat`, `to_lon`, `to_lat`, `type`（可选） | `yalk earth distance` |
| `earth_convert` | WGS84 坐标转换 | `lon`, `lat`, `alt`（可选）, `to` | `yalk earth convert` |
| `earth_bearing` | 计算方位角 | `from_lon`, `from_lat`, `to_lon`, `to_lat` | `yalk earth bearing` |
| `earth_is_visible` | 判断两点通视 | `from_lon`, `from_lat`, `from_alt`, `to_lon`, `to_lat`, `to_alt` | `yalk earth is-visible` |
| `earth_is_point_in_polygon` | 点在多边形判断 | `point_lon`, `point_lat`, `polygon`, `projection`（可选） | `yalk earth is-point-in-polygon` |

#### 数学工具

| 工具名称 | 功能描述 | 参数 | 对应 CLI 命令 |
|---------|---------|------|-------------|
| `math_angle_convert` | 角度/弧度转换 | `value`, `from`, `to` | `yalk math angle` |
| `math_power_convert` | 功率单位转换 | `value`, `from`, `to` | `yalk math power` |

#### 系统工具

| 工具名称 | 功能描述 | 参数 | 对应 CLI 命令 |
|---------|---------|------|-------------|
| `utils_exec_path` | 获取可执行路径 | 无 | `yalk utils exec-path` |
| `utils_system_type` | 获取系统类型 | 无 | `yalk utils system-type` |

---

## 4. 常见问题

### Q1: yalk.exe 提示 "无法启动，找不到动态库"

**原因：** SDK 动态库（`libyalgo_earth.dll` 等）不在 `yalk.exe` 所在目录或系统 PATH 中。

**解决：** 将动态库复制到同一目录：

```bash
# 从构建目录复制
cp build-mingw/sdk/earth/*.dll build-mingw/bin/
cp build-mingw/sdk/math/*.dll build-mingw/bin/
cp build-mingw/sdk/utils/*.dll build-mingw/bin/
```

### Q2: MCP Server 提示 "yalk executable not found"

**原因：** 找不到 `yalk.exe`。

**解决：** 设置 `YALK_PATH` 环境变量指向正确的 `yalk.exe` 路径：

```bash
$env:YALK_PATH = "E:\Code\LearnCode\cpp_prj\yAlgo\build-mingw\bin\yalk.exe"
```

### Q3: pip install mcp 库安装失败

**原因：** Python 环境问题。

**解决：**
```bash
python -m pip install --upgrade pip
python -m pip install mcp
```

### Q4: CLI 输出格式非 JSON

**原因：** 内部错误或异常。

**解决：** 检查参数是否正确，使用 `--help` 查看命令用法。如确认参数正确仍有问题，请检查 SDK 动态库版本是否匹配。

### Q5: 如何在脚本中集成 CLI？

**Bash 脚本示例：**
```bash
RESULT=$(yalk earth distance --from 116.4,39.9 --to 121.5,31.2)
DISTANCE=$(echo $RESULT | python -c "import sys,json; print(json.load(sys.stdin)['distance'])")
echo "距离: $DISTANCE 米"
```

**Python 脚本示例：**
```python
import subprocess
import json

result = subprocess.run(
    ["yalk", "earth", "convert", "--input", "116.4,39.9,50",
     "--from", "wgs84", "--to", "utm"],
    capture_output=True, text=True
)
data = json.loads(result.stdout)
print(f"UTM 坐标: {data['easting']}, {data['northing']}")
```