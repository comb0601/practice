/*
 * Program: Memory Management Best Practices
 * Description: Summary of memory management best practices
 * Compilation: g++ -std=c++17 15_memory_best_practices.cpp -o 15_memory_best_practices
 * Execution: ./15_memory_best_practices
 */

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// BAD: Manual memory management
class BadString {
    char* data;
public:
    BadString(const char* str) {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }
    // Missing destructor - leak!
    // Missing copy constructor - shallow copy!
    // Missing copy assignment - shallow copy!
};

// GOOD: RAII with rule of 5
class GoodString {
    char* data;
public:
    GoodString(const char* str) {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    ~GoodString() {
        delete[] data;
    }

    GoodString(const GoodString& other) {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
    }

    GoodString& operator=(const GoodString& other) {
        if (this != &other) {
            delete[] data;
            data = new char[strlen(other.data) + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    GoodString(GoodString&& other) noexcept : data(other.data) {
        other.data = nullptr;
    }

    GoodString& operator=(GoodString&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
};

// BEST: Use standard library
void bestPractice() {
    string str = "Use std::string!";  // Automatic memory management
    vector<int> vec = {1, 2, 3, 4, 5};  // Automatic memory management
    auto ptr = make_unique<int>(42);  // Smart pointer
}

int main() {
    cout << "=== Memory Management Best Practices ===" << endl << endl;

    cout << "1. PREFER AUTOMATIC MEMORY:" << endl;
    cout << "   - Use stack when possible" << endl;
    cout << "   - Use std::vector instead of arrays" << endl;
    cout << "   - Use std::string instead of char*" << endl << endl;

    cout << "2. USE SMART POINTERS:" << endl;
    cout << "   - unique_ptr for exclusive ownership" << endl;
    cout << "   - shared_ptr for shared ownership" << endl;
    cout << "   - Avoid raw new/delete" << endl << endl;

    cout << "3. FOLLOW RAII:" << endl;
    cout << "   - Resource Acquisition Is Initialization" << endl;
    cout << "   - Constructor acquires, destructor releases" << endl;
    cout << "   - Automatic cleanup" << endl << endl;

    cout << "4. RULE OF 5:" << endl;
    cout << "   - If you define destructor, define copy/move" << endl;
    cout << "   - Destructor, copy constructor, copy assignment" << endl;
    cout << "   - Move constructor, move assignment" << endl << endl;

    cout << "5. AVOID MEMORY LEAKS:" << endl;
    cout << "   - Every new needs a delete" << endl;
    cout << "   - Use delete[] for arrays" << endl;
    cout << "   - Set pointers to nullptr after delete" << endl;
    cout << "   - Use tools: valgrind, ASan" << endl << endl;

    cout << "6. PREVENT DANGLING POINTERS:" << endl;
    cout << "   - Don't return pointers to locals" << endl;
    cout << "   - Set to nullptr after delete" << endl;
    cout << "   - Check for nullptr before use" << endl << endl;

    cout << "7. EXCEPTION SAFETY:" << endl;
    cout << "   - Use RAII for cleanup" << endl;
    cout << "   - Don't leak on exceptions" << endl;
    cout << "   - Smart pointers are exception-safe" << endl << endl;

    cout << "8. PERFORMANCE:" << endl;
    cout << "   - Prefer stack over heap when possible" << endl;
    cout << "   - Minimize allocations" << endl;
    cout << "   - Reuse memory when appropriate" << endl;
    cout << "   - Use move semantics" << endl << endl;

    cout << "9. PREFER STANDARD LIBRARY:" << endl;
    cout << "   - vector, string, array" << endl;
    cout << "   - unique_ptr, shared_ptr" << endl;
    cout << "   - Well-tested and optimized" << endl << endl;

    cout << "10. MODERN C++ FEATURES:" << endl;
    cout << "   - make_unique, make_shared" << endl;
    cout << "   - Move semantics (&&)" << endl;
    cout << "   - std::optional for nullable values" << endl;
    cout << "   - Structured bindings" << endl;

    return 0;
}
