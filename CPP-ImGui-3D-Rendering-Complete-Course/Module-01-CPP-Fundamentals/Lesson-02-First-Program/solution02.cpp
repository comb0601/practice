#include <iostream>

/**
 * Solution 02: Area Calculator
 *
 * Calculates the area of a rectangle given length and width.
 */

int main() {
    double length, width, area;

    std::cout << "Enter length: ";
    std::cin >> length;

    std::cout << "Enter width: ";
    std::cin >> width;

    area = length * width;

    std::cout << "Area: " << area << " square units" << std::endl;

    return 0;
}

/*
 * EXAMPLE OUTPUT:
 *
 * Enter length: 5.5
 * Enter width: 3.2
 * Area: 17.6 square units
 *
 * KEY POINTS:
 * - Using double allows for decimal values
 * - Area formula: length × width
 * - Result includes descriptive text "square units"
 *
 * ENHANCEMENT IDEAS:
 * - Add input validation (check for negative values)
 * - Calculate perimeter too: 2 × (length + width)
 * - Format output to specific decimal places
 * - Support different units (meters, feet, etc.)
 */
