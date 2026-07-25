/**
 * @file test_utils.cpp
 * @brief 工具模块单元测试
 * @author yAlgo Team
 * @date 2026-03-24
 */

#include <iostream>
#include <string>
#include "utils/exe_path.h"
#include "utils/string_utils.h"
#include "utils/file_utils.h"
#include "utils/ini_parser.h"
#include "utils/config_parser.h"

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

    // trim
    if (StringUtils::trim("  hello  ") != "hello") {
        std::cerr << "trim failed" << std::endl; ok = false;
    }
    // toLower
    if (StringUtils::toUpper("hello") != "HELLO") {
        std::cerr << "toUpper failed" << std::endl; ok = false;
    }
    // startsWith
    if (!StringUtils::startsWith("hello world", "hello")) {
        std::cerr << "startsWith failed" << std::endl; ok = false;
    }
    // endsWith
    if (!StringUtils::endsWith("hello world", "world")) {
        std::cerr << "endsWith failed" << std::endl; ok = false;
    }
    // split
    auto parts = StringUtils::split("a,b,c", ',');
    if (parts.size() != 3 || parts[0] != "a" || parts[2] != "c") {
        std::cerr << "split failed" << std::endl; ok = false;
    }
    // join
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
    if (!parser.hasSection("server")) {
        std::cerr << "IniParser hasSection failed" << std::endl; ok = false;
    }

    // roundtrip
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
    if (!parser.hasKey("VERSION")) {
        std::cerr << "ConfigParser hasKey failed" << std::endl; ok = false;
    }

    // set
    parser.set("NEW_KEY", "new_value");
    if (parser.getString("NEW_KEY") != "new_value") {
        std::cerr << "ConfigParser set failed" << std::endl; ok = false;
    }

    if (ok) std::cout << "ConfigParser tests passed!" << std::endl;
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
    
    if (all_passed) {
        std::cout << "All utils tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "Some utils tests failed!" << std::endl;
        return 1;
    }
}
