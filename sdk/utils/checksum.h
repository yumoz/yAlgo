/**
 * @file checksum.h
 * @brief 校验和工具
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_CHECKSUM_H
#define YALGO_SDK_UTILS_CHECKSUM_H

#include <string>
#include <cstdint>
#include <cstddef>
#include "utils_exports.h"

namespace yalgo {
namespace utils {

/**
 * @class Checksum
 * @brief 校验和计算工具
 */
class YALGO_UTILS_API Checksum {
public:
    /**
     * @brief 计算字符串的 CRC32
     * @param data 输入数据
     * @return CRC32 校验值
     */
    static uint32_t crc32(const std::string& data);

    /**
     * @brief 计算二进制数据的 CRC32
     * @param data 输入数据指针
     * @param len 数据长度
     * @return CRC32 校验值
     */
    static uint32_t crc32(const uint8_t* data, size_t len);

    /**
     * @brief 计算文件的 CRC32
     * @param filePath 文件路径
     * @return CRC32 校验值
     * @throws std::runtime_error 文件读取失败
     */
    static uint32_t crc32File(const std::string& filePath);

private:
    static uint32_t crc32Table_[256];
    static bool tableInitialized_;
    static void initTable();

    Checksum() = delete;
    ~Checksum() = delete;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_CHECKSUM_H
