/**
 * @file ini_parser.h
 * @brief INI 配置文件解析器
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_INI_PARSER_H
#define YALGO_SDK_UTILS_INI_PARSER_H

#include <string>
#include <map>
#include <vector>
#include "utils_exports.h"

namespace yalgo {
namespace utils {

/**
 * @class IniParser
 * @brief INI 格式配置文件解析器
 *
 * 支持标准 INI 格式：[section]、key = value、# 注释、空行
 */
class YALGO_UTILS_API IniParser {
public:
    /**
     * @brief 从文件加载 INI 配置
     * @param filePath 文件路径
     * @return 加载成功返回 true
     */
    bool load(const std::string& filePath);

    /**
     * @brief 从字符串解析 INI 配置
     * @param content INI 格式字符串
     * @return 解析成功返回 true
     */
    bool parse(const std::string& content);

    /**
     * @brief 获取字符串值
     * @param section 节名
     * @param key 键名
     * @param defaultVal 默认值
     * @return 对应值，不存在返回默认值
     */
    std::string getString(const std::string& section, const std::string& key,
                          const std::string& defaultVal = "") const;

    /**
     * @brief 获取整数值
     */
    int getInt(const std::string& section, const std::string& key,
               int defaultVal = 0) const;

    /**
     * @brief 获取浮点值
     */
    double getDouble(const std::string& section, const std::string& key,
                     double defaultVal = 0.0) const;

    /**
     * @brief 获取布尔值（支持 true/false/1/0/yes/no）
     */
    bool getBool(const std::string& section, const std::string& key,
                 bool defaultVal = false) const;

    /**
     * @brief 获取所有节名
     */
    std::vector<std::string> sections() const;

    /**
     * @brief 检查节是否存在
     */
    bool hasSection(const std::string& section) const;

    /**
     * @brief 检查键是否存在
     */
    bool hasKey(const std::string& section, const std::string& key) const;

    /**
     * @brief 序列化为 INI 格式字符串
     */
    std::string toString() const;

    /**
     * @brief 保存到文件
     */
    bool save(const std::string& filePath) const;

private:
    std::map<std::string, std::map<std::string, std::string>> data_;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_INI_PARSER_H
