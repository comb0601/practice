#include <iostream>

/**
 * Example 05: Ternary Operator
 */

int main() {
    int a = 10, b = 20;

    // Ternary operator: condition ? value_if_true : value_if_false
    int max = (a > b) ? a : b;
    std::cout << "Maximum of " << a << " and " << b << " is: " << max << std::endl;

    int num = 7;
    std::string parity = (num % 2 == 0) ? "even" : "odd";
    std::cout << num << " is " << parity << std::endl;

    return 0;
}
