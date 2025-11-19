/*
 * Program: Null Pointers
 * Description: Working with null pointers and nullptr
 * Compilation: g++ -std=c++17 06_null_pointers.cpp -o 06_null_pointers
 * Execution: ./06_null_pointers
 */

#include <iostream>
using namespace std;

void processValue(int* ptr) {
    if (ptr != nullptr) {
        cout << "Processing value: " << *ptr << endl;
        *ptr *= 2;
        cout << "After doubling: " << *ptr << endl;
    } else {
        cout << "Null pointer - cannot process!" << endl;
    }
}

int* findValue(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return &arr[i];  // Return pointer to found element
        }
    }
    return nullptr;  // Not found
}

int main() {
    cout << "=== Null Pointers ===" << endl << endl;

    // Null pointer initialization
    cout << "1. Null Pointer Initialization:" << endl;
    int* ptr1 = nullptr;  // Modern C++ (C++11 and later)
    int* ptr2 = NULL;     // Old C-style (still works)
    int* ptr3 = 0;        // Also works but not recommended

    cout << "ptr1 (nullptr): " << ptr1 << endl;
    cout << "ptr2 (NULL): " << ptr2 << endl;
    cout << "ptr3 (0): " << ptr3 << endl << endl;

    // Check if pointer is null
    cout << "2. Checking for Null:" << endl;
    if (ptr1 == nullptr) {
        cout << "ptr1 is null" << endl;
    }

    if (!ptr1) {  // Also works
        cout << "ptr1 is null (using !ptr1)" << endl;
    }

    int x = 42;
    int* ptr4 = &x;

    if (ptr4 != nullptr) {
        cout << "ptr4 is not null, points to: " << *ptr4 << endl;
    }
    cout << endl;

    // Safe dereferencing
    cout << "3. Safe Dereferencing:" << endl;
    int* safePtr = nullptr;

    cout << "Attempting to process null pointer:" << endl;
    processValue(safePtr);

    cout << "\nProcessing valid pointer:" << endl;
    safePtr = &x;
    processValue(safePtr);
    cout << "x is now: " << x << endl << endl;

    // Returning null from functions
    cout << "4. Returning Null from Functions:" << endl;
    int numbers[] = {10, 20, 30, 40, 50};
    int size = 5;

    int* found = findValue(numbers, size, 30);
    if (found != nullptr) {
        cout << "Found 30 at address " << found << ", value: " << *found << endl;
    } else {
        cout << "30 not found" << endl;
    }

    found = findValue(numbers, size, 100);
    if (found != nullptr) {
        cout << "Found 100 at address " << found << ", value: " << *found << endl;
    } else {
        cout << "100 not found" << endl;
    }
    cout << endl;

    // Null pointer pitfalls
    cout << "5. Common Null Pointer Mistakes:" << endl;
    int* dangerous = nullptr;

    // DANGEROUS - dereferencing null pointer!
    // cout << *dangerous << endl;  // Would crash!

    // SAFE - check before dereferencing
    if (dangerous != nullptr) {
        cout << *dangerous << endl;
    } else {
        cout << "Cannot dereference null pointer!" << endl;
    }
    cout << endl;

    // Pointer initialization best practices
    cout << "6. Best Practices:" << endl;
    cout << "- Always initialize pointers (to nullptr or valid address)" << endl;
    cout << "- Check for nullptr before dereferencing" << endl;
    cout << "- Use nullptr (not NULL or 0) in modern C++" << endl;
    cout << "- Set pointers to nullptr after delete" << endl;
    cout << endl;

    // Optional pointer
    cout << "7. Optional Pointer Pattern:" << endl;
    int value = 100;
    int* optionalPtr = nullptr;

    cout << "optionalPtr is ";
    if (optionalPtr) {
        cout << "set to: " << *optionalPtr << endl;
    } else {
        cout << "not set (nullptr)" << endl;
    }

    optionalPtr = &value;

    cout << "After assignment, optionalPtr is ";
    if (optionalPtr) {
        cout << "set to: " << *optionalPtr << endl;
    } else {
        cout << "not set (nullptr)" << endl;
    }

    return 0;
}
