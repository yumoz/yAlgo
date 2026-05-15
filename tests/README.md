# yAlgo 测试套件

## 目录结构

```
tests/
├── unit/           # 单元测试
│   ├── test_log.cpp
│   ├── test_utils.cpp
│   ├── test_math.cpp
│   └── test_earth.cpp
├── integration/    # 集成测试
│   └── test_integration.cpp
├── CMakeLists.txt
└── README.md
```

## 已注册的测试

| 测试名 | 类型 | 测试目标 | 说明 |
|---|---|---|---|
| `unit_log` | 单元测试 | `test_log` | 日志模块 |
| `unit_utils` | 单元测试 | `test_utils` | 工具模块 |
| `unit_math` | 单元测试 | `test_math` | 数学模块 |
| `unit_earth` | 单元测试 | `test_earth` | 地球坐标模块 |
| `integration_modules` | 集成测试 | `test_integration` | 多模块联合测试 |

## 运行测试

在项目构建目录下执行：

```bash
# 运行所有测试
ctest

# 详细输出
ctest -V

# 指定构建配置（多配置生成器如 Visual Studio 需要）
ctest --build-config Release
ctest --build-config Debug

# 仅运行单元测试
ctest -R unit

# 仅运行集成测试
ctest -R integration
```

## 添加新测试

1. 在 `tests/unit/` 或 `tests/integration/` 下创建 `test_xxx.cpp` 文件（`main` 函数返回 0 为通过，非 0 为失败）
2. 编辑对应目录的 `CMakeLists.txt`，添加测试目标
3. 使用 `add_test(NAME <name> COMMAND <target>)` 注册到 CTest
4. 重新配置并构建：`cmake --build <build_dir> && ctest`
