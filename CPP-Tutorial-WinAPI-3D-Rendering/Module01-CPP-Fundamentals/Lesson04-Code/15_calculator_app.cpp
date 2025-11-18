/*
 * Program: Calculator Application
 * Description: Complete calculator using functions for various operations
 * Compilation: g++ 15_calculator_app.cpp -o 15_calculator_app
 * Execution: ./15_calculator_app
 */

#include <iostream>
#include <cmath>
using namespace std;

// Basic operations
double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        cout << "Error: Division by zero!" << endl;
        return 0;
    }
    return a / b;
}

double powerOf(double base, double exponent) {
    return pow(base, exponent);
}

double squareRoot(double n) {
    if (n < 0) {
        cout << "Error: Cannot calculate square root of negative number!" << endl;
        return 0;
    }
    return sqrt(n);
}

double percentage(double number, double percent) {
    return (number * percent) / 100.0;
}

long long factorial(int n) {
    if (n < 0) return 0;
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double circleArea(double radius) {
    return 3.14159265359 * radius * radius;
}

double rectangleArea(double length, double width) {
    return length * width;
}

void displayMenu() {
    cout << "\n================================" << endl;
    cout << "      CALCULATOR MENU" << endl;
    cout << "================================" << endl;
    cout << "1.  Addition" << endl;
    cout << "2.  Subtraction" << endl;
    cout << "3.  Multiplication" << endl;
    cout << "4.  Division" << endl;
    cout << "5.  Power" << endl;
    cout << "6.  Square Root" << endl;
    cout << "7.  Percentage" << endl;
    cout << "8.  Factorial" << endl;
    cout << "9.  Circle Area" << endl;
    cout << "10. Rectangle Area" << endl;
    cout << "0.  Exit" << endl;
    cout << "================================" << endl;
    cout << "Enter choice: ";
}

int main() {
    int choice;
    double num1, num2, result;

    cout << "=== Advanced Calculator Application ===" << endl;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:  // Addition
                cout << "Enter two numbers: ";
                cin >> num1 >> num2;
                result = add(num1, num2);
                cout << num1 << " + " << num2 << " = " << result << endl;
                break;

            case 2:  // Subtraction
                cout << "Enter two numbers: ";
                cin >> num1 >> num2;
                result = subtract(num1, num2);
                cout << num1 << " - " << num2 << " = " << result << endl;
                break;

            case 3:  // Multiplication
                cout << "Enter two numbers: ";
                cin >> num1 >> num2;
                result = multiply(num1, num2);
                cout << num1 << " * " << num2 << " = " << result << endl;
                break;

            case 4:  // Division
                cout << "Enter two numbers: ";
                cin >> num1 >> num2;
                result = divide(num1, num2);
                if (num2 != 0) {
                    cout << num1 << " / " << num2 << " = " << result << endl;
                }
                break;

            case 5:  // Power
                cout << "Enter base and exponent: ";
                cin >> num1 >> num2;
                result = powerOf(num1, num2);
                cout << num1 << "^" << num2 << " = " << result << endl;
                break;

            case 6:  // Square Root
                cout << "Enter number: ";
                cin >> num1;
                result = squareRoot(num1);
                if (num1 >= 0) {
                    cout << "√" << num1 << " = " << result << endl;
                }
                break;

            case 7:  // Percentage
                cout << "Enter number and percentage: ";
                cin >> num1 >> num2;
                result = percentage(num1, num2);
                cout << num2 << "% of " << num1 << " = " << result << endl;
                break;

            case 8:  // Factorial
                int n;
                cout << "Enter number: ";
                cin >> n;
                if (n >= 0) {
                    cout << n << "! = " << factorial(n) << endl;
                } else {
                    cout << "Error: Factorial undefined for negative numbers!" << endl;
                }
                break;

            case 9:  // Circle Area
                cout << "Enter radius: ";
                cin >> num1;
                result = circleArea(num1);
                cout << "Area of circle = " << result << endl;
                break;

            case 10:  // Rectangle Area
                cout << "Enter length and width: ";
                cin >> num1 >> num2;
                result = rectangleArea(num1, num2);
                cout << "Area of rectangle = " << result << endl;
                break;

            case 0:  // Exit
                cout << "\nThank you for using the calculator!" << endl;
                cout << "Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice! Please enter 0-10." << endl;
        }

    } while (choice != 0);

    return 0;
}
