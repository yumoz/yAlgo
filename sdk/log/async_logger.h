/**
 * @file AsyncLogger.h
 * @brief 高性能异步日志模块头文件
 * @author yUtils Team
 * @date 2025-12-07
 * @version 1.0.0
 */

#ifndef YUTILS_SDK_LOG_ASYNC_LOGGER_H
#define YUTILS_SDK_LOG_ASYNC_LOGGER_H

#include "log_exports.h"

#include <string>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <fstream>
#include <cstdarg>
#include <sstream>

// 定义命名空间
namespace yutils {
namespace log {

/**
 * @brief 日志级别枚举
 */
enum class LogLevel {
    OFF = 0,       ///< 关闭所有日志
    LOG_ERROR = 1, ///< 仅错误日志
    WARN = 2,      ///< 警告 + 错误日志
    INFO = 3,      ///< 信息 + 警告 + 错误日志
    DEBUG = 4      ///< 调试 + 所有日志
};

/**
 * @brief 日志配置结构体
 */
struct LogConfig {
    std::string log_file = "yutils_log.txt";  ///< 日志文件路径
    LogLevel runtime_level = LogLevel::INFO;  ///< 运行时日志级别
    bool enable_console = true;               ///< 是否启用控制台输出
    bool enable_file = true;                  ///< 是否启用文件输出
    bool enable_color = true;                 ///< 控制台是否启用颜色
    size_t max_file_size = 500 * 1024 * 1024; ///< 单日志文件最大大小（500MB）
    int max_backup_files = 10;                ///< 保留最大历史日志数
    bool rotate_by_day = true;                ///< 是否按天轮转
    std::vector<std::string> enable_modules;  ///< 启用的模块列表
    std::vector<std::string> filter_keywords; ///< 关键词过滤
    bool enable_syslog = false;               ///< 是否启用系统日志
    std::string syslog_ident = "yutils";     ///< 系统日志标识
};

/**
 * @brief 日志性能统计信息
 */
struct LogStats {
    uint64_t total_logs = 0;        ///< 总日志数
    uint64_t dropped_logs = 0;      ///< 丢弃的日志数
    uint64_t total_write_time = 0;  ///< 总写入耗时（微秒）
    size_t max_queue_size = 0;      ///< 队列最大长度
};

/**
 * @brief 高性能异步日志器类
 * 
 * @details 单例模式，线程安全的异步日志实现，支持多输出和动态配置
 */
class LOG_API AsyncLogger {
private:
    /**
     * @brief 私有构造函数（单例模式）
     */
    AsyncLogger();

public:
    /**
     * @brief 获取日志器单例实例
     * @return AsyncLogger& 日志器实例引用
     */
    static AsyncLogger& getInstance();

    /**
     * @brief 禁用拷贝构造函数
     */
    AsyncLogger(const AsyncLogger&) = delete;

    /**
     * @brief 禁用赋值运算符
     */
    AsyncLogger& operator=(const AsyncLogger&) = delete;

    /**
     * @brief 禁用移动构造函数
     */
    AsyncLogger(AsyncLogger&&) = delete;

    /**
     * @brief 禁用移动赋值运算符
     */
    AsyncLogger& operator=(AsyncLogger&&) = delete;

    /**
     * @brief 初始化日志器
     * @param config 日志配置参数
     */
    void init(const LogConfig& config);

    /**
    * @brief 从配置文件加载日志配置
    * @param config_file 配置文件路径
    * @return 是否加载成功
    */
    bool loadConfigFromFile(const std::string& config_file);

    /**
     * @brief 动态设置运行时日志级别
     * @param level 目标日志级别
     */
    void setRuntimeLogLevel(LogLevel level);

    /**
     * @brief 动态修改日志配置
     * @param config 新的日志配置
     */
    void updateConfig(const LogConfig& config);

    /**
     * @brief 提交日志消息（内部使用）
     * @param level 日志级别
     * @param level_str 日志级别字符串
     * @param message 日志内容
     */
    void log(LogLevel level, const std::string& level_str, const std::string& message);

    /**
     * @brief 从环境变量加载日志配置
     */
    void loadConfigFromEnv();

    /**
     * @brief 获取日志性能统计
     * @return LogStats 统计信息
     */
    LogStats getStats() const;

    /**
     * @brief 重置统计信息
     */
    void resetStats();

    /**
     * @brief 析构函数
     */
    ~AsyncLogger();

private:
    /**
     * @brief 格式化当前时间
     * @return std::string 格式化后的时间字符串
     */
    static std::string getFormattedTime();

