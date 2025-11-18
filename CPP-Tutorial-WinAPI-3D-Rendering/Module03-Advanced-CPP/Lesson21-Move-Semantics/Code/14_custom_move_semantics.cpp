/*
 * Lesson 21: Move Semantics
 * File: 14_custom_move_semantics.cpp
 *
 * Implementing Custom Move Semantics
 *
 * This program shows how to implement move semantics for various custom
 * types with different resource management needs.
 *
 * Compile: g++ -std=c++17 -Wall 14_custom_move_semantics.cpp -o 14_custom_move_semantics
 */

#include <iostream>
#include <cstring>
#include <algorithm>

// Example 1: Simple dynamic array with move semantics
class DynamicArray {
private:
    int* data;
    size_t size;

public:
    // Constructor
    explicit DynamicArray(size_t s = 0) : size(s) {
        data = size > 0 ? new int[size]() : nullptr;
        std::cout << "  DynamicArray constructed (size=" << size << ")\n";
    }

    // Destructor
    ~DynamicArray() {
        delete[] data;
        std::cout << "  DynamicArray destroyed (size=" << size << ")\n";
    }

    // Copy constructor
    DynamicArray(const DynamicArray& other) : size(other.size) {
        data = size > 0 ? new int[size] : nullptr;
        std::copy(other.data, other.data + size, data);
        std::cout << "  DynamicArray copy constructed (size=" << size << ")\n";
    }

    // Copy assignment
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = size > 0 ? new int[size] : nullptr;
            std::copy(other.data, other.data + size, data);
            std::cout << "  DynamicArray copy assigned (size=" << size << ")\n";
        }
        return *this;
    }

    // Move constructor
    DynamicArray(DynamicArray&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        std::cout << "  DynamicArray move constructed (size=" << size << ")\n";
    }

    // Move assignment
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            std::cout << "  DynamicArray move assigned (size=" << size << ")\n";
        }
        return *this;
    }

    size_t getSize() const { return size; }
};

// Example 2: Matrix class with move semantics
class Matrix {
private:
    double** data;
    size_t rows;
    size_t cols;

public:
    Matrix(size_t r, size_t c) : rows(r), cols(c) {
        data = new double*[rows];
        for (size_t i = 0; i < rows; ++i) {
            data[i] = new double[cols]();
        }
        std::cout << "  Matrix constructed (" << rows << "x" << cols << ")\n";
    }

    ~Matrix() {
        for (size_t i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data;
        std::cout << "  Matrix destroyed (" << rows << "x" << cols << ")\n";
    }

    // Copy constructor (deep copy)
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = new double*[rows];
        for (size_t i = 0; i < rows; ++i) {
            data[i] = new double[cols];
            std::copy(other.data[i], other.data[i] + cols, data[i]);
        }
        std::cout << "  Matrix copy constructed (" << rows << "x" << cols << ")\n";
    }

    // Move constructor (steal pointers)
    Matrix(Matrix&& other) noexcept
        : data(other.data), rows(other.rows), cols(other.cols) {
        other.data = nullptr;
        other.rows = 0;
        other.cols = 0;
        std::cout << "  Matrix move constructed (" << rows << "x" << cols << ")\n";
    }

    // Copy assignment
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            // Deallocate existing
            for (size_t i = 0; i < rows; ++i) {
                delete[] data[i];
            }
            delete[] data;

            // Deep copy
            rows = other.rows;
            cols = other.cols;
            data = new double*[rows];
            for (size_t i = 0; i < rows; ++i) {
                data[i] = new double[cols];
                std::copy(other.data[i], other.data[i] + cols, data[i]);
            }
            std::cout << "  Matrix copy assigned (" << rows << "x" << cols << ")\n";
        }
        return *this;
    }

    // Move assignment
    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            // Deallocate existing
            for (size_t i = 0; i < rows; ++i) {
                delete[] data[i];
            }
            delete[] data;

            // Transfer ownership
            data = other.data;
            rows = other.rows;
            cols = other.cols;

            // Leave other in valid state
            other.data = nullptr;
            other.rows = 0;
            other.cols = 0;
            std::cout << "  Matrix move assigned (" << rows << "x" << cols << ")\n";
        }
        return *this;
    }

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
};

// Example 3: Node-based container (linked list node)
struct Node {
    int data;
    Node* next;

    Node(int d) : data(d), next(nullptr) {
        std::cout << "  Node created (data=" << data << ")\n";
    }

    ~Node() {
        std::cout << "  Node destroyed (data=" << data << ")\n";
    }
};

