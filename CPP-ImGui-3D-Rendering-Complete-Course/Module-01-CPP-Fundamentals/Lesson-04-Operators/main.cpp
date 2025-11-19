#include <iostream>

/**
 * Lesson 04: Operators
 * Demonstrates all C++ operators
 */

int main() {
    std::cout << "=== C++ Operators ===" << std::endl;
    std::cout << std::endl;

    // Arithmetic Operators
    std::cout << "Arithmetic Operators:" << std::endl;
    int a = 10, b = 3;
    std::cout << a << " + " << b << " = " << (a + b) << std::endl;
    std::cout << a << " - " << b << " = " << (a - b) << std::endl;
    std::cout << a << " * " << b << " = " << (a * b) << std::endl;
    std::cout << a << " / " << b << " = " << (a / b) << std::endl;
    std::cout << a << " % " << b << " = " << (a % b) << std::endl;
    std::cout << std::endl;

    // Comparison Operators
    std::cout << "Comparison Operators:" << std::endl;
    std::cout << std::boolalpha;
    std::cout << a << " == " << b << " : " << (a == b) << std::endl;
    std::cout << a << " != " << b << " : " << (a != b) << std::endl;
    std::cout << a << " > " << b << "  : " << (a > b) << std::endl;
    std::cout << a << " < " << b << "  : " << (a < b) << std::endl;
    std::cout << std::endl;

    // Logical Operators
    std::cout << "Logical Operators:" << std::endl;
    bool x = true, y = false;
    std::cout << "true && false = " << (x && y) << std::endl;
    std::cout << "true || false = " << (x || y) << std::endl;
    std::cout << "!true = " << (!x) << std::endl;

    return 0;
}
