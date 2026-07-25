/**
 * @file ring_buffer.h
 * @brief 环形缓冲区（header-only）
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_RING_BUFFER_H
#define YALGO_SDK_UTILS_RING_BUFFER_H

#include <vector>
#include <cstddef>

namespace yalgo {
namespace utils {

/**
 * @class RingBuffer
 * @brief 固定容量的环形缓冲区
 * @tparam T 元素类型
 */
template<typename T>
class RingBuffer {
public:
    /**
     * @brief 构造指定容量的环形缓冲区
     * @param capacity 缓冲区容量
     */
    explicit RingBuffer(size_t capacity)
        : buffer_(capacity), capacity_(capacity), head_(0), tail_(0), size_(0) {}

    /**
     * @brief 推入一个元素，缓冲区满时覆盖最旧的元素
     * @param item 要推入的元素
     * @return 是否覆盖了旧数据
     */
    bool push(const T& item) {
        bool overwritten = (size_ == capacity_);
        buffer_[tail_] = item;
        tail_ = (tail_ + 1) % capacity_;
        if (overwritten) {
            head_ = (head_ + 1) % capacity_;
        } else {
            ++size_;
        }
        return overwritten;
    }

    /**
     * @brief 弹出最旧的元素
     * @param item 输出参数
     * @return 成功弹出返回 true，缓冲区为空返回 false
     */
    bool pop(T& item) {
        if (size_ == 0) return false;
        item = buffer_[head_];
        head_ = (head_ + 1) % capacity_;
        --size_;
        return true;
    }

    /**
     * @brief 查看最旧的元素（不弹出）
     * @return 最旧元素的引用
     * @throws std::out_of_range 缓冲区为空
     */
    const T& front() const {
        return buffer_[head_];
    }

    /**
     * @brief 缓冲区是否已满
     */
    bool full() const { return size_ == capacity_; }

    /**
     * @brief 缓冲区是否为空
     */
    bool empty() const { return size_ == 0; }

    /**
     * @brief 当前元素数量
     */
    size_t size() const { return size_; }

    /**
     * @brief 缓冲区总容量
     */
    size_t capacity() const { return capacity_; }

    /**
     * @brief 清空缓冲区
     */
    void clear() {
        head_ = 0;
        tail_ = 0;
        size_ = 0;
    }

private:
    std::vector<T> buffer_;
    size_t capacity_;
    size_t head_;
    size_t tail_;
    size_t size_;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_RING_BUFFER_H
