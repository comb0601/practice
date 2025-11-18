/*
 * Program: Function Overloading Demo
 * Description: Demonstrates function overloading with different parameter types and counts
 * Compilation: g++ 03_function_overloading.cpp -o 03_function_overloading
 * Execution: ./03_function_overloading
 */

#include <iostream>
using namespace std;

// Overloaded print functions
void print(int x) {
    cout << "Integer: " << x << endl;
}

void print(double x) {
    cout << "Double: " << x << endl;
}

void print(string x) {
    cout << "String: " << x << endl;
}

void print(int x, int y) {
    cout << "Two integers: " << x << ", " << y << endl;
}

// Overloaded area functions
double area(double radius) {
    const double PI = 3.14159265359;
    return PI * radius * radius;  // Circle
}

double area(double length, double width) {
    return length * width;  // Rectangle
}

double area(double base, double height, bool isTriangle) {
    if (isTriangle) {
        return 0.5 * base * height;  // Triangle
    }
    return base * height;
}

// Overloaded max functions
int max(int a, int b) {
    return (a > b) ? a : b;
}

double max(double a, double b) {
    return (a > b) ? a : b;
}

int max(int a, int b, int c) {
    return max(max(a, b), c);
}

// Overloaded display functions
void display(int arr[], int size) {
    cout << "Integer array: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void display(double arr[], int size) {
    cout << "Double array: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    cout << "=== Function Overloading Demo ===" << endl << endl;

    // Overloaded print
    cout << "1. Overloaded Print Functions:" << endl;
    print(42);
    print(3.14);
    print("Hello World");
    print(10, 20);
    cout << endl;

    // Overloaded area
    cout << "2. Overloaded Area Functions:" << endl;
    cout << "Circle (radius=5): " << area(5.0) << endl;
    cout << "Rectangle (4x6): " << area(4.0, 6.0) << endl;
    cout << "Triangle (base=4, height=5): " << area(4.0, 5.0, true) << endl;
    cout << endl;

    // Overloaded max
    cout << "3. Overloaded Max Functions:" << endl;
    cout << "Max of 5 and 10: " << max(5, 10) << endl;
    cout << "Max of 5.5 and 3.2: " << max(5.5, 3.2) << endl;
    cout << "Max of 5, 10, 7: " << max(5, 10, 7) << endl;
    cout << endl;

    // Overloaded display
    cout << "4. Overloaded Display Functions:" << endl;
    int intArr[] = {1, 2, 3, 4, 5};
    double doubleArr[] = {1.1, 2.2, 3.3, 4.4};
    display(intArr, 5);
    display(doubleArr, 4);

    return 0;
}
