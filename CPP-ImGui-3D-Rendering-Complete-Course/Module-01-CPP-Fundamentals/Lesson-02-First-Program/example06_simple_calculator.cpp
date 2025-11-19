#include <iostream>

/**
 * Example 06: Simple Calculator
 *
 * A practical example combining:
 * - Input/output
 * - Variables
 * - Basic arithmetic
 * - Conditional logic
 * - Error handling
 */

int main() {
    double num1, num2, result;
    char operation;

    // Display program header
    std::cout << "========================================" << std::endl;
    std::cout << "        Simple Calculator" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Get first number
    std::cout << "Enter first number: ";
    std::cin >> num1;

    // Get operation
    std::cout << "Enter operation (+, -, *, /): ";
    std::cin >> operation;

    // Get second number
    std::cout << "Enter second number: ";
    std::cin >> num2;

    std::cout << std::endl;

    // Perform calculation based on operation
    if (operation == '+') {
        result = num1 + num2;
        std::cout << num1 << " + " << num2 << " = " << result << std::endl;
    }
    else if (operation == '-') {
        result = num1 - num2;
        std::cout << num1 << " - " << num2 << " = " << result << std::endl;
    }
    else if (operation == '*') {
        result = num1 * num2;
        std::cout << num1 << " * " << num2 << " = " << result << std::endl;
    }
    else if (operation == '/') {
        if (num2 != 0) {
            result = num1 / num2;
            std::cout << num1 << " / " << num2 << " = " << result << std::endl;
        } else {
            std::cout << "Error: Cannot divide by zero!" << std::endl;
            return 1;  // Return error code
        }
    }
    else {
        std::cout << "Error: Invalid operation '" << operation << "'" << std::endl;
        std::cout << "Please use +, -, *, or /" << std::endl;
        return 1;  // Return error code
    }

    std::cout << std::endl;
    std::cout << "Calculation completed successfully!" << std::endl;
    return 0;  // Return success
}

/*
 * EXAMPLE USAGE:
 *
 * $ ./example06_simple_calculator
 * ========================================
 *         Simple Calculator
 * ========================================
 *
 * Enter first number: 10
 * Enter operation (+, -, *, /): *
 * Enter second number: 5
 *
 * 10 * 5 = 50
 *
 * Calculation completed successfully!
 *
 * TRY THIS:
 * - Test all four operations
 * - Try dividing by zero to see error handling
 * - Enter an invalid operation like '%'
 * - Use decimal numbers
 */