    /**
     * @brief 获取控制台颜色转义序列
     * @param level 日志级别
     * @return std::string 颜色转义序列
     */
    std::string getColorCode(LogLevel level) const;

    /**
     * @brief 检查并切割日志文件
     */
    void checkLogRotation();

    /**
     * @brief 执行日志文件轮转
     */
    void rotateLogFile();

    /**
     * @brief 后台线程处理日志队列
     */
    void processLogs();

    /**
     * @brief 解析日志级别字符串
     * @param level_str 级别字符串
     * @return LogLevel 对应的日志级别
     */
    LogLevel parseLogLevel(const std::string& level_str);

    std::atomic<bool> running_;          ///< 后台线程运行标志
    std::atomic<LogLevel> runtime_level_;///< 运行时日志级别
    LogConfig config_;                   ///< 当前日志配置
    mutable std::mutex config_mutex_;    ///< 配置修改互斥锁
    std::mutex queue_mutex_;             ///< 日志队列互斥锁
    std::condition_variable queue_cv_;   ///< 队列条件变量
    std::queue<std::string> log_queue_;  ///< 日志消息队列
    std::thread log_thread_;             ///< 后台日志线程
    std::ofstream log_file_;             ///< 日志文件流
    LogStats stats_;                     ///< 日志性能统计
    mutable std::mutex stats_mutex_;     ///< 统计信息互斥锁
    size_t current_queue_size_ = 0;      ///< 当前队列长度
    std::string last_rotate_date_;       ///< 上次轮转日期
    const size_t MAX_QUEUE_SIZE = 100000;///< 队列最大长度
};

/**
 * @brief 格式化字符串（printf风格）
 * @param format 格式化模板
 * @param ... 可变参数
 * @return std::string 格式化后的字符串
 */
LOG_API std::string formatLog(const char* format, ...);

// 流式日志辅助类
class LOG_API LogStream {
public:
    LogStream(LogLevel level, const std::string& level_str,
              const char* file, int line, const char* func,
              const std::string& module = "");

    // 析构时自动提交日志
    ~LogStream();

    // 重载<<运算符，支持各种类型
    template<typename T>
    LogStream& operator<<(const T& value) {
        stream_ << value;
        return *this;
    }

private:
    LogLevel level_;
    std::string level_str_;
    const char* file_;
    int line_;
    const char* func_;
    std::string module_;
    std::ostringstream stream_;
};

} // namespace log
} // namespace yutils

// 编译期日志级别控制宏（默认开启DEBUG级别）
#ifndef YUTILS_LOG_LEVEL
#define YUTILS_LOG_LEVEL 4 // DEBUG级别
#endif

// 日志宏定义（使用命名空间）
#if YUTILS_LOG_LEVEL >= 1 // LOG_ERROR级别
#define YLOG_ERROR(format, ...) do { \
    std::ostringstream oss; \
    oss << "[" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] " \
        << yutils::log::formatLog(format, ##__VA_ARGS__); \
    yutils::log::AsyncLogger::getInstance().log(yutils::log::LogLevel::LOG_ERROR, "ERROR", oss.str()); \
} while(0)
#else
#define YLOG_ERROR(format, ...) do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 2 // WARN级别
#define YLOG_WARN(format, ...) do { \
    std::ostringstream oss; \
    oss << "[" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] " \
        << yutils::log::formatLog(format, ##__VA_ARGS__); \
    yutils::log::AsyncLogger::getInstance().log(yutils::log::LogLevel::WARN, "WARN", oss.str()); \
} while(0)
#else
#define YLOG_WARN(format, ...) do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 3 // INFO级别
#define YLOG_INFO(format, ...) do { \
    std::ostringstream oss; \
    oss << "[" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] " \
        << yutils::log::formatLog(format, ##__VA_ARGS__); \
    yutils::log::AsyncLogger::getInstance().log(yutils::log::LogLevel::INFO, "INFO", oss.str()); \
} while(0)
#else
#define YLOG_INFO(format, ...) do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 4 // DEBUG级别
#define YLOG_DEBUG(format, ...) do { \
    std::ostringstream oss; \
    oss << "[" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] " \
        << yutils::log::formatLog(format, ##__VA_ARGS__); \
    yutils::log::AsyncLogger::getInstance().log(yutils::log::LogLevel::DEBUG, "DEBUG", oss.str()); \
} while(0)
#else
#define YLOG_DEBUG(format, ...) do {} while(0)
#endif

