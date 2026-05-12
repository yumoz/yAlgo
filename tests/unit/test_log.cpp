/**
 * @file test_log.cpp
 * @brief 日志模块单元测试
 * @author yAlgo Team
 * @date 2026-03-24
 */

#include <iostream>
#include <string>
#include "log/async_logger.h"

/**
 * @brief 测试日志格式化功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_formatLog() {
    std::string result = yalgo::log::formatLog("val=%d, str=%s", 42, "foo");
    if (result.find("42") == std::string::npos || result.find("foo") == std::string::npos) {
        std::cerr << "formatLog test failed" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 测试日志级别枚举
 * @return 测试通过返回 true，否则返回 false
 */
bool test_logLevel() {
    yalgo::log::LogLevel lvl = yalgo::log::LogLevel::INFO;
    if (lvl != yalgo::log::LogLevel::INFO) {
        std::cerr << "LogLevel test failed" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 主函数 - 运行所有日志模块测试
 * @return 所有测试通过返回 0，否则返回 1
 */
int main() {
    std::cout << "Running log module unit tests..." << std::endl;
    
    bool all_passed = true;
    
    all_passed &= test_formatLog();
    all_passed &= test_logLevel();
    
    if (all_passed) {
        std::cout << "All log tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "Some log tests failed!" << std::endl;
        return 1;
    }
}
