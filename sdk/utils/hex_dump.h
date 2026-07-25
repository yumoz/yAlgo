/**
 * @file hex_dump.h
 * @brief 十六进制转储工具
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_HEX_DUMP_H
#define YALGO_SDK_UTILS_HEX_DUMP_H

#include <string>
#include <cstdint>
#include <cstddef>
#include "utils_exports.h"

namespace yalgo {
namespace utils {

/**
 * @class HexDump
 * @brief 十六进制转储工具，用于调试二进制数据
 */
class YALGO_UTILS_API HexDump {
public:
    /**
     * @brief 将二进制数据转储为十六进制格式
     * @param data 数据指针
     * @param len 数据长度
     * @param bytesPerLine 每行显示的字节数
     * @return 格式化的十六进制字符串
     */
    static std::string dump(const uint8_t* data, size_t len, size_t bytesPerLine = 16);

    /**
     * @brief 将字符串转储为十六进制格式
     * @param data 输入字符串
     * @param bytesPerLine 每行显示的字节数
     * @return 格式化的十六进制字符串
     */
    static std::string dump(const std::string& data, size_t bytesPerLine = 16);

private:
    HexDump() = delete;
    ~HexDump() = delete;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_HEX_DUMP_H
