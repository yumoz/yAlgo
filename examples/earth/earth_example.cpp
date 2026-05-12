#include "earth_test.h"

/**
 * @file earth_example.cpp
 * @brief 地球坐标SDK示例程序主入口
 * @author yAlgo Team
 * @date 2025-12-13
 */

/**
 * @brief 设置控制台UTF-8编码（跨平台）
 * 
 * Windows: 使用 SetConsoleOutputCP
 * Linux/macOS: 使用 setlocale
 */
void setupConsoleUtf8() {
#ifdef _WIN32
    // Windows 平台：设置控制台输出编码为UTF-8
    SetConsoleOutputCP(CP_UTF8);
#elif defined(__linux__) || defined(__APPLE__)
    // Linux/macOS 平台：设置本地化环境
    setlocale(LC_ALL, "en_US.UTF-8");
#endif
}

int main(int argc, char* argv[]) {
    // 设置控制台输出编码为UTF-8
    setupConsoleUtf8();
    
    // 运行地球坐标模块的所有测试
    yalgo::examples::EarthTest::runAllTests();
    
    // 等待用户输入
    std::cout << "\n按回车键退出...";
    std::cin.ignore();
    
    return 0;
}
