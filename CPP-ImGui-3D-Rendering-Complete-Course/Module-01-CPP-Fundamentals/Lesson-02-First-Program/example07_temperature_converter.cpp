#include <iostream>
#include <iomanip>  // For std::fixed and std::setprecision

/**
 * Example 07: Temperature Converter
 *
 * Converts between Celsius and Fahrenheit.
 * Demonstrates:
 * - User input validation
 * - Floating-point arithmetic
 * - Output formatting
 * - Case-insensitive input handling
 */

int main() {
    double temperature;
    char unit;

    std::cout << "========================================" << std::endl;
    std::cout << "      Temperature Converter" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Get temperature value
    std::cout << "Enter temperature: ";
    std::cin >> temperature;

    // Get unit
    std::cout << "Enter unit (C for Celsius, F for Fahrenheit): ";
    std::cin >> unit;

    std::cout << std::endl;

    // Set precision for decimal output
    std::cout << std::fixed << std::setprecision(2);

    // Convert based on input unit
    if (unit == 'C' || unit == 'c') {
        // Convert Celsius to Fahrenheit
        double fahrenheit = (temperature * 9.0 / 5.0) + 32.0;
        std::cout << temperature << " °C = " << fahrenheit << " °F" << std::endl;
    }
    else if (unit == 'F' || unit == 'f') {
        // Convert Fahrenheit to Celsius
        double celsius = (temperature - 32.0) * 5.0 / 9.0;
        std::cout << temperature << " °F = " << celsius << " °C" << std::endl;
    }
    else {
        std::cout << "Error: Invalid unit '" << unit << "'" << std::endl;
        std::cout << "Please use 'C' or 'F'" << std::endl;
        return 1;
    }

    // Display conversion formulas
    std::cout << std::endl;
    std::cout << "Conversion Formulas:" << std::endl;
    std::cout << "  C to F: (C × 9/5) + 32" << std::endl;
    std::cout << "  F to C: (F - 32) × 5/9" << std::endl;

    return 0;
}

/*
 * TEMPERATURE REFERENCE POINTS:
 *
 * -40 °C = -40 °F   (same in both scales)
 *   0 °C =  32 °F   (freezing point of water)
 *  37 °C =  98.6 °F (normal body temperature)
 * 100 °C = 212 °F   (boiling point of water)
 *
 * EXAMPLE USAGE:
 *
 * $ ./example07_temperature_converter
 * ========================================
 *       Temperature Converter
 * ========================================
 *
 * Enter temperature: 25
 * Enter unit (C for Celsius, F for Fahrenheit): C
 *
 * 25.00 °C = 77.00 °F
 *
 * Conversion Formulas:
 *   C to F: (C × 9/5) + 32
 *   F to C: (F - 32) × 5/9
 */
