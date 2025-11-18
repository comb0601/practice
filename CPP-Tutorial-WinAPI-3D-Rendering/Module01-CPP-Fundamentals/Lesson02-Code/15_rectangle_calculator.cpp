/*
 * Program: Rectangle Calculator
 * Description: Calculates area, perimeter, and diagonal of a rectangle
 * Compilation: g++ 15_rectangle_calculator.cpp -o 15_rectangle_calculator
 * Execution: ./15_rectangle_calculator
 */

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    double length, width;

    cout << "=== Rectangle Calculator ===" << endl << endl;

    cout << "Enter length: ";
    cin >> length;

    cout << "Enter width: ";
    cin >> width;

    if (length <= 0 || width <= 0) {
        cout << "Error: Dimensions must be positive!" << endl;
        return 1;
    }

    // Calculations
    double area = length * width;
    double perimeter = 2 * (length + width);
    double diagonal = sqrt(length * length + width * width);

    // Output results
    cout << fixed << setprecision(2);
    cout << "\n=== Rectangle Properties ===" << endl;
    cout << "Length: " << length << endl;
    cout << "Width: " << width << endl;
    cout << "Area: " << area << " square units" << endl;
    cout << "Perimeter: " << perimeter << " units" << endl;
    cout << "Diagonal: " << diagonal << " units" << endl;

    // Check if it's a square
    cout << "\n=== Additional Information ===" << endl;
    if (abs(length - width) < 0.0001) {
        cout << "This is a SQUARE!" << endl;
        cout << "Side length: " << length << endl;
    } else {
        cout << "This is a RECTANGLE" << endl;
        double ratio = (length > width) ? (length / width) : (width / length);
        cout << "Aspect ratio: 1:" << ratio << endl;
    }

    // Calculate area of inscribed circle (for square) or ellipse
    if (abs(length - width) < 0.0001) {
        const double PI = 3.14159265359;
        double radius = length / 2;
        double circleArea = PI * radius * radius;
        cout << "Area of inscribed circle: " << circleArea << " square units" << endl;
    }

    return 0;
}
