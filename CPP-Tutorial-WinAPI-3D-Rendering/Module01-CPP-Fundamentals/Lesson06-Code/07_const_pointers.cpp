/*
 * Program: Const Pointers
 * Description: Pointer to const, const pointer, const pointer to const
 * Compilation: g++ -std=c++17 07_const_pointers.cpp -o 07_const_pointers
 * Execution: ./07_const_pointers
 */

#include <iostream>
using namespace std;

void printArray(const int* arr, int size) {
    cout << "Array: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
        // arr[i] = 0;  // ERROR - cannot modify through const pointer
    }
    cout << endl;
}

int main() {
    cout << "=== Const Pointers ===" << endl << endl;

    int x = 42;
    int y = 100;

    // 1. Pointer to const (const int*)
    cout << "1. Pointer to Const (const int*):" << endl;
    const int* ptr1 = &x;

    cout << "Value: " << *ptr1 << endl;
    // *ptr1 = 50;  // ERROR - cannot modify value through pointer
    ptr1 = &y;      // OK - can change where pointer points
    cout << "After changing pointer: " << *ptr1 << endl << endl;

    // 2. Const pointer (int* const)
    cout << "2. Const Pointer (int* const):" << endl;
    int* const ptr2 = &x;

    cout << "Value: " << *ptr2 << endl;
    *ptr2 = 50;     // OK - can modify value
    cout << "After modifying value: " << *ptr2 << " (x = " << x << ")" << endl;
    // ptr2 = &y;   // ERROR - cannot change where pointer points
    cout << endl;

    // 3. Const pointer to const (const int* const)
    cout << "3. Const Pointer to Const (const int* const):" << endl;
    const int* const ptr3 = &x;

    cout << "Value: " << *ptr3 << endl;
    // *ptr3 = 75;  // ERROR - cannot modify value
    // ptr3 = &y;   // ERROR - cannot change where pointer points
    cout << "Cannot modify value or pointer!" << endl << endl;

    // 4. Comparison
    cout << "4. Comparison Summary:" << endl;
    int a = 10, b = 20;

    int* ptr4 = &a;              // Regular pointer
    const int* ptr5 = &a;        // Pointer to const
    int* const ptr6 = &a;        // Const pointer
    const int* const ptr7 = &a;  // Const pointer to const

    cout << "Regular pointer (int*):" << endl;
    *ptr4 = 15;                  // OK
    ptr4 = &b;                   // OK
    cout << "  Can modify value and pointer" << endl;

    cout << "\nPointer to const (const int*):" << endl;
    // *ptr5 = 15;               // ERROR
    ptr5 = &b;                   // OK
    cout << "  Can only modify pointer" << endl;

    cout << "\nConst pointer (int* const):" << endl;
    *ptr6 = 15;                  // OK
    // ptr6 = &b;                // ERROR
    cout << "  Can only modify value" << endl;

    cout << "\nConst pointer to const (const int* const):" << endl;
    // *ptr7 = 15;               // ERROR
    // ptr7 = &b;                // ERROR
    cout << "  Cannot modify value or pointer" << endl << endl;

    // 5. Function parameters
    cout << "5. Const Pointers in Functions:" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;

    printArray(arr, size);  // Prevents accidental modification
    cout << endl;

    // 6. Reading pointer declarations
    cout << "6. Reading Pointer Declarations:" << endl;
    cout << "Read from right to left:" << endl;
    cout << "  const int* p    = p is a pointer to const int" << endl;
    cout << "  int* const p    = p is a const pointer to int" << endl;
    cout << "  const int* const p = p is a const pointer to const int" << endl;
    cout << endl;

    // 7. Practical use cases
    cout << "7. Practical Use Cases:" << endl;
    cout << "Pointer to const:" << endl;
    cout << "  - Function parameters (prevent modification)" << endl;
    cout << "  - Reading from const data" << endl;

    cout << "\nConst pointer:" << endl;
    cout << "  - Fixed memory location" << endl;
    cout << "  - Hardware registers" << endl;

    cout << "\nConst pointer to const:" << endl;
    cout << "  - Read-only access to fixed location" << endl;
    cout << "  - String literals" << endl;

    return 0;
}
