# 贡献指南

## 开发流程

1. Fork 本仓库
2. 创建 feature 分支：`git checkout -b feat/your-feature`
3. 提交变更
4. 确保通过 CI（GitHub Actions）
5. 提交 Pull Request 到 `main` 分支

## 代码规范

### C++

- 使用 C++17 标准
- 命名空间 `yalgo::<module>`
- 类名使用 PascalCase，函数/变量使用 camelCase
- Doxygen 风格注释（参考现有代码）
- 头文件使用 `#pragma once`

### 提交信息

```
<type>: <简短描述>

<详细说明（可选）>
```

类型：`feat`、`fix`、`docs`、`refactor`、`test`、`chore`

### Pull Request

- 标题清晰描述变更内容
- 关联相关 Issue（如有）
- 确保 CI 全部通过

## 构建与测试

```bash
# 配置
cmake --preset windows-release

# 构建
cmake --build --preset windows-release

# 测试
ctest --build-config Release
```

## RPC 模块开发

### 依赖安装

RPC 模块依赖 gRPC/Protobuf，有两种方式：

1. **系统包安装**（推荐用于开发）：
   ```bash
   # macOS
   brew install grpc protobuf
   # Ubuntu
   apt install libgrpc-dev libprotobuf-dev
   ```

2. **自动下载**：未安装系统包时，CMake 会通过 FetchContent 自动下载 gRPC 源码编译

### 自定义查找路径

```bash
cmake -DGRPC_ROOT=/path/to/grpc -DPROTOBUF_ROOT=/path/to/protobuf ..
```

或设置环境变量：
```bash
export GRPC_ROOT=/path/to/grpc
export PROTOBUF_ROOT=/path/to/protobuf
```

### Proto 文件编译

在 `examples/rpc/` 目录下的 `.proto` 文件会通过 `FindYalgoGrpc` 提供的 `YALGO_PROTOC_EXECUTABLE` 和 `YALGO_GRPC_CPP_PLUGIN` 自动编译。

### 测试 RPC 模块

```bash
# 运行 RPC 单元测试
ctest -R unit_rpc --build-config Release

# 运行 RPC 示例
./bin/<os>/release/rpc_server [port]   # 终端1
./bin/<os>/release/rpc_client [host:port]  # 终端2
```
