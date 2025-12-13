#ifndef YALGO_EXAMPLES_LOG_LOG_TEST_H
#define YALGO_EXAMPLES_LOG_LOG_TEST_H

#include "../../sdk/log/logger.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

namespace yalgo {
namespace examples {

/**
 * 日志模块测试类
 * 封装所有日志功能的测试方法
 */
class LogTest {
public:
    /**
     * 初始化日志系统
     */
    static void initLogger();
    
    /**
     * 演示基本日志功能
     */
    static void demoBasicLogging();
    
    /**
     * 演示模块日志
     */
    static void demoModuleLogging();
    
    /**
     * 演示流式日志
     */
    static void demoStreamingLogging();
    
    /**
     * 演示动态日志级别调整
     */
    static void demoRuntimeLevelAdjustment();
    
    /**
     * 演示多线程日志
     */
    static void demoMultiThreadLogging();
    
    /**
     * 演示性能统计
     */
    static void demoPerformanceStats();
    
    /**
     * 演示配置更新
     */
    static void demoConfigUpdate();
    
    /**
     * 运行所有测试
     */
    static void runAllTests();
};

} // namespace examples
} // namespace yalgo

#endif // YALGO_EXAMPLES_LOG_LOG_TEST_H