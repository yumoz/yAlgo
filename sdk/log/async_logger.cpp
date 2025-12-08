/**
 * @file AsyncLogger.cpp
 * @brief 高性能异步日志模块实现文件
 * @author yUtils Team
 * @date 2025-12-07
 * @version 1.0.0
 */

#include "async_logger.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <limits>
#include <cstdio>

// 跨平台系统日志头文件
#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#else
#include <syslog.h>
#include <usleep.h>
#include <sys/stat.h>
#endif

namespace yutils {
namespace log {

// 格式化字符串实现
std::string formatLog(const char* format, ...) {
    va_list args;
    va_start(args, format);

    // 第一步：计算所需缓冲区大小
    va_list args_copy;
    va_copy(args_copy, args);
    int buf_size = vsnprintf(nullptr, 0, format, args_copy) + 1;
    va_end(args_copy);

    // 处理缓冲区大小计算失败的情况
    if (buf_size <= 0) {
        va_end(args);
        return std::string("Format error: invalid format string");
    }

    // 第二步：分配缓冲区并格式化
    std::vector<char> buf(buf_size);
    vsnprintf(buf.data(), buf_size, format, args);
    va_end(args);

    return std::string(buf.data());
}

// LogStream构造函数
LogStream::LogStream(LogLevel level, const std::string& level_str,
                  const char* file, int line, const char* func,
                  const std::string& module)
    : level_(level), level_str_(level_str),
      file_(file), line_(line), func_(func), module_(module) {}

// LogStream析构函数
LogStream::~LogStream() {
    std::ostringstream oss;
    if (!module_.empty()) {
        oss << "[" << module_ << "] ";
    }
    oss << "[" << file_ << ":" << line_ << ":" << func_ << "] " << stream_.str();
    AsyncLogger::getInstance().log(level_, level_str_, oss.str());
}

// 单例实例获取
AsyncLogger& AsyncLogger::getInstance() {
    static AsyncLogger instance;
    return instance;
}

// 私有构造函数
AsyncLogger::AsyncLogger()
    : running_(false),
      runtime_level_(LogLevel::OFF),
      config_(),
      stats_(),
      current_queue_size_(0),
      last_rotate_date_(),
      log_queue_(),
      queue_mutex_(),
      queue_cv_(),
      config_mutex_(),
      stats_mutex_(),
      log_file_() {}

// 析构函数
AsyncLogger::~AsyncLogger() {
    running_ = false;
    queue_cv_.notify_one(); // 唤醒后台线程

    if (log_thread_.joinable()) {
        log_thread_.join();
    }

    if (log_file_.is_open()) {
        log_file_.close();
    }
}

// 初始化日志器
void AsyncLogger::init(const LogConfig& config) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    config_ = config;
    runtime_level_.store(config.runtime_level);

    // Windows启用虚拟终端支持颜色和设置UTF-8编码
#ifdef _WIN32
    // 设置控制台输出代码页为UTF-8，解决中文字符乱码问题
    SetConsoleOutputCP(CP_UTF8);
    // 同时设置输入代码页为UTF-8，确保完整的UTF-8支持
    SetConsoleCP(CP_UTF8);
    
    if (config_.enable_color) {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD mode = 0;
            if (GetConsoleMode(hOut, &mode)) {
                SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        }
    }
#endif

    // 初始化日志文件
    if (config_.enable_file) {
        log_file_.open(config_.log_file, std::ios::out | std::ios::app);
        if (!log_file_.is_open()) {
            std::cerr << "AsyncLogger: Failed to open log file: " << config_.log_file << std::endl;
        }
    }

    // 初始化轮转日期
    time_t now = time(nullptr);
    struct tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &now);
#else
    localtime_r(&now, &local_tm);
#endif
    char date_buf[16];
    strftime(date_buf, sizeof(date_buf), "%Y%m%d", &local_tm);
    last_rotate_date_ = date_buf;

    // 启动后台线程
    if (!running_) {
        running_ = true;
        log_thread_ = std::thread(&AsyncLogger::processLogs, this);
    }
}

