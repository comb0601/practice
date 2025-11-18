/*
 * Lesson 01 - Example 02: Enhanced Hello World
 *
 * Demonstrates multiple output statements and formatting.
 * Shows how to use \n for newlines and create formatted output.
 *
 * Compile: g++ 02_EnhancedHelloWorld.cpp -o EnhancedHelloWorld
 */

#include <iostream>

int main()
{
    // Multiple output statements with formatting
    std::cout << "=================================\n";
    std::cout << "   Welcome to C++ Programming!  \n";
    std::cout << "=================================\n";
    std::cout << "\n";
    std::cout << "This is my first C++ program.\n";
    std::cout << "I'm learning step by step!\n";
    std::cout << "\n";
    std::cout << "Key concepts:\n";
    std::cout << "  - Using std::cout for output\n";
    std::cout << "  - Using << operator for stream insertion\n";
    std::cout << "  - Using \\n for newlines\n";

    return 0;
}
