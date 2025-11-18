/*
 * Lesson 11: Operator Overloading
 * File: 06_subscript_operator.cpp
 *
 * Demonstrates subscript operator [] overloading for array-like access.
 * Shows both const and non-const versions.
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <initializer_list>

// Dynamic array class with bounds checking
class IntArray {
private:
    int* data;
    size_t size;

public:
    // Constructor
    IntArray(size_t s) : size(s) {
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = 0;
        }
        std::cout << "IntArray created with size " << size << "\n";
    }

    // Constructor with initializer list
    IntArray(std::initializer_list<int> list) : size(list.size()) {
        data = new int[size];
        size_t i = 0;
        for (int val : list) {
            data[i++] = val;
        }
        std::cout << "IntArray created with " << size << " initial values\n";
    }

    // Destructor
    ~IntArray() {
        delete[] data;
        std::cout << "IntArray destroyed\n";
    }

    // Copy constructor
    IntArray(const IntArray& other) : size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Assignment operator
    IntArray& operator=(const IntArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Non-const subscript operator (for modification)
    int& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        std::cout << "Non-const operator[] called for index " << index << "\n";
        return data[index];
    }

    // Const subscript operator (for read-only access)
    const int& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        std::cout << "Const operator[] called for index " << index << "\n";
        return data[index];
    }

    // Get size
    size_t getSize() const { return size; }

    // Display array
    void display() const {
        std::cout << "[";
        for (size_t i = 0; i < size; i++) {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

// 2D Matrix class with double subscript
class Matrix {
private:
    int** data;
    size_t rows, cols;

public:
    // Constructor
    Matrix(size_t r, size_t c) : rows(r), cols(c) {
        data = new int*[rows];
        for (size_t i = 0; i < rows; i++) {
            data[i] = new int[cols];
            for (size_t j = 0; j < cols; j++) {
                data[i][j] = 0;
            }
        }
    }

    // Destructor
    ~Matrix() {
        for (size_t i = 0; i < rows; i++) {
            delete[] data[i];
        }
        delete[] data;
    }

    // Subscript operator returns pointer to row (allows matrix[i][j] syntax)
    int* operator[](size_t row) {
        if (row >= rows) {
            throw std::out_of_range("Row index out of bounds");
        }
        return data[row];
    }

    // Const version
    const int* operator[](size_t row) const {
        if (row >= rows) {
            throw std::out_of_range("Row index out of bounds");
        }
        return data[row];
    }

    // Display matrix
    void display() const {
        for (size_t i = 0; i < rows; i++) {
            std::cout << "[";
            for (size_t j = 0; j < cols; j++) {
                std::cout << data[i][j];
                if (j < cols - 1) std::cout << ", ";
            }
            std::cout << "]\n";
        }
    }

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
};

// String-indexed map (simplified associative array)
class SimpleMap {
private:
    static const size_t MAX_SIZE = 100;
    std::string keys[MAX_SIZE];
    int values[MAX_SIZE];
    size_t count;

    // Find index of key
    int findIndex(const std::string& key) const {
        for (size_t i = 0; i < count; i++) {
            if (keys[i] == key) {
                return i;
            }
        }
        return -1;
    }

public:
    // Constructor
    SimpleMap() : count(0) {}

    // Subscript operator (creates entry if doesn't exist)
    int& operator[](const std::string& key) {
        int index = findIndex(key);
        if (index == -1) {
            if (count >= MAX_SIZE) {
                throw std::runtime_error("Map is full");
            }
            keys[count] = key;
            values[count] = 0;
            index = count++;
        }
        return values[index];
    }

    // Check if key exists
    bool contains(const std::string& key) const {
        return findIndex(key) != -1;
    }

    // Display map
    void display() const {
        std::cout << "{\n";
        for (size_t i = 0; i < count; i++) {
            std::cout << "  \"" << keys[i] << "\": " << values[i] << "\n";
        }
        std::cout << "}\n";
    }

    size_t size() const { return count; }
};

// Helper function to demonstrate const vs non-const
void printArray(const IntArray& arr) {
    std::cout << "Printing const array:\n";
    for (size_t i = 0; i < arr.getSize(); i++) {
        std::cout << arr[i] << " ";  // Calls const version
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== Subscript Operator Demo ===\n\n";

    // IntArray demo
    std::cout << "--- IntArray Class ---\n";
    IntArray arr1(5);

    std::cout << "\nSetting values:\n";
    arr1[0] = 10;  // Calls non-const version
    arr1[1] = 20;
    arr1[2] = 30;
    arr1[3] = 40;
    arr1[4] = 50;

    std::cout << "\nReading values:\n";
    for (size_t i = 0; i < arr1.getSize(); i++) {
        std::cout << "arr1[" << i << "] = ";
        std::cout << arr1[i] << "\n";  // Calls non-const version
    }

    std::cout << "\nDisplay: ";
    arr1.display();

    // Const array (calls const version of operator[])
    std::cout << "\n";
    const IntArray constArr({1, 2, 3, 4, 5});
    printArray(constArr);

    // Modify and read in same expression
    std::cout << "\nModify and read in one line:\n";
    arr1[2] = arr1[2] + 100;
    std::cout << "After arr1[2] = arr1[2] + 100: ";
    arr1.display();

    // Error handling
    std::cout << "\n--- Bounds Checking ---\n";
    try {
        arr1[10] = 999;  // Out of bounds
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
    }

    // Matrix demo
    std::cout << "\n--- Matrix Class (2D Subscript) ---\n";
    Matrix mat(3, 4);
    std::cout << "Setting values in 3x4 matrix:\n";

    int value = 1;
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 4; j++) {
            mat[i][j] = value++;  // Double subscript!
        }
    }

    std::cout << "\nMatrix contents:\n";
    mat.display();

    std::cout << "\nAccessing specific elements:\n";
    std::cout << "mat[0][0] = " << mat[0][0] << "\n";
    std::cout << "mat[1][2] = " << mat[1][2] << "\n";
    std::cout << "mat[2][3] = " << mat[2][3] << "\n";

    // SimpleMap demo
    std::cout << "\n--- SimpleMap Class (String Subscript) ---\n";
    SimpleMap scores;

    std::cout << "Setting values with string keys:\n";
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;
    scores["Diana"] = 88;

    std::cout << "\nMap contents:\n";
    scores.display();

    std::cout << "\nAccessing and modifying:\n";
    std::cout << "Alice's score: " << scores["Alice"] << "\n";
    scores["Alice"] = 98;
    std::cout << "After update: " << scores["Alice"] << "\n";

    std::cout << "\nAdding new entry:\n";
    scores["Eve"] = 90;
    scores.display();

    // Initializer list constructor
    std::cout << "\n--- Initializer List Constructor ---\n";
    IntArray arr2 = {100, 200, 300, 400, 500};
    std::cout << "Array created with initializer list: ";
    arr2.display();

    return 0;
}

/*
 * EXPECTED OUTPUT:
 * =================================
 * === Subscript Operator Demo ===
 *
 * --- IntArray Class ---
 * IntArray created with size 5
 *
 * Setting values:
 * Non-const operator[] called for index 0
 * Non-const operator[] called for index 1
 * Non-const operator[] called for index 2
 * Non-const operator[] called for index 3
 * Non-const operator[] called for index 4
 *
 * Reading values:
 * arr1[0] = Non-const operator[] called for index 0
 * 10
 * arr1[1] = Non-const operator[] called for index 1
 * 20
 * arr1[2] = Non-const operator[] called for index 2
 * 30
 * arr1[3] = Non-const operator[] called for index 3
 * 40
 * arr1[4] = Non-const operator[] called for index 4
 * 50
 *
 * Display: [10, 20, 30, 40, 50]
 *
 * IntArray created with 5 initial values
 * Printing const array:
 * Const operator[] called for index 0
 * 1 Const operator[] called for index 1
 * 2 Const operator[] called for index 2
 * 3 Const operator[] called for index 3
 * 4 Const operator[] called for index 4
 * 5
 *
 * Modify and read in one line:
 * Non-const operator[] called for index 2
 * Non-const operator[] called for index 2
 * After arr1[2] = arr1[2] + 100: [10, 20, 130, 40, 50]
 *
 * --- Bounds Checking ---
 * Exception caught: Index out of bounds
 *
 * --- Matrix Class (2D Subscript) ---
 * Setting values in 3x4 matrix:
 *
 * Matrix contents:
 * [1, 2, 3, 4]
 * [5, 6, 7, 8]
 * [9, 10, 11, 12]
 *
 * Accessing specific elements:
 * mat[0][0] = 1
 * mat[1][2] = 7
 * mat[2][3] = 12
 *
 * --- SimpleMap Class (String Subscript) ---
 * Setting values with string keys:
 *
 * Map contents:
 * {
 *   "Alice": 95
 *   "Bob": 87
 *   "Charlie": 92
 *   "Diana": 88
 * }
 *
 * Accessing and modifying:
 * Alice's score: 95
 * After update: 98
 *
 * Adding new entry:
 * {
 *   "Alice": 98
 *   "Bob": 87
 *   "Charlie": 92
 *   "Diana": 88
 *   "Eve": 90
 * }
 *
 * --- Initializer List Constructor ---
 * IntArray created with 5 initial values
 * Array created with initializer list: [100, 200, 300, 400, 500]
 * IntArray destroyed
 * IntArray destroyed
 * IntArray destroyed
 */
