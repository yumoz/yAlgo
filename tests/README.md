# yAlgo 测试套件

## 目录结构

```
tests/
├── unit/           # 单元测试 - 测试各个模块的独立功能
└── integration/    # 集成测试 - 测试模块间的协作
```

## 运行测试

```bash
# 进入构建目录
cd build

# 运行所有测试
ctest

# 详细输出
ctest -V

# 运行特定测试
ctest -R unit
```

## 添加新测试

1. 在对应目录创建 `test_xxx.cpp` 文件
2. 在 `CMakeLists.txt` 中添加测试目标
3. 使用 `add_test()` 注册测试
