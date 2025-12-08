#include "../sdk/log/logger.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// 示例函数：演示基本日志功能
void demoBasicLogging() {
    std::cout << "=== 基本日志功能演示 ===" << std::endl;
    
    // 基本日志输出
    YLOG_INFO("这是一条信息日志");
    YLOG_WARN("这是一条警告日志");
    YLOG_ERROR("这是一条错误日志");
    YLOG_DEBUG("这是一条调试日志");
    
    std::cout << "\n";
}

// 示例函数：演示模块日志
void demoModuleLogging() {
    std::cout << "=== 模块日志功能演示 ===" << std::endl;
    
    // 模块日志输出
    YLOG_MODULE_INFO("Network", "网络连接已建立，IP: %s, 端口: %d", "127.0.0.1", 8080);
    YLOG_MODULE_WARN("Database", "数据库连接超时，重试中...");
    YLOG_MODULE_ERROR("FileIO", "无法打开文件: %s, 错误码: %d", "config.json", 404);
    YLOG_MODULE_DEBUG("Parser", "解析JSON数据成功，共%d个字段", 10);
    
    std::cout << "\n";
}

// 示例函数：演示流式日志
void demoStreamingLogging() {
    std::cout << "=== 流式日志功能演示 ===" << std::endl;
    
    // 流式日志输出
    YLOG_INFO_STREAM << "用户登录: " << "admin" << ", IP: " << "192.168.1.1" << ", 时间: " << __TIMESTAMP__;
    YLOG_WARN_STREAM << "磁盘空间不足: " << "剩余" << " 500MB" << " 请及时清理";
    YLOG_ERROR_STREAM << "系统异常: " << "错误类型: " << "权限拒绝" << ", 操作: " << "删除文件";
    YLOG_DEBUG_STREAM << "内存使用情况: " << "已用" << " 2GB" << " 总共" << " 8GB";
    
    std::cout << "\n";
}

// 示例函数：演示动态日志级别调整
void demoRuntimeLevelAdjustment() {
    std::cout << "=== 动态日志级别调整演示 ===" << std::endl;
    
    // 调整为WARN级别
    std::cout << "将日志级别设置为WARN:" << std::endl;
    yutils::log::AsyncLogger::getInstance().setRuntimeLogLevel(yutils::log::LogLevel::WARN);
    YLOG_DEBUG("这条调试日志不会显示");
    YLOG_INFO("这条信息日志不会显示");
    YLOG_WARN("这条警告日志会显示");
    YLOG_ERROR("这条错误日志会显示");
    
    // 调整回INFO级别
    std::cout << "将日志级别设置回INFO:" << std::endl;
    yutils::log::AsyncLogger::getInstance().setRuntimeLogLevel(yutils::log::LogLevel::INFO);
    YLOG_INFO("现在信息日志可以显示了");
    
    std::cout << "\n";
}

// 示例函数：演示多线程日志
void demoMultiThreadLogging() {
    std::cout << "=== 多线程日志功能演示 ===" << std::endl;
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            for (int j = 0; j < 3; ++j) {
                YLOG_MODULE_INFO("Thread", "线程 %d - 日志消息 %d", i, j);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "\n";
}

// 示例函数：演示性能统计
void demoPerformanceStats() {
    std::cout << "=== 性能统计功能演示 ===" << std::endl;
    
    // 输出统计信息
    auto stats = yutils::log::AsyncLogger::getInstance().getStats();
    std::cout << "日志统计信息:" << std::endl;
    std::cout << "  - 总日志数: " << stats.total_logs << std::endl;
    std::cout << "  - 丢弃的日志数: " << stats.dropped_logs << std::endl;
    std::cout << "  - 最大队列长度: " << stats.max_queue_size << std::endl;
    std::cout << "  - 总写入时间(μs): " << stats.total_write_time << std::endl;
    
    std::cout << "\n";
}

// 示例函数：演示配置更新
void demoConfigUpdate() {
    std::cout << "=== 配置更新功能演示 ===" << std::endl;
    
    yutils::log::LogConfig newConfig;
    newConfig.runtime_level = yutils::log::LogLevel::INFO;
    newConfig.enable_console = true;
    newConfig.enable_file = false;  // 临时关闭文件日志
    newConfig.enable_color = true;
    
    std::cout << "更新日志配置，关闭文件输出:" << std::endl;
    yutils::log::AsyncLogger::getInstance().updateConfig(newConfig);
    YLOG_INFO("这条日志只会输出到控制台");
    
    std::cout << "\n";
}

// 主函数
int main() {
    std::cout << "====================================================" << std::endl;
    std::cout << "            yUtils 日志SDK 使用示例                " << std::endl;
    std::cout << "====================================================" << std::endl;
    
    // 初始化日志器 - 使用默认配置
    yutils::log::LogConfig config;
    config.runtime_level = yutils::log::LogLevel::INFO;
    config.enable_console = true;
    config.enable_file = true;
    config.enable_color = true;
    config.log_file = "log_example.log";
    config.max_file_size = 10 * 1024 * 1024;  // 10MB
    config.max_backup_files = 5;
    
    std::cout << "初始化日志系统..." << std::endl;
    yutils::log::AsyncLogger::getInstance().init(config);
    std::cout << "日志系统初始化完成!" << std::endl;
    std::cout << "\n";
    
    // 运行各种功能演示
    demoBasicLogging();
    demoModuleLogging();
    demoStreamingLogging();
    demoRuntimeLevelAdjustment();
    demoMultiThreadLogging();
    demoConfigUpdate();
    
    // 等待日志队列处理完成
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // 显示性能统计
    demoPerformanceStats();
    
    std::cout << "====================================================" << std::endl;
    std::cout << "                 示例演示结束                      " << std::endl;
    std::cout << "====================================================" << std::endl;
    
    return 0;
}