/**
 * @file time_counter.h
 * @brief 高性能计时器，支持多种时间精度
 * @author yAlgo Team
 * @date 2026-03-25
 */

#ifndef YALGO_SDK_UTILS_TIME_COUNTER_H
#define YALGO_SDK_UTILS_TIME_COUNTER_H

#include "utils_exports.h"
#include <chrono>
#include <cstdint>

namespace yalgo {
namespace utils {

/**
 * @class TimeCounter
 * @brief 高性能计时器，基于 std::chrono::high_resolution_clock
 *
 * 支持纳秒、微秒、毫秒、秒、分、时六种时间精度，
 * 适用于性能基准测试和耗时统计。
 */
class TimeCounter {
public:
    TimeCounter() : begin_(std::chrono::high_resolution_clock::now()) {}

    /** @brief 重置计时起点 */
    void reset() {
        begin_ = std::chrono::high_resolution_clock::now();
    }

    /** @brief 返回自起点经过的纳秒数 */
    int64_t elapsedNano() const {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - begin_).count();
    }

    /** @brief 返回自起点经过的微秒数 */
    int64_t elapsedMicro() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - begin_).count();
    }

    /** @brief 返回自起点经过的毫秒数 */
    int64_t elapsedMilli() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - begin_).count();
    }

    /** @brief 返回自起点经过的秒数 */
    int64_t elapsedSeconds() const {
        return std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::high_resolution_clock::now() - begin_).count();
    }

    /** @brief 返回自起点经过的分钟数 */
    int64_t elapsedMinutes() const {
        return std::chrono::duration_cast<std::chrono::minutes>(
            std::chrono::high_resolution_clock::now() - begin_).count();
    }

    /** @brief 返回自起点经过的小时数 */
    int64_t elapsedHours() const {
        return std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::high_resolution_clock::now() - begin_).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> begin_;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_TIME_COUNTER_H
