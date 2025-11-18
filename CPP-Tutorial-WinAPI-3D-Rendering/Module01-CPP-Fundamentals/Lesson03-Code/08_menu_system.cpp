/*
 * Program: Interactive Menu System
 * Description: Demonstrates menu-driven program with multiple options
 * Compilation: g++ 08_menu_system.cpp -o 08_menu_system
 * Execution: ./08_menu_system
 */

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

void displayMenu() {
    cout << "\n==================================" << endl;
    cout << "     CALCULATOR MENU SYSTEM" << endl;
    cout << "==================================" << endl;
    cout << "1. Addition" << endl;
    cout << "2. Subtraction" << endl;
    cout << "3. Multiplication" << endl;
    cout << "4. Division" << endl;
    cout << "5. Power" << endl;
    cout << "6. Square Root" << endl;
    cout << "7. Percentage" << endl;
    cout << "8. Area of Circle" << endl;
    cout << "9. Area of Rectangle" << endl;
    cout << "0. Exit" << endl;
    cout << "==================================" << endl;
    cout << "Enter your choice: ";
}

int main() {
    int choice;
    double num1, num2, result;
    const double PI = 3.14159265359;

    cout << "=== Interactive Calculator ===" << endl;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:  // Addition
                cout << "Enter first number: ";
                cin >> num1;
                cout << "Enter second number: ";
                cin >> num2;
                result = num1 + num2;
                cout << "Result: " << num1 << " + " << num2 << " = " << result << endl;
                break;

            case 2:  // Subtraction
                cout << "Enter first number: ";
                cin >> num1;
                cout << "Enter second number: ";
                cin >> num2;
                result = num1 - num2;
                cout << "Result: " << num1 << " - " << num2 << " = " << result << endl;
                break;

            case 3:  // Multiplication
                cout << "Enter first number: ";
                cin >> num1;
                cout << "Enter second number: ";
                cin >> num2;
                result = num1 * num2;
                cout << "Result: " << num1 << " * " << num2 << " = " << result << endl;
                break;

            case 4:  // Division
                cout << "Enter numerator: ";
                cin >> num1;
                cout << "Enter denominator: ";
                cin >> num2;
                if (num2 != 0) {
                    result = num1 / num2;
                    cout << "Result: " << num1 << " / " << num2 << " = " << result << endl;
                } else {
                    cout << "Error: Division by zero!" << endl;
                }
                break;

            case 5:  // Power
                cout << "Enter base: ";
                cin >> num1;
                cout << "Enter exponent: ";
                cin >> num2;
                result = pow(num1, num2);
                cout << "Result: " << num1 << "^" << num2 << " = " << result << endl;
                break;

            case 6:  // Square Root
                cout << "Enter number: ";
                cin >> num1;
                if (num1 >= 0) {
                    result = sqrt(num1);
                    cout << "Result: √" << num1 << " = " << result << endl;
                } else {
                    cout << "Error: Cannot calculate square root of negative number!" << endl;
                }
                break;

            case 7:  // Percentage
                cout << "Enter number: ";
                cin >> num1;
                cout << "Enter percentage: ";
                cin >> num2;
                result = (num1 * num2) / 100.0;
                cout << "Result: " << num2 << "% of " << num1 << " = " << result << endl;
                break;

            case 8:  // Area of Circle
                cout << "Enter radius: ";
                cin >> num1;
                if (num1 > 0) {
                    result = PI * num1 * num1;
                    cout << fixed << setprecision(2);
                    cout << "Area of circle: " << result << " square units" << endl;
                } else {
                    cout << "Error: Radius must be positive!" << endl;
                }
                break;

            case 9:  // Area of Rectangle
                cout << "Enter length: ";
                cin >> num1;
                cout << "Enter width: ";
                cin >> num2;
                if (num1 > 0 && num2 > 0) {
                    result = num1 * num2;
                    cout << "Area of rectangle: " << result << " square units" << endl;
                } else {
                    cout << "Error: Dimensions must be positive!" << endl;
                }
                break;

            case 0:  // Exit
                cout << "\nThank you for using the calculator!" << endl;
                cout << "Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice! Please enter a number between 0 and 9." << endl;
        }

    } while (choice != 0);

    return 0;
}
