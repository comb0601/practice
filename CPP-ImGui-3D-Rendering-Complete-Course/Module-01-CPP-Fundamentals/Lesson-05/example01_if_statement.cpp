#include <iostream>

/**
 * Example 01: Basic if Statement
 */

int main() {
    int age;

    std::cout << "Enter your age: ";
    std::cin >> age;

    if (age >= 18) {
        std::cout << "You are an adult." << std::endl;
    }

    if (age >= 65) {
        std::cout << "You qualify for senior discount!" << std::endl;
    }

    if (age < 13) {
        std::cout << "You are a child." << std::endl;
    }

    return 0;
}
