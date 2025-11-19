/*
 * Program: Smart Pointers Preview
 * Description: Introduction to modern C++ smart pointers
 * Compilation: g++ -std=c++17 07_smart_pointers_preview.cpp -o 07_smart_pointers_preview
 * Execution: ./07_smart_pointers_preview
 */

#include <iostream>
#include <memory>  // For smart pointers
using namespace std;

class Resource {
public:
    Resource() { cout << "Resource acquired" << endl; }
    ~Resource() { cout << "Resource released" << endl; }
    void use() { cout << "Using resource" << endl; }
};

int main() {
    cout << "=== Smart Pointers Preview ===" << endl << endl;

    // Raw pointer (manual management)
    cout << "1. Raw Pointer (Manual Management):" << endl;
    {
        Resource* raw = new Resource();
        raw->use();
        delete raw;  // Must remember to delete!
    }
    cout << endl;

    // unique_ptr (exclusive ownership)
    cout << "2. unique_ptr (Exclusive Ownership):" << endl;
    {
        unique_ptr<Resource> ptr(new Resource());
        ptr->use();
        // Automatically deleted when ptr goes out of scope!
    }
    cout << "Automatically deleted!" << endl << endl;

    // unique_ptr with make_unique (C++14)
    cout << "3. make_unique (Preferred Way):" << endl;
    {
        auto ptr = make_unique<Resource>();
        ptr->use();
    }
    cout << "Automatically deleted!" << endl << endl;

    // shared_ptr (shared ownership)
    cout << "4. shared_ptr (Shared Ownership):" << endl;
    {
        shared_ptr<Resource> ptr1 = make_shared<Resource>();
        cout << "Use count: " << ptr1.use_count() << endl;

        {
            shared_ptr<Resource> ptr2 = ptr1;
            cout << "Use count: " << ptr1.use_count() << endl;
            ptr2->use();
        }

        cout << "Use count: " << ptr1.use_count() << endl;
    }
    cout << "Deleted when last shared_ptr destroyed!" << endl << endl;

    // Benefits
    cout << "5. Smart Pointer Benefits:" << endl;
    cout << "- Automatic memory management" << endl;
    cout << "- Exception safe" << endl;
    cout << "- No manual delete needed" << endl;
    cout << "- Prevents memory leaks" << endl;
    cout << "- Clear ownership semantics" << endl;

    return 0;
}
