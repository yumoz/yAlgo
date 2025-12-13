#include "exec_path_utils.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#endif

namespace yalgo {
namespace utils {

// 获取可执行文件完整路径（核心实现）
std::string ExecPathUtils::getExecutablePath() {
#ifdef _WIN32
    std::vector<wchar_t> buffer(MAX_PATH + 1, 0);
    DWORD len = GetModuleFileNameW(NULL, buffer.data(), static_cast<DWORD>(buffer.size()));
    if (len == 0) {
        DWORD error = GetLastError();
        throw std::runtime_error(std::string("GetModuleFileNameW failed with error code: ") + std::to_string(error));
    }
    if (len >= buffer.size()) {
        throw std::runtime_error("Executable path exceeds maximum length");
    }
    
    // 计算所需的多字节缓冲区大小
    int buf_len = WideCharToMultiByte(CP_UTF8, 0, buffer.data(), static_cast<int>(len), NULL, 0, NULL, NULL);
    if (buf_len <= 0) {
        DWORD error = GetLastError();
        throw std::runtime_error(std::string("WideCharToMultiByte (size calculation) failed with error: ") + std::to_string(error));
    }
    
    // 使用vector管理多字节字符串内存
    std::vector<char> mb_str(buf_len, 0);
    if (WideCharToMultiByte(CP_UTF8, 0, buffer.data(), static_cast<int>(len), mb_str.data(), buf_len, NULL, NULL) == 0) {
        DWORD error = GetLastError();
        throw std::runtime_error(std::string("WideCharToMultiByte (conversion) failed with error: ") + std::to_string(error));
    }
    
    return normalizePath(std::string(mb_str.data(), buf_len - 1));
#else
    std::vector<char> buffer(PATH_MAX + 1, 0);
    ssize_t len = readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
    if (len == -1) {
        // 尝试多种平台特定的获取路径方法
        const char* argv0 = getenv("_");
        if (argv0 == nullptr || strlen(argv0) == 0) {
#ifdef __APPLE__
            // macOS特定实现
            char path[PATH_MAX] = {0};
            uint32_t size = sizeof(path);
            if (_NSGetExecutablePath(path, &size) != 0) {
                throw std::runtime_error("Failed to get executable path on macOS");
            }
            return normalizePath(std::string(path));
#else
            throw std::runtime_error("Failed to get executable path: readlink and getenv both failed");
#endif
        }
        return normalizePath(std::string(argv0));
    }
    return normalizePath(std::string(buffer.data(), len));
#endif
}

// 获取可执行文件目录
std::string ExecPathUtils::getExecutableDir() {
    std::string exe_path = getExecutablePath();
    size_t sep_pos = exe_path.find_last_of(getPathSeparator());
    if (sep_pos == std::string::npos) {
        return "."; // 兜底：当前目录
    }
    return exe_path.substr(0, sep_pos);
}

// 路径拼接（跨平台）
std::string ExecPathUtils::pathJoin(const std::string& dir, const std::string& filename) {
    if (dir.empty()) return filename;
    if (filename.empty()) return dir;

    char sep = getPathSeparator();
    std::string result = dir;
    
    // 检查目录末尾是否已有分隔符
    if (result.back() != sep) {
        result += sep;
    }
    
    // 避免重复的分隔符
    if (!filename.empty() && filename.front() == sep) {
        result += filename.substr(1);
    } else {
        result += filename;
    }
    
    return normalizePath(result);
}

// 获取路径分隔符
char ExecPathUtils::getPathSeparator() {
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

// 拼接可执行目录下的文件路径
std::string ExecPathUtils::getFileInExeDir(const std::string& filename) {
    if (filename.empty()) {
        throw std::runtime_error("Filename cannot be empty");
    }
    
    // 安全性检查：防止路径遍历攻击
    if (filename.find("..") != std::string::npos) {
        // 可以选择抛出异常或自动规范化路径
        // throw std::runtime_error("Relative path traversal (..) not allowed");
    }
    
    std::string exe_dir = getExecutableDir();
    return pathJoin(exe_dir, filename);
}

// 检查文件是否存在
bool ExecPathUtils::checkFileInExeDir(const std::string& filename) {
    try {
        std::string file_path = getFileInExeDir(filename);
#ifdef _WIN32
        DWORD attr = GetFileAttributesA(file_path.c_str());
        return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
#else
        struct stat stat_buf;
        if (stat(file_path.c_str(), &stat_buf) != 0) {
            return false;
        }
        return S_ISREG(stat_buf.st_mode); // 检查是否为普通文件
#endif
    } catch (const std::exception&) {
        return false;
    }
}

// 规范化路径（新增函数）
std::string ExecPathUtils::normalizePath(const std::string& path) {
    // 简单实现：处理连续的分隔符
    std::string result;
    char sep = getPathSeparator();
    
    for (size_t i = 0; i < path.length(); ++i) {
        if (i > 0 && path[i] == sep && path[i-1] == sep) {
            // 跳过连续的分隔符
            continue;
        }
        result += path[i];
    }
    
    // 可以扩展更复杂的路径规范化逻辑
    return result;
}

} // namespace utils
} // namespace yalgo