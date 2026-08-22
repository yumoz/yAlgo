# 一键编译脚本使用指南

项目提供两个一键编译脚本，封装了 `cmake --preset` 配置与 `cmake --build --preset` 编译流程，支持 Windows 下 **Debug / Release** 环境的快速构建。

| 脚本 | 运行环境 | 编码要求 |
|---|---|---|
| `build.ps1` | PowerShell 5.1+ / pwsh | UTF-8 BOM（已内置，勿转编码） |
| `build.bat` | Windows 命令提示符 (cmd) | ANSI/GBK（勿转 UTF-8，否则中文乱码） |

两个脚本行为一致，按你的习惯二选一即可。默认编译 **windows-debug**，可通过参数切换为 release。

## 参数

| 参数 | 说明 | 默认值 | 可选值 |
|---|---|---|---|
| `-Platform` / 第 1 参数 | 目标平台 | `windows` | `windows`、`linux`、`macos`、`kylin` |
| `-Arch` / 第 2 参数 | CPU 架构（仅 kylin 生效） | `arm` | `arm`（默认）、`x86` |
| `-Config` / 第 3 参数 | 构建类型 | `debug` | `debug`、`release` |
| `-Clean` / 第 4 参数 | 先删除对应 `build/<preset>` 目录 | 否 | `-Clean` |
| `-Jobs` / 第 5 参数 | 并行编译线程数（`0` = cmake 默认） | `0` | `-Jobs 8` |
| `-Install` | 编译后执行 `cmake --install` | 否 | `-Install` |
| `-NoTest` | 跳过末尾自动 CTest | 否 | `-NoTest` |
| `-Help` | 打印用法并退出 | 否 | `-Help`（`build.bat` 也可 `-h` / `/?`） |

## 预设映射

脚本将 **平台 + 架构 + 配置** 组合成 `CMakePresets.json` 中的预设名：

| Platform | Arch | Config | CMake Preset |
|---|---|---|---|
| windows | - | debug | `windows-debug` |
| windows | - | release | `windows-release` |
| linux | - | debug | `linux-debug` |
| linux | - | release | `linux-release` |
| macos | - | debug | `macos-debug` |
| macos | - | release | `macos-release` |
| kylin | arm（默认） | debug | `kylin-arm-debug` |
| kylin | arm（默认） | release | `kylin-arm-release` |
| kylin | x86 | debug | `kylin-x86-debug` |
| kylin | x86 | release | `kylin-x86-release` |

## PowerShell 用法（`build.ps1`）

```powershell
# 默认 windows + debug
.\build.ps1

# 指定配置
.\build.ps1 -Config release          # windows-release
.\build.ps1 -Config debug -Clean     # 清理后重建 windows-debug

# 指定平台
.\build.ps1 -Platform linux          # linux-debug
.\build.ps1 -Platform macos -Config release

# Kylin：默认 arm；x86 需指定 -Arch
.\build.ps1 -Platform kylin -Config release        # kylin-arm-release
.\build.ps1 -Platform kylin -Arch x86 -Config release  # kylin-x86-release
```

## 命令提示符用法（`build.bat`）

```bat
# 顺序：Platform  Arch  Config  -Clean
build.bat                              # windows / arm / debug
build.bat windows "" release           # windows-release
build.bat linux "" debug -Clean        # 清理后重建 linux-debug

build.bat macos "" release             # macos-release
build.bat kylin arm release            # kylin-arm-release
build.bat kylin x86 release            # kylin-x86-release
```

> 注意：`.bat` 的参数是**位置顺序**（Platform、Arch、Config、`-Clean`），空占位用 `""`。PowerShell 版可用具名参数，顺序任意。

## 查看帮助

```powershell
.\build.ps1 -Help
```

```bat
build.bat -Help
```

会打印参数说明、预设映射表与示例后退出，不执行任何 cmake 命令。

## 自动运行 CTest

编译成功后，脚本会**自动运行 CTest**，但仅当**目标平台与当前宿主机 OS 一致**时才执行：

| 目标平台 | 宿主机 | 是否自动测 |
|---|---|---|
| windows | Windows | 是 |
| linux | Linux | 是 |
| macos | macOS | 是 |
| kylin | Linux | 是（kylin 视为 Linux 系） |
| linux / macos / kylin | Windows | 否（跳过，打印提示） |

跳过的原因：在 Windows 上配置 `linux-debug` 等预设通常只是生成交叉/远程构建工程，本机无法直接运行其测试。若宿主机平台不匹配，脚本会打印 `Skip CTest` 而不是失败退出。

如需手动运行测试：

```bash
ctest --build-config Debug  --test-dir build/windows-debug
ctest --build-config Release --test-dir build/windows-release
```

如果只想编译、不跑测试，加 `-NoTest`：

```powershell
.\build.ps1 -NoTest
.\build.ps1 -Config release -NoTest
```

```bat
build.bat debug -NoTest
build.bat windows "" release -NoTest
```

## 并行编译（-Jobs）

`-Jobs N` 会把 `-j N` 传给 `cmake --build`，加速编译。`0`（默认）表示不指定、由 cmake 自行决定（Visual Studio 生成器默认并行，Ninja/Make 默认串行）。

```powershell
.\build.ps1 -Jobs 8                 # 8 线程并行
.\build.ps1 -Platform linux -Jobs 16
```

```bat
build.bat windows "" release -Jobs 8
build.bat -Platform linux -Jobs 16
```

## 安装（-Install）

加 `-Install` 会在编译成功后执行 `cmake --install <buildDir>`，产物安装到 `install/` 前缀目录（等价于 `cmake --install build`）。

```powershell
.\build.ps1 -Install
.\build.ps1 -Config release -Install -NoTest
```

```bat
build.bat debug -Install
build.bat windows "" release -Install
```

## 脚本做了什么

1. 以脚本所在目录作为项目根目录。
2. 检查 `cmake` 是否在 `PATH` 中，缺失则报错退出。
3. （可选）`-Clean` 删除对应的 `build/<preset>` 目录。
4. `cmake --preset <preset>` 完成配置。
5. `cmake --build --preset <preset>` 完成编译。
6. 任一步骤失败立即退出，末尾打印完成提示。

## 构建产物

编译完成后，产物位于：

- 可执行文件 / DLL：`bin/windows/<debug|release>/`
- 导入库：`lib/windows/<debug|release>/`

Debug 构建的库文件带 `d` 后缀（如 `yalgo_logd.dll`）。

## 常见问题

- **`build.ps1` 报“字符串缺少终止符 / 缺少右 }”**：通常是文件被存成无 BOM 的 UTF-8 且 Windows PowerShell 按 GBK 读取导致中文乱码。本仓库的 `build.ps1` 为纯 ASCII + UTF-8 BOM，正常无此问题；若你用编辑器另存过，请恢复 UTF-8 BOM 或重新拉取。
- **`build.bat` 中文乱码**：请确保文件以 ANSI/GBK 编码保存，不要转成 UTF-8。
- **想编译 Linux / macOS / Kylin 预设**：当前脚本仅封装 Windows 预设。这些平台请直接使用 `cmake --preset <name>`（详见 `CMakePresets.json`）。后续可扩充脚本支持 `-Platform` 参数。

## 与手工命令等价

```bash
# 等价于 .\build.ps1 -Config release
cmake --preset windows-release
cmake --build --preset windows-release
```
