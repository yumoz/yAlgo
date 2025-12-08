#ifndef YUTILS_SDK_UTILS_EXEC_PATH_UTILS_H
#define YUTILS_SDK_UTILS_EXEC_PATH_UTILS_H

#include <string>
#include <stdexcept>
#include "utils_exports.h"

namespace yutils {
namespace utils {

/**
 * 跨平台可执行文件路径工具类
 * 兼容 C++11/C++14/C++17，不依赖 C++17 filesystem 库
 */
class YUTILS_UTILS_API ExecPathUtils {
public:
  /**
   * 获取当前可执行文件的目录路径
   * @return 可执行文件所在目录的绝对路径（末尾无路径分隔符）
   * @throws std::runtime_error 获取失败时抛出异常（可捕获）
   */
  static std::string getExecutableDir();

  /**
   * 获取当前可执行文件的完整路径
   * @return 可执行文件的绝对路径
   * @throws std::runtime_error 获取失败时抛出异常
   */
  static std::string getExecutablePath();

  /**
   * 拼接可执行文件目录下的文件路径
   * @param filename 要拼接的文件名（支持相对子路径，如 "config/settings.ini"）
   * @return 拼接后的绝对路径
   * @throws std::runtime_error 路径无效时抛出异常
   */
  static std::string getFileInExeDir(const std::string& filename);

  /**
   * 检查可执行文件目录下的文件是否存在
   * @param filename 要检查的文件名/路径
   * @return 存在且是普通文件返回true，否则返回false
   */
  static bool checkFileInExeDir(const std::string& filename);

private:
  // 私有工具函数：路径拼接（跨平台）
  static std::string pathJoin(const std::string& dir, const std::string& filename);

  // 私有工具函数：获取路径分隔符（\ 或 /）
  static char getPathSeparator();
  
  // 私有工具函数：规范化路径（处理相对路径、多余分隔符等）
  static std::string normalizePath(const std::string& path);

  // 禁用构造函数（工具类无需实例化）
  ExecPathUtils() = delete;
  ~ExecPathUtils() = delete;
  ExecPathUtils(const ExecPathUtils&) = delete;
  ExecPathUtils& operator=(const ExecPathUtils&) = delete;
};

} // namespace utils
} // namespace yutils

#endif // YUTILS_SDK_UTILS_EXEC_PATH_UTILS_H