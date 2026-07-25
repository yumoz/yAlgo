#include "base64.h"
#include <stdexcept>

namespace yalgo {
namespace utils {

static const char kEncodeTable[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const int kDecodeTable[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
    52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
    15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
    -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
    41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

std::string Base64::encode(const std::string& data) {
    return encode(reinterpret_cast<const uint8_t*>(data.data()), data.size());
}

std::string Base64::encode(const uint8_t* data, size_t len) {
    std::string result;
    result.reserve(((len + 2) / 3) * 4);

    size_t i = 0;
    while (i + 2 < len) {
        uint32_t n = (static_cast<uint32_t>(data[i]) << 16) |
                     (static_cast<uint32_t>(data[i + 1]) << 8) |
                     static_cast<uint32_t>(data[i + 2]);
        result += kEncodeTable[(n >> 18) & 0x3F];
        result += kEncodeTable[(n >> 12) & 0x3F];
        result += kEncodeTable[(n >> 6) & 0x3F];
        result += kEncodeTable[n & 0x3F];
        i += 3;
    }

    if (i < len) {
        uint32_t n = static_cast<uint32_t>(data[i]) << 16;
        if (i + 1 < len) {
            n |= static_cast<uint32_t>(data[i + 1]) << 8;
        }

        result += kEncodeTable[(n >> 18) & 0x3F];
        result += kEncodeTable[(n >> 12) & 0x3F];

        if (i + 1 < len) {
            result += kEncodeTable[(n >> 6) & 0x3F];
        } else {
            result += '=';
        }
        result += '=';
    }

    return result;
}

std::string Base64::decode(const std::string& encoded) {
    std::string result;
    result.reserve(encoded.size() * 3 / 4);

    uint32_t accum = 0;
    int bits = 0;

    for (char c : encoded) {
        if (c == '=' || c == '\n' || c == '\r') continue;

        unsigned char uc = static_cast<unsigned char>(c);
        if (uc > 255 || kDecodeTable[uc] == -1) {
            throw std::runtime_error("Invalid Base64 character");
        }

        accum = (accum << 6) | static_cast<uint32_t>(kDecodeTable[uc]);
        bits += 6;

        if (bits >= 8) {
            bits -= 8;
            result += static_cast<char>((accum >> bits) & 0xFF);
        }
    }

    return result;
}

} // namespace utils
} // namespace yalgo
