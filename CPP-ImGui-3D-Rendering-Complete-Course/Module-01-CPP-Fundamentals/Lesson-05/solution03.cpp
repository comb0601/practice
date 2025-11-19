#include <iostream>

/**
 * Solution 03: Leap Year Checker
 */

int main() {
    int year;
    bool isLeapYear;

    std::cout << "Enter a year: ";
    std::cin >> year;

    if (year % 400 == 0) {
        isLeapYear = true;
    } else if (year % 100 == 0) {
        isLeapYear = false;
    } else if (year % 4 == 0) {
        isLeapYear = true;
    } else {
        isLeapYear = false;
    }

    if (isLeapYear) {
        std::cout << year << " is a leap year." << std::endl;
    } else {
        std::cout << year << " is not a leap year." << std::endl;
    }

    return 0;
}
