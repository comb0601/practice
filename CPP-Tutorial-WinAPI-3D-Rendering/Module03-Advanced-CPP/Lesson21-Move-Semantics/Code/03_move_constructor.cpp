/*
 * Lesson 21: Move Semantics
 * File: 03_move_constructor.cpp
 *
 * Move Constructor Implementation
 *
 * This program demonstrates how to implement a move constructor and shows
 * the performance benefits over copy construction.
 *
 * Compile: g++ -std=c++17 -Wall 03_move_constructor.cpp -o 03_move_constructor
 */

#include <iostream>
#include <cstring>
#include <chrono>
#include <vector>

// Simple dynamic array class to demonstrate move constructor
class DynamicArray {
private:
    int* data;
    size_t size;

public:
    // Constructor
    DynamicArray(size_t s = 0) : size(s) {
        if (size > 0) {
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<int>(i);
            }
            std::cout << "  [Constructed array of size " << size << "]\n";
        } else {
            data = nullptr;
        }
    }

    // Copy constructor (expensive!)
    DynamicArray(const DynamicArray& other) : size(other.size) {
        if (size > 0) {
            data = new int[size];
            std::memcpy(data, other.data, size * sizeof(int));
            std::cout << "  [COPY constructor: allocated and copied " << size << " elements]\n";
        } else {
            data = nullptr;
        }
    }

    // Move constructor (cheap!)
    DynamicArray(DynamicArray&& other) noexcept
        : data(other.data), size(other.size) {
        // Transfer ownership
        other.data = nullptr;
        other.size = 0;
        std::cout << "  [MOVE constructor: transferred ownership of " << size << " elements]\n";
    }

    // Destructor
    ~DynamicArray() {
        if (data) {
            std::cout << "  [Destroying array of size " << size << "]\n";
            delete[] data;
        }
    }

    // Get size
    size_t getSize() const { return size; }

    // Get element
    int& operator[](size_t index) { return data[index]; }
    const int& operator[](size_t index) const { return data[index]; }

    // Print first few elements
    void print() const {
        std::cout << "  Array[" << size << "]: ";
        if (size > 0) {
            size_t limit = (size < 5) ? size : 5;
            for (size_t i = 0; i < limit; ++i) {
                std::cout << data[i] << " ";
            }
            if (size > 5) std::cout << "...";
        }
        std::cout << "\n";
    }
};

// String class with move constructor
class MyString {
private:
    char* data;
    size_t length;

public:
    // Constructor
    MyString(const char* str = "") {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
        std::cout << "  [String constructed: \"" << data << "\"]\n";
    }

    // Copy constructor
    MyString(const MyString& other) : length(other.length) {
        data = new char[length + 1];
        std::strcpy(data, other.data);
        std::cout << "  [String COPIED: \"" << data << "\"]\n";
    }

    // Move constructor
    MyString(MyString&& other) noexcept
        : data(other.data), length(other.length) {
        std::cout << "  [String MOVED: \"" << data << "\"]\n";
        other.data = nullptr;
        other.length = 0;
    }

    // Destructor
    ~MyString() {
        if (data) {
            std::cout << "  [String destroyed: \"" << data << "\"]\n";
            delete[] data;
        } else {
            std::cout << "  [Empty string destroyed]\n";
        }
    }

    const char* c_str() const { return data ? data : ""; }
};

// Function returning by value (enables move)
DynamicArray createArray(size_t size) {
    std::cout << "Creating array in function:\n";
    DynamicArray arr(size);
    return arr;  // Move or RVO
}

// Function accepting by value (enables move)
void processArray(DynamicArray arr) {
    std::cout << "Processing array:\n";
    arr.print();
}

