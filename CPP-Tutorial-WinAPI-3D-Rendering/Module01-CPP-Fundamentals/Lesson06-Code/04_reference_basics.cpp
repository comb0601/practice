/*
 * Program: Reference Basics
 * Description: Introduction to references - alias for variables
 * Compilation: g++ -std=c++17 04_reference_basics.cpp -o 04_reference_basics
 * Execution: ./04_reference_basics
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== Reference Basics ===" << endl << endl;

    // Reference declaration
    cout << "1. Reference Declaration:" << endl;
    int x = 42;
    int& ref = x;  // ref is a reference to x

    cout << "x = " << x << endl;
    cout << "ref = " << ref << endl;
    cout << "Address of x: " << &x << endl;
    cout << "Address of ref: " << &ref << endl;
    cout << "Same address - ref is an alias for x!" << endl << endl;

    // Modifying through reference
    cout << "2. Modifying Through Reference:" << endl;
    cout << "Before: x = " << x << ", ref = " << ref << endl;

    ref = 100;

    cout << "After ref = 100:" << endl;
    cout << "x = " << x << ", ref = " << ref << endl;
    cout << "Changing ref changes x!" << endl << endl;

    // Modifying original
    cout << "3. Modifying Original Variable:" << endl;
    cout << "Before: x = " << x << ", ref = " << ref << endl;

    x = 200;

    cout << "After x = 200:" << endl;
    cout << "x = " << x << ", ref = " << ref << endl;
    cout << "Changing x changes ref!" << endl << endl;

    // References must be initialized
    cout << "4. References vs Pointers:" << endl;
    int y = 50;

    int* ptr = &y;      // Pointer (can be null, can be reassigned)
    int& ref2 = y;      // Reference (must be initialized, cannot be reassigned)

    cout << "Using pointer: *ptr = " << *ptr << endl;
    cout << "Using reference: ref2 = " << ref2 << endl;

    // Change what pointer points to
    int z = 75;
    ptr = &z;  // OK - pointer can be reassigned

    cout << "After ptr = &z: *ptr = " << *ptr << endl;

    // ref2 = z;  // This doesn't make ref2 refer to z, it assigns z's value to y!
    ref2 = z;
    cout << "After ref2 = z: y = " << y << " (value copied, not reference changed)" << endl << endl;

    // Multiple references
    cout << "5. Multiple References to Same Variable:" << endl;
    int num = 10;
    int& r1 = num;
    int& r2 = num;
    int& r3 = r1;  // Reference to a reference (still refers to num)

    cout << "num = " << num << endl;
    cout << "r1 = " << r1 << endl;
    cout << "r2 = " << r2 << endl;
    cout << "r3 = " << r3 << endl;

    r2 = 99;

    cout << "After r2 = 99:" << endl;
    cout << "num = " << num << ", r1 = " << r1 << ", r2 = " << r2 << ", r3 = " << r3 << endl << endl;

    // Const references
    cout << "6. Const References:" << endl;
    int value = 42;
    const int& constRef = value;

    cout << "value = " << value << endl;
    cout << "constRef = " << constRef << endl;

    value = 100;  // OK - can modify value
    cout << "After value = 100: constRef = " << constRef << endl;

    // constRef = 200;  // ERROR - cannot modify through const reference

    // Const reference to literal
    const int& literalRef = 42;  // OK - creates temporary
    cout << "literalRef = " << literalRef << endl << endl;

    // Reference with different types
    cout << "7. References with Different Types:" << endl;
    double d = 3.14;
    char c = 'A';

    double& dref = d;
    char& cref = c;

    cout << "double d = " << d << ", dref = " << dref << endl;
    cout << "char c = " << c << ", cref = " << cref << endl;

    return 0;
}
