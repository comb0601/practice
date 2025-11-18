# Lesson 21: Advanced Classes and Objects

## Overview
Building upon the OOP fundamentals from Part 1, this lesson explores advanced class design, the Rule of Three/Five/Zero, copy semantics, move semantics, and RAII (Resource Acquisition Is Initialization). You'll learn professional C++ class design patterns used in production code.

## Topics Covered
- Copy semantics (copy constructor, copy assignment)
- Rule of Three, Rule of Five, Rule of Zero
- RAII (Resource Acquisition Is Initialization)
- Deleted and defaulted functions
- Object lifecycle management
- Value semantics vs reference semantics
- Const correctness in classes
- Static class members in depth

---

## 1. Copy Constructor and Copy Assignment

### Example 1: Deep Copy vs Shallow Copy

```cpp
#include <iostream>
#include <cstring>

// BAD: Shallow copy (default behavior)
class ShallowString {
private:
    char* data;

public:
    ShallowString(const char* str) {
        data = new char[std::strlen(str) + 1];
        std::strcpy(data, str);
        std::cout << "Constructor: allocated " << data << std::endl;
    }

    // Using default copy constructor (DANGEROUS with raw pointers!)
    // ShallowString(const ShallowString& other) = default;

    ~ShallowString() {
        std::cout << "Destructor: deleting " << data << std::endl;
        delete[] data;
    }

    const char* get() const { return data; }
};

// GOOD: Deep copy (custom implementation)
class DeepString {
private:
    char* data;
    size_t length;

public:
    // Constructor
    DeepString(const char* str) {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
        std::cout << "Constructor: " << data << " at " << (void*)data << std::endl;
    }

    // Copy constructor (deep copy)
    DeepString(const DeepString& other) {
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);
        std::cout << "Copy constructor: copied " << data
                  << " from " << (void*)other.data
                  << " to " << (void*)data << std::endl;
    }

    // Copy assignment operator
    DeepString& operator=(const DeepString& other) {
        std::cout << "Copy assignment operator called" << std::endl;

        // Check for self-assignment
        if (this == &other) {
            return *this;
        }

        // Free existing resource
        delete[] data;

        // Copy from other
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);

        return *this;
    }

    // Destructor
    ~DeepString() {
        std::cout << "Destructor: " << data << " at " << (void*)data << std::endl;
        delete[] data;
    }

    const char* get() const { return data; }
};

int main() {
    std::cout << "=== Deep Copy Example ===" << std::endl;

    DeepString str1("Hello");
    DeepString str2 = str1;  // Copy constructor
    DeepString str3("World");
    str3 = str1;  // Copy assignment

    std::cout << "str1: " << str1.get() << std::endl;
    std::cout << "str2: " << str2.get() << std::endl;
    std::cout << "str3: " << str3.get() << std::endl;

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example1.cpp -o example1`

---

## 2. The Rule of Three

### Example 2: Implementing Rule of Three