int main() {
    std::cout << "=== Move Constructor ===\n\n";

    // 1. Basic move constructor usage
    std::cout << "1. BASIC MOVE CONSTRUCTOR:\n";
    DynamicArray arr1(5);
    std::cout << "Created arr1:\n";
    arr1.print();

    std::cout << "\nMove constructing arr2 from arr1:\n";
    DynamicArray arr2(std::move(arr1));
    arr2.print();
    std::cout << "arr1 size after move: " << arr1.getSize() << " (moved-from)\n\n";

    // 2. Copy vs Move
    std::cout << "2. COPY VS MOVE:\n";
    DynamicArray arr3(3);

    std::cout << "\nCopy construction:\n";
    DynamicArray arr4 = arr3;  // Copy

    std::cout << "\nMove construction:\n";
    DynamicArray arr5 = std::move(arr3);  // Move
    std::cout << "\n";

    // 3. Return value optimization (RVO)
    std::cout << "3. RETURN VALUE (RVO or Move):\n";
    DynamicArray arr6 = createArray(10);
    arr6.print();
    std::cout << "\n";

    // 4. Passing to function
    std::cout << "4. PASSING TO FUNCTION:\n";
    DynamicArray arr7(4);
    std::cout << "\nPassing by move:\n";
    processArray(std::move(arr7));
    std::cout << "arr7 size after move: " << arr7.getSize() << "\n\n";

    // 5. Vector push_back
    std::cout << "5. VECTOR OPERATIONS:\n";
    std::vector<DynamicArray> vec;
    vec.reserve(3);  // Prevent reallocation

    std::cout << "\nPush back temporary (move):\n";
    vec.push_back(DynamicArray(3));

    std::cout << "\nPush back lvalue (copy):\n";
    DynamicArray arr8(2);
    vec.push_back(arr8);

    std::cout << "\nPush back with std::move:\n";
    DynamicArray arr9(4);
    vec.push_back(std::move(arr9));

    std::cout << "\n";

    // 6. String move constructor
    std::cout << "6. STRING MOVE CONSTRUCTOR:\n";
    MyString s1("Hello, World!");

    std::cout << "\nCopy construct:\n";
    MyString s2 = s1;

    std::cout << "\nMove construct:\n";
    MyString s3 = std::move(s1);

    std::cout << "\nString values:\n";
    std::cout << "s1: \"" << s1.c_str() << "\" (moved-from)\n";
    std::cout << "s2: \"" << s2.c_str() << "\"\n";
    std::cout << "s3: \"" << s3.c_str() << "\"\n\n";

    // 7. Performance comparison
    std::cout << "7. PERFORMANCE COMPARISON:\n";

    const size_t SIZE = 1000000;
    const int ITERATIONS = 100;

    // Measure copy time
    auto copyStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        DynamicArray temp(SIZE);
        DynamicArray copy = temp;  // Copy
    }
    auto copyEnd = std::chrono::high_resolution_clock::now();
    auto copyTime = std::chrono::duration_cast<std::chrono::milliseconds>(copyEnd - copyStart);

    std::cout << "Copy time: " << copyTime.count() << " ms\n";

    // Measure move time
    auto moveStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        DynamicArray temp(SIZE);
        DynamicArray moved = std::move(temp);  // Move
    }
    auto moveEnd = std::chrono::high_resolution_clock::now();
    auto moveTime = std::chrono::duration_cast<std::chrono::milliseconds>(moveEnd - moveStart);

    std::cout << "Move time: " << moveTime.count() << " ms\n";
    std::cout << "Speedup: " << (double)copyTime.count() / moveTime.count() << "x faster\n\n";

    // 8. Move constructor requirements
    std::cout << "8. MOVE CONSTRUCTOR REQUIREMENTS:\n";
    std::cout << "Requirements:\n";
    std::cout << "  1. Mark as noexcept (enables STL optimizations)\n";
    std::cout << "  2. Transfer ownership of resources\n";
    std::cout << "  3. Leave source in valid but unspecified state\n";
    std::cout << "  4. Don't allocate new memory (defeats purpose)\n";
    std::cout << "  5. Don't throw exceptions\n\n";

    // 9. Moved-from state
    std::cout << "9. MOVED-FROM STATE:\n";
    DynamicArray arr10(5);
    DynamicArray arr11 = std::move(arr10);

    std::cout << "After move:\n";
    std::cout << "  arr10 size: " << arr10.getSize() << " (moved-from, valid but empty)\n";
    std::cout << "  arr11 size: " << arr11.getSize() << " (owns resources)\n";
    std::cout << "  arr10 can be safely destroyed or assigned to\n\n";

    // 10. Move in initialization vs assignment
    std::cout << "10. MOVE IN INITIALIZATION:\n";
    DynamicArray arr12(3);

    std::cout << "\nMove initialization (calls move constructor):\n";
    DynamicArray arr13 = std::move(arr12);

    std::cout << "\nNote: Move assignment would be different (next example)\n";

    std::cout << "\nDestructing objects:\n";
    return 0;
}
