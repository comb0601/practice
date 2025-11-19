/*
 * Program: Pointer to Pointer
 * Description: Double pointers and multi-level indirection
 * Compilation: g++ -std=c++17 08_pointer_to_pointer.cpp -o 08_pointer_to_pointer
 * Execution: ./08_pointer_to_pointer
 */

#include <iostream>
using namespace std;

void modifyPointer(int** pptr, int* newAddress) {
    *pptr = newAddress;  // Change where the original pointer points
}

void allocateMemory(int** pptr, int value) {
    *pptr = new int(value);  // Allocate and assign
}

int main() {
    cout << "=== Pointer to Pointer ===" << endl << endl;

    // Basic pointer to pointer
    cout << "1. Basic Pointer to Pointer:" << endl;
    int x = 42;
    int* ptr = &x;       // Pointer to int
    int** pptr = &ptr;   // Pointer to pointer to int

    cout << "x = " << x << endl;
    cout << "ptr points to x: *ptr = " << *ptr << endl;
    cout << "pptr points to ptr: **pptr = " << **pptr << endl;
    cout << endl;

    cout << "Address relationships:" << endl;
    cout << "Address of x: " << &x << endl;
    cout << "Value of ptr: " << ptr << " (same as &x)" << endl;
    cout << "Address of ptr: " << &ptr << endl;
    cout << "Value of pptr: " << pptr << " (same as &ptr)" << endl;
    cout << "Value of *pptr: " << *pptr << " (same as ptr)" << endl;
    cout << "Value of **pptr: " << **pptr << " (same as x)" << endl << endl;

    // Modifying values
    cout << "2. Modifying Values:" << endl;
    cout << "Original: x = " << x << endl;

    **pptr = 100;
    cout << "After **pptr = 100: x = " << x << endl;

    *ptr = 200;
    cout << "After *ptr = 200: x = " << x << endl << endl;

    // Changing what pointer points to
    cout << "3. Changing Pointer Targets:" << endl;
    int y = 99;
    int* ptr2 = &y;

    cout << "Before: *ptr = " << *ptr << ", *ptr2 = " << *ptr2 << endl;
    cout << "**pptr = " << **pptr << endl;

    *pptr = ptr2;  // Make ptr point to y instead of x

    cout << "After *pptr = ptr2:" << endl;
    cout << "*ptr = " << *ptr << " (now points to y)" << endl;
    cout << "**pptr = " << **pptr << endl << endl;

    // Function modifying pointer
    cout << "4. Function Modifying Pointer:" << endl;
    int a = 10, b = 20;
    int* myPtr = &a;

    cout << "Before: *myPtr = " << *myPtr << endl;
    modifyPointer(&myPtr, &b);
    cout << "After modifyPointer: *myPtr = " << *myPtr << endl << endl;

    // 2D array with pointer to pointer
    cout << "5. Pointer to Pointer with Arrays:" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    int* arrPtr = arr;
    int** arrPtrPtr = &arrPtr;

    cout << "Accessing array through pointer to pointer:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "**arrPtrPtr + " << i << " = " << *(*arrPtrPtr + i) << endl;
    }
    cout << endl;

    // Array of pointers accessed by pointer to pointer
    cout << "6. Array of Pointers:" << endl;
    int val1 = 10, val2 = 20, val3 = 30;
    int* ptrArray[] = {&val1, &val2, &val3};
    int** p = ptrArray;

    cout << "Accessing array of pointers:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "*(p + " << i << ") points to: " << **(p + i) << endl;
    }
    cout << endl;

    // Triple pointer (just for demonstration)
    cout << "7. Triple Pointer:" << endl;
    int value = 123;
    int* p1 = &value;
    int** p2 = &p1;
    int*** p3 = &p2;

    cout << "value = " << value << endl;
    cout << "*p1 = " << *p1 << endl;
    cout << "**p2 = " << **p2 << endl;
    cout << "***p3 = " << ***p3 << endl;
    cout << "All refer to the same value!" << endl << endl;

    // Practical use: modifying pointer in function
    cout << "8. Allocating Memory Through Function:" << endl;
    int* dynamicPtr = nullptr;
    cout << "Before: dynamicPtr = " << dynamicPtr << endl;

    allocateMemory(&dynamicPtr, 456);

    cout << "After allocateMemory:" << endl;
    cout << "dynamicPtr points to: " << *dynamicPtr << endl;

    delete dynamicPtr;  // Clean up
    dynamicPtr = nullptr;
    cout << endl;

    // Use cases
    cout << "9. Common Use Cases:" << endl;
    cout << "- Modifying pointer in function (must pass pointer to pointer)" << endl;
    cout << "- Dynamic allocation in functions" << endl;
    cout << "- Multi-dimensional arrays" << endl;
    cout << "- Linked lists and trees" << endl;
    cout << "- Command line arguments (char** argv)" << endl;

    return 0;
}
