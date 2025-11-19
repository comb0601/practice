#include <iostream>

/**
 * Solution 03: Age Calculator
 *
 * Calculates a person's age based on current year and birth year.
 */

int main() {
    int currentYear, birthYear, age;

    std::cout << "Enter current year: ";
    std::cin >> currentYear;

    std::cout << "Enter birth year: ";
    std::cin >> birthYear;

    age = currentYear - birthYear;

    std::cout << "You are " << age << " years old." << std::endl;

    return 0;
}

/*
 * EXAMPLE OUTPUT:
 *
 * Enter current year: 2024
 * Enter birth year: 1995
 * You are 29 years old.
 *
 * KEY POINTS:
 * - Using int for years (no decimals needed)
 * - Simple subtraction calculates age
 * - Output is user-friendly and clear
 *
 * NOTE:
 * This is a simplified calculation. In reality, age depends on
 * whether the person has had their birthday this year or not.
 *
 * ENHANCEMENT IDEAS:
 * - Ask for birth month and day for exact age
 * - Show age in years, months, and days
 * - Calculate age in dog years, cat years, etc.
 * - Validate that birth year is not in the future
 */
