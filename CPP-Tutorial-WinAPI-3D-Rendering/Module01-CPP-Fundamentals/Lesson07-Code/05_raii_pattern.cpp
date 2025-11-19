/*
 * Program: RAII Pattern
 * Description: Resource Acquisition Is Initialization - automatic resource management
 * Compilation: g++ -std=c++17 05_raii_pattern.cpp -o 05_raii_pattern
 * Execution: ./05_raii_pattern
 */

#include <iostream>
#include <fstream>
using namespace std;

// Bad: Manual resource management
void badResourceManagement() {
    cout << "BAD: Manual management" << endl;

    int* data = new int[100];
    cout << "  Allocated memory" << endl;

    // If exception thrown here, memory leaks!
    // Do something...

    delete[] data;  // Might not execute if exception thrown
    cout << "  Deleted memory" << endl;
}

// Good: RAII - Resource Acquisition Is Initialization
class IntArray {
private:
    int* data;
    int size;

public:
    IntArray(int s) : size(s) {
        data = new int[size];
        cout << "  IntArray: Acquired resource (allocated " << size << " ints)" << endl;
    }

    ~IntArray() {
        delete[] data;
        cout << "  IntArray: Released resource (deleted array)" << endl;
    }

    int& operator[](int index) {
        return data[index];
    }

    int getSize() const { return size; }
};

// RAII with file handling
class File {
private:
    ofstream file;
    string filename;

public:
    File(const string& fname) : filename(fname) {
        file.open(filename);
        if (file.is_open()) {
            cout << "  File: Opened " << filename << endl;
        }
    }

    ~File() {
        if (file.is_open()) {
            file.close();
            cout << "  File: Closed " << filename << endl;
        }
    }

    void write(const string& text) {
        if (file.is_open()) {
            file << text << endl;
        }
    }
};

// Generic RAII wrapper
template<typename T>
class ResourceGuard {
private:
    T* resource;

public:
    ResourceGuard(T* res) : resource(res) {
        cout << "  ResourceGuard: Acquired resource" << endl;
    }

    ~ResourceGuard() {
        delete resource;
        cout << "  ResourceGuard: Released resource" << endl;
    }

    T* get() { return resource; }
    T& operator*() { return *resource; }
    T* operator->() { return resource; }

    // Prevent copying
    ResourceGuard(const ResourceGuard&) = delete;
    ResourceGuard& operator=(const ResourceGuard&) = delete;
};

// Multi-resource RAII
class MultiResource {
private:
    int* data1;
    int* data2;
    int* data3;

public:
    MultiResource() {
        cout << "  MultiResource: Allocating resources..." << endl;
        data1 = new int[10];
        cout << "    Allocated data1" << endl;
        data2 = new int[20];
        cout << "    Allocated data2" << endl;
        data3 = new int[30];
        cout << "    Allocated data3" << endl;
    }

    ~MultiResource() {
        cout << "  MultiResource: Deallocating resources..." << endl;
        delete[] data3;
        cout << "    Deleted data3" << endl;
        delete[] data2;
        cout << "    Deleted data2" << endl;
        delete[] data1;
        cout << "    Deleted data1" << endl;
    }
};

int main() {
    cout << "=== RAII Pattern ===" << endl << endl;

    // Example 1: Basic RAII
    cout << "1. Basic RAII:" << endl;
    {
        IntArray arr(10);
        arr[0] = 42;
        arr[1] = 100;
        cout << "  Using array: arr[0] = " << arr[0] << endl;
        // Destructor automatically called when arr goes out of scope
    }
    cout << "  Scope ended - resource automatically released!" << endl << endl;

    // Example 2: RAII with exceptions
    cout << "2. RAII with Exception Safety:" << endl;
    try {
        IntArray arr(5);
        arr[0] = 1;
        cout << "  Working with array..." << endl;

        // throw runtime_error("Error!");  // Even if exception thrown...

        cout << "  No exception thrown" << endl;
        // Destructor still called when leaving scope
    } catch (...) {
        cout << "  Exception caught" << endl;
    }
    cout << "  Resource still cleaned up!" << endl << endl;

    // Example 3: File RAII
    cout << "3. File RAII:" << endl;
    {
        File f("test.txt");
        f.write("Hello, RAII!");
        f.write("File automatically closed");
        // File automatically closed when f goes out of scope
    }
    cout << "  File automatically closed!" << endl << endl;

    // Example 4: Generic RAII wrapper
    cout << "4. Generic RAII Wrapper:" << endl;
    {
        ResourceGuard<int> guard(new int(42));
        cout << "  Value: " << *guard << endl;
        // Automatically deleted when guard goes out of scope
    }
    cout << "  Resource automatically released!" << endl << endl;

    // Example 5: Multiple resources
    cout << "5. Multiple Resources:" << endl;
    {
        MultiResource mr;
        cout << "  Using resources..." << endl;
        // All resources automatically cleaned up
    }
    cout << "  All resources automatically released!" << endl << endl;

    // Example 6: Nested RAII
    cout << "6. Nested RAII:" << endl;
    {
        cout << "  Outer scope" << endl;
        IntArray outer(5);
        {
            cout << "  Inner scope" << endl;
            IntArray inner(3);
            cout << "  Using both arrays" << endl;
        }
        cout << "  Inner destructed, outer still alive" << endl;
    }
    cout << "  Both destructed!" << endl << endl;

    // Benefits
    cout << "7. RAII Benefits:" << endl;
    cout << "- Automatic resource management" << endl;
    cout << "- Exception safe" << endl;
    cout << "- No manual cleanup needed" << endl;
    cout << "- Deterministic destruction" << endl;
    cout << "- Prevents resource leaks" << endl;
    cout << "- Clear ownership" << endl << endl;

    // Standard library RAII examples
    cout << "8. Standard Library RAII:" << endl;
    cout << "- std::unique_ptr, std::shared_ptr (smart pointers)" << endl;
    cout << "- std::vector, std::string (containers)" << endl;
    cout << "- std::fstream (file streams)" << endl;
    cout << "- std::lock_guard, std::unique_lock (mutexes)" << endl;
    cout << "- std::thread (threads)" << endl;

    return 0;
}
