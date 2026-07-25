/**
 * @file retry_utils.h
 * @brief 重试/退避工具（header-only）
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_RETRY_UTILS_H
#define YALGO_SDK_UTILS_RETRY_UTILS_H

#include <thread>
#include <chrono>

namespace yalgo {
namespace utils {

/**
 * @class RetryUtils
 * @brief 重试和指数退避工具
 */
class RetryUtils {
public:
    /**
     * @brief 固定间隔重试
     * @param func 要执行的函数，返回 true 表示成功
     * @param maxRetries 最大重试次数
     * @param delayMs 每次重试间隔（毫秒）
     * @return 是否成功
     */
    template<typename Func>
    static bool retry(Func&& func, int maxRetries, int delayMs = 100) {
        for (int i = 0; i <= maxRetries; ++i) {
            if (func()) return true;
            if (i < maxRetries) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
            }
        }
        return false;
    }

    /**
     * @brief 指数退避重试
     * @param func 要执行的函数，返回 true 表示成功
     * @param maxRetries 最大重试次数
     * @param baseDelayMs 基础延迟（毫秒）
     * @param multiplier 退避倍数
     * @return 是否成功
     */
    template<typename Func>
    static bool retryWithBackoff(Func&& func, int maxRetries,
                                  int baseDelayMs = 100, double multiplier = 2.0) {
        int delay = baseDelayMs;
        for (int i = 0; i <= maxRetries; ++i) {
            if (func()) return true;
            if (i < maxRetries) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                delay = static_cast<int>(delay * multiplier);
            }
        }
        return false;
    }

private:
    RetryUtils() = delete;
    ~RetryUtils() = delete;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_RETRY_UTILS_H
