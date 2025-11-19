/*
 * Lesson 27: Const with Pointers
 * Compile: cl Lesson27_ConstPointers.cpp
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Const Pointers Demo ===" << endl << endl;

    int value = 42;
    int other = 100;

    // POINTER TO CONST
    cout << "=== POINTER TO CONST ===" << endl;
    const int *ptr1 = &value;
    cout << "Value: " << *ptr1 << endl;
    // *ptr1 = 50; // Error: cannot modify value
    ptr1 = &other;  // OK: can change pointer
    cout << "After redirect: " << *ptr1 << endl << endl;

    // CONST POINTER
    cout << "=== CONST POINTER ===" << endl;
    int *const ptr2 = &value;
    cout << "Value: " << *ptr2 << endl;
    *ptr2 = 50;  // OK: can modify value
    cout << "After modify: " << *ptr2 << endl;
    // ptr2 = &other; // Error: cannot change pointer
    cout << endl;

    // CONST POINTER TO CONST
    cout << "=== CONST POINTER TO CONST ===" << endl;
    const int *const ptr3 = &value;
    cout << "Value: " << *ptr3 << endl;
    // *ptr3 = 60; // Error: cannot modify value
    // ptr3 = &other; // Error: cannot change pointer

    return 0;
}
