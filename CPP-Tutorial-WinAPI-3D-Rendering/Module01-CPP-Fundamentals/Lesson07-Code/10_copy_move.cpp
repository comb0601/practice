/*
 * Program: Copy vs Move
 * Description: Understanding deep copy and move semantics
 * Compilation: g++ -std=c++17 10_copy_move.cpp -o 10_copy_move
 * Execution: ./10_copy_move
 */

#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    char* data;
    size_t length;

public:
    // Constructor
    String(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        cout << "Constructor: created \"" << data << "\"" << endl;
    }

    // Destructor
    ~String() {
        cout << "Destructor: deleting \"" << data << "\"" << endl;
        delete[] data;
    }

    // Copy constructor (deep copy)
    String(const String& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        cout << "Copy constructor: copied \"" << data << "\"" << endl;
    }

    // Copy assignment (deep copy)
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;

            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
            cout << "Copy assignment: copied \"" << data << "\"" << endl;
        }
        return *this;
    }

    // Move constructor (C++11)
    String(String&& other) noexcept {
        data = other.data;
        length = other.length;

        other.data = nullptr;
        other.length = 0;
        cout << "Move constructor: moved \"" << data << "\"" << endl;
    }

    // Move assignment (C++11)
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;

            data = other.data;
            length = other.length;

            other.data = nullptr;
            other.length = 0;
            cout << "Move assignment: moved \"" << data << "\"" << endl;
        }
        return *this;
    }

    void print() const {
        if (data) {
            cout << "String: \"" << data << "\"" << endl;
        } else {
            cout << "String: (empty/moved)" << endl;
        }
    }
};

int main() {
    cout << "=== Copy vs Move ===" << endl << endl;

    // Copy constructor
    cout << "1. Copy Constructor:" << endl;
    String s1("Hello");
    String s2(s1);  // Copy
    s1.print();
    s2.print();
    cout << endl;

    // Copy assignment
    cout << "2. Copy Assignment:" << endl;
    String s3("World");
    s3 = s1;  // Copy assignment
    s1.print();
    s3.print();
    cout << endl;

    // Move constructor
    cout << "3. Move Constructor:" << endl;
    String s4(move(s1));  // Move
    s1.print();  // s1 is now empty
    s4.print();
    cout << endl;

    // Move assignment
    cout << "4. Move Assignment:" << endl;
    String s5("Temporary");
    s5 = move(s4);  // Move assignment
    s4.print();  // s4 is now empty
    s5.print();
    cout << endl;

    cout << "5. Benefits of Move Semantics:" << endl;
    cout << "- Avoids expensive deep copies" << endl;
    cout << "- Transfers ownership efficiently" << endl;
    cout << "- Perfect for temporary objects" << endl;
    cout << "- Enables move-only types" << endl;

    return 0;
}
