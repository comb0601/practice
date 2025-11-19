/*
 * Program: Pointer Basics
 * Description: Introduction to pointers - declaration, initialization, dereferencing
 * Compilation: g++ -std=c++17 01_pointer_basics.cpp -o 01_pointer_basics
 * Execution: ./01_pointer_basics
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Pointer Basics ===" << endl << endl;

    // Variable declaration
    cout << "1. Variable and Pointer Declaration:" << endl;
    int x = 42;
    int* ptr = &x;  // Pointer to x

    cout << "Value of x: " << x << endl;
    cout << "Address of x: " << &x << endl;
    cout << "Value of ptr (address it holds): " << ptr << endl;
    cout << "Value pointed to by ptr: " << *ptr << endl;
    cout << "Address of ptr itself: " << &ptr << endl << endl;

    // Dereferencing
    cout << "2. Dereferencing (accessing value):" << endl;
    cout << "x = " << x << endl;
    cout << "*ptr = " << *ptr << endl;
    cout << "Both refer to the same value!" << endl << endl;

    // Modifying through pointer
    cout << "3. Modifying Value Through Pointer:" << endl;
    cout << "Before: x = " << x << ", *ptr = " << *ptr << endl;

    *ptr = 100;  // Change value through pointer

    cout << "After *ptr = 100:" << endl;
    cout << "x = " << x << ", *ptr = " << *ptr << endl;
    cout << "Changing *ptr also changes x!" << endl << endl;

    // Multiple pointers to same variable
    cout << "4. Multiple Pointers to Same Variable:" << endl;
    int* ptr2 = &x;

    cout << "ptr points to: " << *ptr << endl;
    cout << "ptr2 points to: " << *ptr2 << endl;

    *ptr2 = 200;

    cout << "After *ptr2 = 200:" << endl;
    cout << "x = " << x << endl;
    cout << "*ptr = " << *ptr << endl;
    cout << "*ptr2 = " << *ptr2 << endl << endl;

    // Pointer reassignment
    cout << "5. Pointer Reassignment:" << endl;
    int y = 99;
    cout << "Created y = " << y << endl;
    cout << "ptr points to: " << *ptr << " (at address " << ptr << ")" << endl;

    ptr = &y;  // Point to different variable

    cout << "After ptr = &y:" << endl;
    cout << "ptr now points to: " << *ptr << " (at address " << ptr << ")" << endl;
    cout << "x is still: " << x << endl;
    cout << "y is: " << y << endl << endl;

    // Different data types
    cout << "6. Pointers with Different Data Types:" << endl;
    double d = 3.14159;
    char c = 'A';

    double* dptr = &d;
    char* cptr = &c;

    cout << "double d = " << d << ", *dptr = " << *dptr << endl;
    cout << "char c = " << c << ", *cptr = " << *cptr << endl << endl;

    // Size of pointers
    cout << "7. Size of Pointers:" << endl;
    cout << "Size of int: " << sizeof(int) << " bytes" << endl;
    cout << "Size of int*: " << sizeof(int*) << " bytes" << endl;
    cout << "Size of double: " << sizeof(double) << " bytes" << endl;
    cout << "Size of double*: " << sizeof(double*) << " bytes" << endl;
    cout << "Size of char: " << sizeof(char) << " bytes" << endl;
    cout << "Size of char*: " << sizeof(char*) << " bytes" << endl;
    cout << "Note: All pointers have the same size!" << endl;

    return 0;
}
