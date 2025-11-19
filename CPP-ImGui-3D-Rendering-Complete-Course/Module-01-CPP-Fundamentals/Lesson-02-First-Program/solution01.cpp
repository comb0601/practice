#include <iostream>
#include <string>

/**
 * Solution 01: Hello with Name
 *
 * A program that asks for the user's name and greets them personally.
 */

int main() {
    std::string name;

    std::cout << "Enter your name: ";
    std::cin >> name;

    std::cout << "Hello, " << name << "!" << std::endl;

    return 0;
}

/*
 * EXAMPLE OUTPUT:
 *
 * Enter your name: Alice
 * Hello, Alice!
 *
 * KEY POINTS:
 * - std::string is used to store text
 * - std::cin >> reads one word (stops at whitespace)
 * - std::cout << displays output
 * - Chaining << operators combines multiple items in output
 *
 * ENHANCEMENT IDEAS:
 * - Use std::getline(std::cin, name) to read full names with spaces
 * - Add more personal questions
 * - Create a more elaborate greeting
 */
