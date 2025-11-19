/*
 * Program: Advanced Dynamic Arrays
 * Description: Dynamic arrays with resizing, copying, and operations
 * Compilation: g++ -std=c++17 04_dynamic_arrays_advanced.cpp -o 04_dynamic_arrays_advanced
 * Execution: ./04_dynamic_arrays_advanced
 */

#include <iostream>
using namespace std;

class DynamicArray {
private:
    int* data;
    int size;
    int capacity;

    void resize(int newCapacity) {
        int* newData = new int[newCapacity];

        // Copy existing data
        int copySize = (size < newCapacity) ? size : newCapacity;
        for (int i = 0; i < copySize; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    DynamicArray() : data(nullptr), size(0), capacity(0) {}

    DynamicArray(int initialCapacity) {
        data = new int[initialCapacity];
        size = 0;
        capacity = initialCapacity;
    }

    ~DynamicArray() {
        delete[] data;
    }

    void push_back(int value) {
        if (size >= capacity) {
            int newCapacity = (capacity == 0) ? 1 : capacity * 2;
            resize(newCapacity);
        }
        data[size++] = value;
    }

    int get(int index) const {
        if (index >= 0 && index < size) {
            return data[index];
        }
        return -1;
    }

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }

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
    cout << "=== Advanced Dynamic Arrays ===" << endl << endl;

    // Manual dynamic array management
    cout << "1. Manual Dynamic Array:" << endl;
    int capacity = 5;
    int size = 0;
    int* arr = new int[capacity];

    // Add elements
    for (int i = 0; i < 3; i++) {
        arr[size++] = i * 10;
    }

    cout << "Array (size=" << size << ", capacity=" << capacity << "): ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl << endl;

    // Resizing
    cout << "2. Resizing Array:" << endl;
    cout << "Need to add more elements..." << endl;

    if (size >= capacity) {
        int newCapacity = capacity * 2;
        int* newArr = new int[newCapacity];

        // Copy existing data
        for (int i = 0; i < size; i++) {
            newArr[i] = arr[i];
        }

        delete[] arr;
        arr = newArr;
        capacity = newCapacity;

        cout << "Resized to capacity: " << capacity << endl;
    }

    // Add more elements
    for (int i = 3; i < 7; i++) {
        arr[size++] = i * 10;
    }

    cout << "Array (size=" << size << ", capacity=" << capacity << "): ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr;
    cout << endl;

    // Using DynamicArray class
    cout << "3. Dynamic Array Class:" << endl;
    DynamicArray myArray(2);

    cout << "Initial capacity: " << myArray.getCapacity() << endl;

    for (int i = 1; i <= 10; i++) {
        myArray.push_back(i * 5);
        cout << "After adding " << i << " elements: ";
        cout << "size=" << myArray.getSize();
        cout << ", capacity=" << myArray.getCapacity() << endl;
    }

    cout << "Final array: ";
    myArray.print();
    cout << endl;

    // 2D dynamic array
    cout << "4. 2D Dynamic Array:" << endl;
    int rows = 3, cols = 4;
    int** matrix = new int*[rows];

    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    // Fill matrix
    int value = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = value++;
        }
    }

    // Print matrix
    cout << "Matrix:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }

    // Cleanup 2D array
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    cout << endl;

    // Deep copy
    cout << "5. Deep Copy:" << endl;
    int* original = new int[5]{1, 2, 3, 4, 5};
    int* copy = new int[5];

    for (int i = 0; i < 5; i++) {
        copy[i] = original[i];
    }

    cout << "Original: ";
    for (int i = 0; i < 5; i++) cout << original[i] << " ";
    cout << endl;

    cout << "Copy:     ";
    for (int i = 0; i < 5; i++) cout << copy[i] << " ";
    cout << endl;

    // Modify original
    original[0] = 99;

    cout << "After modifying original[0]:" << endl;
    cout << "Original: ";
    for (int i = 0; i < 5; i++) cout << original[i] << " ";
    cout << endl;

    cout << "Copy:     ";
    for (int i = 0; i < 5; i++) cout << copy[i] << " ";
    cout << endl;
    cout << "Copy is independent!" << endl;

    delete[] original;
    delete[] copy;

    return 0;
}
