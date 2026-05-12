/**
 * @file test_utils.cpp
 * @brief 工具模块单元测试
 * @author yAlgo Team
 * @date 2026-03-24
 */

#include <iostream>
#include <string>
#include "utils/exec_path_utils.h"

/**
 * @brief 测试获取系统类型功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_getSystemType() {
    std::string system_type = yalgo::utils::ExecPathUtils::getSystemType();
    if (system_type.empty()) {
        std::cerr << "getSystemType returned empty string" << std::endl;
        return false;
    }
    std::cout << "System type: " << system_type << std::endl;
    return true;
}

/**
 * @brief 测试获取可执行文件路径功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_getExecutablePath() {
    try {
        std::string exe_path = yalgo::utils::ExecPathUtils::getExecutablePath();
        if (exe_path.empty()) {
            std::cerr << "getExecutablePath returned empty string" << std::endl;
            return false;
        }
        std::cout << "Executable path: " << exe_path << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "getExecutablePath exception: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief 测试获取可执行文件目录功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_getExecutableDir() {
    try {
        std::string exe_dir = yalgo::utils::ExecPathUtils::getExecutableDir();
        if (exe_dir.empty()) {
            std::cerr << "getExecutableDir returned empty string" << std::endl;
            return false;
        }
        std::cout << "Executable dir: " << exe_dir << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "getExecutableDir exception: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief 主函数 - 运行所有工具模块测试
 * @return 所有测试通过返回 0，否则返回 1
 */
int main() {
    std::cout << "Running utils module unit tests..." << std::endl;
    
    bool all_passed = true;
    
    all_passed &= test_getSystemType();
    all_passed &= test_getExecutablePath();
    all_passed &= test_getExecutableDir();
    
    if (all_passed) {
        std::cout << "All utils tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "Some utils tests failed!" << std::endl;
        return 1;
    }
}
