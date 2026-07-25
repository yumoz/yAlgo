#include "file_utils.h"
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#include <sys/stat.h>
#else
#include <sys/stat.h>
#endif

namespace yalgo {
namespace utils {

bool FileUtils::exists(const std::string& path) {
#ifdef _WIN32
    DWORD attr = GetFileAttributesA(path.c_str());
    return attr != INVALID_FILE_ATTRIBUTES;
#else
    struct stat stat_buf;
    return stat(path.c_str(), &stat_buf) == 0;
#endif
}

bool FileUtils::isFile(const std::string& path) {
#ifdef _WIN32
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat stat_buf;
    if (stat(path.c_str(), &stat_buf) != 0) return false;
    return S_ISREG(stat_buf.st_mode);
#endif
}

bool FileUtils::isDir(const std::string& path) {
#ifdef _WIN32
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat stat_buf;
    if (stat(path.c_str(), &stat_buf) != 0) return false;
    return S_ISDIR(stat_buf.st_mode);
#endif
}

std::string FileUtils::readFile(const std::string& path) {
    std::ifstream file(path, std::ios::in);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FileUtils::writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    return file.good();
}

std::string FileUtils::getFileName(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

std::string FileUtils::getDirName(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return ".";
    return path.substr(0, pos);
}

std::string FileUtils::getExtension(const std::string& path) {
    std::string filename = getFileName(path);
    size_t pos = filename.rfind('.');
    if (pos == std::string::npos || pos == 0) return "";
    return filename.substr(pos);
}

} // namespace utils
} // namespace yalgo
