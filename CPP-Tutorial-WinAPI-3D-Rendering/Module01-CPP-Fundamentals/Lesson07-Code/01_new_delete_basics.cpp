/*
 * Program: New and Delete Basics
 * Description: Introduction to dynamic memory allocation with new and delete
 * Compilation: g++ -std=c++17 01_new_delete_basics.cpp -o 01_new_delete_basics
 * Execution: ./01_new_delete_basics
 */

#include <iostream>
using namespace std;

int main() {
    cout << "=== New and Delete Basics ===" << endl << endl;

    // Single integer allocation
    cout << "1. Allocating Single Integer:" << endl;
    int* ptr = new int;        // Allocate memory
    *ptr = 42;                 // Assign value

    cout << "Allocated int at address: " << ptr << endl;
    cout << "Value: " << *ptr << endl;

    delete ptr;                // Deallocate memory
    ptr = nullptr;             // Good practice
    cout << "Memory deleted" << endl << endl;

    // Allocation with initialization
    cout << "2. Allocation with Initialization:" << endl;
    int* ptr2 = new int(100);  // Allocate and initialize

    cout << "Allocated and initialized: " << *ptr2 << endl;

    delete ptr2;
    ptr2 = nullptr;
    cout << endl;

    // Different data types
    cout << "3. Different Data Types:" << endl;
    double* dptr = new double(3.14159);
    char* cptr = new char('A');
    bool* bptr = new bool(true);

    cout << "double: " << *dptr << endl;
    cout << "char: " << *cptr << endl;
    cout << "bool: " << (*bptr ? "true" : "false") << endl;

    delete dptr;
    delete cptr;
    delete bptr;
    cout << endl;

    // Dynamic array allocation
    cout << "4. Dynamic Array Allocation:" << endl;
    int size = 5;
    int* arr = new int[size];  // Allocate array

    cout << "Allocated array of " << size << " integers" << endl;
    cout << "Enter " << size << " numbers:" << endl;

    for (int i = 0; i < size; i++) {
        cout << "  [" << i << "]: ";
        cin >> arr[i];
    }

    cout << "Your array: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr;              // Note: delete[] for arrays!
    arr = nullptr;
    cout << endl;

    // Array with initialization
    cout << "5. Array with Initialization (C++11):" << endl;
    int* arr2 = new int[5]{10, 20, 30, 40, 50};

    cout << "Initialized array: ";
    for (int i = 0; i < 5; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;

    delete[] arr2;
    cout << endl;

    // Stack vs Heap
    cout << "6. Stack vs Heap Memory:" << endl;
    int stackVar = 100;             // Stack allocation (automatic)
    int* heapVar = new int(200);    // Heap allocation (manual)

    cout << "Stack variable: " << stackVar << " at " << &stackVar << endl;
    cout << "Heap variable: " << *heapVar << " at " << heapVar << endl;
    cout << endl;

    cout << "Stack variable automatically destroyed at end of scope" << endl;
    cout << "Heap variable must be manually deleted" << endl;

    delete heapVar;
    cout << endl;

    // Memory size
    cout << "7. Memory Size:" << endl;
    int* intArr = new int[100];
    double* doubleArr = new double[100];

    cout << "100 integers: " << (100 * sizeof(int)) << " bytes" << endl;
    cout << "100 doubles: " << (100 * sizeof(double)) << " bytes" << endl;

    delete[] intArr;
    delete[] doubleArr;
    cout << endl;

    // Best practices
    cout << "8. Best Practices:" << endl;
    cout << "- Always delete what you new" << endl;
    cout << "- Use delete[] for arrays" << endl;
    cout << "- Set pointers to nullptr after delete" << endl;
    cout << "- Match new/delete pairs" << endl;
    cout << "- Prefer smart pointers (covered later)" << endl;

    return 0;
}
