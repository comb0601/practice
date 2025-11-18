# Lesson 95: Cross-Platform Development

**Duration**: 8 hours
**Difficulty**: Advanced
**Prerequisites**: Lessons 1-94

## Table of Contents
1. Platform Abstractions
2. Compiler Differences
3. File System Operations
4. Threading Across Platforms
5. Build Systems

---

## 1. Cross-Platform File Operations

```cpp
#include <iostream>
#include <filesystem>
#include <string>

namespace CrossPlatform {

#ifdef _WIN32
    #include <windows.h>
    #define PATH_SEPARATOR '\\'
#else
    #include <unistd.h>
    #define PATH_SEPARATOR '/'
#endif

class FileSystem {
public:
    static std::string getCurrentDirectory() {
        #ifdef _WIN32
            char buffer[MAX_PATH];
            GetCurrentDirectoryA(MAX_PATH, buffer);
            return std::string(buffer);
        #else
            char buffer[PATH_MAX];
            getcwd(buffer, PATH_MAX);
            return std::string(buffer);
        #endif
    }

    static bool exists(const std::string& path) {
        return std::filesystem::exists(path);
    }

    static std::string joinPath(const std::string& a, const std::string& b) {
        return (std::filesystem::path(a) / b).string();
    }
};

// Platform-specific threading
class Thread {
public:
    static int getProcessorCount() {
        #ifdef _WIN32
            SYSTEM_INFO sysinfo;
            GetSystemInfo(&sysinfo);
            return sysinfo.dwNumberOfProcessors;
        #else
            return sysconf(_SC_NPROCESSORS_ONLN);
        #endif
    }
};

} // namespace CrossPlatform

int main() {
    std::cout << "Current Directory: "
              << CrossPlatform::FileSystem::getCurrentDirectory() << "\n";
    std::cout << "Processors: "
              << CrossPlatform::Thread::getProcessorCount() << "\n";

    return 0;
}
```

---

## Summary

✅ **Platform Abstractions**: Hide OS differences
✅ **Filesystem**: Cross-platform file ops
✅ **Threading**: Portable concurrency
✅ **CMake**: Universal build system

**Compilation**:
```bash
# All platforms
cmake -B build
cmake --build build
```
