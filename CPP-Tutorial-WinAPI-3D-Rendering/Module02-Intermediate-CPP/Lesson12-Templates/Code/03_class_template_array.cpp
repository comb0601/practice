/**
 * Class Template: Generic Array
 * Demonstrates creating a reusable array class template
 */

#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

template <typename T>
class Array {
private:
    T* data;
    size_t size;

public:
    // Constructor
    Array(size_t s) : size(s) {
        data = new T[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = T();  // Default initialize
        }
    }

    // Destructor
    ~Array() {
        delete[] data;
    }

    // Copy constructor
    Array(const Array& other) : size(other.size) {
        data = new T[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Assignment operator
    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new T[size];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Subscript operator
    T& operator[](size_t index) {
        if (index >= size) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    // Getters
    size_t getSize() const { return size; }

    // Fill array with value
    void fill(const T& value) {
        for (size_t i = 0; i < size; i++) {
            data[i] = value;
        }
    }

    // Print array
    void print() const {
        cout << "[";
        for (size_t i = 0; i < size; i++) {
            cout << data[i];
            if (i < size - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

int main() {
    cout << "=== Generic Array Template ===" << endl << endl;

    // Integer array
    cout << "Integer Array:" << endl;
    Array<int> intArray(5);
    for (size_t i = 0; i < intArray.getSize(); i++) {
        intArray[i] = (i + 1) * 10;
    }
    intArray.print();
    cout << "Element at index 2: " << intArray[2] << endl;
    cout << endl;

    // Double array
    cout << "Double Array:" << endl;
    Array<double> doubleArray(4);
    doubleArray[0] = 3.14;
    doubleArray[1] = 2.71;
    doubleArray[2] = 1.41;
    doubleArray[3] = 1.73;
    doubleArray.print();
    cout << endl;

    // String array
    cout << "String Array:" << endl;
    Array<string> stringArray(3);
    stringArray[0] = "Hello";
    stringArray[1] = "World";
    stringArray[2] = "Templates";
    stringArray.print();
    cout << endl;

    // Character array
    cout << "Character Array:" << endl;
    Array<char> charArray(6);
    charArray.fill('*');
    charArray.print();
    charArray[2] = 'X';
    charArray.print();
    cout << endl;

    // Copy constructor test
    cout << "Copy Constructor Test:" << endl;
    Array<int> original(3);
    original[0] = 1;
    original[1] = 2;
    original[2] = 3;
    cout << "Original: ";
    original.print();

    Array<int> copy = original;
    cout << "Copy: ";
    copy.print();

    copy[1] = 999;
    cout << "After modifying copy:" << endl;
    cout << "Original: ";
    original.print();
    cout << "Copy: ";
    copy.print();
    cout << endl;

    // Exception handling
    cout << "Exception Handling:" << endl;
    try {
        Array<int> arr(5);
        arr[10] = 100;  // Out of bounds
    } catch (const out_of_range& e) {
        cout << "Caught exception: " << e.what() << endl;
    }

    return 0;
}
