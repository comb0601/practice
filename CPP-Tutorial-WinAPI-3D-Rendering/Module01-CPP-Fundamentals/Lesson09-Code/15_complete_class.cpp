/*
 * Program: Complete Class with All Features
 * Description: Class with constructors, destructor, copy, move
 * Compilation: g++ -std=c++17 15_complete_class.cpp -o 15_complete_class
 * Execution: ./15_complete_class
 */

#include <iostream>
#include <cstring>
using namespace std;

class CompleteClass {
private:
    char* data;
    int size;
    int id;
    static int nextId;

public:
    // Default constructor
    CompleteClass() : data(nullptr), size(0), id(nextId++) {
        cout << "[" << id << "] Default constructor" << endl;
    }

    // Parameterized constructor
    CompleteClass(const char* str) : id(nextId++) {
        size = strlen(str);
        data = new char[size + 1];
        strcpy(data, str);
        cout << "[" << id << "] Parameterized constructor: \"" << data << "\"" << endl;
    }

    // Destructor
    ~CompleteClass() {
        cout << "[" << id << "] Destructor";
        if (data) {
            cout << ": deleting \"" << data << "\"";
            delete[] data;
        }
        cout << endl;
    }

    // Copy constructor
    CompleteClass(const CompleteClass& other) : id(nextId++) {
        size = other.size;
        if (other.data) {
            data = new char[size + 1];
            strcpy(data, other.data);
            cout << "[" << id << "] Copy constructor: \"" << data << "\"" << endl;
        } else {
            data = nullptr;
        }
    }

    // Copy assignment
    CompleteClass& operator=(const CompleteClass& other) {
        cout << "[" << id << "] Copy assignment";
        if (this != &other) {
            delete[] data;

            size = other.size;
            if (other.data) {
                data = new char[size + 1];
                strcpy(data, other.data);
                cout << ": \"" << data << "\"";
            } else {
                data = nullptr;
            }
        }
        cout << endl;
        return *this;
    }

    // Move constructor
    CompleteClass(CompleteClass&& other) noexcept
        : data(other.data), size(other.size), id(nextId++) {
        other.data = nullptr;
        other.size = 0;
        cout << "[" << id << "] Move constructor" << endl;
    }

    // Move assignment
    CompleteClass& operator=(CompleteClass&& other) noexcept {
        cout << "[" << id << "] Move assignment" << endl;
        if (this != &other) {
            delete[] data;

            data = other.data;
            size = other.size;

            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    void display() const {
        cout << "[" << id << "] Data: ";
        if (data) {
            cout << "\"" << data << "\"";
        } else {
            cout << "(null)";
        }
        cout << endl;
    }
};

int CompleteClass::nextId = 1;

int main() {
    cout << "=== Complete Class ===" << endl << endl;

    cout << "1. Default construction:" << endl;
    CompleteClass c1;
    c1.display();

    cout << "\n2. Parameterized construction:" << endl;
    CompleteClass c2("Hello");
    c2.display();

    cout << "\n3. Copy construction:" << endl;
    CompleteClass c3 = c2;
    c3.display();

    cout << "\n4. Copy assignment:" << endl;
    CompleteClass c4;
    c4 = c2;
    c4.display();

    cout << "\n5. Move construction:" << endl;
    CompleteClass c5 = move(c2);
    c2.display();
    c5.display();

    cout << "\n6. Move assignment:" << endl;
    CompleteClass c6;
    c6 = move(c3);
    c3.display();
    c6.display();

    cout << "\n7. Exiting (destructors):" << endl;
    return 0;
}
