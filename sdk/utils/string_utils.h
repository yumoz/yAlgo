/**
 * @file string_utils.h
 * @brief 字符串工具类（header-only）
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_STRING_UTILS_H
#define YALGO_SDK_UTILS_STRING_UTILS_H

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

namespace yalgo {
namespace utils {

/**
 * @class StringUtils
 * @brief 字符串操作工具类（header-only）
 */
class StringUtils {
public:
    /**
     * @brief 去除字符串首尾空白字符
     * @param str 输入字符串
     * @return 去除空白后的字符串
     */
    static std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        if (start == std::string::npos) return "";
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(start, end - start + 1);
    }

    /**
     * @brief 转换为小写
     * @param str 输入字符串
     * @return 小写字符串
     */
    static std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    /**
     * @brief 转换为大写
     * @param str 输入字符串
     * @return 大写字符串
     */
    static std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    /**
     * @brief 检查字符串是否以指定前缀开头
     * @param str 输入字符串
     * @param prefix 前缀
     * @return 以此前缀开头返回 true
     */
    static bool startsWith(const std::string& str, const std::string& prefix) {
        if (prefix.size() > str.size()) return false;
        return str.compare(0, prefix.size(), prefix) == 0;
    }

    /**
     * @brief 检查字符串是否以指定后缀结尾
     * @param str 输入字符串
     * @param suffix 后缀
     * @return 以此后缀结尾返回 true
     */
    static bool endsWith(const std::string& str, const std::string& suffix) {
        if (suffix.size() > str.size()) return false;
        return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    /**
     * @brief 按指定分隔符拆分字符串
     * @param str 输入字符串
     * @param delimiter 分隔符
     * @return 拆分后的字符串向量
     */
    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> result;
        std::string token;
        for (char c : str) {
            if (c == delimiter) {
                result.push_back(token);
                token.clear();
            } else {
                token += c;
            }
        }
        result.push_back(token);
        return result;
    }

    /**
     * @brief 将字符串向量用指定分隔符连接
     * @param parts 字符串向量
     * @param delimiter 分隔符
     * @return 连接后的字符串
     */
    static std::string join(const std::vector<std::string>& parts, const std::string& delimiter) {
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) result += delimiter;
            result += parts[i];
        }
        return result;
    }

private:
    StringUtils() = delete;
    ~StringUtils() = delete;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_STRING_UTILS_H