// 从配置文件加载配置
bool AsyncLogger::loadConfigFromFile(const std::string& config_file) {
    // 简单的配置文件解析实现
    std::ifstream file(config_file);
    if (!file.is_open()) {
        return false;
    }

    std::lock_guard<std::mutex> lock(config_mutex_);
    std::string line;
    std::string current_section;

    while (std::getline(file, line)) {
        // 移除首尾空格
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // 跳过空行和注释
        if (line.empty() || line[0] == ';' || line[0] == '#') {
            continue;
        }

        // 处理节 [section]
        if (line[0] == '[' && line.back() == ']') {
            current_section = line.substr(1, line.size() - 2);
            current_section.erase(0, current_section.find_first_not_of(" \t"));
            current_section.erase(current_section.find_last_not_of(" \t") + 1);
            continue;
        }

        // 处理键值对 key=value
        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos) {
            continue;
        }

        std::string key = line.substr(0, eq_pos);
        std::string value = line.substr(eq_pos + 1);
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // 只处理logger节
        if (current_section == "logger") {
            if (key == "level") {
                config_.runtime_level = parseLogLevel(value);
                runtime_level_.store(config_.runtime_level);
            } else if (key == "enable_console") {
                config_.enable_console = (value == "true" || value == "1" || value == "yes");
            } else if (key == "enable_file") {
                config_.enable_file = (value == "true" || value == "1" || value == "yes");
            } else if (key == "enable_color") {
                config_.enable_color = (value == "true" || value == "1" || value == "yes");
            } else if (key == "log_file") {
                config_.log_file = value;
            } else if (key == "max_file_size") {
                try {
                    config_.max_file_size = std::stoi(value) * 1024 * 1024; // MB转字节
                } catch (...) {
                    // 忽略解析错误
                }
            } else if (key == "max_backup_files") {
                try {
                    config_.max_backup_files = std::stoi(value);
                } catch (...) {
                    // 忽略解析错误
                }
            }
        }
    }

    // 启动后台线程
    if (!running_) {
        running_ = true;
        log_thread_ = std::thread(&AsyncLogger::processLogs, this);
    }

    return true;
}

// 动态设置日志级别
void AsyncLogger::setRuntimeLogLevel(LogLevel level) {
    if (level >= LogLevel::OFF && level <= LogLevel::DEBUG) {
        runtime_level_.store(level);
    }
}

// 动态更新配置
void AsyncLogger::updateConfig(const LogConfig& config) {
    std::lock_guard<std::mutex> lock(config_mutex_);

    // 更新文件输出（如果路径变化）
    if (config.enable_file && config.log_file != config_.log_file) {
        if (log_file_.is_open()) {
            log_file_.close();
        }
        log_file_.open(config.log_file, std::ios::out | std::ios::app);
    }

    config_ = config;
    runtime_level_.store(config.runtime_level);
}

// 解析日志级别字符串
LogLevel AsyncLogger::parseLogLevel(const std::string& level_str) {
    if (level_str == "DEBUG") return LogLevel::DEBUG;
    else if (level_str == "INFO") return LogLevel::INFO;
    else if (level_str == "WARN") return LogLevel::WARN;
    else if (level_str == "ERROR") return LogLevel::LOG_ERROR;
    else return LogLevel::OFF;
}

// 从环境变量加载配置
void AsyncLogger::loadConfigFromEnv() {
    std::lock_guard<std::mutex> lock(config_mutex_);

    // 1. 加载日志级别
    const char* level_env = getenv("YUTILS_LOG_LEVEL");
    if (level_env) {
        LogLevel level = parseLogLevel(level_env);
        runtime_level_.store(level);
        config_.runtime_level = level;
    }

    // 2. 加载颜色配置
    const char* color_env = getenv("YUTILS_LOG_COLOR");
    if (color_env) {
        config_.enable_color = (std::string(color_env) == "1" || std::string(color_env) == "true");
    }
}