```cpp
#include <iostream>
#include <algorithm>

// Rule of Three: If you define one of these, you should define all three:
// 1. Destructor
// 2. Copy constructor
// 3. Copy assignment operator

template<typename T>
class DynamicArray {
private:
    T* data;
    size_t size;
    size_t capacity;

    void resize() {
        capacity = capacity == 0 ? 1 : capacity * 2;
        T* new_data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
    }

public:
    // Constructor
    DynamicArray() : data(nullptr), size(0), capacity(0) {
        std::cout << "Default constructor" << std::endl;
    }

    explicit DynamicArray(size_t initial_capacity)
        : data(new T[initial_capacity]),
          size(0),
          capacity(initial_capacity) {
        std::cout << "Parameterized constructor (capacity: " << capacity << ")" << std::endl;
    }

    // 1. Destructor
    ~DynamicArray() {
        std::cout << "Destructor (size: " << size << ", capacity: " << capacity << ")" << std::endl;
        delete[] data;
    }

    // 2. Copy constructor
    DynamicArray(const DynamicArray& other)
        : data(new T[other.capacity]),
          size(other.size),
          capacity(other.capacity) {
        std::cout << "Copy constructor (copying " << size << " elements)" << std::endl;
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // 3. Copy assignment operator
    DynamicArray& operator=(const DynamicArray& other) {
        std::cout << "Copy assignment operator" << std::endl;

        if (this == &other) {
            return *this;
        }

        // Free existing resource
        delete[] data;

        // Copy from other
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }

        return *this;
    }

    // Member functions
    void push_back(const T& value) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = value;
    }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    size_t get_size() const { return size; }
    size_t get_capacity() const { return capacity; }

    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

int main() {
    std::cout << "=== Rule of Three Example ===" << std::endl;

    DynamicArray<int> arr1(5);
    arr1.push_back(10);
    arr1.push_back(20);
    arr1.push_back(30);

    std::cout << "\nOriginal array:" << std::endl;
    arr1.print();

    std::cout << "\nCopy construction:" << std::endl;
    DynamicArray<int> arr2 = arr1;
    arr2.print();

    std::cout << "\nCopy assignment:" << std::endl;
    DynamicArray<int> arr3;
    arr3 = arr1;
    arr3.print();

    std::cout << "\nModifying arr2:" << std::endl;
    arr2.push_back(40);
    std::cout << "arr1: "; arr1.print();
    std::cout << "arr2: "; arr2.print();
    std::cout << "arr3: "; arr3.print();

    std::cout << "\n=== Objects going out of scope ===" << std::endl;
    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example2.cpp -o example2`

---

## 3. RAII (Resource Acquisition Is Initialization)

### Example 3: RAII Pattern

```cpp
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <mutex>

// RAII: File Handle Wrapper
class FileHandle {
private:
    std::FILE* file;
    std::string filename;

public:
    FileHandle(const std::string& name, const char* mode)
        : file(std::fopen(name.c_str(), mode)), filename(name) {
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        std::cout << "File opened: " << filename << std::endl;
    }

    ~FileHandle() {
        if (file) {
            std::fclose(file);
            std::cout << "File closed: " << filename << std::endl;
        }
    }

    // Delete copy operations (files shouldn't be copied)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    std::FILE* get() { return file; }

    void write(const std::string& data) {
        if (file) {
            std::fputs(data.c_str(), file);
        }
    }

    std::string read() {
        std::string result;
        if (file) {
            char buffer[256];
            while (std::fgets(buffer, sizeof(buffer), file)) {
                result += buffer;
            }
        }
        return result;
    }
};

// RAII: Scoped Lock (like std::lock_guard)
template<typename Mutex>
class ScopedLock {
private:
    Mutex& mutex;

public:
    explicit ScopedLock(Mutex& m) : mutex(m) {
        mutex.lock();
        std::cout << "Lock acquired" << std::endl;
    }

    ~ScopedLock() {
        mutex.unlock();
        std::cout << "Lock released" << std::endl;
    }

    // Delete copy operations
    ScopedLock(const ScopedLock&) = delete;
    ScopedLock& operator=(const ScopedLock&) = delete;
};

// RAII: Memory Management
template<typename T>
class ScopedArray {
private:
    T* ptr;
    size_t size;

public:
    explicit ScopedArray(size_t n) : ptr(new T[n]), size(n) {
        std::cout << "Allocated array of " << size << " elements" << std::endl;
    }

    ~ScopedArray() {
        delete[] ptr;
        std::cout << "Deallocated array of " << size << " elements" << std::endl;
    }

    // Delete copy operations
    ScopedArray(const ScopedArray&) = delete;
    ScopedArray& operator=(const ScopedArray&) = delete;

    T& operator[](size_t index) { return ptr[index]; }
    const T& operator[](size_t index) const { return ptr[index]; }

    size_t get_size() const { return size; }
};

void demonstrateRAII() {
    std::cout << "=== RAII File Handle ===" << std::endl;
    try {
        FileHandle file("test.txt", "w");
        file.write("Hello, RAII!\n");
        file.write("Automatic resource management.\n");
        // File automatically closed when 'file' goes out of scope
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n=== RAII Scoped Lock ===" << std::endl;
    std::mutex mtx;
    {
        ScopedLock<std::mutex> lock(mtx);
        std::cout << "Critical section" << std::endl;
        // Lock automatically released when 'lock' goes out of scope
    }

    std::cout << "\n=== RAII Scoped Array ===" << std::endl;
    {
        ScopedArray<int> arr(10);
        for (size_t i = 0; i < arr.get_size(); ++i) {
            arr[i] = i * 10;
        }
        std::cout << "Array contents: ";
        for (size_t i = 0; i < arr.get_size(); ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
        // Array automatically deallocated when 'arr' goes out of scope
    }
}

int main() {
    demonstrateRAII();
    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example3.cpp -o example3`

