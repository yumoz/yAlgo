/**
 * @file Logger.h
 * @brief 日志SDK统一入口头文件
 * @author yUtils Team
 * @date 2025-12-07
 * @version 1.0.0
 *
 * 高性能异步日志模块，提供以下特性：
 * 1. 编译期+运行时双层日志级别控制
 * 2. 自动打印文件名、行号、函数名
 * 3. 异步日志写入，不阻塞主线程
 * 4. 多输出支持（控制台+文件+系统日志）
 * 5. 日志文件自动轮转
 * 6. 模块/关键词过滤
 */

#ifndef YUTILS_LOG_LOGGER_H
#define YUTILS_LOG_LOGGER_H

#include "version.h"
#include "async_logger.h"

#endif // YUTILS_LOG_LOGGER_H