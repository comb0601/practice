/*
 * Program: Parameter Passing Demo
 * Description: Demonstrates pass by value, pass by reference, and pass by const reference
 * Compilation: g++ 02_parameter_passing.cpp -o 02_parameter_passing
 * Execution: ./02_parameter_passing
 */

#include <iostream>
using namespace std;

// Pass by value - copies the value
void passByValue(int x) {
    x = x * 2;
    cout << "Inside passByValue: " << x << endl;
}

// Pass by reference - modifies original variable
void passByReference(int& x) {
    x = x * 2;
    cout << "Inside passByReference: " << x << endl;
}

// Pass by const reference - cannot modify, efficient for large objects
void passByConstReference(const int& x) {
    cout << "Inside passByConstReference: " << x << endl;
    // x = x * 2;  // ERROR! Cannot modify const reference
}

// Swap function using references
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Calculate multiple values using references
void calculateCircle(double radius, double& area, double& circumference) {
    const double PI = 3.14159265359;
    area = PI * radius * radius;
    circumference = 2 * PI * radius;
}

// Passing arrays (always passed by reference)
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void doubleArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= 2;
    }
}

int main() {
    cout << "=== Parameter Passing Demo ===" << endl << endl;

    // Pass by value
    cout << "1. Pass by Value:" << endl;
    int value = 10;
    cout << "Before: value = " << value << endl;
    passByValue(value);
    cout << "After: value = " << value << " (unchanged)" << endl << endl;

    // Pass by reference
    cout << "2. Pass by Reference:" << endl;
    value = 10;
    cout << "Before: value = " << value << endl;
    passByReference(value);
    cout << "After: value = " << value << " (changed)" << endl << endl;

    // Pass by const reference
    cout << "3. Pass by Const Reference:" << endl;
    value = 10;
    passByConstReference(value);
    cout << "Value remains: " << value << endl << endl;

    // Swap function
    cout << "4. Swap Function:" << endl;
    int a = 5, b = 10;
    cout << "Before swap: a = " << a << ", b = " << b << endl;
    swap(a, b);
    cout << "After swap: a = " << a << ", b = " << b << endl << endl;

    // Multiple return values
    cout << "5. Multiple Return Values (using references):" << endl;
    double radius = 5.0, area, circumference;
    calculateCircle(radius, area, circumference);
    cout << "Radius: " << radius << endl;
    cout << "Area: " << area << endl;
    cout << "Circumference: " << circumference << endl << endl;

    // Array passing
    cout << "6. Array Passing:" << endl;
    int numbers[] = {1, 2, 3, 4, 5};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    cout << "Original array: ";
    printArray(numbers, size);

    doubleArray(numbers, size);
    cout << "After doubling: ";
    printArray(numbers, size);

    return 0;
}
