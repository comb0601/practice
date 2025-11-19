#include <iostream>

/**
 * Exercise 02: Multiple Outputs
 *
 * TASK: Create a program that displays information about your computer setup.
 *
 * Requirements:
 * 1. Print "Operating System: [Your OS]"
 * 2. Print "Compiler: [Your Compiler]"
 * 3. Print "IDE/Editor: [Your IDE]"
 * 4. Print "C++ Version: [Version you're using]"
 * 5. Use proper formatting with blank lines between sections
 *
 * Bonus: Use the preprocessor directives from example03 to automatically
 * detect some of this information.
 */

int main() {
    std::cout << "=== My Development Environment ===" << std::endl;
    std::cout << std::endl;

    // TODO: Print operating system information

    // TODO: Print compiler information

    // TODO: Print IDE/Editor information

    // TODO: Print C++ version information

    std::cout << std::endl;
    std::cout << "=== End of Environment Info ===" << std::endl;

    return 0;
}

/*
 * HINTS:
 *
 * 1. You can detect OS with:
 *    #ifdef _WIN32
 *    #ifdef __linux__
 *    #ifdef __APPLE__
 *
 * 2. You can detect compiler with:
 *    #ifdef __GNUC__
 *    #ifdef __clang__
 *    #ifdef _MSC_VER
 *
 * 3. C++ version is in __cplusplus macro
 *
 * Try to implement automatic detection!
 */
