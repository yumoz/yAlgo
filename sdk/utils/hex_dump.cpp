#include "hex_dump.h"
#include <sstream>
#include <iomanip>

namespace yalgo {
namespace utils {

std::string HexDump::dump(const uint8_t* data, size_t len, size_t bytesPerLine) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (size_t offset = 0; offset < len; offset += bytesPerLine) {
        // 偏移量
        oss << std::setw(8) << offset << "  ";

        // 十六进制部分
        size_t lineLen = std::min(bytesPerLine, len - offset);
        for (size_t i = 0; i < bytesPerLine; ++i) {
            if (i < lineLen) {
                oss << std::setw(2) << static_cast<int>(data[offset + i]) << " ";
            } else {
                oss << "   ";
            }
            if (i == bytesPerLine / 2 - 1) oss << " ";
        }

        oss << " |";

        // ASCII 部分
        for (size_t i = 0; i < lineLen; ++i) {
            uint8_t c = data[offset + i];
            oss << (c >= 0x20 && c <= 0x7E ? static_cast<char>(c) : '.');
        }

        oss << "|" << "\n";
    }

    return oss.str();
}

std::string HexDump::dump(const std::string& data, size_t bytesPerLine) {
    return dump(reinterpret_cast<const uint8_t*>(data.data()), data.size(), bytesPerLine);
}

} // namespace utils
} // namespace yalgo
