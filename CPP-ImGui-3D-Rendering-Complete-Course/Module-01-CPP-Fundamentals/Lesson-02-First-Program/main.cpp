#include <iostream>
#include <string>

/**
 * Lesson 02: Your First C++ Program
 * Main demonstration program
 *
 * This program demonstrates:
 * - The complete structure of a C++ program
 * - Input and output operations
 * - Variable declarations and usage
 * - Proper formatting and style
 */

int main() {
    // Display program header
    std::cout << "========================================" << std::endl;
    std::cout << "  Lesson 02: Your First C++ Program" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Demonstrate variable declaration
    std::string name;
    int age;
    double height;

    // Collect user input
    std::cout << "Please enter your information:" << std::endl;
    std::cout << std::endl;

    std::cout << "What is your name? ";
    std::cin >> name;

    std::cout << "How old are you? ";
    std::cin >> age;

    std::cout << "What is your height in meters? ";
    std::cin >> height;

    // Display collected information
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "           Summary of Information        " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Name:   " << name << std::endl;
    std::cout << "Age:    " << age << " years old" << std::endl;
    std::cout << "Height: " << height << " meters" << std::endl;
    std::cout << "========================================" << std::endl;

    // Demonstrate simple calculations
    std::cout << std::endl;
    std::cout << "Fun fact: In dog years, you would be "
              << (age * 7) << " years old!" << std::endl;
    std::cout << "Your height in centimeters is: "
              << (height * 100) << " cm" << std::endl;

    // Return success
    std::cout << std::endl;
    std::cout << "Program completed successfully!" << std::endl;
    return 0;
}
