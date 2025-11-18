/*
 * Lesson 01 - Example 06: Multiple Statements
 *
 * Demonstrates that a program can have many statements.
 * Each statement must end with a semicolon.
 *
 * Compile: g++ 06_MultipleStatements.cpp -o MultipleStatements
 */

#include <iostream>

int main()
{
    // Each line below is a separate statement
    std::cout << "Statement 1\n";
    std::cout << "Statement 2\n";
    std::cout << "Statement 3\n";
    std::cout << "Statement 4\n";
    std::cout << "Statement 5\n";

    std::cout << "\n";

    // You can also chain output in one statement
    std::cout << "Part 1 " << "Part 2 " << "Part 3\n";

    std::cout << "\n";

    // Statements can be split across multiple lines
    std::cout << "This is a very long line that "
              << "I'm splitting across multiple lines "
              << "for better readability.\n";

    std::cout << "\n";

    // Multiple operations
    std::cout << "=== Program Information ===\n";
    std::cout << "Program: Multiple Statements Demo\n";
    std::cout << "Version: 1.0\n";
    std::cout << "Author: C++ Student\n";
    std::cout << "Date: 2024\n";
    std::cout << "===========================\n";

    return 0;
}
