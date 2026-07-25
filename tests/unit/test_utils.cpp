/**
 * @file test_utils.cpp
 * @brief 工具模块单元测试
 * @author yAlgo Team
 * @date 2026-03-24
 */

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "utils/exe_path.h"
#include "utils/string_utils.h"
#include "utils/file_utils.h"
#include "utils/ini_parser.h"
#include "utils/config_parser.h"
#include "utils/datetime_utils.h"
#include "utils/retry_utils.h"
#include "utils/thread_queue.h"
#include "utils/base64.h"
#include "utils/checksum.h"
#include "utils/hex_dump.h"
#include "utils/ring_buffer.h"

bool test_getSystemType() {
    std::string system_type = yalgo::utils::ExePath::getSystemType();
    if (system_type.empty()) {
        std::cerr << "getSystemType returned empty string" << std::endl;
        return false;
    }
    std::cout << "System type: " << system_type << std::endl;
    return true;
}

bool test_getExecutablePath() {
    try {
        std::string exe_path = yalgo::utils::ExePath::getExecutablePath();
        if (exe_path.empty()) {
            std::cerr << "getExecutablePath returned empty string" << std::endl;
            return false;
        }
        std::cout << "Executable path: " << exe_path << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "getExecutablePath exception: " << e.what() << std::endl;
        return false;
    }
}

bool test_getExecutableDir() {
    try {
        std::string exe_dir = yalgo::utils::ExePath::getExecutableDir();
        if (exe_dir.empty()) {
            std::cerr << "getExecutableDir returned empty string" << std::endl;
            return false;
        }
        std::cout << "Executable dir: " << exe_dir << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "getExecutableDir exception: " << e.what() << std::endl;
        return false;
    }
}

bool test_stringUtils() {
    using namespace yalgo::utils;
    bool ok = true;

    if (StringUtils::trim("  hello  ") != "hello") {
        std::cerr << "trim failed" << std::endl; ok = false;
    }
    if (StringUtils::toUpper("hello") != "HELLO") {
        std::cerr << "toUpper failed" << std::endl; ok = false;
    }
    if (StringUtils::toLower("HELLO") != "hello") {
        std::cerr << "toLower failed" << std::endl; ok = false;
    }
    if (!StringUtils::startsWith("hello world", "hello")) {
        std::cerr << "startsWith failed" << std::endl; ok = false;
    }
    if (!StringUtils::endsWith("hello world", "world")) {
        std::cerr << "endsWith failed" << std::endl; ok = false;
    }
    auto parts = StringUtils::split("a,b,c", ',');
    if (parts.size() != 3 || parts[0] != "a" || parts[2] != "c") {
        std::cerr << "split failed" << std::endl; ok = false;
    }
    std::string joined = StringUtils::join({"x", "y", "z"}, "-");
    if (joined != "x-y-z") {
        std::cerr << "join failed" << std::endl; ok = false;
    }

    if (ok) std::cout << "StringUtils tests passed!" << std::endl;
    return ok;
}

bool test_iniParser() {
    using namespace yalgo::utils;
    IniParser parser;
    std::string ini = R"(
# comment
[server]
host = 127.0.0.1
port = 8080
debug = true

[database]
driver = mysql
timeout = 30
)";
    if (!parser.parse(ini)) {
        std::cerr << "IniParser::parse failed" << std::endl;
        return false;
    }

    bool ok = true;
    if (parser.getString("server", "host") != "127.0.0.1") {
        std::cerr << "IniParser getString failed" << std::endl; ok = false;
    }
    if (parser.getInt("server", "port") != 8080) {
        std::cerr << "IniParser getInt failed" << std::endl; ok = false;
    }
    if (!parser.getBool("server", "debug")) {
        std::cerr << "IniParser getBool failed" << std::endl; ok = false;
    }
    if (parser.getString("database", "driver") != "mysql") {
        std::cerr << "IniParser section failed" << std::endl; ok = false;
    }

    std::string output = parser.toString();
    IniParser parser2;
    parser2.parse(output);
    if (parser2.getString("server", "host") != "127.0.0.1") {
        std::cerr << "IniParser roundtrip failed" << std::endl; ok = false;
    }

    if (ok) std::cout << "IniParser tests passed!" << std::endl;
    return ok;
}

