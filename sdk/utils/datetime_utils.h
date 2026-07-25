/**
 * @file datetime_utils.h
 * @brief 日期时间工具类
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_DATETIME_UTILS_H
#define YALGO_SDK_UTILS_DATETIME_UTILS_H

#include <string>
#include <cstdint>
#include "utils_exports.h"

namespace yalgo {
namespace utils {

/**
 * @class DatetimeUtils
 * @brief 日期时间工具类
 */
class YALGO_UTILS_API DatetimeUtils {
public:
    /**
     * @brief 获取当前时间字符串
     * @return 格式 "YYYY-MM-DD HH:MM:SS"
     */
    static std::string now();

    /**
     * @brief 获取当前时间字符串（含毫秒）
     * @return 格式 "YYYY-MM-DD HH:MM:SS.mmm"
     */
    static std::string nowMs();

    /**
     * @brief 获取当前日期字符串
     * @return 格式 "YYYY-MM-DD"
     */
    static std::string today();

    /**
     * @brief 获取 Unix 时间戳（秒）
     * @return Unix 时间戳
     */
    static int64_t timestamp();

    /**
     * @brief 获取 Unix 时间戳（毫秒）
     * @return Unix 毫秒时间戳
     */
    static int64_t timestampMs();

private:
    DatetimeUtils() = delete;
    ~DatetimeUtils() = delete;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_DATETIME_UTILS_H
