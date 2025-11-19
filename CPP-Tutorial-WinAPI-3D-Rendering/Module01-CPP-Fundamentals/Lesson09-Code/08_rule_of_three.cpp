/*
 * Program: Rule of Three
 * Description: Destructor, copy constructor, copy assignment
 * Compilation: g++ -std=c++17 08_rule_of_three.cpp -o 08_rule_of_three
 * Execution: ./08_rule_of_three
 */

#include <iostream>
#include <cstring>
using namespace std;

class DynamicArray {
private:
    int* data;
    int size;

public:
    // Constructor
    DynamicArray(int s) : size(s) {
        data = new int[size];
        cout << "Constructor: allocated array of " << size << endl;
    }

    // Destructor
    ~DynamicArray() {
        cout << "Destructor: deallocating array" << endl;
        delete[] data;
    }

    // Copy constructor
    DynamicArray(const DynamicArray& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "Copy constructor: deep copy" << endl;
    }

    // Copy assignment operator
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;

            size = other.size;
            data = new int[size];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
            cout << "Copy assignment: deep copy" << endl;
        }
        return *this;
    }

    void set(int index, int value) {
        if (index >= 0 && index < size) {
            data[index] = value;
        }
    }

    int get(int index) const {
        return (index >= 0 && index < size) ? data[index] : 0;
    }

    void print() const {
        cout << "[";
        for (int i = 0; i < size; i++) {
            cout << data[i];
            if (i < size - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

int main() {
    cout << "=== Rule of Three ===" << endl << endl;

    cout << "Creating arr1:" << endl;
    DynamicArray arr1(5);
    for (int i = 0; i < 5; i++) {
        arr1.set(i, i * 10);
    }
    arr1.print();

    cout << "\nCopy construction (arr2 = arr1):" << endl;
    DynamicArray arr2 = arr1;
    arr2.print();

    cout << "\nModifying arr2:" << endl;
    arr2.set(0, 99);
    cout << "arr1: ";
    arr1.print();
    cout << "arr2: ";
    arr2.print();

    cout << "\nCopy assignment (arr3 = arr1):" << endl;
    DynamicArray arr3(3);
    arr3 = arr1;
    arr3.print();

    cout << "\nExiting (destructors will be called):" << endl;
    return 0;
}
