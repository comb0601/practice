/*
 * Program: BMI Calculator
 * Description: Calculates Body Mass Index and provides health category
 * Compilation: g++ 05_bmi_calculator.cpp -o 05_bmi_calculator
 * Execution: ./05_bmi_calculator
 */

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double weight, height, bmi;
    int system;

    cout << "=== BMI Calculator ===" << endl << endl;

    cout << "Select measurement system:" << endl;
    cout << "1. Metric (kg, meters)" << endl;
    cout << "2. Imperial (lbs, inches)" << endl;
    cout << "Enter choice (1-2): ";
    cin >> system;

    if (system == 1) {
        cout << "Enter weight in kilograms: ";
        cin >> weight;
        cout << "Enter height in meters: ";
        cin >> height;

        bmi = weight / (height * height);
    }
    else if (system == 2) {
        cout << "Enter weight in pounds: ";
        cin >> weight;
        cout << "Enter height in inches: ";
        cin >> height;

        // BMI formula for imperial: (weight in lbs / (height in inches)^2) * 703
        bmi = (weight / (height * height)) * 703;
    }
    else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    cout << fixed << setprecision(2);
    cout << "\n=== Results ===" << endl;
    cout << "Your BMI: " << bmi << endl;

    // Determine health category
    cout << "Category: ";
    if (bmi < 18.5) {
        cout << "Underweight" << endl;
    }
    else if (bmi >= 18.5 && bmi < 25) {
        cout << "Normal weight" << endl;
    }
    else if (bmi >= 25 && bmi < 30) {
        cout << "Overweight" << endl;
    }
    else {
        cout << "Obese" << endl;
    }

    cout << "\nBMI Categories:" << endl;
    cout << "  Underweight: < 18.5" << endl;
    cout << "  Normal weight: 18.5 - 24.9" << endl;
    cout << "  Overweight: 25 - 29.9" << endl;
    cout << "  Obese: >= 30" << endl;

    return 0;
}
