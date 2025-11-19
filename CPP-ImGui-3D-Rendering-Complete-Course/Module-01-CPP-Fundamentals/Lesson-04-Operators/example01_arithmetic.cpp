#include <iostream>

/**
 * Example 01: Arithmetic Operators
 */

int main() {
    int x = 15, y = 4;

    std::cout << "x = " << x << ", y = " << y << std::endl;
    std::cout << std::endl;

    std::cout << "Addition:       " << x << " + " << y << " = " << (x + y) << std::endl;
    std::cout << "Subtraction:    " << x << " - " << y << " = " << (x - y) << std::endl;
    std::cout << "Multiplication: " << x << " * " << y << " = " << (x * y) << std::endl;
    std::cout << "Division:       " << x << " / " << y << " = " << (x / y) << std::endl;
    std::cout << "Modulus:        " << x << " % " << y << " = " << (x % y) << std::endl;

    return 0;
}
