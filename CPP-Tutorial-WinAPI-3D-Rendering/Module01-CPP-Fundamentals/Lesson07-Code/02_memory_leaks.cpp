/*
 * Program: Memory Leaks
 * Description: Demonstrating and preventing memory leaks
 * Compilation: g++ -std=c++17 02_memory_leaks.cpp -o 02_memory_leaks
 * Execution: ./02_memory_leaks
 */

#include <iostream>
using namespace std;

void memoryLeakExample() {
    cout << "Creating memory leak..." << endl;
    int* ptr = new int(100);
    cout << "Allocated memory: " << *ptr << endl;
    // Forgot to delete! Memory leaked!
    // This function ends, ptr is destroyed, but memory still allocated
}

void properCleanup() {
    cout << "Proper memory management..." << endl;
    int* ptr = new int(200);
    cout << "Allocated memory: " << *ptr << endl;
    delete ptr;
    ptr = nullptr;
    cout << "Memory properly deleted" << endl;
}

void reassignmentLeak() {
    cout << "Reassignment causing leak..." << endl;
    int* ptr = new int(10);
    cout << "First allocation: " << *ptr << endl;

    ptr = new int(20);  // LEAK! Lost reference to first allocation
    cout << "Second allocation: " << *ptr << endl;

    delete ptr;  // Only deletes second allocation!
    // First allocation still leaked
}

void properReassignment() {
    cout << "Proper reassignment..." << endl;
    int* ptr = new int(10);
    cout << "First allocation: " << *ptr << endl;

    delete ptr;  // Delete before reassigning

    ptr = new int(20);
    cout << "Second allocation: " << *ptr << endl;

    delete ptr;
    ptr = nullptr;
    cout << "Both allocations properly deleted" << endl;
}

void arrayLeak() {
    cout << "Array memory leak..." << endl;
    int* arr = new int[100];
    cout << "Allocated array of 100 integers" << endl;
    // Forgot to delete[]! Array leaked!
}

void properArrayCleanup() {
    cout << "Proper array cleanup..." << endl;
    int* arr = new int[100];
    cout << "Allocated array of 100 integers" << endl;
    delete[] arr;
    arr = nullptr;
    cout << "Array properly deleted" << endl;
}

class ResourceHolder {
private:
    int* data;
public:
    ResourceHolder(int size) {
        data = new int[size];
        cout << "ResourceHolder: allocated array of " << size << endl;
    }

    ~ResourceHolder() {
        delete[] data;
        cout << "ResourceHolder: deallocated array" << endl;
    }
};

int main() {
    cout << "=== Memory Leaks ===" << endl << endl;

    // Example 1: Function scope leak
    cout << "1. Function Scope Leak:" << endl;
    memoryLeakExample();
    cout << "Memory leaked - still allocated but inaccessible!" << endl << endl;

    // Example 2: Proper cleanup
    cout << "2. Proper Cleanup:" << endl;
    properCleanup();
    cout << endl;

    // Example 3: Reassignment leak
    cout << "3. Reassignment Leak:" << endl;
    reassignmentLeak();
    cout << endl;

    // Example 4: Proper reassignment
    cout << "4. Proper Reassignment:" << endl;
    properReassignment();
    cout << endl;

    // Example 5: Array leak
    cout << "5. Array Leak:" << endl;
    arrayLeak();
    cout << endl;

    // Example 6: Proper array cleanup
    cout << "6. Proper Array Cleanup:" << endl;
    properArrayCleanup();
    cout << endl;

    // Example 7: Exception causing leak
    cout << "7. Exception Safety:" << endl;
    try {
        int* ptr = new int(42);
        cout << "Memory allocated" << endl;

        // If exception thrown here, memory leaks!
        // throw runtime_error("Error!");

        delete ptr;  // This line might not execute if exception thrown
        cout << "Memory deleted" << endl;
    } catch (...) {
        cout << "Exception caught - memory may have leaked!" << endl;
    }
    cout << endl;

    // Example 8: RAII - Resource Acquisition Is Initialization
    cout << "8. RAII Pattern (Automatic Cleanup):" << endl;
    {
        ResourceHolder holder(10);
        cout << "Using resource..." << endl;
        // Destructor automatically called when holder goes out of scope
    }
    cout << "Resource automatically cleaned up!" << endl << endl;

    // Example 9: Loop allocation leak
    cout << "9. Loop Allocation Leak:" << endl;
    cout << "BAD: Allocating in loop without tracking" << endl;
    // for (int i = 0; i < 10; i++) {
    //     int* leak = new int(i);  // 10 allocations, all leaked!
    // }

    cout << "GOOD: Allocating and immediately deallocating" << endl;
    for (int i = 0; i < 3; i++) {
        int* temp = new int(i);
        cout << "  Allocated and used: " << *temp << endl;
        delete temp;
    }
    cout << endl;

    // Tools for detecting leaks
    cout << "10. Detecting Memory Leaks:" << endl;
    cout << "Tools:" << endl;
    cout << "- Valgrind (Linux): valgrind --leak-check=full ./program" << endl;
    cout << "- Dr. Memory (Windows/Linux)" << endl;
    cout << "- AddressSanitizer: g++ -fsanitize=address" << endl;
    cout << "- Visual Studio Memory Profiler" << endl;
    cout << endl;

    cout << "Prevention:" << endl;
    cout << "- Always match new with delete" << endl;
    cout << "- Use smart pointers (unique_ptr, shared_ptr)" << endl;
    cout << "- Follow RAII pattern" << endl;
    cout << "- Use standard containers (vector, string)" << endl;

    return 0;
}
