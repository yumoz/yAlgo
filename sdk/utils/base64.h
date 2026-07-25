/**
 * @file base64.h
 * @brief Base64 编解码工具
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_BASE64_H
#define YALGO_SDK_UTILS_BASE64_H

#include <string>
#include <cstdint>
#include <cstddef>
#include "utils_exports.h"

namespace yalgo {
namespace utils {

/**
 * @class Base64
 * @brief Base64 编解码工具
 */
class YALGO_UTILS_API Base64 {
public:
    /**
     * @brief 编码字符串为 Base64
     * @param data 输入数据
     * @return Base64 编码字符串
     */
    static std::string encode(const std::string& data);

    /**
     * @brief 编码二进制数据为 Base64
     * @param data 输入数据指针
     * @param len 数据长度
     * @return Base64 编码字符串
     */
    static std::string encode(const uint8_t* data, size_t len);

    /**
     * @brief 解码 Base64 字符串
     * @param encoded Base64 编码字符串
     * @return 解码后的数据
     * @throws std::runtime_error 无效的 Base64 字符串
     */
    static std::string decode(const std::string& encoded);

private:
    Base64() = delete;
    ~Base64() = delete;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_BASE64_H
