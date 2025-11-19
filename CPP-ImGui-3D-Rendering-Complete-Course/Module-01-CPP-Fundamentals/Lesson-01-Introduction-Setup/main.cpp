#include <iostream>
#include <string>
#include <iomanip>

/**
 * Lesson 01: Introduction and Development Environment Setup
 * Main demonstration program
 *
 * This program demonstrates:
 * - Basic C++ program structure
 * - Output to console
 * - Compiler version detection
 * - System information display
 */

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++ Development Environment Setup" << std::endl;
    std::cout << "  Lesson 01 - Main Program" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Display C++ version
    std::cout << "C++ Standard Version: ";
    #if __cplusplus == 199711L
        std::cout << "C++98" << std::endl;
    #elif __cplusplus == 201103L
        std::cout << "C++11" << std::endl;
    #elif __cplusplus == 201402L
        std::cout << "C++14" << std::endl;
    #elif __cplusplus == 201703L
        std::cout << "C++17" << std::endl;
    #elif __cplusplus == 202002L
        std::cout << "C++20" << std::endl;
    #elif __cplusplus > 202002L
        std::cout << "C++23 or later" << std::endl;
    #else
        std::cout << "Unknown (" << __cplusplus << ")" << std::endl;
    #endif

    std::cout << "Raw version value: " << __cplusplus << std::endl;
    std::cout << std::endl;

    // Display compiler information
    std::cout << "Compiler Information:" << std::endl;
    #if defined(__clang__)
        std::cout << "  Compiler: Clang " << __clang_major__ << "."
                  << __clang_minor__ << "." << __clang_patchlevel__ << std::endl;
    #elif defined(__GNUC__)
        std::cout << "  Compiler: GCC " << __GNUC__ << "."
                  << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << std::endl;
    #elif defined(_MSC_VER)
        std::cout << "  Compiler: MSVC " << _MSC_VER << std::endl;
    #else
        std::cout << "  Compiler: Unknown" << std::endl;
    #endif

    // Display platform information
    std::cout << "Platform: ";
    #if defined(_WIN32) || defined(_WIN64)
        std::cout << "Windows";
        #ifdef _WIN64
            std::cout << " (64-bit)";
        #else
            std::cout << " (32-bit)";
        #endif
    #elif defined(__linux__)
        std::cout << "Linux";
    #elif defined(__APPLE__)
        std::cout << "macOS";
    #elif defined(__unix__)
        std::cout << "Unix";
    #else
        std::cout << "Unknown";
    #endif
    std::cout << std::endl;
    std::cout << std::endl;

    // Display feature support
    std::cout << "C++ Feature Support:" << std::endl;
    std::cout << "  Auto keyword: ";
    #if __cplusplus >= 201103L
        std::cout << "Yes" << std::endl;
    #else
        std::cout << "No" << std::endl;
    #endif

    std::cout << "  Lambda expressions: ";
    #if __cplusplus >= 201103L
        std::cout << "Yes" << std::endl;
    #else
        std::cout << "No" << std::endl;
    #endif

    std::cout << "  Range-based for loops: ";
    #if __cplusplus >= 201103L
        std::cout << "Yes" << std::endl;
    #else
        std::cout << "No" << std::endl;
    #endif

    std::cout << "  Structured bindings: ";
    #if __cplusplus >= 201703L
        std::cout << "Yes" << std::endl;
    #else
        std::cout << "No" << std::endl;
    #endif

    std::cout << "  Concepts: ";
    #if __cplusplus >= 202002L
        std::cout << "Yes" << std::endl;
    #else
        std::cout << "No" << std::endl;
    #endif

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Setup verification successful!" << std::endl;
    std::cout << "You are ready to begin C++ programming!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
