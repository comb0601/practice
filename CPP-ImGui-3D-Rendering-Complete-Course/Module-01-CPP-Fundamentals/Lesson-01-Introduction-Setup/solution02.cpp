#include <iostream>

/**
 * Solution 02: Multiple Outputs with Automatic Detection
 *
 * This solution demonstrates automatic detection of system information
 * using preprocessor directives.
 */

int main() {
    std::cout << "=== My Development Environment ===" << std::endl;
    std::cout << std::endl;

    // Operating System Detection
    std::cout << "Operating System: ";
    #if defined(_WIN32) || defined(_WIN64)
        std::cout << "Windows";
        #ifdef _WIN64
            std::cout << " (64-bit)";
        #else
            std::cout << " (32-bit)";
        #endif
    #elif defined(__linux__)
        std::cout << "Linux";
    #elif defined(__APPLE__) && defined(__MACH__)
        std::cout << "macOS";
    #elif defined(__unix__)
        std::cout << "Unix";
    #else
        std::cout << "Unknown";
    #endif
    std::cout << std::endl;

    // Compiler Detection
    std::cout << "Compiler: ";
    #if defined(__clang__)
        std::cout << "Clang " << __clang_major__ << "."
                  << __clang_minor__ << "." << __clang_patchlevel__;
    #elif defined(__GNUC__)
        std::cout << "GCC " << __GNUC__ << "."
                  << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
    #elif defined(_MSC_VER)
        std::cout << "MSVC " << _MSC_VER;
    #else
        std::cout << "Unknown";
    #endif
    std::cout << std::endl;

    // IDE/Editor (user can customize this)
    std::cout << "IDE/Editor: Visual Studio Code" << std::endl;

    // C++ Version Detection
    std::cout << "C++ Version: ";
    #if __cplusplus == 199711L
        std::cout << "C++98";
    #elif __cplusplus == 201103L
        std::cout << "C++11";
    #elif __cplusplus == 201402L
        std::cout << "C++14";
    #elif __cplusplus == 201703L
        std::cout << "C++17";
    #elif __cplusplus == 202002L
        std::cout << "C++20";
    #elif __cplusplus > 202002L
        std::cout << "C++23 or later";
    #else
        std::cout << "Unknown";
    #endif
    std::cout << " (" << __cplusplus << ")" << std::endl;

    std::cout << std::endl;
    std::cout << "=== End of Environment Info ===" << std::endl;

    return 0;
}

/*
 * EXPECTED OUTPUT (will vary by system):
 *
 * === My Development Environment ===
 *
 * Operating System: Linux
 * Compiler: GCC 11.3.0
 * IDE/Editor: Visual Studio Code
 * C++ Version: C++17 (201703)
 *
 * === End of Environment Info ===
 *
 * KEY LEARNING POINTS:
 *
 * 1. Preprocessor directives (#if, #ifdef, #elif, #else, #endif)
 * 2. Conditional compilation based on platform
 * 3. Predefined macros for system detection
 * 4. How to make portable code that adapts to different systems
 * 5. Combining preprocessor directives with runtime output
 */