---

## 4. The Rule of Five (C++11)

### Example 4: Move Semantics and Rule of Five

```cpp
#include <iostream>
#include <utility>  // for std::move
#include <algorithm>

// Rule of Five adds:
// 4. Move constructor
// 5. Move assignment operator

template<typename T>
class Buffer {
private:
    T* data;
    size_t size;

public:
    // Constructor
    explicit Buffer(size_t n = 0) : data(n ? new T[n] : nullptr), size(n) {
        std::cout << "Constructor (size: " << size << ")" << std::endl;
    }

    // 1. Destructor
    ~Buffer() {
        std::cout << "Destructor (size: " << size << ")" << std::endl;
        delete[] data;
    }

    // 2. Copy constructor
    Buffer(const Buffer& other) : data(new T[other.size]), size(other.size) {
        std::cout << "Copy constructor (size: " << size << ")" << std::endl;
        std::copy(other.data, other.data + size, data);
    }

    // 3. Copy assignment
    Buffer& operator=(const Buffer& other) {
        std::cout << "Copy assignment (size: " << other.size << ")" << std::endl;
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new T[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // 4. Move constructor
    Buffer(Buffer&& other) noexcept
        : data(other.data), size(other.size) {
        std::cout << "Move constructor (size: " << size << ")" << std::endl;
        other.data = nullptr;
        other.size = 0;
    }

    // 5. Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        std::cout << "Move assignment (size: " << other.size << ")" << std::endl;
        if (this != &other) {
            delete[] data;

            data = other.data;
            size = other.size;

            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // Accessors
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    size_t get_size() const { return size; }

    void print() const {
        std::cout << "Buffer [";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

Buffer<int> createBuffer(size_t n) {
    Buffer<int> buf(n);
    for (size_t i = 0; i < n; ++i) {
        buf[i] = i * 10;
    }
    return buf;  // Move constructor called (RVO may optimize this away)
}

int main() {
    std::cout << "=== Rule of Five Example ===" << std::endl;

    std::cout << "\n1. Regular construction:" << std::endl;
    Buffer<int> buf1(3);
    buf1[0] = 1; buf1[1] = 2; buf1[2] = 3;
    buf1.print();

    std::cout << "\n2. Copy construction:" << std::endl;
    Buffer<int> buf2 = buf1;  // Copy constructor
    buf2.print();

    std::cout << "\n3. Copy assignment:" << std::endl;
    Buffer<int> buf3(2);
    buf3 = buf1;  // Copy assignment
    buf3.print();

    std::cout << "\n4. Move construction:" << std::endl;
    Buffer<int> buf4 = std::move(buf1);  // Move constructor
    std::cout << "buf1 after move: size = " << buf1.get_size() << std::endl;
    buf4.print();

    std::cout << "\n5. Move assignment:" << std::endl;
    Buffer<int> buf5(1);
    buf5 = std::move(buf2);  // Move assignment
    std::cout << "buf2 after move: size = " << buf2.get_size() << std::endl;
    buf5.print();

    std::cout << "\n6. Return value optimization:" << std::endl;
    Buffer<int> buf6 = createBuffer(5);
    buf6.print();

    std::cout << "\n=== Destruction phase ===" << std::endl;
    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example4.cpp -o example4`

---

## 5. The Rule of Zero

### Example 5: Using Standard Library (No Manual Memory Management)

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <memory>

// Rule of Zero: If you can use standard library containers,
// you don't need to define any special member functions

class Student {
private:
    std::string name;
    int age;
    std::vector<double> grades;
    // Using std::string and std::vector means compiler-generated
    // special member functions are sufficient!

public:
    Student(const std::string& n, int a)
        : name(n), age(a) {
        std::cout << "Constructor: " << name << std::endl;
    }

