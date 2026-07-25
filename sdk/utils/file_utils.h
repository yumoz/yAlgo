/**
 * @file file_utils.h
 * @brief 文件系统工具类
 * @author yAlgo Team
 * @date 2026-07-25
 */
#ifndef YALGO_SDK_UTILS_FILE_UTILS_H
#define YALGO_SDK_UTILS_FILE_UTILS_H

#include <string>
#include "utils_exports.h"

namespace yalgo {
namespace utils {

/**
 * @class FileUtils
 * @brief 文件系统操作工具类
 */
class YALGO_UTILS_API FileUtils {
public:
    /**
     * @brief 检查文件或目录是否存在
     * @param path 文件或目录路径
     * @return 存在返回 true
     */
    static bool exists(const std::string& path);

    /**
     * @brief 检查路径是否为普通文件
     * @param path 文件路径
     * @return 是普通文件返回 true
     */
    static bool isFile(const std::string& path);

    /**
     * @brief 检查路径是否为目录
     * @param path 目录路径
     * @return 是目录返回 true
     */
    static bool isDir(const std::string& path);

    /**
     * @brief 读取文件全部内容
     * @param path 文件路径
     * @return 文件内容字符串
     * @throws std::runtime_error 读取失败时抛出异常
     */
    static std::string readFile(const std::string& path);

    /**
     * @brief 将内容写入文件（覆盖已有内容）
     * @param path 文件路径
     * @param content 要写入的内容
     * @return 写入成功返回 true
     */
    static bool writeFile(const std::string& path, const std::string& content);

    /**
     * @brief 获取文件名（含扩展名）
     * @param path 文件路径
     * @return 文件名
     */
    static std::string getFileName(const std::string& path);

    /**
     * @brief 获取目录部分
     * @param path 文件路径
     * @return 目录路径
     */
    static std::string getDirName(const std::string& path);

    /**
     * @brief 获取文件扩展名（含点号）
     * @param path 文件路径
     * @return 扩展名，如 ".txt"
     */
    static std::string getExtension(const std::string& path);

private:
    FileUtils() = delete;
    ~FileUtils() = delete;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_FILE_UTILS_H
