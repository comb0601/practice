/*
 * Lesson 3: Working Calculator - Practical Operator Usage
 *
 * This program demonstrates a functional calculator that performs:
 * - Basic arithmetic operations (+, -, *, /)
 * - Advanced operations (power, sqrt, modulus)
 * - Input validation
 * - Error handling
 *
 * Compile: cl Lesson03_Calculator.cpp
 */

#include <iostream>
#include <cmath>     // For pow and sqrt
#include <iomanip>   // For formatting
#include <limits>    // For input validation
using namespace std;

int main() {
    cout << "==========================================" << endl;
    cout << "       C++ CALCULATOR APPLICATION        " << endl;
    cout << "==========================================" << endl << endl;

    // Variables for calculator
    double num1, num2, result;
    char operation;
    bool continueCalc = true;

    while (continueCalc) {
        // Display menu
        cout << "\nAvailable Operations:" << endl;
        cout << "  + : Addition" << endl;
        cout << "  - : Subtraction" << endl;
        cout << "  * : Multiplication" << endl;
        cout << "  / : Division" << endl;
        cout << "  % : Modulus (integers only)" << endl;
        cout << "  ^ : Power" << endl;
        cout << "  s : Square Root" << endl;
        cout << "  q : Quit" << endl;
        cout << "-----------------------------------" << endl;

        // Get first number
        cout << "Enter first number: ";
        while (!(cin >> num1)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Enter a number: ";
        }

        // Get operation
        cout << "Enter operation: ";
        cin >> operation;

        // Handle quit option
        if (operation == 'q' || operation == 'Q') {
            cout << "\nThank you for using the calculator!" << endl;
            break;
        }

        // Get second number (not needed for square root)
        if (operation != 's' && operation != 'S') {
            cout << "Enter second number: ";
            while (!(cin >> num2)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Enter a number: ";
            }
        }

        // Perform calculation
        cout << "\nResult: ";

        switch (operation) {
            case '+':
                result = num1 + num2;
                cout << num1 << " + " << num2 << " = " << result << endl;
                break;

            case '-':
                result = num1 - num2;
                cout << num1 << " - " << num2 << " = " << result << endl;
                break;

            case '*':
                result = num1 * num2;
                cout << num1 << " * " << num2 << " = " << result << endl;
                break;

            case '/':
                if (num2 == 0) {
                    cout << "Error: Division by zero is not allowed!" << endl;
                } else {
                    result = num1 / num2;
                    cout << fixed << setprecision(4);
                    cout << num1 << " / " << num2 << " = " << result << endl;
                    cout << defaultfloat;  // Reset formatting
                }
                break;

            case '%':
                if (num2 == 0) {
                    cout << "Error: Modulus by zero is not allowed!" << endl;
                } else {
                    // Modulus works with integers
                    int int1 = static_cast<int>(num1);
                    int int2 = static_cast<int>(num2);
                    result = int1 % int2;
                    cout << int1 << " % " << int2 << " = " << result << endl;
                }
                break;

            case '^':
                result = pow(num1, num2);
                cout << fixed << setprecision(4);
                cout << num1 << " ^ " << num2 << " = " << result << endl;
                cout << defaultfloat;
                break;

            case 's':
            case 'S':
                if (num1 < 0) {
                    cout << "Error: Cannot calculate square root of negative number!" << endl;
                } else {
                    result = sqrt(num1);
                    cout << fixed << setprecision(4);
                    cout << "sqrt(" << num1 << ") = " << result << endl;
                    cout << defaultfloat;
                }
                break;

            default:
                cout << "Error: Invalid operation!" << endl;
                break;
        }

        // Additional calculations with result
        if (operation != 's' && operation != 'S' &&
            operation != 'q' && operation != 'Q') {
            cout << "\nAdditional Information:" << endl;
            cout << "  Sum: " << (num1 + num2) << endl;
            cout << "  Difference: " << (num1 - num2) << endl;
            cout << "  Product: " << (num1 * num2) << endl;
            if (num2 != 0) {
                cout << fixed << setprecision(2);
                cout << "  Quotient: " << (num1 / num2) << endl;
                cout << defaultfloat;
            }
            cout << "  Average: " << ((num1 + num2) / 2) << endl;
        }

        // Ask if user wants to continue
        char choice;
        cout << "\nContinue? (y/n): ";
        cin >> choice;
        if (choice == 'n' || choice == 'N') {
            continueCalc = false;
            cout << "\nThank you for using the calculator!" << endl;
        }
    }

    // Display some calculation examples
    cout << "\n==========================================" << endl;
    cout << "         CALCULATION EXAMPLES             " << endl;
    cout << "==========================================" << endl;

    cout << fixed << setprecision(2);

    cout << "\n1. Basic Arithmetic:" << endl;
    cout << "   15 + 7 = " << (15 + 7) << endl;
    cout << "   20 - 8 = " << (20 - 8) << endl;
    cout << "   6 * 9 = " << (6 * 9) << endl;
    cout << "   50 / 4 = " << (50.0 / 4.0) << endl;

    cout << "\n2. Order of Operations:" << endl;
    cout << "   2 + 3 * 4 = " << (2 + 3 * 4) << " (multiplication first)" << endl;
    cout << "   (2 + 3) * 4 = " << ((2 + 3) * 4) << " (parentheses first)" << endl;
    cout << "   10 - 2 * 3 + 8 / 4 = " << (10 - 2 * 3 + 8 / 4) << endl;

    cout << "\n3. Real-world Calculations:" << endl;

    // Calculate area of circle
    double radius = 5.0;
    double pi = 3.14159;
    double area = pi * radius * radius;
    cout << "   Area of circle (r=" << radius << "): " << area << " sq units" << endl;

    // Calculate compound interest
    double principal = 1000.0;
    double rate = 0.05;  // 5%
    int years = 10;
    double amount = principal * pow(1 + rate, years);
    cout << "   Compound interest ($1000 at 5% for 10 years): $" << amount << endl;

    // Calculate distance
    double speed = 60.0;  // km/h
    double time = 2.5;    // hours
    double distance = speed * time;
    cout << "   Distance traveled (60 km/h for 2.5 hours): " << distance << " km" << endl;

    // Calculate BMI
    double weight = 70.0;  // kg
    double height = 1.75;  // meters
    double bmi = weight / (height * height);
    cout << "   BMI (70kg, 1.75m): " << bmi << endl;

    return 0;
}

/*
 * Sample Run:
 * ==========================================
 *        C++ CALCULATOR APPLICATION
 * ==========================================
 *
 * Available Operations:
 *   + : Addition
 *   - : Subtraction
 *   * : Multiplication
 *   / : Division
 *   % : Modulus (integers only)
 *   ^ : Power
 *   s : Square Root
 *   q : Quit
 * -----------------------------------
 * Enter first number: 25
 * Enter operation: +
 * Enter second number: 17
 *
 * Result: 25 + 17 = 42
 *
 * Additional Information:
 *   Sum: 42
 *   Difference: 8
 *   Product: 425
 *   Quotient: 1.47
 *   Average: 21
 *
 * Continue? (y/n): n
 *
 * Thank you for using the calculator!
 */
