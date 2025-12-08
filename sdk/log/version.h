/**
 * @file Version.h
 * @brief 日志SDK版本信息
 * @author yUtils Team
 * @date 2025-12-07
 */

#ifndef YUTILS_SDK_LOG_VERSION_H
#define YUTILS_SDK_LOG_VERSION_H

// 版本宏定义
#define YUTILS_LOG_MAJOR_VERSION    1       ///< 主版本号
#define YUTILS_LOG_MINOR_VERSION    0       ///< 次版本号
#define YUTILS_LOG_PATCH_VERSION    0       ///< 补丁版本号

// 版本号字符串
#define YUTILS_LOG_VERSION_STRING   "1.0.0" ///< 版本字符串

// 版本号（用于比较）
#define YUTILS_LOG_VERSION_CODE     ((YUTILS_LOG_MAJOR_VERSION << 16) | \
                                     (YUTILS_LOG_MINOR_VERSION << 8) | \
                                     YUTILS_LOG_PATCH_VERSION)

#endif // YUTILS_SDK_LOG_VERSION_H