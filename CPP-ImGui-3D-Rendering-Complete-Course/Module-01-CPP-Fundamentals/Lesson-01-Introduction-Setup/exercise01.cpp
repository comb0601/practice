#include <iostream>

/**
 * Exercise 01: Your First Program
 *
 * TASK: Modify this program to output your name and favorite programming language.
 *
 * Requirements:
 * 1. Print "My name is: [Your Name]"
 * 2. Print "My favorite language is: [Language]"
 * 3. Print "I am learning C++ to: [Your Goal]"
 *
 * Replace the TODO comments with your code.
 */

int main() {
    std::cout << "=== Exercise 01: About Me ===" << std::endl;
    std::cout << std::endl;

    // TODO: Print your name

    // TODO: Print your favorite programming language

    // TODO: Print why you're learning C++

    std::cout << std::endl;
    std::cout << "Exercise 01 complete!" << std::endl;

    return 0;
}

/*
 * COMPILATION INSTRUCTIONS:
 *
 * Linux/Mac:
 *   g++ exercise01.cpp -o exercise01
 *   ./exercise01
 *
 * Windows:
 *   g++ exercise01.cpp -o exercise01.exe
 *   exercise01.exe
 *
 * With CMake:
 *   mkdir build && cd build
 *   cmake ..
 *   cmake --build .
 *   ./exercise01 (or exercise01.exe on Windows)
 */
