/*
 * Program: Simple Calculator
 * Description: Performs basic arithmetic operations on two numbers
 * Compilation: g++ 02_simple_calculator.cpp -o 02_simple_calculator
 * Execution: ./02_simple_calculator
 */

#include <iostream>
using namespace std;

int main() {
    double num1, num2;

    cout << "=== Simple Calculator ===" << endl << endl;

    // Input
    cout << "Enter first number: ";
    cin >> num1;
    cout << "Enter second number: ";
    cin >> num2;

    // Calculations
    double sum = num1 + num2;
    double difference = num1 - num2;
    double product = num1 * num2;
    double quotient = (num2 != 0) ? num1 / num2 : 0;
    int modulo = (int)num1 % (int)num2; // Only works with integers

    // Output results
    cout << "\n=== Results ===" << endl;
    cout << num1 << " + " << num2 << " = " << sum << endl;
    cout << num1 << " - " << num2 << " = " << difference << endl;
    cout << num1 << " * " << num2 << " = " << product << endl;

    if (num2 != 0) {
        cout << num1 << " / " << num2 << " = " << quotient << endl;
        cout << (int)num1 << " % " << (int)num2 << " = " << modulo << endl;
    } else {
        cout << "Cannot divide by zero!" << endl;
    }

    return 0;
}
