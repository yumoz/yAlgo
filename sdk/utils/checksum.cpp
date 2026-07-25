#include "checksum.h"
#include <fstream>

namespace yalgo {
namespace utils {

uint32_t Checksum::crc32Table_[256];
bool Checksum::tableInitialized_ = false;

void Checksum::initTable() {
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (int j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
        crc32Table_[i] = crc;
    }
    tableInitialized_ = true;
}

uint32_t Checksum::crc32(const std::string& data) {
    return crc32(reinterpret_cast<const uint8_t*>(data.data()), data.size());
}

uint32_t Checksum::crc32(const uint8_t* data, size_t len) {
    if (!tableInitialized_) initTable();

    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc = crc32Table_[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

uint32_t Checksum::crc32File(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    if (!tableInitialized_) initTable();

    uint32_t crc = 0xFFFFFFFF;
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        for (std::streamsize i = 0; i < file.gcount(); ++i) {
            crc = crc32Table_[(crc ^ static_cast<uint8_t>(buffer[i])) & 0xFF] ^ (crc >> 8);
        }
    }
    // 处理剩余字节
    for (std::streamsize i = 0; i < file.gcount(); ++i) {
        crc = crc32Table_[(crc ^ static_cast<uint8_t>(buffer[i])) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

} // namespace utils
} // namespace yalgo
