#include <iostream>

/**
 * Example 02: Basic Output Operations
 *
 * Demonstrates different ways to output text and numbers
 * Shows:
 * - Multiple output statements
 * - Different data types in output
 * - Using std::endl vs \n
 * - Output chaining with <<
 */

int main() {
    // Simple string output
    std::cout << "Welcome to C++ Programming!" << std::endl;

    // Output multiple items in one statement
    std::cout << "The answer is: " << 42 << std::endl;

    // Output without std::endl (no newline)
    std::cout << "This is on one line ";
    std::cout << "and this continues it" << std::endl;

    // Using \n instead of std::endl
    std::cout << "Line 1\n";
    std::cout << "Line 2\n";
    std::cout << "Line 3\n";

    // Chaining multiple outputs
    std::cout << "You can chain: " << 10 << " + " << 20 << " = " << 30 << std::endl;

    // Outputting different types
    std::cout << "Integer: " << 100 << std::endl;
    std::cout << "Decimal: " << 3.14159 << std::endl;
    std::cout << "Character: " << 'A' << std::endl;
    std::cout << "Boolean: " << true << std::endl;

    return 0;
}
