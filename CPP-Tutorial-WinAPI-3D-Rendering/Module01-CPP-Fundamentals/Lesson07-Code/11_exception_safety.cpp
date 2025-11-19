/*
 * Program: Exception Safety
 * Description: Exception-safe resource management
 * Compilation: g++ -std=c++17 11_exception_safety.cpp -o 11_exception_safety
 * Execution: ./11_exception_safety
 */

#include <iostream>
#include <stdexcept>
using namespace std;

// UNSAFE: Exception causes leak
void unsafeAllocation() {
    int* arr = new int[100];
    cout << "Allocated array" << endl;

    // Simulate error
    if (true) {
        throw runtime_error("Error occurred!");
    }

    delete[] arr;  // Never reached!
    cout << "Deleted array" << endl;
}

// SAFE: RAII ensures cleanup
class SafeArray {
private:
    int* data;
    int size;
public:
    SafeArray(int s) : size(s) {
        data = new int[size];
        cout << "SafeArray allocated" << endl;
    }

    ~SafeArray() {
        delete[] data;
        cout << "SafeArray destroyed" << endl;
    }
};

void safeAllocation() {
    SafeArray arr(100);
    cout << "Using safe array" << endl;

    if (true) {
        throw runtime_error("Error occurred!");
    }

    // Destructor still called even if exception thrown!
}

int main() {
    cout << "=== Exception Safety ===" << endl << endl;

    // Unsafe
    cout << "1. Unsafe (Memory Leak):" << endl;
    try {
        unsafeAllocation();
    } catch (const exception& e) {
        cout << "Caught: " << e.what() << endl;
        cout << "Memory leaked!" << endl;
    }
    cout << endl;

    // Safe with RAII
    cout << "2. Safe with RAII:" << endl;
    try {
        safeAllocation();
    } catch (const exception& e) {
        cout << "Caught: " << e.what() << endl;
        cout << "Memory automatically cleaned up!" << endl;
    }
    cout << endl;

    cout << "Always use RAII for exception safety!" << endl;

    return 0;
}
