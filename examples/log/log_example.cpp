#include "log_test.h"

// 主函数：只负责调用日志模块的测试类
int main() {
    // 运行日志模块测试
    yalgo::examples::LogTest::runAllTests();
    
    return 0;
}