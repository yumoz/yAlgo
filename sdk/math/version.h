/**
 * @file Version.h
 * @brief 数学工具SDK版本信息
 * @author yAlgo Team
 * @date 2025-12-13
 */

#ifndef YALGO_SDK_MATH_VERSION_H
#define YALGO_SDK_MATH_VERSION_H

// 版本宏定义
#define YALGO_MATH_MAJOR_VERSION    1       ///< 主版本号
#define YALGO_MATH_MINOR_VERSION    0       ///< 次版本号
#define YALGO_MATH_PATCH_VERSION    0       ///< 补丁版本号

// 版本号字符串
#define YALGO_MATH_VERSION_STRING   "1.0.0" ///< 版本字符串

// 版本号（用于比较）
#define YALGO_MATH_VERSION_CODE     ((YALGO_MATH_MAJOR_VERSION << 16) | \
                                     (YALGO_MATH_MINOR_VERSION << 8) | \
                                     YALGO_MATH_PATCH_VERSION)

#endif // YALGO_SDK_MATH_VERSION_H
