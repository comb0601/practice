/*
 * Program: Circle Calculator
 * Description: Calculates area, circumference, and diameter of a circle
 * Compilation: g++ 11_circle_calculator.cpp -o 11_circle_calculator
 * Execution: ./11_circle_calculator
 */

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    const double PI = 3.14159265359;
    double radius;

    cout << "=== Circle Calculator ===" << endl << endl;

    cout << "Enter the radius of the circle: ";
    cin >> radius;

    if (radius <= 0) {
        cout << "Error: Radius must be positive!" << endl;
        return 1;
    }

    // Calculations
    double diameter = 2 * radius;
    double circumference = 2 * PI * radius;
    double area = PI * radius * radius;

    // Output results
    cout << fixed << setprecision(2);
    cout << "\n=== Results ===" << endl;
    cout << "Radius: " << radius << endl;
    cout << "Diameter: " << diameter << endl;
    cout << "Circumference: " << circumference << endl;
    cout << "Area: " << area << endl << endl;

    // Additional calculations
    cout << "=== Additional Information ===" << endl;
    double sector90 = (PI * radius * radius) / 4;  // Area of 90-degree sector
    double arcLength90 = (PI * radius) / 2;  // Arc length of 90-degree sector

    cout << "Area of 90-degree sector: " << sector90 << endl;
    cout << "Arc length of 90-degree sector: " << arcLength90 << endl;

    return 0;
}