// 获取格式化时间
std::string AsyncLogger::getFormattedTime() {
    try {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
            now.time_since_epoch()
        ) % 1000000;

        char time_buf[64] = {0};
        struct tm local_tm;

        // 线程安全的本地时间转换
        #ifdef _WIN32
            if (localtime_s(&local_tm, &time_t_now) != 0) {
                snprintf(time_buf, sizeof(time_buf), "TimeError[%d]", errno);
                return time_buf;
            }
        #else
            if (localtime_r(&time_t_now, &local_tm) == nullptr) {
                snprintf(time_buf, sizeof(time_buf), "TimeError[%d]", errno);
                return time_buf;
            }
        #endif

        std::ostringstream oss;
        std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &local_tm);
        oss << time_buf << "." << std::setfill('0') << std::setw(6) << micros.count();

        return oss.str();
    }
    catch (const std::exception& e) {
        return "TimeError[" + std::string(e.what()) + "]";
    }
    catch (...) {
        return "TimeError[Unknown]";
    }
}

// 获取控制台颜色码
std::string AsyncLogger::getColorCode(LogLevel level) const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    if (!config_.enable_color) {
        return "";
    }

    // ANSI颜色转义序列
    switch (level) {
        case LogLevel::LOG_ERROR: return "\033[31m"; // 红色
        case LogLevel::WARN:  return "\033[33m"; // 黄色
        case LogLevel::INFO:  return "\033[32m"; // 绿色
        case LogLevel::DEBUG: return "\033[36m"; // 青色
        default:              return "\033[0m";  // 默认颜色
    }
}

// 检查日志文件轮转
void AsyncLogger::checkLogRotation() {
    std::lock_guard<std::mutex> lock(config_mutex_);
    if (!config_.enable_file || !log_file_.is_open()) {
        return;
    }

    // 1. 按天轮转（优先级更高）
    if (config_.rotate_by_day) {
        time_t now = time(nullptr);
        struct tm local_tm;
#ifdef _WIN32
        localtime_s(&local_tm, &now);
#else
        localtime_r(&now, &local_tm);
#endif
        char date_buf[16];
        strftime(date_buf, sizeof(date_buf), "%Y%m%d", &local_tm);
        std::string current_date = date_buf;

        if (current_date != last_rotate_date_) {
            rotateLogFile();
            last_rotate_date_ = current_date;
            return;
        }
    }

    // 2. 按大小轮转
    log_file_.seekp(0, std::ios::end);
    size_t current_size = log_file_.tellp();
    if (current_size >= config_.max_file_size) {
        rotateLogFile();
    }
}

// 执行日志文件轮转
void AsyncLogger::rotateLogFile() {
    log_file_.close();

    // 生成新文件名（原文件名+时间戳）
    time_t now = time(nullptr);
    struct tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &now);
#else
    localtime_r(&now, &local_tm);
#endif
    char time_buf[32];
    strftime(time_buf, sizeof(time_buf), "%Y%m%d_%H%M%S", &local_tm);

    std::string new_filename = config_.log_file + "_" + time_buf;
#ifdef _WIN32
    MoveFileA(config_.log_file.c_str(), new_filename.c_str());
#else
    rename(config_.log_file.c_str(), new_filename.c_str());
#endif

    // 重新打开新日志文件
    log_file_.open(config_.log_file, std::ios::out | std::ios::app);
}

// 提交日志消息
void AsyncLogger::log(LogLevel level, const std::string& level_str, const std::string& message) {
    // 运行时级别检查
    if (!running_ || level > runtime_level_.load()) {
        return;
    }

    // 性能统计：总日志数
    {
        std::lock_guard<std::mutex> lock(stats_mutex_);
        stats_.total_logs++;
    }

    // 格式化最终日志消息（添加时间戳）
    std::string time_str = getFormattedTime();
    std::ostringstream oss;
    oss << "[" << time_str << "] [" << level_str << "] " << message;
    std::string final_msg = oss.str();

    // 入队（加锁保护）
    std::lock_guard<std::mutex> lock(queue_mutex_);
    // 队列满时丢弃日志
    if (log_queue_.size() >= MAX_QUEUE_SIZE) {
        std::lock_guard<std::mutex> s_lock(stats_mutex_);
        stats_.dropped_logs++;
        return;
    }
    log_queue_.push(final_msg);
    current_queue_size_ = log_queue_.size();
    // 更新最大队列长度
    {
        std::lock_guard<std::mutex> s_lock(stats_mutex_);
        if (current_queue_size_ > stats_.max_queue_size) {
            stats_.max_queue_size = current_queue_size_;
        }
    }
    queue_cv_.notify_one();
}

// 获取性能统计
LogStats AsyncLogger::getStats() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    return stats_;
}

