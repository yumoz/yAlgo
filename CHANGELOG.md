# Changelog

## 1.0.0 (2026-03-24)

### 新增
- 异步日志模块 `yalgo_log`：控制台/文件/Syslog 输出、日志轮转、ANSI 颜色、性能统计
- 数学工具模块 `yalgo_math`：角度弧度转换、功率单位换算、分贝计算
- 工具模块 `yalgo_utils`：可执行文件路径、系统类型检测
- 地球坐标模块 `yalgo_earth`：WGS84↔ECEF↔UTM↔墨卡托转换、多椭球体、多边形包含、通视判断
- CMake Presets 支持：Windows、Linux、macOS、Kylin（ARM/x86）
- GitHub Actions CI：MSVC、GCC、Clang 三矩阵构建与测试
- 自定义 CMake 模块：YuMakeSDK、YuMakeApp、YuPath、YuLibInstall
