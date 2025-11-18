/*
 * Program: Comparison Operators Demo
 * Description: Demonstrates all comparison operators
 * Compilation: g++ 11_comparison_operators.cpp -o 11_comparison_operators
 * Execution: ./11_comparison_operators
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Comparison Operators Demo ===" << endl << endl;

    int a, b;

    // Get input
    cout << "Enter first number: ";
    cin >> a;
    cout << "Enter second number: ";
    cin >> b;

    cout << "\nGiven: a = " << a << ", b = " << b << endl << endl;

    // Equal to (==)
    cout << "1. Equal to (==):" << endl;
    cout << "a == b: " << (a == b ? "True" : "False") << endl;
    if (a == b) {
        cout << "The numbers are equal" << endl;
    }
    cout << endl;

    // Not equal to (!=)
    cout << "2. Not equal to (!=):" << endl;
    cout << "a != b: " << (a != b ? "True" : "False") << endl;
    if (a != b) {
        cout << "The numbers are different" << endl;
    }
    cout << endl;

    // Greater than (>)
    cout << "3. Greater than (>):" << endl;
    cout << "a > b: " << (a > b ? "True" : "False") << endl;
    if (a > b) {
        cout << a << " is greater than " << b << endl;
    }
    cout << endl;

    // Less than (<)
    cout << "4. Less than (<):" << endl;
    cout << "a < b: " << (a < b ? "True" : "False") << endl;
    if (a < b) {
        cout << a << " is less than " << b << endl;
    }
    cout << endl;

    // Greater than or equal to (>=)
    cout << "5. Greater than or equal to (>=):" << endl;
    cout << "a >= b: " << (a >= b ? "True" : "False") << endl;
    if (a >= b) {
        cout << a << " is greater than or equal to " << b << endl;
    }
    cout << endl;

    // Less than or equal to (<=)
    cout << "6. Less than or equal to (<=):" << endl;
    cout << "a <= b: " << (a <= b ? "True" : "False") << endl;
    if (a <= b) {
        cout << a << " is less than or equal to " << b << endl;
    }
    cout << endl;

    // Find maximum
    cout << "7. Finding Maximum:" << endl;
    int max = (a > b) ? a : b;
    cout << "Maximum: " << max << endl << endl;

    // Find minimum
    cout << "8. Finding Minimum:" << endl;
    int min = (a < b) ? a : b;
    cout << "Minimum: " << min << endl << endl;

    // Three number comparison
    cout << "9. Three Number Comparison:" << endl;
    int c;
    cout << "Enter third number: ";
    cin >> c;

    cout << "\nComparing " << a << ", " << b << ", and " << c << endl;

    if (a > b && a > c) {
        cout << a << " is the largest" << endl;
    } else if (b > a && b > c) {
        cout << b << " is the largest" << endl;
    } else {
        cout << c << " is the largest" << endl;
    }

    if (a < b && a < c) {
        cout << a << " is the smallest" << endl;
    } else if (b < a && b < c) {
        cout << b << " is the smallest" << endl;
    } else {
        cout << c << " is the smallest" << endl;
    }
    cout << endl;

    // Comparison table
    cout << "10. Complete Comparison Table:" << endl;
    cout << "Operation\tResult" << endl;
    cout << "-----------------------" << endl;
    cout << "a == b\t\t" << (a == b) << endl;
    cout << "a != b\t\t" << (a != b) << endl;
    cout << "a > b\t\t" << (a > b) << endl;
    cout << "a < b\t\t" << (a < b) << endl;
    cout << "a >= b\t\t" << (a >= b) << endl;
    cout << "a <= b\t\t" << (a <= b) << endl;

    return 0;
}