class LinkedList {
private:
    Node* head;
    size_t count;

public:
    LinkedList() : head(nullptr), count(0) {
        std::cout << "  LinkedList constructed\n";
    }

    ~LinkedList() {
        clear();
        std::cout << "  LinkedList destroyed\n";
    }

    // Copy constructor
    LinkedList(const LinkedList& other) : head(nullptr), count(0) {
        Node* current = other.head;
        while (current) {
            append(current->data);
            current = current->next;
        }
        std::cout << "  LinkedList copy constructed (count=" << count << ")\n";
    }

    // Move constructor
    LinkedList(LinkedList&& other) noexcept
        : head(other.head), count(other.count) {
        other.head = nullptr;
        other.count = 0;
        std::cout << "  LinkedList move constructed (count=" << count << ")\n";
    }

    // Copy assignment
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current) {
                append(current->data);
                current = current->next;
            }
            std::cout << "  LinkedList copy assigned (count=" << count << ")\n";
        }
        return *this;
    }

    // Move assignment
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            count = other.count;
            other.head = nullptr;
            other.count = 0;
            std::cout << "  LinkedList move assigned (count=" << count << ")\n";
        }
        return *this;
    }

    void append(int data) {
        Node* newNode = new Node(data);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++count;
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }

    size_t size() const { return count; }
};

// Example 4: Object with multiple resources
class ComplexObject {
private:
    int* intArray;
    char* charArray;
    size_t intSize;
    size_t charSize;

public:
    ComplexObject(size_t iSize, size_t cSize)
        : intSize(iSize), charSize(cSize) {
        intArray = new int[intSize]();
        charArray = new char[charSize]();
        std::cout << "  ComplexObject constructed\n";
    }

    ~ComplexObject() {
        delete[] intArray;
        delete[] charArray;
        std::cout << "  ComplexObject destroyed\n";
    }

    // Copy constructor
    ComplexObject(const ComplexObject& other)
        : intSize(other.intSize), charSize(other.charSize) {
        intArray = new int[intSize];
        charArray = new char[charSize];
        std::copy(other.intArray, other.intArray + intSize, intArray);
        std::copy(other.charArray, other.charArray + charSize, charArray);
        std::cout << "  ComplexObject copy constructed\n";
    }

    // Move constructor - must move ALL resources
    ComplexObject(ComplexObject&& other) noexcept
        : intArray(other.intArray),
          charArray(other.charArray),
          intSize(other.intSize),
          charSize(other.charSize) {
        // Nullify ALL pointers in source
        other.intArray = nullptr;
        other.charArray = nullptr;
        other.intSize = 0;
        other.charSize = 0;
        std::cout << "  ComplexObject move constructed\n";
    }

    // Copy assignment
    ComplexObject& operator=(const ComplexObject& other) {
        if (this != &other) {
            delete[] intArray;
            delete[] charArray;

            intSize = other.intSize;
            charSize = other.charSize;
            intArray = new int[intSize];
            charArray = new char[charSize];
            std::copy(other.intArray, other.intArray + intSize, intArray);
            std::copy(other.charArray, other.charArray + charSize, charArray);
            std::cout << "  ComplexObject copy assigned\n";
        }
        return *this;
    }

    // Move assignment - must move ALL resources
    ComplexObject& operator=(ComplexObject&& other) noexcept {
        if (this != &other) {
            // Clean up existing resources
            delete[] intArray;
            delete[] charArray;

            // Transfer ALL resources
            intArray = other.intArray;
            charArray = other.charArray;
            intSize = other.intSize;
            charSize = other.charSize;

            // Nullify ALL pointers in source
            other.intArray = nullptr;
            other.charArray = nullptr;
            other.intSize = 0;
            other.charSize = 0;
            std::cout << "  ComplexObject move assigned\n";
        }
        return *this;
    }
};

