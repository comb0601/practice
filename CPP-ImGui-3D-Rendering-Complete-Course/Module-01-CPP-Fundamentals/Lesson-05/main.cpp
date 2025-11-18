#include <iostream>
#include <string>

/**
 * Lesson 05: Control Flow - if, else, switch
 * Comprehensive demonstration of conditional statements
 */

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Lesson 05: Control Flow" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // IF STATEMENT
    std::cout << "=== if Statement ===" << std::endl;
    int age = 20;
    if (age >= 18) {
        std::cout << "You are an adult (age >= 18)" << std::endl;
    }
    std::cout << std::endl;

    // IF-ELSE STATEMENT
    std::cout << "=== if-else Statement ===" << std::endl;
    int temperature = 25;
    if (temperature > 30) {
        std::cout << "It's hot outside!" << std::endl;
    } else {
        std::cout << "Weather is pleasant." << std::endl;
    }
    std::cout << std::endl;

    // IF-ELSE IF-ELSE CHAIN
    std::cout << "=== if-else if-else Chain ===" << std::endl;
    int score = 85;
    if (score >= 90) {
        std::cout << "Grade: A" << std::endl;
    } else if (score >= 80) {
        std::cout << "Grade: B" << std::endl;
    } else if (score >= 70) {
        std::cout << "Grade: C" << std::endl;
    } else if (score >= 60) {
        std::cout << "Grade: D" << std::endl;
    } else {
        std::cout << "Grade: F" << std::endl;
    }
    std::cout << std::endl;

    // SWITCH STATEMENT
    std::cout << "=== switch Statement ===" << std::endl;
    int day = 3;
    std::cout << "Day " << day << " is: ";
    switch (day) {
        case 1:
            std::cout << "Monday";
            break;
        case 2:
            std::cout << "Tuesday";
            break;
        case 3:
            std::cout << "Wednesday";
            break;
        case 4:
            std::cout << "Thursday";
            break;
        case 5:
            std::cout << "Friday";
            break;
        case 6:
        case 7:
            std::cout << "Weekend";
            break;
        default:
            std::cout << "Invalid day";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    // TERNARY OPERATOR
    std::cout << "=== Ternary Operator ===" << std::endl;
    int num = 10;
    std::string result = (num % 2 == 0) ? "even" : "odd";
    std::cout << num << " is " << result << std::endl;

    return 0;
}
