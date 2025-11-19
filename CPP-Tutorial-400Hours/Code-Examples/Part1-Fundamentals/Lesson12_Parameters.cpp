/*
 * Lesson 12: Function Parameters - Pass by Value and Reference
 *
 * Demonstrates different parameter passing mechanisms
 *
 * Compile: cl Lesson12_Parameters.cpp
 */

#include <iostream>
using namespace std;

// Pass by value
void modifyValue(int x) {
    x = 100;
    cout << "Inside modifyValue: " << x << endl;
}

// Pass by reference
void modifyReference(int &x) {
    x = 100;
    cout << "Inside modifyReference: " << x << endl;
}

// Pass by pointer
void modifyPointer(int *x) {
    *x = 100;
    cout << "Inside modifyPointer: " << *x << endl;
}

// Swap using references
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// Array parameter (always passed by reference)
void doubleArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= 2;
    }
}

// Const reference (read-only)
void printValue(const int &x) {
    cout << "Value: " << x << endl;
    // x = 10; // Error: cannot modify const
}

int main() {
    cout << "=== Function Parameters Demo ===" << endl << endl;

    // PASS BY VALUE
    cout << "=== PASS BY VALUE ===" << endl;
    int num1 = 10;
    cout << "Before: " << num1 << endl;
    modifyValue(num1);
    cout << "After: " << num1 << endl << endl;

    // PASS BY REFERENCE
    cout << "=== PASS BY REFERENCE ===" << endl;
    int num2 = 10;
    cout << "Before: " << num2 << endl;
    modifyReference(num2);
    cout << "After: " << num2 << endl << endl;

    // PASS BY POINTER
    cout << "=== PASS BY POINTER ===" << endl;
    int num3 = 10;
    cout << "Before: " << num3 << endl;
    modifyPointer(&num3);
    cout << "After: " << num3 << endl << endl;

    // SWAP FUNCTION
    cout << "=== SWAP FUNCTION ===" << endl;
    int a = 5, b = 10;
    cout << "Before swap: a=" << a << ", b=" << b << endl;
    swap(a, b);
    cout << "After swap: a=" << a << ", b=" << b << endl << endl;

    // ARRAY PARAMETER
    cout << "=== ARRAY PARAMETER ===" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    cout << "Before: ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << endl;

    doubleArray(arr, 5);
    cout << "After: ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << endl << endl;

    // CONST REFERENCE
    cout << "=== CONST REFERENCE ===" << endl;
    int value = 42;
    printValue(value);

    return 0;
}
