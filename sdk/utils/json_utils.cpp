#include "json_utils.h"
#include "file_utils.h"

namespace yalgo {
namespace utils {

nlohmann::json JsonUtils::load(const std::string& filePath) {
    std::string content = FileUtils::readFile(filePath);
    return nlohmann::json::parse(content);
}

void JsonUtils::save(const std::string& filePath, const nlohmann::json& j,
                     bool prettyPrint) {
    std::string content = prettyPrint ? j.dump(4) : j.dump();
    FileUtils::writeFile(filePath, content);
}

nlohmann::json JsonUtils::parseString(const std::string& content) {
    return nlohmann::json::parse(content);
}

std::string JsonUtils::stringify(const nlohmann::json& j, bool prettyPrint) {
    return prettyPrint ? j.dump(4) : j.dump();
}

nlohmann::json JsonUtils::loadOrDefault(const std::string& filePath,
                                         const nlohmann::json& defaultVal) {
    try {
        return load(filePath);
    } catch (...) {
        return defaultVal;
    }
}

} // namespace utils
} // namespace yalgo
