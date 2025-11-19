/*
 * Program: Dangling Pointers
 * Description: Understanding and avoiding dangling pointers
 * Compilation: g++ -std=c++17 03_dangling_pointers.cpp -o 03_dangling_pointers
 * Execution: ./03_dangling_pointers
 */

#include <iostream>
using namespace std;

int* returnLocalAddress() {
    int local = 42;
    return &local;  // DANGER! Returning address of local variable
    // local is destroyed when function returns
}

int* returnDynamicMemory() {
    int* ptr = new int(100);
    return ptr;  // OK - dynamic memory persists
    // Caller responsible for deleting
}

void demonstrateDeletedMemory() {
    cout << "Demonstrating deleted memory access:" << endl;

    int* ptr = new int(42);
    cout << "Allocated: " << *ptr << endl;

    delete ptr;
    cout << "Memory deleted" << endl;

    // ptr is now dangling! Points to deallocated memory
    // cout << *ptr << endl;  // DANGER! Undefined behavior

    // SOLUTION: Set to nullptr
    ptr = nullptr;

    if (ptr != nullptr) {
        cout << "Safe to use: " << *ptr << endl;
    } else {
        cout << "Pointer is null, cannot use" << endl;
    }
}

void demonstrateMultiplePointers() {
    cout << "\nMultiple pointers to same memory:" << endl;

    int* ptr1 = new int(50);
    int* ptr2 = ptr1;  // Both point to same memory

    cout << "ptr1: " << *ptr1 << endl;
    cout << "ptr2: " << *ptr2 << endl;

    delete ptr1;
    ptr1 = nullptr;

    // ptr2 is now dangling!
    // cout << *ptr2 << endl;  // DANGER!

    // SOLUTION: Set both to nullptr
    ptr2 = nullptr;

    cout << "Both pointers set to nullptr after delete" << endl;
}

void demonstrateScopeIssues() {
    cout << "\nScope issues:" << endl;

    int* dangling;

    {
        int* local = new int(75);
        dangling = local;
        cout << "Inside scope: " << *dangling << endl;

        delete local;
        // dangling now points to deleted memory
    }

    // dangling is now dangling pointer
    // cout << *dangling << endl;  // DANGER!

    cout << "Pointer became dangling after inner scope" << endl;
}

class Container {
private:
    int* data;
public:
    Container(int value) {
        data = new int(value);
    }

    ~Container() {
        delete data;
        data = nullptr;
    }

    int* getData() {
        return data;  // Returning raw pointer - can become dangling!
    }

    int getValue() {
        return *data;  // Safer - return value instead
    }
};

int main() {
    cout << "=== Dangling Pointers ===" << endl << endl;

    // 1. Deleted memory
    cout << "1. Deleted Memory:" << endl;
    demonstrateDeletedMemory();
    cout << endl;

    // 2. Multiple pointers
    cout << "2. Multiple Pointers to Same Memory:" << endl;
    demonstrateMultiplePointers();
    cout << endl;

    // 3. Scope issues
    cout << "3. Scope Issues:" << endl;
    demonstrateScopeIssues();
    cout << endl;

    // 4. Returning local address
    cout << "4. Returning Local Address (DANGEROUS):" << endl;
    // int* bad = returnLocalAddress();  // NEVER DO THIS!
    // cout << *bad << endl;  // Undefined behavior!
    cout << "Never return pointer to local variable!" << endl << endl;

    // 5. Proper dynamic memory return
    cout << "5. Returning Dynamic Memory (OK):" << endl;
    int* good = returnDynamicMemory();
    cout << "Value: " << *good << endl;
    delete good;
    good = nullptr;
    cout << "Caller must delete returned dynamic memory" << endl << endl;

    // 6. Object destruction
    cout << "6. Object Destruction:" << endl;
    int* external;

    {
        Container container(99);
        external = container.getData();
        cout << "Inside scope: " << *external << endl;
        // When container is destroyed, data is deleted
    }

    // external is now dangling!
    // cout << *external << endl;  // DANGER!
    cout << "Pointer became dangling after object destruction" << endl << endl;

    // 7. Array deletion
    cout << "7. Array Deletion:" << endl;
    int* arr = new int[5]{1, 2, 3, 4, 5};
    int* element = &arr[2];  // Points to third element

    cout << "Element: " << *element << endl;

    delete[] arr;
    // element is now dangling!

    element = nullptr;
    cout << "Element pointer set to nullptr after array deletion" << endl << endl;

    // Prevention strategies
    cout << "8. Prevention Strategies:" << endl;
    cout << "- Always set pointers to nullptr after delete" << endl;
    cout << "- Check for nullptr before dereferencing" << endl;
    cout << "- Don't return pointers to local variables" << endl;
    cout << "- Be careful with multiple pointers to same memory" << endl;
    cout << "- Use smart pointers (unique_ptr, shared_ptr)" << endl;
    cout << "- Follow RAII pattern" << endl;
    cout << "- Return by value or reference when possible" << endl << endl;

    // Safe pattern
    cout << "9. Safe Pattern:" << endl;
    int* safe = new int(42);

    if (safe != nullptr) {
        cout << "Using: " << *safe << endl;
        delete safe;
        safe = nullptr;
    }

    if (safe != nullptr) {
        cout << "Still safe to use" << endl;
    } else {
        cout << "Pointer is null, properly cleaned up" << endl;
    }

    return 0;
}
