/*
 * Program: Basic Variables Demo
 * Description: Demonstrates declaration and initialization of basic data types
 * Compilation: g++ 01_basic_variables.cpp -o 01_basic_variables
 * Execution: ./01_basic_variables
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
    cout << "=== Basic Variables Demo ===" << endl << endl;

    // Integer types
    int age = 25;
    short temperature = -5;
    long population = 7800000000;
    long long bigNumber = 9223372036854775807;

    cout << "Integer Types:" << endl;
    cout << "Age (int): " << age << endl;
    cout << "Temperature (short): " << temperature << endl;
    cout << "Population (long): " << population << endl;
    cout << "Big Number (long long): " << bigNumber << endl << endl;

    // Floating-point types
    float price = 19.99f;
    double pi = 3.14159265359;
    long double preciseValue = 3.141592653589793238L;

    cout << "Floating-Point Types:" << endl;
    cout << "Price (float): $" << price << endl;
    cout << "Pi (double): " << pi << endl;
    cout << "Precise Value (long double): " << preciseValue << endl << endl;

    // Character types
    char grade = 'A';
    unsigned char byteValue = 255;

    cout << "Character Types:" << endl;
    cout << "Grade (char): " << grade << endl;
    cout << "Byte Value (unsigned char): " << (int)byteValue << endl << endl;

    // Boolean type
    bool isStudent = true;
    bool hasLicense = false;

    cout << "Boolean Types:" << endl;
    cout << "Is Student: " << (isStudent ? "Yes" : "No") << endl;
    cout << "Has License: " << (hasLicense ? "Yes" : "No") << endl << endl;

    // String type
    string name = "John Doe";
    string message = "Welcome to C++ Programming!";

    cout << "String Types:" << endl;
    cout << "Name: " << name << endl;
    cout << "Message: " << message << endl;

    return 0;
}