int main() {
    std::cout << "=== Implementing Custom Move Semantics ===\n\n";

    // 1. Dynamic array
    std::cout << "1. DYNAMIC ARRAY:\n";
    {
        DynamicArray arr1(10);
        std::cout << "\nCopy:\n";
        DynamicArray arr2 = arr1;

        std::cout << "\nMove:\n";
        DynamicArray arr3 = std::move(arr1);

        std::cout << "\nSizes:\n";
        std::cout << "arr1: " << arr1.getSize() << " (moved-from)\n";
        std::cout << "arr2: " << arr2.getSize() << "\n";
        std::cout << "arr3: " << arr3.getSize() << "\n";

        std::cout << "\nDestroying:\n";
    }
    std::cout << "\n";

    // 2. Matrix
    std::cout << "2. MATRIX (2D ARRAY):\n";
    {
        Matrix m1(100, 100);

        std::cout << "\nMove constructor:\n";
        Matrix m2 = std::move(m1);

        std::cout << "\nMove assignment:\n";
        Matrix m3(50, 50);
        m3 = std::move(m2);

        std::cout << "\nDimensions:\n";
        std::cout << "m1: " << m1.getRows() << "x" << m1.getCols() << "\n";
        std::cout << "m2: " << m2.getRows() << "x" << m2.getCols() << "\n";
        std::cout << "m3: " << m3.getRows() << "x" << m3.getCols() << "\n";

        std::cout << "\nDestroying:\n";
    }
    std::cout << "\n";

    // 3. Linked list
    std::cout << "3. LINKED LIST:\n";
    {
        LinkedList list1;
        list1.append(1);
        list1.append(2);
        list1.append(3);

        std::cout << "\nMove constructor:\n";
        LinkedList list2 = std::move(list1);

        std::cout << "\nSizes:\n";
        std::cout << "list1: " << list1.size() << " (moved-from)\n";
        std::cout << "list2: " << list2.size() << "\n";

        std::cout << "\nDestroying:\n";
    }
    std::cout << "\n";

    // 4. Complex object with multiple resources
    std::cout << "4. COMPLEX OBJECT (MULTIPLE RESOURCES):\n";
    {
        ComplexObject obj1(100, 200);

        std::cout << "\nMove constructor:\n";
        ComplexObject obj2 = std::move(obj1);

        std::cout << "\nMove assignment:\n";
        ComplexObject obj3(50, 75);
        obj3 = std::move(obj2);

        std::cout << "\nDestroying:\n";
    }
    std::cout << "\n";

    // 5. Implementation checklist
    std::cout << "5. IMPLEMENTATION CHECKLIST:\n";
    std::cout << "Move Constructor:\n";
    std::cout << "  [x] Mark as noexcept\n";
    std::cout << "  [x] Transfer ALL resources from source\n";
    std::cout << "  [x] Nullify ALL pointers in source\n";
    std::cout << "  [x] Reset ALL sizes/counts in source\n";
    std::cout << "  [x] Don't allocate new memory\n";
    std::cout << "  [x] Leave source in valid state\n\n";

    std::cout << "Move Assignment:\n";
    std::cout << "  [x] Mark as noexcept\n";
    std::cout << "  [x] Check for self-assignment\n";
    std::cout << "  [x] Clean up existing resources\n";
    std::cout << "  [x] Transfer ALL resources from source\n";
    std::cout << "  [x] Nullify ALL pointers in source\n";
    std::cout << "  [x] Reset ALL sizes/counts in source\n";
    std::cout << "  [x] Return *this\n\n";

    // 6. Common patterns
    std::cout << "6. COMMON PATTERNS:\n";
    std::cout << "\nMove Constructor Pattern:\n";
    std::cout << "T(T&& other) noexcept\n";
    std::cout << "    : member1(std::move(other.member1)),\n";
    std::cout << "      member2(std::move(other.member2)) {\n";
    std::cout << "    // Transfer resources\n";
    std::cout << "    // Nullify source\n";
    std::cout << "}\n\n";

    std::cout << "Move Assignment Pattern:\n";
    std::cout << "T& operator=(T&& other) noexcept {\n";
    std::cout << "    if (this != &other) {\n";
    std::cout << "        // Clean up current resources\n";
    std::cout << "        // Transfer from other\n";
    std::cout << "        // Nullify other\n";
    std::cout << "    }\n";
    std::cout << "    return *this;\n";
    std::cout << "}\n\n";

    // 7. Best practices
    std::cout << "7. BEST PRACTICES:\n";
    std::cout << "1. Always mark move operations noexcept\n";
    std::cout << "2. Move ALL members in move constructor initialization list\n";
    std::cout << "3. Check for self-assignment in move assignment\n";
    std::cout << "4. Leave moved-from object in valid but unspecified state\n";
    std::cout << "5. Don't throw exceptions in move operations\n";
    std::cout << "6. Nullify ALL pointers after transferring ownership\n";
    std::cout << "7. Reset ALL sizes and counts\n";
    std::cout << "8. Test with ASan and Valgrind for memory leaks\n";
    std::cout << "9. Implement both move constructor and move assignment\n";
    std::cout << "10. Consider Rule of Zero when possible\n";

    return 0;
}
