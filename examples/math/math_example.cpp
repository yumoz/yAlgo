#include "math_test.h"

#ifdef _WIN32
#include <windows.h>
#endif

// 主函数：只负责调用数学模块的测试类
int main() {
#ifdef _WIN32
    // 设置控制台输出编码为UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif
    
    // 运行数学模块测试
    yalgo::examples::MathTest::runAllTests();
    
    return 0;
}