bool test_configParser() {
    using namespace yalgo::utils;
    ConfigParser parser;
    std::string config = R"(
# app config
APP_NAME = yAlgo
VERSION = 1.0.0
DEBUG = true
PORT = 9090
)";
    if (!parser.parse(config)) {
        std::cerr << "ConfigParser::parse failed" << std::endl;
        return false;
    }

    bool ok = true;
    if (parser.getString("APP_NAME") != "yAlgo") {
        std::cerr << "ConfigParser getString failed" << std::endl; ok = false;
    }
    if (parser.getInt("PORT") != 9090) {
        std::cerr << "ConfigParser getInt failed" << std::endl; ok = false;
    }
    if (!parser.getBool("DEBUG")) {
        std::cerr << "ConfigParser getBool failed" << std::endl; ok = false;
    }

    parser.set("NEW_KEY", "new_value");
    if (parser.getString("NEW_KEY") != "new_value") {
        std::cerr << "ConfigParser set failed" << std::endl; ok = false;
    }

    if (ok) std::cout << "ConfigParser tests passed!" << std::endl;
    return ok;
}

bool test_datetimeUtils() {
    using namespace yalgo::utils;
    bool ok = true;

    std::string n = DatetimeUtils::now();
    if (n.size() != 19) {  // "YYYY-MM-DD HH:MM:SS" = 19 chars
        std::cerr << "now() format wrong: " << n << std::endl; ok = false;
    }
    std::cout << "now: " << n << std::endl;

    std::string nm = DatetimeUtils::nowMs();
    if (nm.size() != 23) {  // + ".mmm"
        std::cerr << "nowMs() format wrong: " << nm << std::endl; ok = false;
    }

    std::string t = DatetimeUtils::today();
    if (t.size() != 10) {  // "YYYY-MM-DD" = 10 chars
        std::cerr << "today() format wrong: " << t << std::endl; ok = false;
    }

    int64_t ts = DatetimeUtils::timestamp();
    if (ts <= 0) {
        std::cerr << "timestamp() returned non-positive" << std::endl; ok = false;
    }

    int64_t tsMs = DatetimeUtils::timestampMs();
    if (tsMs <= 0) {
        std::cerr << "timestampMs() returned non-positive" << std::endl; ok = false;
    }

    if (ok) std::cout << "DatetimeUtils tests passed!" << std::endl;
    return ok;
}

bool test_retryUtils() {
    using namespace yalgo::utils;
    bool ok = true;

    // 固定间隔重试：第 3 次成功
    int attempt = 0;
    bool success = RetryUtils::retry([&]() {
        return ++attempt == 3;
    }, 5, 10);
    if (!success || attempt != 3) {
        std::cerr << "retry failed" << std::endl; ok = false;
    }

    // 指数退避重试：始终失败
    attempt = 0;
    success = RetryUtils::retryWithBackoff([&]() {
        ++attempt;
        return false;
    }, 3, 10, 2.0);
    if (success || attempt != 4) {  // 1 initial + 3 retries = 4 attempts
        std::cerr << "retryWithBackoff failed" << std::endl; ok = false;
    }

    if (ok) std::cout << "RetryUtils tests passed!" << std::endl;
    return ok;
}

bool test_threadQueue() {
    using namespace yalgo::utils;
    bool ok = true;

    ThreadQueue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    if (q.size() != 3) {
        std::cerr << "ThreadQueue size failed" << std::endl; ok = false;
    }

    int item;
    if (!q.tryPop(item) || item != 1) {
        std::cerr << "ThreadQueue tryPop failed" << std::endl; ok = false;
    }
    if (!q.tryPop(item) || item != 2) {
        std::cerr << "ThreadQueue tryPop order failed" << std::endl; ok = false;
    }

    q.push(4);
    item = q.pop();
    if (item != 3) {
        std::cerr << "ThreadQueue pop failed" << std::endl; ok = false;
    }

    // 多线程测试
    ThreadQueue<int> mq;
    std::atomic<int> sum{0};
    std::thread producer([&]() {
        for (int i = 0; i < 100; ++i) mq.push(i);
    });
    std::thread consumer([&]() {
        for (int i = 0; i < 100; ++i) {
            int v;
            mq.tryPop(v, 1000);
            sum += v;
        }
    });
    producer.join();
    consumer.join();

    int expected = 100 * 99 / 2;
    if (sum.load() != expected) {
        std::cerr << "ThreadQueue multi-thread sum failed: " << sum.load()
                  << " != " << expected << std::endl; ok = false;
    }

    if (ok) std::cout << "ThreadQueue tests passed!" << std::endl;
    return ok;
}

