/**
 * @file json_utils.h
 * @brief JSON 文件读写工具
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_JSON_UTILS_H
#define YALGO_SDK_UTILS_JSON_UTILS_H

#include <string>
#include "utils_exports.h"
#include "../../3rd/cli/json.hpp"

namespace yalgo {
namespace utils {

/**
 * @class JsonUtils
 * @brief JSON 文件读写工具，封装 nlohmann/json
 */
class YALGO_UTILS_API JsonUtils {
public:
    /**
     * @brief 从文件加载 JSON
     * @param filePath 文件路径
     * @return 解析后的 JSON 对象
     * @throws std::runtime_error 读取或解析失败时抛出异常
     */
    static nlohmann::json load(const std::string& filePath);

    /**
     * @brief 保存 JSON 到文件
     * @param filePath 文件路径
     * @param j JSON 对象
     * @param prettyPrint 是否格式化输出
     */
    static void save(const std::string& filePath, const nlohmann::json& j,
                     bool prettyPrint = true);

    /**
     * @brief 从字符串解析 JSON
     * @param content JSON 字符串
     * @return 解析后的 JSON 对象
     */
    static nlohmann::json parseString(const std::string& content);

    /**
     * @brief 将 JSON 序列化为字符串
     * @param j JSON 对象
     * @param prettyPrint 是否格式化输出
     * @return JSON 字符串
     */
    static std::string stringify(const nlohmann::json& j, bool prettyPrint = true);

    /**
     * @brief 从文件加载 JSON，失败时返回默认值
     * @param filePath 文件路径
     * @param defaultVal 默认值
     * @return 解析后的 JSON 对象或默认值
     */
    static nlohmann::json loadOrDefault(const std::string& filePath,
                                         const nlohmann::json& defaultVal);

private:
    JsonUtils() = delete;
    ~JsonUtils() = delete;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_JSON_UTILS_H
