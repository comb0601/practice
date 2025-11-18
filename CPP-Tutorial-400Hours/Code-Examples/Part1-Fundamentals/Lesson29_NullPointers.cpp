/*
 * Lesson 29: Null Pointers and nullptr (C++11)
 * Compile: cl /std:c++17 Lesson29_NullPointers.cpp
 */

#include <iostream>
using namespace std;

void processPointer(int *ptr) {
    if (ptr != nullptr) {
        cout << "Valid pointer: " << *ptr << endl;
    } else {
        cout << "Null pointer received" << endl;
    }
}

int* findElement(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return &arr[i];
        }
    }
    return nullptr;  // Not found
}

int main() {
    cout << "=== Null Pointers Demo ===" << endl << endl;

    // NULLPTR (C++11)
    cout << "=== NULLPTR ===" << endl;
    int *ptr = nullptr;
    cout << "Pointer value: " << ptr << endl;
    cout << "Is null? " << (ptr == nullptr ? "Yes" : "No") << endl << endl;

    // CHECKING BEFORE USE
    cout << "=== SAFE POINTER USE ===" << endl;
    int value = 42;
    int *safePtr = &value;

    if (safePtr != nullptr) {
        cout << "Safe to dereference: " << *safePtr << endl;
    }
    cout << endl;

    // FUNCTION WITH NULL CHECK
    cout << "=== FUNCTION WITH NULL CHECK ===" << endl;
    processPointer(&value);
    processPointer(nullptr);
    cout << endl;

    // RETURNING NULLPTR
    cout << "=== RETURNING NULLPTR ===" << endl;
    int arr[] = {10, 20, 30, 40, 50};

    int *found = findElement(arr, 5, 30);
    if (found != nullptr) {
        cout << "Found: " << *found << endl;
    }

    found = findElement(arr, 5, 99);
    if (found == nullptr) {
        cout << "Element not found" << endl;
    }
    cout << endl;

    // AVOIDING DANGLING POINTERS
    cout << "=== DANGLING POINTER PREVENTION ===" << endl;
    int *dynPtr = new int(100);
    cout << "Before delete: " << *dynPtr << endl;
    delete dynPtr;
    dynPtr = nullptr;  // Set to null after delete
    cout << "After delete and null: pointer is safe" << endl;

    return 0;
}
