/*
 * Program: Input/Output Demo
 * Description: Demonstrates various input/output operations in C++
 * Compilation: g++ 07_input_output_demo.cpp -o 07_input_output_demo
 * Execution: ./07_input_output_demo
 */

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int main() {
    cout << "=== Input/Output Demo ===" << endl << endl;

    // Basic input and output
    string name;
    int age;

    cout << "Enter your name: ";
    getline(cin, name);  // Use getline to read entire line with spaces

    cout << "Enter your age: ";
    cin >> age;

    cout << "\nHello, " << name << "! You are " << age << " years old." << endl << endl;

    // Formatted output
    cout << "=== Formatted Output ===" << endl;

    double price = 1234.567;
    cout << "Default: " << price << endl;
    cout << fixed << setprecision(2);
    cout << "Fixed 2 decimals: " << price << endl;

    cout << scientific;
    cout << "Scientific notation: " << price << endl;

    cout << fixed;  // Reset to fixed
    cout << endl;

    // Width and alignment
    cout << "=== Width and Alignment ===" << endl;
    cout << setw(15) << left << "Product" << setw(10) << right << "Price" << endl;
    cout << setw(15) << left << "Apple" << setw(10) << right << "$1.99" << endl;
    cout << setw(15) << left << "Banana" << setw(10) << right << "$0.59" << endl;
    cout << setw(15) << left << "Orange" << setw(10) << right << "$2.49" << endl;
    cout << endl;

    // Fill characters
    cout << "=== Fill Characters ===" << endl;
    cout << setfill('*') << setw(30) << "RECEIPT" << setfill(' ') << endl;
    cout << "Total: $" << setw(10) << right << 45.67 << endl;
    cout << endl;

    // Boolean output
    cout << "=== Boolean Output ===" << endl;
    bool isTrue = true;
    cout << "Default: " << isTrue << endl;
    cout << boolalpha;
    cout << "Boolalpha: " << isTrue << endl;
    cout << noboolalpha;
    cout << endl;

    // Hexadecimal and octal
    cout << "=== Number Bases ===" << endl;
    int number = 255;
    cout << "Decimal: " << dec << number << endl;
    cout << "Hexadecimal: " << hex << number << endl;
    cout << "Octal: " << oct << number << endl;

    return 0;
}
