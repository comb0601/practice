/*
 * Program: Constants Demo
 * Description: Demonstrates different ways to define and use constants
 * Compilation: g++ 08_constants_demo.cpp -o 08_constants_demo
 * Execution: ./08_constants_demo
 */

#include <iostream>
using namespace std;

// Global constants
const double PI = 3.14159265359;
const int MAX_STUDENTS = 100;

// #define preprocessor constants (old C-style)
#define SPEED_OF_LIGHT 299792458  // meters per second
#define COMPANY_NAME "TechCorp Inc."

int main() {
    cout << "=== Constants Demo ===" << endl << endl;

    // Using const keyword
    cout << "1. Const Keyword:" << endl;
    const double GRAVITY = 9.81;  // m/s^2
    const int DAYS_IN_WEEK = 7;

    cout << "Pi: " << PI << endl;
    cout << "Gravity: " << GRAVITY << " m/s^2" << endl;
    cout << "Days in week: " << DAYS_IN_WEEK << endl;
    cout << "Max students: " << MAX_STUDENTS << endl << endl;

    // Using #define (preprocessor)
    cout << "2. Preprocessor Constants (#define):" << endl;
    cout << "Speed of light: " << SPEED_OF_LIGHT << " m/s" << endl;
    cout << "Company: " << COMPANY_NAME << endl << endl;

    // Constexpr (compile-time constants)
    cout << "3. Constexpr (C++11):" << endl;
    constexpr int ARRAY_SIZE = 10;
    constexpr double E = 2.71828;

    cout << "Array size: " << ARRAY_SIZE << endl;
    cout << "Euler's number: " << E << endl << endl;

    // Practical example: Circle calculations
    cout << "4. Practical Example - Circle Calculations:" << endl;
    double radius;
    cout << "Enter circle radius: ";
    cin >> radius;

    double area = PI * radius * radius;
    double circumference = 2 * PI * radius;

    cout << "Area: " << area << endl;
    cout << "Circumference: " << circumference << endl << endl;

    // Const pointer
    cout << "5. Const Pointers:" << endl;
    int value = 42;
    const int* ptrToConst = &value;  // Pointer to constant int
    cout << "Value through pointer: " << *ptrToConst << endl;

    // value can be changed, but not through ptrToConst
    value = 100;
    cout << "Modified value: " << *ptrToConst << endl;

    // Attempting to modify through pointer would cause compile error:
    // *ptrToConst = 200;  // ERROR!

    return 0;
}
