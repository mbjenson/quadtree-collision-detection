#include <string.h>
#include <filesystem>

#ifndef H_PROJECT_PATH
#define H_PROJECT_PATH

inline std::string getProjectPath() {
    std::string exePath(std::filesystem::canonical("/proc/self/exe"));
    std::size_t lfs = exePath.find_last_of("/");
    exePath = exePath.substr(0, lfs);
    lfs = exePath.find_last_of("/");
    exePath = exePath.substr(0, lfs);
    lfs = exePath.find_last_of("/");
    exePath = exePath.substr(0, lfs + 1);
    return exePath;
}

#endif