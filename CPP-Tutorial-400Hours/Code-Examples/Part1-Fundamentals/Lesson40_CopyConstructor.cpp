/*
 * Lesson 40: Copy Constructor and Assignment Operator
 * Compile: cl Lesson40_CopyConstructor.cpp
 */

#include <iostream>
#include <cstring>
using namespace std;

class Array {
private:
    int *data;
    int size;

public:
    // Constructor
    Array(int s) : size(s) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = i + 1;
        }
        cout << "Constructor called (size=" << size << ")" << endl;
    }

    // Copy constructor (deep copy)
    Array(const Array& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "Copy constructor called" << endl;
    }

    // Assignment operator
    Array& operator=(const Array& other) {
        cout << "Assignment operator called" << endl;
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Destructor
    ~Array() {
        delete[] data;
        cout << "Destructor called (size=" << size << ")" << endl;
    }

    void display() const {
        cout << "Array: ";
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    void set(int index, int value) {
        if (index >= 0 && index < size) {
            data[index] = value;
        }
    }
};

class String {
private:
    char *str;
    int length;

public:
    String(const char *s = "") {
        length = strlen(s);
        str = new char[length + 1];
        strcpy(str, s);
        cout << "String constructor: \"" << str << "\"" << endl;
    }

    // Copy constructor
    String(const String& other) {
        length = other.length;
        str = new char[length + 1];
        strcpy(str, other.str);
        cout << "String copy constructor: \"" << str << "\"" << endl;
    }

    // Assignment operator
    String& operator=(const String& other) {
        cout << "String assignment operator" << endl;
        if (this != &other) {
            delete[] str;
            length = other.length;
            str = new char[length + 1];
            strcpy(str, other.str);
        }
        return *this;
    }

    ~String() {
        cout << "String destructor: \"" << str << "\"" << endl;
        delete[] str;
    }

    void display() const {
        cout << str;
    }
};

int main() {
    cout << "=== Copy Constructor Demo ===" << endl << endl;

    // ARRAY EXAMPLE
    cout << "=== ARRAY ===" << endl;
    Array arr1(5);
    arr1.display();

    cout << "\nCreating arr2 as copy of arr1:" << endl;
    Array arr2 = arr1;  // Copy constructor
    arr2.display();

    cout << "\nModifying arr2:" << endl;
    arr2.set(0, 100);
    arr1.display();
    arr2.display();

    cout << "\nAssignment operator:" << endl;
    Array arr3(3);
    arr3 = arr1;  // Assignment operator
    arr3.display();

    cout << endl;

    // STRING EXAMPLE
    cout << "=== STRING ===" << endl;
    String s1("Hello");
    String s2 = s1;  // Copy constructor

    cout << "s1: "; s1.display(); cout << endl;
    cout << "s2: "; s2.display(); cout << endl;

    String s3("World");
    s3 = s1;  // Assignment operator
    cout << "s3: "; s3.display(); cout << endl;

    cout << "\nDestructors will be called at program end:" << endl;

    return 0;
}
