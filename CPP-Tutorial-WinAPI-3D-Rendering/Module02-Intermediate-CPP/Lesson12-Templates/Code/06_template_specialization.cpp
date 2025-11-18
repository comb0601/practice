/**
 * Template Specialization
 * Demonstrates full template specialization for specific types
 */

#include <iostream>
#include <cstring>
#include <string>
using namespace std;

// Generic template
template <typename T>
class Printer {
public:
    void print(T value) {
        cout << "Generic: " << value << endl;
    }
};

// Specialization for bool
template <>
class Printer<bool> {
public:
    void print(bool value) {
        cout << "Boolean: " << (value ? "true" : "false") << endl;
    }
};

// Specialization for const char*
template <>
class Printer<const char*> {
public:
    void print(const char* value) {
        cout << "C-String: \"" << value << "\" (length: " << strlen(value) << ")" << endl;
    }
};

// Specialization for char
template <>
class Printer<char> {
public:
    void print(char value) {
        cout << "Character: '" << value << "' (ASCII: " << static_cast<int>(value) << ")" << endl;
    }
};

// -------------------
// Generic Storage class
template <typename T>
class Storage {
private:
    T data;

public:
    Storage(T d) : data(d) {}

    void display() {
        cout << "Storing: " << data << " (generic type)" << endl;
    }

    T getData() { return data; }
};

// Specialization for pointers
template <typename T>
class Storage<T*> {
private:
    T* data;

public:
    Storage(T* d) : data(d) {}

    void display() {
        if (data != nullptr) {
            cout << "Storing pointer: " << *data << " at address " << data << endl;
        } else {
            cout << "Storing null pointer" << endl;
        }
    }

    T* getData() { return data; }
};

// -------------------
// Function template specialization
template <typename T>
T absolute(T value) {
    return (value < 0) ? -value : value;
}

// Specialization for strings (length)
template <>
string absolute<string>(string value) {
    return "Length: " + to_string(value.length());
}

int main() {
    cout << "=== Template Specialization ===" << endl << endl;

    // Printer specializations
    cout << "Printer Class Specializations:" << endl;
    Printer<int> intPrinter;
    intPrinter.print(42);

    Printer<double> doublePrinter;
    doublePrinter.print(3.14159);

    Printer<bool> boolPrinter;
    boolPrinter.print(true);
    boolPrinter.print(false);

    Printer<const char*> stringPrinter;
    stringPrinter.print("Hello, World!");

    Printer<char> charPrinter;
    charPrinter.print('A');
    charPrinter.print('Z');
    cout << endl;

    // Storage specializations
    cout << "Storage Class Specializations:" << endl;
    Storage<int> s1(100);
    s1.display();

    Storage<double> s2(3.14);
    s2.display();

    int value = 42;
    Storage<int*> s3(&value);
    s3.display();

    double pi = 3.14159;
    Storage<double*> s4(&pi);
    s4.display();

    Storage<int*> s5(nullptr);
    s5.display();
    cout << endl;

    // Function specializations
    cout << "Function Template Specializations:" << endl;
    cout << "absolute(-10) = " << absolute(-10) << endl;
    cout << "absolute(-3.14) = " << absolute(-3.14) << endl;
    cout << "absolute(\"hello\") = " << absolute(string("hello")) << endl;
    cout << "absolute(\"C++ Templates\") = " << absolute(string("C++ Templates")) << endl;

    return 0;
}
