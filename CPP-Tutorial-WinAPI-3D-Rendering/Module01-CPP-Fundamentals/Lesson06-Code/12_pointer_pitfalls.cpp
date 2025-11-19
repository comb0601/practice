/*
 * Program: Pointer Pitfalls
 * Description: Common pointer mistakes and how to avoid them
 * Compilation: g++ -std=c++17 12_pointer_pitfalls.cpp -o 12_pointer_pitfalls
 * Execution: ./12_pointer_pitfalls
 */

#include <iostream>
using namespace std;

void demonstrateDanglingPointer() {
    cout << "1. Dangling Pointer:" << endl;

    int* ptr = new int(42);
    cout << "Allocated memory, value: " << *ptr << endl;

    delete ptr;
    cout << "Memory deleted" << endl;

    // ptr is now a dangling pointer!
    // cout << *ptr << endl;  // DANGEROUS - undefined behavior

    // SOLUTION: Set to nullptr after delete
    ptr = nullptr;

    if (ptr != nullptr) {
        cout << "Safe to use: " << *ptr << endl;
    } else {
        cout << "Pointer is null, cannot dereference" << endl;
    }
    cout << endl;
}

void demonstrateUninitializedPointer() {
    cout << "2. Uninitialized Pointer:" << endl;

    // WRONG: Uninitialized pointer
    // int* badPtr;  // Contains garbage address
    // *badPtr = 100;  // CRASH - writing to random memory!

    // RIGHT: Initialize pointers
    int* goodPtr = nullptr;  // Safe initialization

    if (goodPtr != nullptr) {
        *goodPtr = 100;
    } else {
        cout << "Pointer not initialized, allocating memory" << endl;
        goodPtr = new int(100);
        cout << "Value: " << *goodPtr << endl;
        delete goodPtr;
    }
    cout << endl;
}

void demonstrateMemoryLeak() {
    cout << "3. Memory Leak:" << endl;

    // WRONG: Memory leak
    int* ptr1 = new int(10);
    cout << "Allocated ptr1, value: " << *ptr1 << endl;

    ptr1 = new int(20);  // Lost reference to first allocation!
    cout << "Allocated again, value: " << *ptr1 << endl;
    // First allocation leaked!

    // RIGHT: Delete before reassigning
    delete ptr1;

    ptr1 = new int(30);
    cout << "Properly managed, value: " << *ptr1 << endl;

    delete ptr1;
    cout << "Cleaned up properly" << endl << endl;
}

void demonstrateWildPointer() {
    cout << "4. Wild Pointer:" << endl;

    int x = 42;
    int* ptr = &x;

    cout << "Valid pointer, value: " << *ptr << endl;

    // Pointer becomes wild when x goes out of scope
    // In this case, x is still in scope, so it's safe

    // DANGEROUS: Returning pointer to local variable
    // int* dangerousFunction() {
    //     int local = 100;
    //     return &local;  // local destroyed, pointer becomes wild!
    // }

    cout << "Avoid returning pointers to local variables!" << endl << endl;
}

int* createArray(int size) {
    // CORRECT: Dynamically allocated memory persists
    return new int[size];
}

void demonstrateArrayDeleteMismatch() {
    cout << "5. Array Delete Mismatch:" << endl;

    // Single object
    int* single = new int(42);
    cout << "Single object allocated" << endl;

    // WRONG: Using delete[] on single object
    // delete[] single;  // Undefined behavior!

    // RIGHT: Use delete for single object
    delete single;
    cout << "Single object deleted with delete" << endl;

    // Array
    int* arr = new int[5];
    cout << "Array allocated" << endl;

    // WRONG: Using delete on array
    // delete arr;  // Memory leak, undefined behavior!

    // RIGHT: Use delete[] for arrays
    delete[] arr;
    cout << "Array deleted with delete[]" << endl << endl;
}

void demonstrateDoubleDelete() {
    cout << "6. Double Delete:" << endl;

    int* ptr = new int(100);
    cout << "Memory allocated" << endl;

    delete ptr;
    cout << "Memory deleted once" << endl;

    // WRONG: Deleting again
    // delete ptr;  // CRASH - double delete!

    // SOLUTION: Set to nullptr after delete
    ptr = nullptr;

    delete ptr;  // Safe - deleting nullptr is OK
    cout << "Safe to delete nullptr" << endl << endl;
}

void demonstratePointerArithmeticOverflow() {
    cout << "7. Pointer Arithmetic Overflow:" << endl;

    int arr[5] = {1, 2, 3, 4, 5};
    int* ptr = arr;

    cout << "Array: ";
    for (int i = 0; i < 5; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // DANGEROUS: Accessing beyond array bounds
    // cout << *(ptr + 10) << endl;  // Undefined behavior!

    // SAFE: Check bounds
    int index = 2;
    if (index >= 0 && index < 5) {
        cout << "Safe access arr[" << index << "] = " << *(ptr + index) << endl;
    }
    cout << endl;
}

void demonstrateConstCasting() {
    cout << "8. Modifying Const Data:" << endl;

    const int value = 42;
    const int* ptr = &value;

    cout << "Const value: " << *ptr << endl;

    // WRONG: Casting away const
    // int* badPtr = const_cast<int*>(ptr);
    // *badPtr = 100;  // Undefined behavior!

    cout << "Don't cast away const and modify!" << endl << endl;
}

int main() {
    cout << "=== Common Pointer Pitfalls ===" << endl << endl;

    demonstrateDanglingPointer();
    demonstrateUninitializedPointer();
    demonstrateMemoryLeak();
    demonstrateWildPointer();
    demonstrateArrayDeleteMismatch();
    demonstrateDoubleDelete();
    demonstratePointerArithmeticOverflow();
    demonstrateConstCasting();

    cout << "=== Best Practices ===" << endl;
    cout << "1. Always initialize pointers (nullptr or valid address)" << endl;
    cout << "2. Set pointers to nullptr after delete" << endl;
    cout << "3. Check for nullptr before dereferencing" << endl;
    cout << "4. Match new/delete and new[]/delete[]" << endl;
    cout << "5. Avoid returning pointers to local variables" << endl;
    cout << "6. Check array bounds" << endl;
    cout << "7. Use smart pointers (covered later)" << endl;
    cout << "8. Don't cast away const" << endl;

    return 0;
}
