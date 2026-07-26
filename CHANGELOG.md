# Changelog

## 1.1.0 (2026-07-26)

### 新增
- RPC 模块 `yalgo_rpc`：gRPC 双向流式通信封装（GrpcServer、GrpcClient、LogInterceptor）
- `FindYalgoGrpc.cmake`：统一 gRPC/Protobuf 依赖查找（系统包优先 + FetchContent 降级）
- RPC 示例：独立服务端 (`rpc_server`) + 独立客户端 (`rpc_client`) + 集成测试 (`rpc_example`)
- `unit_rpc` 单元测试：10 个测试用例覆盖 server/client 工具函数和完整通信

### 变更
- 依赖管理重构：gRPC/Protobuf 查找逻辑统一封装到 `cmake/FindYalgoGrpc.cmake`
- 移除各 CMakeLists.txt 中重复的 `find_package(Protobuf/gRPC)` 和 macOS 头文件修复代码
- 移除 protoc 路径硬编码，改用 `YALGO_PROTOC_EXECUTABLE` 变量

## 1.0.0 (2026-03-24)

### 新增
- 异步日志模块 `yalgo_log`：控制台/文件/Syslog 输出、日志轮转、ANSI 颜色、性能统计
- 数学工具模块 `yalgo_math`：角度弧度转换、功率单位换算、分贝计算
- 工具模块 `yalgo_utils`：可执行文件路径、系统类型检测
- 地球坐标模块 `yalgo_earth`：WGS84↔ECEF↔UTM↔墨卡托转换、多椭球体、多边形包含、通视判断
- CMake Presets 支持：Windows、Linux、macOS、Kylin（ARM/x86）
- GitHub Actions CI：MSVC、GCC、Clang 三矩阵构建与测试
- 自定义 CMake 模块：YuMakeSDK、YuMakeApp、YuPath、YuLibInstall
