#include <iostream>
#include <iomanip>  // For formatting output

/**
 * Solution 05: BMI Calculator
 *
 * Calculates Body Mass Index from weight and height.
 */

int main() {
    double weight, height, bmi;

    std::cout << "=== BMI Calculator ===" << std::endl;
    std::cout << std::endl;

    std::cout << "Enter weight (kg): ";
    std::cin >> weight;

    std::cout << "Enter height (m): ";
    std::cin >> height;

    bmi = weight / (height * height);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nYour BMI is: " << bmi << std::endl;

    // Bonus: Show BMI category
    std::cout << "\nBMI Categories:" << std::endl;
    std::cout << "  Below 18.5:    Underweight" << std::endl;
    std::cout << "  18.5 - 24.9:   Normal weight" << std::endl;
    std::cout << "  25.0 - 29.9:   Overweight" << std::endl;
    std::cout << "  30.0 and above: Obese" << std::endl;

    return 0;
}

/*
 * EXAMPLE OUTPUT:
 *
 * === BMI Calculator ===
 *
 * Enter weight (kg): 70
 * Enter height (m): 1.75
 *
 * Your BMI is: 22.86
 *
 * BMI Categories:
 *   Below 18.5:     Underweight
 *   18.5 - 24.9:    Normal weight
 *   25.0 - 29.9:    Overweight
 *   30.0 and above: Obese
 *
 * KEY POINTS:
 * - BMI formula: weight / height²
 * - height * height calculates height squared
 * - Using double for precise calculations
 * - std::setprecision(2) formats to 2 decimal places
 *
 * BMI FORMULA EXPLAINED:
 * BMI = kg/m²
 * Example: 70 kg / (1.75 m)² = 70 / 3.0625 = 22.86
 *
 * ENHANCEMENT IDEAS:
 * - Automatically categorize the user's BMI
 * - Support imperial units (pounds, inches)
 * - Calculate ideal weight range
 * - Add age and gender considerations
 * - Show healthy BMI range for the user's height
 */
