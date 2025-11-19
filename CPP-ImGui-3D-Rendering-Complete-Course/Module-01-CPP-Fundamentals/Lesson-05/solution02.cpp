#include <iostream>

/**
 * Solution 02: Calculator with Menu
 */

int main() {
    char operation;
    double num1, num2;

    std::cout << "=== Calculator Menu ===" << std::endl;
    std::cout << "+ : Addition" << std::endl;
    std::cout << "- : Subtraction" << std::endl;
    std::cout << "* : Multiplication" << std::endl;
    std::cout << "/ : Division" << std::endl;
    std::cout << std::endl;

    std::cout << "Select operation: ";
    std::cin >> operation;

    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    std::cout << std::endl;

    switch (operation) {
        case '+':
            std::cout << num1 << " + " << num2 << " = " << (num1 + num2) << std::endl;
            break;
        case '-':
            std::cout << num1 << " - " << num2 << " = " << (num1 - num2) << std::endl;
            break;
        case '*':
            std::cout << num1 << " * " << num2 << " = " << (num1 * num2) << std::endl;
            break;
        case '/':
            if (num2 != 0) {
                std::cout << num1 << " / " << num2 << " = " << (num1 / num2) << std::endl;
            } else {
                std::cout << "Error: Division by zero!" << std::endl;
            }
            break;
        default:
            std::cout << "Error: Invalid operation!" << std::endl;
    }

    return 0;
}