    // No need to define:
    // - Destructor
    // - Copy constructor
    // - Copy assignment
    // - Move constructor
    // - Move assignment
    // Compiler-generated ones work perfectly!

    void addGrade(double grade) {
        grades.push_back(grade);
    }

    double getAverage() const {
        if (grades.empty()) return 0.0;
        double sum = 0.0;
        for (double grade : grades) {
            sum += grade;
        }
        return sum / grades.size();
    }

    void print() const {
        std::cout << name << " (age " << age << "): ";
        for (double grade : grades) {
            std::cout << grade << " ";
        }
        std::cout << "| Average: " << getAverage() << std::endl;
    }

    const std::string& getName() const { return name; }
};

// Another Rule of Zero example with smart pointers
class Department {
private:
    std::string name;
    std::vector<std::shared_ptr<Student>> students;

public:
    explicit Department(const std::string& n) : name(n) {}

    // Compiler-generated special members are perfect!

    void addStudent(std::shared_ptr<Student> student) {
        students.push_back(student);
    }

    void printAll() const {
        std::cout << "\nDepartment: " << name << std::endl;
        std::cout << "Students:" << std::endl;
        for (const auto& student : students) {
            std::cout << "  ";
            student->print();
        }
    }

    size_t getStudentCount() const { return students.size(); }
};