// 模块日志宏
#if YUTILS_LOG_LEVEL >= 1 // ERROR级别
#define YLOG_MODULE_ERROR(module, format, ...) do { \
    std::ostringstream oss; \
    oss << "[" << module << "] [" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] " \
        << yutils::log::formatLog(format, ##__VA_ARGS__); \
    yutils::log::AsyncLogger::getInstance().log(yutils::log::LogLevel::LOG_ERROR, "ERROR", oss.str()); \
} while(0)
#else
#define YLOG_MODULE_ERROR(module, format, ...) do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 2 // WARN级别
#define YLOG_MODULE_WARN(module, format, ...) do { \
    std::ostringstream oss; \
    oss << "[" << module << "] [" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] " \
        << yutils::log::formatLog(format, ##__VA_ARGS__); \
    yutils::log::AsyncLogger::getInstance().log(yutils::log::LogLevel::WARN, "WARN", oss.str()); \
} while(0)
#else
#define YLOG_MODULE_WARN(module, format, ...) do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 4 // DEBUG级别
#define YLOG_MODULE_DEBUG(module, format, ...) do { \
    std::ostringstream oss; \
    oss << "[" << module << "] [" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] " \
        << yutils::log::formatLog(format, ##__VA_ARGS__); \
    yutils::log::AsyncLogger::getInstance().log(yutils::log::LogLevel::DEBUG, "DEBUG", oss.str()); \
} while(0)
#else
#define YLOG_MODULE_DEBUG(module, format, ...) do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 3 // INFO级别
#define YLOG_MODULE_INFO(module, format, ...) do { \
    std::ostringstream oss; \
    oss << "[" << module << "] [" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] " \
        << yutils::log::formatLog(format, ##__VA_ARGS__); \
    yutils::log::AsyncLogger::getInstance().log(yutils::log::LogLevel::INFO, "INFO", oss.str()); \
} while(0)
#else
#define YLOG_MODULE_INFO(module, format, ...) do {} while(0)
#endif

// 流式日志宏
#if YUTILS_LOG_LEVEL >= 1 // LOG_ERROR级别
#define YLOG_ERROR_STREAM yutils::log::LogStream(yutils::log::LogLevel::LOG_ERROR, "ERROR", __FILE__, __LINE__, __func__)
#else
#define YLOG_ERROR_STREAM do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 2 // WARN级别
#define YLOG_WARN_STREAM yutils::log::LogStream(yutils::log::LogLevel::WARN, "WARN", __FILE__, __LINE__, __func__)
#else
#define YLOG_WARN_STREAM do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 3 // INFO级别
#define YLOG_INFO_STREAM yutils::log::LogStream(yutils::log::LogLevel::INFO, "INFO", __FILE__, __LINE__, __func__)
#else
#define YLOG_INFO_STREAM do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 4 // DEBUG级别
#define YLOG_DEBUG_STREAM yutils::log::LogStream(yutils::log::LogLevel::DEBUG, "DEBUG", __FILE__, __LINE__, __func__)
#else
#define YLOG_DEBUG_STREAM do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 1 // ERROR级别
#define YLOG_MODULE_ERROR_STREAM(module) yutils::log::LogStream(yutils::log::LogLevel::LOG_ERROR, "ERROR", __FILE__, __LINE__, __func__, module)
#else
#define YLOG_MODULE_ERROR_STREAM(module) do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 2 // WARN级别
#define YLOG_MODULE_WARN_STREAM(module) yutils::log::LogStream(yutils::log::LogLevel::WARN, "WARN", __FILE__, __LINE__, __func__, module)
#else
#define YLOG_MODULE_WARN_STREAM(module) do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 3 // INFO级别
#define YLOG_MODULE_INFO_STREAM(module) yutils::log::LogStream(yutils::log::LogLevel::INFO, "INFO", __FILE__, __LINE__, __func__, module)
#else
#define YLOG_MODULE_INFO_STREAM(module) do {} while(0)
#endif

#if YUTILS_LOG_LEVEL >= 4 // DEBUG级别
#define YLOG_MODULE_DEBUG_STREAM(module) yutils::log::LogStream(yutils::log::LogLevel::DEBUG, "DEBUG", __FILE__, __LINE__, __func__, module)
#else
#define YLOG_MODULE_DEBUG_STREAM(module) do {} while(0)
#endif

#endif // YUTILS_SDK_LOG_ASYNC_LOGGER_H