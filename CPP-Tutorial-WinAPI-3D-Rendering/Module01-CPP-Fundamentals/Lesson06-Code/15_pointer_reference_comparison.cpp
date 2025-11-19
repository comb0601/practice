/*
 * Program: Pointer vs Reference Comparison
 * Description: Comprehensive comparison between pointers and references
 * Compilation: g++ -std=c++17 15_pointer_reference_comparison.cpp -o 15_pointer_reference_comparison
 * Execution: ./15_pointer_reference_comparison
 */

#include <iostream>
using namespace std;

// Functions demonstrating differences
void modifyByPointer(int* ptr) {
    if (ptr != nullptr) {
        *ptr = 100;
    }
}

void modifyByReference(int& ref) {
    ref = 200;
}

// Return by pointer
int* getPointerToValue() {
    static int value = 42;  // Static so it persists
    return &value;
}

// Return by reference
int& getReferenceToValue() {
    static int value = 99;
    return value;
}

// Pointer can be reassigned
void demonstrateReassignment(int* ptr, int& ref, int a, int b) {
    cout << "Initial pointer points to: " << *ptr << endl;
    cout << "Initial reference refers to: " << ref << endl;

    ptr = &b;  // Can reassign pointer
    cout << "After reassigning pointer: " << *ptr << endl;

    // ref = b;  // This copies b's value to what ref refers to!
    ref = b;
    cout << "After ref = b (value copy): ref = " << ref << endl;
}

int main() {
    cout << "=== Pointer vs Reference Comparison ===" << endl << endl;

    int x = 10;
    int y = 20;

    // 1. Declaration and initialization
    cout << "1. Declaration and Initialization:" << endl;
    cout << "POINTER:" << endl;
    int* ptr;              // Can be declared without initialization (dangerous!)
    ptr = &x;              // Can be initialized later
    ptr = nullptr;         // Can be null
    ptr = &y;              // Can be reassigned

    cout << "  Pointer can be: uninitialized, null, reassigned" << endl;

    cout << "\nREFERENCE:" << endl;
    int& ref = x;          // MUST be initialized at declaration
    // int& ref2;          // ERROR - must be initialized
    // ref = y;            // This assigns y's value to x (not rebinding!)
    // ref = nullptr;      // ERROR - cannot be null

    cout << "  Reference must be: initialized, cannot be null, cannot be rebound" << endl << endl;

    // 2. Syntax
    cout << "2. Syntax:" << endl;
    cout << "POINTER:" << endl;
    int* p = &x;
    cout << "  Value through pointer: *p = " << *p << endl;
    cout << "  Dereference needed: *" << endl;

    cout << "\nREFERENCE:" << endl;
    int& r = x;
    cout << "  Value through reference: r = " << r << endl;
    cout << "  No dereference needed (cleaner syntax)" << endl << endl;

    // 3. Null check
    cout << "3. Null Check:" << endl;
    cout << "POINTER:" << endl;
    int* nullPtr = nullptr;
    if (nullPtr != nullptr) {
        cout << "  Safe to use" << endl;
    } else {
        cout << "  Pointer is null - must check!" << endl;
    }

    cout << "\nREFERENCE:" << endl;
    cout << "  References cannot be null" << endl;
    cout << "  No need to check - always valid" << endl << endl;

    // 4. Reassignment
    cout << "4. Reassignment:" << endl;
    int a = 5, b = 15;

    cout << "POINTER:" << endl;
    demonstrateReassignment(&a, ref, a, b);

    cout << "\nREFERENCE:" << endl;
    cout << "  Cannot rebind to different variable" << endl;
    cout << "  Assignment copies value, not reference" << endl << endl;

    // 5. Function parameters
    cout << "5. Function Parameters:" << endl;
    int value1 = 50;
    int value2 = 75;

    cout << "POINTER:" << endl;
    cout << "  Before: " << value1 << endl;
    modifyByPointer(&value1);
    cout << "  After:  " << value1 << endl;
    cout << "  Call: modifyByPointer(&value1)" << endl;
    cout << "  Can pass nullptr" << endl;

    cout << "\nREFERENCE:" << endl;
    cout << "  Before: " << value2 << endl;
    modifyByReference(value2);
    cout << "  After:  " << value2 << endl;
    cout << "  Call: modifyByReference(value2)" << endl;
    cout << "  Cleaner syntax, no & or * needed" << endl << endl;

    // 6. Return values
    cout << "6. Return Values:" << endl;
    cout << "POINTER:" << endl;
    int* ptrResult = getPointerToValue();
    cout << "  Returned pointer points to: " << *ptrResult << endl;
    cout << "  Can return nullptr to indicate failure" << endl;

    cout << "\nREFERENCE:" << endl;
    int& refResult = getReferenceToValue();
    cout << "  Returned reference refers to: " << refResult << endl;
    cout << "  Cannot return null (always valid)" << endl << endl;

    // 7. Arrays
    cout << "7. Arrays:" << endl;
    int arr[] = {1, 2, 3, 4, 5};

    cout << "POINTER:" << endl;
    int* arrPtr = arr;
    cout << "  Can use pointer arithmetic: *(arrPtr + 2) = " << *(arrPtr + 2) << endl;
    arrPtr++;
    cout << "  After increment: *arrPtr = " << *arrPtr << endl;

    cout << "\nREFERENCE:" << endl;
    int& arrRef = arr[0];
    cout << "  References to individual elements: arrRef = " << arrRef << endl;
    cout << "  No pointer arithmetic" << endl << endl;

    // 8. Memory and indirection
    cout << "8. Memory and Indirection:" << endl;
    cout << "POINTER:" << endl;
    cout << "  Pointer itself has an address and takes memory" << endl;
    cout << "  Size of pointer: " << sizeof(int*) << " bytes" << endl;
    cout << "  Multiple levels: int**, int***, etc." << endl;

    cout << "\nREFERENCE:" << endl;
    cout << "  Reference is an alias (no separate memory)" << endl;
    cout << "  Size: " << sizeof(int&) << " bytes (same as int)" << endl;
    cout << "  No multi-level references (int&& is rvalue reference)" << endl << endl;

    // 9. When to use
    cout << "9. When to Use:" << endl;
    cout << "POINTER:" << endl;
    cout << "  - Need to reassign to different objects" << endl;
    cout << "  - Need null/optional value" << endl;
    cout << "  - Dynamic memory allocation" << endl;
    cout << "  - Pointer arithmetic (arrays)" << endl;
    cout << "  - Data structures (linked lists, trees)" << endl;

    cout << "\nREFERENCE:" << endl;
    cout << "  - Function parameters (avoid copying)" << endl;
    cout << "  - Operator overloading" << endl;
    cout << "  - Range-based for loops" << endl;
    cout << "  - Return multiple values" << endl;
    cout << "  - Cleaner syntax when null not needed" << endl << endl;

    // 10. Summary table
    cout << "10. Summary:" << endl;
    cout << "Feature             Pointer         Reference" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Null allowed        YES             NO" << endl;
    cout << "Reassignment        YES             NO" << endl;
    cout << "Initialization      Optional        Required" << endl;
    cout << "Dereference         *ptr            ref" << endl;
    cout << "Address-of          &var            &var" << endl;
    cout << "Memory              Yes             Alias" << endl;
    cout << "Arithmetic          Yes             No" << endl;
    cout << "Multi-level         Yes             No" << endl;

    return 0;
}