bool test_base64() {
    using namespace yalgo::utils;
    bool ok = true;

    std::string encoded = Base64::encode("Hello, World!");
    std::string decoded = Base64::decode(encoded);
    if (decoded != "Hello, World!") {
        std::cerr << "Base64 roundtrip failed" << std::endl; ok = false;
    }

    // 二进制编码
    uint8_t data[] = {0x00, 0xFF, 0x80, 0x7F};
    std::string binEncoded = Base64::encode(data, sizeof(data));
    std::string binDecoded = Base64::decode(binEncoded);
    if (binDecoded.size() != sizeof(data) ||
        memcmp(binDecoded.data(), data, sizeof(data)) != 0) {
        std::cerr << "Base64 binary roundtrip failed" << std::endl; ok = false;
    }

    // 空字符串
    if (Base64::encode("") != "" || Base64::decode("") != "") {
        std::cerr << "Base64 empty string failed" << std::endl; ok = false;
    }

    if (ok) std::cout << "Base64 tests passed!" << std::endl;
    return ok;
}

bool test_checksum() {
    using namespace yalgo::utils;
    bool ok = true;

    uint32_t c1 = Checksum::crc32("Hello");
    uint32_t c2 = Checksum::crc32("Hello");
    if (c1 != c2) {
        std::cerr << "CRC32 consistency failed" << std::endl; ok = false;
    }

    uint32_t c3 = Checksum::crc32("World");
    if (c1 == c3) {
        std::cerr << "CRC32 different input collision" << std::endl; ok = false;
    }

    // 二进制数据
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    uint32_t c4 = Checksum::crc32(data, sizeof(data));
    if (c4 == 0) {
        std::cerr << "CRC32 binary returned 0" << std::endl; ok = false;
    }

    if (ok) std::cout << "Checksum tests passed!" << std::endl;
    return ok;
}

bool test_hexDump() {
    using namespace yalgo::utils;
    bool ok = true;

    std::string data = "Hello, World!";
    std::string dump = HexDump::dump(data);
    if (dump.empty()) {
        std::cerr << "HexDump returned empty" << std::endl; ok = false;
    }

    // 包含偏移量
    if (dump.find("00000000") == std::string::npos) {
        std::cerr << "HexDump missing offset" << std::endl; ok = false;
    }

    // 包含 ASCII
    if (dump.find("Hello") == std::string::npos) {
        std::cerr << "HexDump missing ASCII" << std::endl; ok = false;
    }

    // 二进制数据
    uint8_t binData[] = {0x00, 0x7F, 0x80, 0xFF};
    std::string binDump = HexDump::dump(binData, sizeof(binData));
    if (binDump.find("7f") == std::string::npos && binDump.find("7F") == std::string::npos) {
        std::cerr << "HexDump binary format wrong" << std::endl; ok = false;
    }

    if (ok) std::cout << "HexDump tests passed!" << std::endl;
    return ok;
}

bool test_ringBuffer() {
    using namespace yalgo::utils;
    bool ok = true;

    RingBuffer<int> rb(4);
    if (!rb.empty()) {
        std::cerr << "RingBuffer should be empty" << std::endl; ok = false;
    }

    rb.push(1);
    rb.push(2);
    rb.push(3);
    if (rb.size() != 3) {
        std::cerr << "RingBuffer size wrong" << std::endl; ok = false;
    }

    // 填满
    rb.push(4);
    if (!rb.full()) {
        std::cerr << "RingBuffer should be full" << std::endl; ok = false;
    }

    // 覆盖最旧的
    rb.push(5);
    if (rb.front() != 2) {
        std::cerr << "RingBuffer overwrite wrong, front=" << rb.front() << std::endl; ok = false;
    }

    int item;
    if (!rb.pop(item) || item != 2) {
        std::cerr << "RingBuffer pop failed" << std::endl; ok = false;
    }
    if (!rb.pop(item) || item != 3) {
        std::cerr << "RingBuffer pop order failed" << std::endl; ok = false;
    }

    rb.clear();
    if (!rb.empty()) {
        std::cerr << "RingBuffer clear failed" << std::endl; ok = false;
    }

    // 空缓冲区 pop
    if (rb.pop(item)) {
        std::cerr << "RingBuffer pop on empty should fail" << std::endl; ok = false;
    }

    if (ok) std::cout << "RingBuffer tests passed!" << std::endl;
    return ok;
}

int main() {
    std::cout << "Running utils module unit tests..." << std::endl;
    
    bool all_passed = true;
    
    all_passed &= test_getSystemType();
    all_passed &= test_getExecutablePath();
    all_passed &= test_getExecutableDir();
    all_passed &= test_stringUtils();
    all_passed &= test_iniParser();
    all_passed &= test_configParser();
    all_passed &= test_datetimeUtils();
    all_passed &= test_retryUtils();
    all_passed &= test_threadQueue();
    all_passed &= test_base64();
    all_passed &= test_checksum();
    all_passed &= test_hexDump();
    all_passed &= test_ringBuffer();
    
    if (all_passed) {
        std::cout << "All utils tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "Some utils tests failed!" << std::endl;
        return 1;
    }
}
