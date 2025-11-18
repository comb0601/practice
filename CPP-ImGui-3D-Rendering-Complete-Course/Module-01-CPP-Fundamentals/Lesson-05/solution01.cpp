#include <iostream>

/**
 * Solution 01: Age Categorizer
 */

int main() {
    int age;

    std::cout << "Enter your age: ";
    std::cin >> age;

    if (age < 0) {
        std::cout << "Invalid age!" << std::endl;
    } else if (age <= 12) {
        std::cout << "Category: Child" << std::endl;
    } else if (age <= 19) {
        std::cout << "Category: Teenager" << std::endl;
    } else if (age <= 64) {
        std::cout << "Category: Adult" << std::endl;
    } else {
        std::cout << "Category: Senior" << std::endl;
    }

    return 0;
}
