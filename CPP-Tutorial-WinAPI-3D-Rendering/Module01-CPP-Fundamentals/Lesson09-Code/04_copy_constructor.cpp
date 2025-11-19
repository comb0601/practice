/*
 * Program: Copy Constructor
 * Description: Copy constructor for deep and shallow copy
 * Compilation: g++ -std=c++17 04_copy_constructor.cpp -o 04_copy_constructor
 * Execution: ./04_copy_constructor
 */

#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    char* data;
    int length;

public:
    // Constructor
    String(const char* str) {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        cout << "Constructor: created \"" << data << "\"" << endl;
    }

    // Copy constructor (deep copy)
    String(const String& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        cout << "Copy constructor: copied \"" << data << "\"" << endl;
    }

    // Destructor
    ~String() {
        cout << "Destructor: deleting \"" << data << "\"" << endl;
        delete[] data;
    }

    void display() const {
        cout << "String: \"" << data << "\"" << endl;
    }

    void modify(char ch) {
        if (length > 0) {
            data[0] = ch;
        }
    }
};

int main() {
    cout << "=== Copy Constructor ===" << endl << endl;

    cout << "Creating s1:" << endl;
    String s1("Hello");
    s1.display();

    cout << "\nCopying s1 to s2:" << endl;
    String s2 = s1;  // Copy constructor called
    s2.display();

    cout << "\nModifying s2:" << endl;
    s2.modify('J');
    s1.display();
    s2.display();
    cout << "(s1 unchanged - deep copy!)" << endl;

    cout << "\nExiting (destructors will be called):" << endl;
    return 0;
}
