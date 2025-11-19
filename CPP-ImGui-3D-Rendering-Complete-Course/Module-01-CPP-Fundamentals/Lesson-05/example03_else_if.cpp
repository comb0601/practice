#include <iostream>

/**
 * Example 03: else-if Chain
 */

int main() {
    double grade;

    std::cout << "Enter your grade (0-100): ";
    std::cin >> grade;

    if (grade >= 90) {
        std::cout << "Excellent! Grade: A" << std::endl;
    } else if (grade >= 80) {
        std::cout << "Good! Grade: B" << std::endl;
    } else if (grade >= 70) {
        std::cout << "Fair. Grade: C" << std::endl;
    } else if (grade >= 60) {
        std::cout << "Passing. Grade: D" << std::endl;
    } else {
        std::cout << "Failed. Grade: F" << std::endl;
    }

    return 0;
}
