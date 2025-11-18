/*
 * Lesson 2: Input/Output - Using cin and cout
 *
 * This program demonstrates:
 * - Output with cout
 * - Input with cin
 * - Reading different data types
 * - Reading multiple values
 * - Reading strings with spaces (getline)
 * - Formatting output
 *
 * Compile: cl Lesson02_InputOutput.cpp
 */

#include <iostream>
#include <string>
#include <iomanip>  // For formatting
using namespace std;

int main() {
    cout << "=== C++ Input/Output Demo ===" << endl << endl;

    // BASIC OUTPUT
    cout << "=== Basic Output (cout) ===" << endl;
    cout << "Hello, World!" << endl;
    cout << "Number: " << 42 << endl;
    cout << "Pi: " << 3.14159 << endl << endl;

    // BASIC INPUT
    cout << "=== Basic Input (cin) ===" << endl;
    cout << "Enter your name: ";
    string name;
    cin >> name;  // Reads until whitespace
    cout << "Hello, " << name << "!" << endl << endl;

    // READING DIFFERENT DATA TYPES
    cout << "=== Reading Different Data Types ===" << endl;

    int age;
    cout << "Enter your age: ";
    cin >> age;

    double height;
    cout << "Enter your height (in meters): ";
    cin >> height;

    char grade;
    cout << "Enter your grade (A-F): ";
    cin >> grade;

    cout << "\nYou entered:" << endl;
    cout << "  Age: " << age << " years" << endl;
    cout << "  Height: " << height << " meters" << endl;
    cout << "  Grade: " << grade << endl << endl;

    // READING MULTIPLE VALUES
    cout << "=== Reading Multiple Values ===" << endl;
    int x, y, z;
    cout << "Enter three numbers separated by spaces: ";
    cin >> x >> y >> z;
    cout << "You entered: " << x << ", " << y << ", " << z << endl;
    cout << "Sum: " << (x + y + z) << endl << endl;

    // READING STRINGS WITH SPACES
    cout << "=== Reading Strings with Spaces ===" << endl;
    cin.ignore();  // Clear the input buffer

    string fullName;
    cout << "Enter your full name: ";
    getline(cin, fullName);  // Reads entire line including spaces
    cout << "Full name: " << fullName << endl << endl;

    string address;
    cout << "Enter your address: ";
    getline(cin, address);
    cout << "Address: " << address << endl << endl;

    // FORMATTING OUTPUT
    cout << "=== Formatting Output ===" << endl;

    double price1 = 19.5;
    double price2 = 7.99;
    double price3 = 123.456;

    cout << "Default formatting:" << endl;
    cout << "Price 1: $" << price1 << endl;
    cout << "Price 2: $" << price2 << endl;
    cout << "Price 3: $" << price3 << endl << endl;

    cout << "Fixed-point notation with 2 decimal places:" << endl;
    cout << fixed << setprecision(2);
    cout << "Price 1: $" << price1 << endl;
    cout << "Price 2: $" << price2 << endl;
    cout << "Price 3: $" << price3 << endl << endl;

    // WIDTH AND ALIGNMENT
    cout << "=== Width and Alignment ===" << endl;
    cout << left << setw(15) << "Item" << setw(10) << "Price" << endl;
    cout << string(25, '-') << endl;
    cout << setw(15) << "Apple" << setw(10) << price1 << endl;
    cout << setw(15) << "Banana" << setw(10) << price2 << endl;
    cout << setw(15) << "Orange" << setw(10) << price3 << endl << endl;

    // BOOLEAN OUTPUT
    cout << "=== Boolean Output ===" << endl;
    bool isValid = true;
    bool isEmpty = false;

    cout << "Default (numeric): " << isValid << ", " << isEmpty << endl;
    cout << boolalpha;  // Display as true/false
    cout << "As text: " << isValid << ", " << isEmpty << endl << endl;

    // ESCAPE SEQUENCES
    cout << "=== Escape Sequences ===" << endl;
    cout << "Tab:\\tHello\\tWorld" << endl;
    cout << "Tab:\tHello\tWorld" << endl;
    cout << "Newline:\\nLine 1\\nLine 2" << endl;
    cout << "Newline:\nLine 1\nLine 2" << endl;
    cout << "Quote: \"Hello\"" << endl;
    cout << "Backslash: \\\\" << endl << endl;

    // SIMPLE CALCULATOR EXAMPLE
    cout << "=== Simple Calculator ===" << endl;
    double num1, num2;
    char operation;

    cout << "Enter first number: ";
    cin >> num1;
    cout << "Enter operation (+, -, *, /): ";
    cin >> operation;
    cout << "Enter second number: ";
    cin >> num2;

    cout << "\nResult: ";
    cout << num1 << " " << operation << " " << num2 << " = ";

    if (operation == '+') {
        cout << (num1 + num2) << endl;
    } else if (operation == '-') {
        cout << (num1 - num2) << endl;
    } else if (operation == '*') {
        cout << (num1 * num2) << endl;
    } else if (operation == '/') {
        if (num2 != 0) {
            cout << (num1 / num2) << endl;
        } else {
            cout << "Error: Division by zero!" << endl;
        }
    } else {
        cout << "Error: Invalid operation!" << endl;
    }

    return 0;
}

/*
 * Sample Run:
 * === C++ Input/Output Demo ===
 *
 * === Basic Output (cout) ===
 * Hello, World!
 * Number: 42
 * Pi: 3.14159
 *
 * === Basic Input (cin) ===
 * Enter your name: John
 * Hello, John!
 *
 * (Program will wait for user input for remaining sections)
 */
