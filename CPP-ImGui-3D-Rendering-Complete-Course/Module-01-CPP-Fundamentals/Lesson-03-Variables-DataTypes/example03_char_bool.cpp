#include <iostream>

/**
 * Example 03: Character and Boolean Types
 */

int main() {
    std::cout << "=== Character Type ===" << std::endl;

    char ch1 = 'A';
    char ch2 = 66;  // ASCII code for 'B'
    char ch3 = '\n';  // Newline character

    std::cout << "ch1: " << ch1 << " (ASCII: " << static_cast<int>(ch1) << ")" << std::endl;
    std::cout << "ch2: " << ch2 << " (ASCII: " << static_cast<int>(ch2) << ")" << std::endl;
    std::cout << "ch3 is a newline character" << std::endl;

    std::cout << std::endl;
    std::cout << "=== Boolean Type ===" << std::endl;

    bool isRaining = false;
    bool isSunny = true;

    std::cout << "Is it raining? " << isRaining << std::endl;
    std::cout << "Is it sunny? " << isSunny << std::endl;

    std::cout << std::boolalpha;  // Print true/false instead of 1/0
    std::cout << "Is it raining? " << isRaining << std::endl;
    std::cout << "Is it sunny? " << isSunny << std::endl;

    return 0;
}
