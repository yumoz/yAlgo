#include "utils_test.h"

namespace yalgo {
namespace examples {

// 演示获取可执行文件完整路径
void UtilsTest::demoExecutablePath() {
    std::cout << "=== 获取可执行文件完整路径演示 ===" << std::endl;
    
    try {
        std::string exe_path = yalgo::utils::ExecPathUtils::getExecutablePath();
        std::cout << "可执行文件完整路径: " << exe_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "获取可执行文件路径失败: " << e.what() << std::endl;
    }
    
    std::cout << "\n";
}

// 演示获取可执行文件目录
void UtilsTest::demoExecutableDir() {
    std::cout << "=== 获取可执行文件目录演示 ===" << std::endl;
    
    try {
        std::string exe_dir = yalgo::utils::ExecPathUtils::getExecutableDir();
        std::cout << "可执行文件目录: " << exe_dir << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "获取可执行文件目录失败: " << e.what() << std::endl;
    }
    
    std::cout << "\n";
}

// 演示在可执行文件目录下拼接文件路径
void UtilsTest::demoFileInExeDir() {
    std::cout << "=== 拼接可执行文件目录下的文件路径演示 ===" << std::endl;
    
    try {
        // 测试拼接简单文件名
        std::string config_path = yalgo::utils::ExecPathUtils::getFileInExeDir("config.json");
        std::cout << "配置文件路径: " << config_path << std::endl;
        
        // 测试拼接子目录下的文件
        std::string log_path = yalgo::utils::ExecPathUtils::getFileInExeDir("logs/app.log");
        std::cout << "日志文件路径: " << log_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "拼接文件路径失败: " << e.what() << std::endl;
    }
    
    std::cout << "\n";
}

// 演示检查可执行文件目录下的文件是否存在
void UtilsTest::demoCheckFileInExeDir() {
    std::cout << "=== 检查可执行文件目录下的文件是否存在演示 ===" << std::endl;
    
    // 检查当前可执行文件是否存在（应该存在）
    bool exe_exists = yalgo::utils::ExecPathUtils::checkFileInExeDir("log_example.exe");
    std::cout << "log_example.exe 存在: " << (exe_exists ? "是" : "否") << std::endl;
    
    // 检查一个不存在的文件（应该不存在）
    bool non_exist = yalgo::utils::ExecPathUtils::checkFileInExeDir("non_existent_file.txt");
    std::cout << "non_existent_file.txt 存在: " << (non_exist ? "是" : "否") << std::endl;
    
    std::cout << "\n";
}

// 运行所有测试
void UtilsTest::runAllTests() {
    std::cout << "====================================================" << std::endl;
    std::cout << "            yAlgo 工具SDK 使用示例                " << std::endl;
    std::cout << "====================================================" << std::endl;
    
    demoExecutablePath();
    demoExecutableDir();
    demoFileInExeDir();
    demoCheckFileInExeDir();
    
    std::cout << "====================================================" << std::endl;
    std::cout << "                 工具模块示例演示结束              " << std::endl;
    std::cout << "====================================================" << std::endl;
}

} // namespace examples
} // namespace yalgo

// 工具模块测试程序的入口点
int main() {
    yalgo::examples::UtilsTest::runAllTests();
    return 0;
}