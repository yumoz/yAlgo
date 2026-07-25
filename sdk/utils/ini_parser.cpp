#include "ini_parser.h"
#include "string_utils.h"
#include "file_utils.h"
#include <fstream>
#include <sstream>

namespace yalgo {
namespace utils {

bool IniParser::load(const std::string& filePath) {
    try {
        std::string content = FileUtils::readFile(filePath);
        return parse(content);
    } catch (...) {
        return false;
    }
}

bool IniParser::parse(const std::string& content) {
    data_.clear();
    std::string currentSection;
    std::istringstream stream(content);
    std::string line;

    while (std::getline(stream, line)) {
        // 去除行首尾空白
        line = StringUtils::trim(line);

        // 跳过空行和注释
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }

        // 节名 [section]
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
            currentSection = StringUtils::trim(currentSection);
            continue;
        }

        // key = value
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string key = StringUtils::trim(line.substr(0, eqPos));
            std::string value = StringUtils::trim(line.substr(eqPos + 1));
            data_[currentSection][key] = value;
        }
    }
    return true;
}

std::string IniParser::getString(const std::string& section, const std::string& key,
                                 const std::string& defaultVal) const {
    auto secIt = data_.find(section);
    if (secIt == data_.end()) return defaultVal;
    auto keyIt = secIt->second.find(key);
    if (keyIt == secIt->second.end()) return defaultVal;
    return keyIt->second;
}

int IniParser::getInt(const std::string& section, const std::string& key,
                      int defaultVal) const {
    std::string val = getString(section, key);
    if (val.empty()) return defaultVal;
    try {
        return std::stoi(val);
    } catch (...) {
        return defaultVal;
    }
}

double IniParser::getDouble(const std::string& section, const std::string& key,
                            double defaultVal) const {
    std::string val = getString(section, key);
    if (val.empty()) return defaultVal;
    try {
        return std::stod(val);
    } catch (...) {
        return defaultVal;
    }
}

bool IniParser::getBool(const std::string& section, const std::string& key,
                        bool defaultVal) const {
    std::string val = StringUtils::toLower(getString(section, key));
    if (val.empty()) return defaultVal;
    if (val == "true" || val == "1" || val == "yes" || val == "on") return true;
    if (val == "false" || val == "0" || val == "no" || val == "off") return false;
    return defaultVal;
}

std::vector<std::string> IniParser::sections() const {
    std::vector<std::string> result;
    for (const auto& pair : data_) {
        result.push_back(pair.first);
    }
    return result;
}

bool IniParser::hasSection(const std::string& section) const {
    return data_.find(section) != data_.end();
}

bool IniParser::hasKey(const std::string& section, const std::string& key) const {
    auto secIt = data_.find(section);
    if (secIt == data_.end()) return false;
    return secIt->second.find(key) != secIt->second.end();
}

std::string IniParser::toString() const {
    std::ostringstream oss;
    for (const auto& section : data_) {
        if (!section.first.empty()) {
            oss << "[" << section.first << "]\n";
        }
        for (const auto& kv : section.second) {
            oss << kv.first << " = " << kv.second << "\n";
        }
        oss << "\n";
    }
    return oss.str();
}

bool IniParser::save(const std::string& filePath) const {
    return FileUtils::writeFile(filePath, toString());
}

} // namespace utils
} // namespace yalgo
