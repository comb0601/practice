/*
 * Program: Default and Deleted Constructors
 * Description: =default and =delete (C++11)
 * Compilation: g++ -std=c++17 12_default_delete.cpp -o 12_default_delete
 * Execution: ./12_default_delete
 */

#include <iostream>
using namespace std;

class NonCopyable {
private:
    int* data;

public:
    NonCopyable() : data(new int(42)) {
        cout << "Constructor" << endl;
    }

    ~NonCopyable() {
        delete data;
        cout << "Destructor" << endl;
    }

    // Delete copy constructor
    NonCopyable(const NonCopyable&) = delete;

    // Delete copy assignment
    NonCopyable& operator=(const NonCopyable&) = delete;

    int getValue() const { return *data; }
};

class Singleton {
private:
    static Singleton* instance;

    Singleton() {
        cout << "Singleton created" << endl;
    }

public:
    // Delete copy operations
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton* getInstance() {
        if (!instance) {
            instance = new Singleton();
        }
        return instance;
    }
};

Singleton* Singleton::instance = nullptr;

int main() {
    cout << "=== Default and Deleted Constructors ===" << endl << endl;

    NonCopyable obj1;
    // NonCopyable obj2 = obj1;  // ERROR - copy deleted
    cout << "Value: " << obj1.getValue() << endl;

    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();
    cout << "Same instance: " << (s1 == s2 ? "Yes" : "No") << endl;

    return 0;
}
