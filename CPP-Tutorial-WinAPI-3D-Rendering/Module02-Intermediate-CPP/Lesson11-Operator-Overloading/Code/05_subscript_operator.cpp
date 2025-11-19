/*
 * Lesson 11: Operator Overloading - Subscript Operator
 *
 * This example demonstrates overloading the subscript operator []
 * to create array-like access for custom classes.
 *
 * Compilation: g++ -std=c++17 05_subscript_operator.cpp -o subscript
 * Execution: ./subscript
 */

#include <iostream>
#include <stdexcept>

class DynamicArray {
private:
    int* data;
    size_t size;

public:
    // Constructor
    DynamicArray(size_t s) : size(s) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = 0;
        }
    }

    // Destructor
    ~DynamicArray() {
        delete[] data;
    }

    // Copy constructor
    DynamicArray(const DynamicArray& other) : size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Assignment operator
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Subscript operator (non-const version for modification)
    int& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    // Subscript operator (const version for read-only)
    const int& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    void display() const {
        std::cout << "[";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]";
    }
};

int main() {
    std::cout << "=== Subscript Operator Demo ===" << std::endl << std::endl;

    DynamicArray arr(5);

    std::cout << "Initial array: ";
    arr.display();
    std::cout << std::endl << std::endl;

    // Using subscript operator to modify elements
    std::cout << "Setting values using [] operator:" << std::endl;
    for (size_t i = 0; i < arr.getSize(); ++i) {
        arr[i] = (i + 1) * 10;
        std::cout << "arr[" << i << "] = " << arr[i] << std::endl;
    }

    std::cout << "\nArray after modification: ";
    arr.display();
    std::cout << std::endl;

    // Test bounds checking
    std::cout << "\nTesting bounds checking:" << std::endl;
    try {
        std::cout << "Trying to access arr[10]..." << std::endl;
        arr[10] = 100;
    } catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    // Using const version
    const DynamicArray constArr = arr;
    std::cout << "\nReading from const array:" << std::endl;
    std::cout << "constArr[2] = " << constArr[2] << std::endl;

    return 0;
}