// 重置性能统计
void AsyncLogger::resetStats() {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    stats_ = LogStats();
}

// 后台处理日志队列
void AsyncLogger::processLogs() {
    const std::string reset_color = "\033[0m"; // 重置颜色

    while (running_) {
        std::string log_msg;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cv_.wait(lock, [this]() {
                return !log_queue_.empty() || !running_;
            });

            if (!running_ && log_queue_.empty()) {
                break;
            }

            if (!log_queue_.empty()) {
                log_msg = log_queue_.front();
                log_queue_.pop();
                current_queue_size_ = log_queue_.size();
            }
        }

        if (log_msg.empty()) {
            continue;
        }

        // 读取当前配置（加锁保护）
        LogConfig current_config;
        {
            std::lock_guard<std::mutex> lock(config_mutex_);
            current_config = config_;
        }

        // 提取日志级别（用于颜色/系统日志）
        size_t level_start = log_msg.find("[", log_msg.find("[") + 1) + 1;
        size_t level_end = log_msg.find("]", level_start);
        std::string level_str = log_msg.substr(level_start, level_end - level_start);
        LogLevel level = LogLevel::OFF;
        if (level_str == "ERROR") level = LogLevel::LOG_ERROR;
        else if (level_str == "WARN")  level = LogLevel::WARN;
        else if (level_str == "INFO")  level = LogLevel::INFO;
        else if (level_str == "DEBUG") level = LogLevel::DEBUG;

        // 记录写入开始时间
        auto write_start = std::chrono::high_resolution_clock::now();

        // 1. 控制台输出（支持颜色）
        if (current_config.enable_console) {
            std::cout << getColorCode(level) << log_msg << reset_color << std::endl;
        }

        // 2. 文件输出（先检查轮转）
        if (current_config.enable_file && log_file_.is_open()) {
            checkLogRotation();
            log_file_ << log_msg << std::endl;
            log_file_.flush();
        }

        // 3. 系统日志输出
        if (current_config.enable_syslog) {
#ifdef _WIN32
            // Windows Event Log（简化实现）
            HANDLE hEventSource = RegisterEventSource(NULL, current_config.syslog_ident.c_str());
            if (hEventSource != NULL) {
                WORD type = EVENTLOG_INFORMATION_TYPE;
                if (level == LogLevel::LOG_ERROR) type = EVENTLOG_ERROR_TYPE;
                else if (level == LogLevel::WARN) type = EVENTLOG_WARNING_TYPE;

                LPCTSTR strings[1] = {TEXT(log_msg.c_str())};
                ReportEvent(hEventSource, type, 0, 0, NULL, 1, 0, strings, NULL);
                DeregisterEventSource(hEventSource);
            }
#else
            // Linux syslog
            int syslog_priority = LOG_INFO;
            if (level == LogLevel::LOG_ERROR) syslog_priority = LOG_ERR;
            else if (level == LogLevel::WARN) syslog_priority = LOG_WARNING;
            else if (level == LogLevel::DEBUG) syslog_priority = LOG_DEBUG;

            openlog(current_config.syslog_ident.c_str(), LOG_PID, LOG_USER);
            syslog(syslog_priority, "%s", log_msg.c_str());
            closelog();
#endif
        }

        // 性能统计：写入耗时
        auto write_end = std::chrono::high_resolution_clock::now();
        uint64_t write_cost = std::chrono::duration_cast<std::chrono::microseconds>(
            write_end - write_start
        ).count();
        {
            std::lock_guard<std::mutex> lock(stats_mutex_);
            stats_.total_write_time += write_cost;
        }
    }

    // 处理剩余日志
    while (true) {
        std::string msg;
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (log_queue_.empty()) {
                break;
            }
            msg = log_queue_.front();
            log_queue_.pop();
        }

        LogConfig current_config;
        {
            std::lock_guard<std::mutex> lock(config_mutex_);
            current_config = config_;
        }

        // 控制台输出剩余日志
        if (current_config.enable_console) {
            std::cout << msg << std::endl;
        }

        // 文件输出剩余日志
        if (current_config.enable_file && log_file_.is_open()) {
            log_file_ << msg << std::endl;
        }
    }
}

// 其他必要的方法实现


} // namespace log
} // namespace yutils