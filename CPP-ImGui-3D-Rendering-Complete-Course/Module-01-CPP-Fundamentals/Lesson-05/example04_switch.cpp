#include <iostream>

/**
 * Example 04: switch Statement
 */

int main() {
    char grade;

    std::cout << "Enter your letter grade (A-F): ";
    std::cin >> grade;

    switch (grade) {
        case 'A':
        case 'a':
            std::cout << "Excellent!" << std::endl;
            break;
        case 'B':
        case 'b':
            std::cout << "Good job!" << std::endl;
            break;
        case 'C':
        case 'c':
            std::cout << "Well done!" << std::endl;
            break;
        case 'D':
        case 'd':
            std::cout << "You passed!" << std::endl;
            break;
        case 'F':
        case 'f':
            std::cout << "Better luck next time." << std::endl;
            break;
        default:
            std::cout << "Invalid grade!" << std::endl;
    }

    return 0;
}
