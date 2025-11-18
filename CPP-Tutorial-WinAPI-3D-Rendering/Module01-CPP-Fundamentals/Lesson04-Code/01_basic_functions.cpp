/*
 * Program: Basic Functions Demo
 * Description: Demonstrates function declaration, definition, and calling
 * Compilation: g++ 01_basic_functions.cpp -o 01_basic_functions
 * Execution: ./01_basic_functions
 */

#include <iostream>
using namespace std;

// Function prototypes (declarations)
void greet();
void greetPerson(string name);
int add(int a, int b);
double calculateArea(double radius);
bool isEven(int number);

int main() {
    cout << "=== Basic Functions Demo ===" << endl << endl;

    // Calling void function with no parameters
    cout << "1. Simple function:" << endl;
    greet();
    cout << endl;

    // Calling void function with parameters
    cout << "2. Function with parameters:" << endl;
    greetPerson("Alice");
    greetPerson("Bob");
    cout << endl;

    // Calling function that returns a value
    cout << "3. Function with return value:" << endl;
    int sum = add(5, 3);
    cout << "5 + 3 = " << sum << endl;
    cout << "10 + 20 = " << add(10, 20) << endl;
    cout << endl;

    // Calling function with calculations
    cout << "4. Area calculation function:" << endl;
    double area = calculateArea(5.0);
    cout << "Area of circle with radius 5: " << area << endl;
    cout << endl;

    // Calling boolean function
    cout << "5. Boolean function:" << endl;
    int num = 7;
    if (isEven(num)) {
        cout << num << " is even" << endl;
    } else {
        cout << num << " is odd" << endl;
    }

    return 0;
}

// Function definitions

// Simple function with no parameters and no return
void greet() {
    cout << "Hello, World!" << endl;
}

// Function with parameter
void greetPerson(string name) {
    cout << "Hello, " << name << "!" << endl;
}

// Function with return value
int add(int a, int b) {
    return a + b;
}

// Function with calculation
double calculateArea(double radius) {
    const double PI = 3.14159265359;
    return PI * radius * radius;
}

// Boolean function
bool isEven(int number) {
    return (number % 2 == 0);
}
