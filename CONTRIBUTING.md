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
