/**
 * @file config_parser.h
 * @brief 简易 key=value 配置解析器
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_CONFIG_PARSER_H
#define YALGO_SDK_UTILS_CONFIG_PARSER_H

#include <string>
#include <map>
#include <vector>
#include "utils_exports.h"

namespace yalgo {
namespace utils {

/**
 * @class ConfigParser
 * @brief 简易 key=value 配置文件解析器
 *
 * 支持 .env / config 格式：KEY=VALUE、# 注释、无 section
 * 比 IniParser 更轻量
 */
class YALGO_UTILS_API ConfigParser {
public:
    /**
     * @brief 从文件加载配置
     * @param filePath 文件路径
     * @return 加载成功返回 true
     */
    bool load(const std::string& filePath);

    /**
     * @brief 从字符串解析配置
     * @param content 配置字符串
     * @return 解析成功返回 true
     */
    bool parse(const std::string& content);

    /**
     * @brief 获取字符串值
     * @param key 键名
     * @param defaultVal 默认值
     * @return 对应值，不存在返回默认值
     */
    std::string getString(const std::string& key, const std::string& defaultVal = "") const;

    /**
     * @brief 获取整数值
     */
    int getInt(const std::string& key, int defaultVal = 0) const;

    /**
     * @brief 获取浮点值
     */
    double getDouble(const std::string& key, double defaultVal = 0.0) const;

    /**
     * @brief 获取布尔值
     */
    bool getBool(const std::string& key, bool defaultVal = false) const;

    /**
     * @brief 检查键是否存在
     */
    bool hasKey(const std::string& key) const;

    /**
     * @brief 设置键值对
     * @param key 键名
     * @param value 值
     */
    void set(const std::string& key, const std::string& value);

    /**
     * @brief 获取所有键名
     */
    std::vector<std::string> keys() const;

    /**
     * @brief 序列化为 key=value 格式字符串
     */
    std::string toString() const;

    /**
     * @brief 保存到文件
     */
    bool save(const std::string& filePath) const;

private:
    std::map<std::string, std::string> data_;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_CONFIG_PARSER_H