int main() {
    std::cout << "=== Rule of Zero Example ===" << std::endl;

    // Create students
    auto student1 = std::make_shared<Student>("Alice", 20);
    student1->addGrade(85.5);
    student1->addGrade(92.0);
    student1->addGrade(88.5);

    auto student2 = std::make_shared<Student>("Bob", 21);
    student2->addGrade(78.0);
    student2->addGrade(82.5);
    student2->addGrade(80.0);

    // Test copying (works automatically!)
    std::cout << "\n--- Testing Copy ---" << std::endl;
    Student student3 = *student1;  // Copy construction
    student3.print();
    student1->print();

    // Create department
    Department cs("Computer Science");
    cs.addStudent(student1);
    cs.addStudent(student2);
    cs.printAll();

    // Test department copying (works automatically!)
    std::cout << "\n--- Testing Department Copy ---" << std::endl;
    Department cs_copy = cs;  // Copy construction
    std::cout << "Original department has " << cs.getStudentCount() << " students" << std::endl;
    std::cout << "Copied department has " << cs_copy.getStudentCount() << " students" << std::endl;

    // Modify original
    auto student4 = std::make_shared<Student>("Charlie", 22);
    student4->addGrade(95.0);
    cs.addStudent(student4);

    std::cout << "\nAfter adding student to original:" << std::endl;
    std::cout << "Original: " << cs.getStudentCount() << " students" << std::endl;
    std::cout << "Copy: " << cs_copy.getStudentCount() << " students" << std::endl;

    // Both share the same student objects (shared_ptr behavior)
    cs.printAll();
    cs_copy.printAll();

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example5.cpp -o example5`

---

## Project: Resource-Managing Vector Class

A complete implementation of a vector-like container demonstrating all special member functions.

```cpp
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

template<typename T>
class Vector {
private:
    T* data;
    size_t size;
    size_t capacity;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        size_t elements_to_copy = std::min(size, new_capacity);

        for (size_t i = 0; i < elements_to_copy; ++i) {
            new_data[i] = std::move(data[i]);
        }

        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    // Default constructor
    Vector() : data(nullptr), size(0), capacity(0) {}

    // Constructor with initial capacity
    explicit Vector(size_t initial_capacity)
        : data(new T[initial_capacity]), size(0), capacity(initial_capacity) {}

    // Constructor with size and default value
    Vector(size_t n, const T& value)
        : data(new T[n]), size(n), capacity(n) {
        for (size_t i = 0; i < n; ++i) {
            data[i] = value;
        }
    }

    // Initializer list constructor
    Vector(std::initializer_list<T> init)
        : data(new T[init.size()]), size(init.size()), capacity(init.size()) {
        size_t i = 0;
        for (const auto& value : init) {
            data[i++] = value;
        }
    }

    // Destructor
    ~Vector() {
        delete[] data;
    }

    // Copy constructor
    Vector(const Vector& other)
        : data(new T[other.capacity]), size(other.size), capacity(other.capacity) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Copy assignment
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move constructor
    Vector(Vector&& other) noexcept
        : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // Move assignment
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;

            data = other.data;
            size = other.size;
            capacity = other.capacity;

            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    // Element access
    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    T& at(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Capacity
    size_t get_size() const { return size; }
    size_t get_capacity() const { return capacity; }
    bool empty() const { return size == 0; }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity) {
            reallocate(new_capacity);
        }
    }

    void shrink_to_fit() {
        if (size < capacity) {
            reallocate(size);
        }
    }

    // Modifiers
    void push_back(const T& value) {
        if (size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }

    void push_back(T&& value) {
        if (size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = std::move(value);
    }

    void pop_back() {
        if (size > 0) {
            --size;
        }
    }

    void clear() {
        size = 0;
    }

    void resize(size_t new_size, const T& value = T()) {
        if (new_size > capacity) {
            reserve(new_size);
        }
        for (size_t i = size; i < new_size; ++i) {
            data[i] = value;
        }
        size = new_size;
    }

    // Iterators (simple implementation)
    T* begin() { return data; }
    const T* begin() const { return data; }
    T* end() { return data + size; }
    const T* end() const { return data + size; }

    // Utility
    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "] (size: " << size << ", capacity: " << capacity << ")" << std::endl;
    }
};

int main() {
    std::cout << "=== Custom Vector Implementation ===" << std::endl;

    // Test construction
    std::cout << "\n1. Default construction:" << std::endl;
    Vector<int> v1;
    v1.print();

    std::cout << "\n2. Initializer list construction:" << std::endl;
    Vector<int> v2 = {1, 2, 3, 4, 5};
    v2.print();

    std::cout << "\n3. Push back:" << std::endl;
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);
    v1.print();

    std::cout << "\n4. Copy construction:" << std::endl;
    Vector<int> v3 = v2;
    v3.print();

    std::cout << "\n5. Copy assignment:" << std::endl;
    Vector<int> v4;
    v4 = v2;
    v4.print();

    std::cout << "\n6. Move construction:" << std::endl;
    Vector<int> v5 = std::move(v3);
    std::cout << "v3 after move: "; v3.print();
    std::cout << "v5: "; v5.print();

    std::cout << "\n7. Modify and resize:" << std::endl;
    v2[0] = 99;
    v2.push_back(6);
    v2.push_back(7);
    v2.print();

    std::cout << "\n8. Range-based for loop:" << std::endl;
    std::cout << "Elements: ";
    for (const auto& elem : v2) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    std::cout << "\n9. Element access:" << std::endl;
    try {
        std::cout << "v2[2] = " << v2[2] << std::endl;
        std::cout << "v2.at(3) = " << v2.at(3) << std::endl;
        // std::cout << "v2.at(100) = " << v2.at(100) << std::endl;  // throws
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n10. Clear and shrink:" << std::endl;
    v2.clear();
    v2.print();
    v2.push_back(1);
    v2.push_back(2);
    v2.shrink_to_fit();
    v2.print();

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall project.cpp -o project`

---

## Summary

In this lesson, you learned about:
- Deep copy vs shallow copy
- Copy constructor and copy assignment operator
- Rule of Three: Destructor, Copy Constructor, Copy Assignment
- RAII (Resource Acquisition Is Initialization) pattern
- Rule of Five: Adding Move Constructor and Move Assignment
- Rule of Zero: Using standard library containers
- Deleted and defaulted special member functions
- Const correctness and best practices

## Key Takeaways

1. **RAII** is fundamental to C++ resource management
2. **Rule of Zero** is preferred when possible
3. **Move semantics** enable efficient resource transfer
4. **Copy semantics** require careful implementation for resources
5. Always implement **const correctness** in your classes

## Exercises

1. Implement a `UniquePtr` class template (similar to `std::unique_ptr`)
2. Create a `SharedPtr` class with reference counting
3. Build a `String` class with SSO (Small String Optimization)
4. Implement a `Matrix` class with proper copy and move semantics
5. Create a resource pool using RAII principles

## Next Lesson
Lesson 22: Constructors and Destructors (Advanced Patterns)
