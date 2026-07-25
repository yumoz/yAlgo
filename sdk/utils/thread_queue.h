/**
 * @file thread_queue.h
 * @brief 线程安全队列（header-only）
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_THREAD_QUEUE_H
#define YALGO_SDK_UTILS_THREAD_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace yalgo {
namespace utils {

/**
 * @class ThreadQueue
 * @brief 线程安全的 FIFO 队列
 * @tparam T 元素类型
 */
template<typename T>
class ThreadQueue {
public:
    /**
     * @brief 推入一个元素
     * @param item 要推入的元素
     */
    void push(T item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(item));
        cond_.notify_one();
    }

    /**
     * @brief 尝试弹出一个元素（带超时）
     * @param item 输出参数
     * @param timeoutMs 超时毫秒数，0 表示不等待
     * @return 成功弹出返回 true
     */
    bool tryPop(T& item, int timeoutMs = 0) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (timeoutMs <= 0) {
            if (queue_.empty()) return false;
        } else {
            cond_.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                          [this] { return !queue_.empty() || stopped_; });
        }
        if (stopped_ && queue_.empty()) return false;
        if (queue_.empty()) return false;
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    /**
     * @brief 阻塞等待并弹出一个元素
     * @return 弹出的元素
     */
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !queue_.empty() || stopped_; });
        if (stopped_ && queue_.empty()) return T{};
        T item = std::move(queue_.front());
        queue_.pop();
        return item;
    }

    /**
     * @brief 检查队列是否为空
     */
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    /**
     * @brief 获取队列大小
     */
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    /**
     * @brief 停止队列，唤醒所有等待线程
     */
    void stop() {
        std::lock_guard<std::mutex> lock(mutex_);
        stopped_ = true;
        cond_.notify_all();
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    bool stopped_ = false;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_THREAD_QUEUE_H
