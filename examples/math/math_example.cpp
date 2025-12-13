#include "math_test.h"
#include <windows.h>

// 主函数：只负责调用数学模块的测试类
int main() {
    // 设置控制台输出编码为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    
    // 运行数学模块测试
    yalgo::examples::MathTest::runAllTests();
    
    return 0;
}
