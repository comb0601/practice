#include <iostream>
#include <string>

/**
 * Example 04: Basic Input and Output
 *
 * Demonstrates:
 * - std::cout for output
 * - std::cin for input
 * - Different data types in I/O
 * - Chaining input/output operations
 */

int main() {
    std::cout << "=== Basic Input/Output ===" << std::endl;
    std::cout << std::endl;

    // OUTPUT EXAMPLES
    std::cout << "Simple output example" << std::endl;
    std::cout << "You can chain: " << "multiple" << " " << "items" << std::endl;
    std::cout << "Numbers work too: " << 42 << ", " << 3.14 << std::endl;
    std::cout << std::endl;

    // INPUT EXAMPLES
    std::string favColor;
    int favNumber;
    double favDecimal;

    std::cout << "What is your favorite color? ";
    std::cin >> favColor;

    std::cout << "What is your favorite number? ";
    std::cin >> favNumber;

    std::cout << "Enter a decimal number: ";
    std::cin >> favDecimal;

    // Display what was entered
    std::cout << std::endl;
    std::cout << "=== What You Entered ===" << std::endl;
    std::cout << "Favorite color:  " << favColor << std::endl;
    std::cout << "Favorite number: " << favNumber << std::endl;
    std::cout << "Decimal number:  " << favDecimal << std::endl;

    // Chained input
    std::cout << std::endl;
    std::cout << "Enter two numbers separated by space: ";
    int a, b;
    std::cin >> a >> b;
    std::cout << "You entered: " << a << " and " << b << std::endl;
    std::cout << "Their sum is: " << (a + b) << std::endl;

    return 0;
}

/*
 * KEY POINTS:
 *
 * std::cout << value;     // Output
 * std::cin >> variable;   // Input
 *
 * << is the "insertion operator" (puts data into stream)
 * >> is the "extraction operator" (takes data from stream)
 *
 * std::endl adds a newline and flushes the buffer
 * \n just adds a newline (faster but doesn't flush)
 */
