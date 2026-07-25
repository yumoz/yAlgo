#include "config_parser.h"
#include "string_utils.h"
#include "file_utils.h"
#include <fstream>
#include <sstream>

namespace yalgo {
namespace utils {

bool ConfigParser::load(const std::string& filePath) {
    try {
        std::string content = FileUtils::readFile(filePath);
        return parse(content);
    } catch (...) {
        return false;
    }
}

bool ConfigParser::parse(const std::string& content) {
    data_.clear();
    std::istringstream stream(content);
    std::string line;

    while (std::getline(stream, line)) {
        line = StringUtils::trim(line);

        // 跳过空行和注释
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }

        // key = value 或 key=value
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string key = StringUtils::trim(line.substr(0, eqPos));
            std::string value = StringUtils::trim(line.substr(eqPos + 1));

            // 去除值两侧的引号
            if (value.size() >= 2) {
                if ((value.front() == '"' && value.back() == '"') ||
                    (value.front() == '\'' && value.back() == '\'')) {
                    value = value.substr(1, value.size() - 2);
                }
            }

            data_[key] = value;
        }
    }
    return true;
}

std::string ConfigParser::getString(const std::string& key, const std::string& defaultVal) const {
    auto it = data_.find(key);
    if (it == data_.end()) return defaultVal;
    return it->second;
}

int ConfigParser::getInt(const std::string& key, int defaultVal) const {
    std::string val = getString(key);
    if (val.empty()) return defaultVal;
    try {
        return std::stoi(val);
    } catch (...) {
        return defaultVal;
    }
}

double ConfigParser::getDouble(const std::string& key, double defaultVal) const {
    std::string val = getString(key);
    if (val.empty()) return defaultVal;
    try {
        return std::stod(val);
    } catch (...) {
        return defaultVal;
    }
}

bool ConfigParser::getBool(const std::string& key, bool defaultVal) const {
    std::string val = StringUtils::toLower(getString(key));
    if (val.empty()) return defaultVal;
    if (val == "true" || val == "1" || val == "yes" || val == "on") return true;
    if (val == "false" || val == "0" || val == "no" || val == "off") return false;
    return defaultVal;
}

bool ConfigParser::hasKey(const std::string& key) const {
    return data_.find(key) != data_.end();
}

void ConfigParser::set(const std::string& key, const std::string& value) {
    data_[key] = value;
}

std::vector<std::string> ConfigParser::keys() const {
    std::vector<std::string> result;
    for (const auto& pair : data_) {
        result.push_back(pair.first);
    }
    return result;
}

std::string ConfigParser::toString() const {
    std::ostringstream oss;
    for (const auto& kv : data_) {
        oss << kv.first << " = " << kv.second << "\n";
    }
    return oss.str();
}

bool ConfigParser::save(const std::string& filePath) const {
    return FileUtils::writeFile(filePath, toString());
}

} // namespace utils
} // namespace yalgo
