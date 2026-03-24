/**
 * @file exec_path_utils.h
 * @brief 跨平台可执行文件路径工具类
 * @author yAlgo Team
 * @date 2026-03-24
 */
#ifndef YALGO_SDK_UTILS_EXEC_PATH_UTILS_H
#define YALGO_SDK_UTILS_EXEC_PATH_UTILS_H

#include <string>
#include <stdexcept>
#include "utils_exports.h"

namespace yalgo {
namespace utils {

/**
 * @class ExecPathUtils
 * @brief 跨平台可执行文件路径工具类
 * 
 * 兼容 C++11/C++14/C++17，不依赖 C++17 filesystem 库
 */
class YALGO_UTILS_API ExecPathUtils {
public:
  /**
   * @brief 获取当前可执行文件的目录路径
   * @return 可执行文件所在目录的绝对路径（末尾无路径分隔符）
   * @throws std::runtime_error 获取失败时抛出异常（可捕获）
   */
  static std::string getExecutableDir();

  /**
   * @brief 获取当前可执行文件的完整路径
   * @return 可执行文件的绝对路径
   * @throws std::runtime_error 获取失败时抛出异常
   */
  static std::string getExecutablePath();

  /**
   * @brief 拼接可执行文件目录下的文件路径
   * @param filename 要拼接的文件名（支持相对子路径，如 "config/settings.ini"）
   * @return 拼接后的绝对路径
   * @throws std::runtime_error 路径无效时抛出异常
   */
  static std::string getFileInExeDir(const std::string& filename);

  /**
   * @brief 检查可执行文件目录下的文件是否存在
   * @param filename 要检查的文件名/路径
   * @return 存在且是普通文件返回true，否则返回false
   */
  static bool checkFileInExeDir(const std::string& filename);

  /**
   * @brief 获取当前系统类型
   * @return 系统类型字符串："windows"、"linux"、"macos"、"kylin_arm"、"kylin_x86" 或 "ubuntu"
   */
  static std::string getSystemType();

private:
  /**
   * @brief 私有工具函数：路径拼接（跨平台）
   * @param dir 目录路径
   * @param filename 文件名
   * @return 拼接后的路径
   */
  static std::string pathJoin(const std::string& dir, const std::string& filename);

  /**
   * @brief 私有工具函数：获取路径分隔符（\ 或 /）
   * @return 路径分隔符
   */
  static char getPathSeparator();
  
  /**
   * @brief 私有工具函数：规范化路径（处理相对路径、多余分隔符等）
   * @param path 原始路径
   * @return 规范化后的路径
   */
  static std::string normalizePath(const std::string& path);

  /**
   * @brief 禁用构造函数（工具类无需实例化）
   */
  ExecPathUtils() = delete;
  
  /**
   * @brief 禁用析构函数
   */
  ~ExecPathUtils() = delete;
  
  /**
   * @brief 禁用拷贝构造函数
   */
  ExecPathUtils(const ExecPathUtils&) = delete;
  
  /**
   * @brief 禁用赋值运算符
   */
  ExecPathUtils& operator=(const ExecPathUtils&) = delete;
};

} // namespace utils
} // namespace yalgo

#endif // YALGO_SDK_UTILS_EXEC_PATH_UTILS_H