#include "earth_test.h"

/**
 * @file earth_example.cpp
 * @brief 地球坐标SDK示例程序主入口
 * @author yAlgo Team
 * @date 2025-12-13
 */

int main(int argc, char* argv[]) {
     // 设置控制台输出编码为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    
    // 运行地球坐标模块的所有测试
    yalgo::examples::EarthTest::runAllTests();
    
    // 等待用户输入
    std::cout << "\n按回车键退出...";
    std::cin.ignore();
    
    return 0;
}