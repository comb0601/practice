#include <iostream>

/**
 * Solution 01: Calculator with all operators
 */

int main() {
    double num1, num2;

    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    std::cout << std::endl;
    std::cout << num1 << " + " << num2 << " = " << (num1 + num2) << std::endl;
    std::cout << num1 << " - " << num2 << " = " << (num1 - num2) << std::endl;
    std::cout << num1 << " * " << num2 << " = " << (num1 * num2) << std::endl;

    if (num2 != 0) {
        std::cout << num1 << " / " << num2 << " = " << (num1 / num2) << std::endl;
    } else {
        std::cout << "Cannot divide by zero!" << std::endl;
    }

    return 0;
}
