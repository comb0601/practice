# Lesson 54: Perfect Forwarding and Move Semantics

**Duration:** 2.5 hours

## Learning Objectives
- Understand rvalue references and move semantics
- Learn perfect forwarding with std::forward
- Master std::move and when to use it
- Implement move constructors and move assignment
- Optimize code with move semantics

## Introduction

Move semantics and perfect forwarding are crucial features introduced in C++11 that enable efficient transfer of resources and perfect argument forwarding in templates. They eliminate unnecessary copies and enable more expressive code.

## Lvalues vs Rvalues

```cpp
#include <iostream>

void process(int& lvalue) {
    std::cout << "Lvalue reference: " << lvalue << std::endl;
}

void process(int&& rvalue) {
    std::cout << "Rvalue reference: " << rvalue << std::endl;
}

int main() {
    int x = 10;
    process(x);          // Calls lvalue version
    process(20);         // Calls rvalue version
    process(x + 5);      // Calls rvalue version

    return 0;
}
```

**Output:**
```
Lvalue reference: 10
Rvalue reference: 20
Rvalue reference: 15
```

## Move Semantics Basics

### Move Constructor and Move Assignment

```cpp
#include <iostream>
#include <cstring>

class String {
private:
    char* data;
    size_t length;

public:
    // Constructor
    String(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        std::cout << "Constructor: " << data << std::endl;
    }

    // Copy constructor
    String(const String& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        std::cout << "Copy constructor: " << data << std::endl;
    }

    // Move constructor
    String(String&& other) noexcept {
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
        std::cout << "Move constructor: " << data << std::endl;
    }

    // Copy assignment
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
            std::cout << "Copy assignment: " << data << std::endl;
        }
        return *this;
    }

    // Move assignment
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            length = other.length;
            other.data = nullptr;
            other.length = 0;
            std::cout << "Move assignment: " << data << std::endl;
        }
        return *this;
    }

    ~Destructor() {
        std::cout << "Destructor: " << (data ? data : "null") << std::endl;
        delete[] data;
    }

    const char* c_str() const { return data ? data : ""; }
};

int main() {
    String s1("Hello");
    String s2(s1);              // Copy constructor
    String s3(std::move(s1));   // Move constructor

    String s4("World");
    s4 = s2;                    // Copy assignment
    s4 = std::move(s3);         // Move assignment

    return 0;
}
```

## Perfect Forwarding

### Basic Perfect Forwarding

```cpp
#include <iostream>
#include <utility>

void process(int& value) {
    std::cout << "Lvalue: " << value << std::endl;
}

void process(int&& value) {
    std::cout << "Rvalue: " << value << std::endl;
}

// Perfect forwarding template
template <typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));
}

int main() {
    int x = 10;

    wrapper(x);           // Forwards as lvalue
    wrapper(20);          // Forwards as rvalue
    wrapper(std::move(x)); // Forwards as rvalue

    return 0;
}
```

### Forwarding Multiple Arguments

```cpp
#include <iostream>
#include <utility>
#include <memory>

class Widget {
private:
    int value;
    std::string name;

public:
    Widget(int v, const std::string& n) : value(v), name(n) {
        std::cout << "Widget(" << value << ", " << name << ")" << std::endl;
    }

    void display() const {
        std::cout << "Widget: " << name << " = " << value << std::endl;
    }
};

// Factory function with perfect forwarding
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int main() {
    auto widget1 = make_unique<Widget>(42, "Answer");
    widget1->display();

    std::string name = "Pi";
    auto widget2 = make_unique<Widget>(314, name);
    widget2->display();

    auto widget3 = make_unique<Widget>(100, std::string("Temp"));
    widget3->display();

    return 0;
}
```

## std::move in Detail

```cpp
#include <iostream>
#include <vector>
#include <string>

class Resource {
private:
    std::vector<int> data;
    std::string name;

public:
    Resource(const std::string& n, size_t size)
        : name(n), data(size, 0) {
        std::cout << "Creating " << name << " with " << size << " elements" << std::endl;
    }

    // Copy constructor
    Resource(const Resource& other)
        : name(other.name), data(other.data) {
        std::cout << "Copying " << name << std::endl;
    }

    // Move constructor
    Resource(Resource&& other) noexcept
        : name(std::move(other.name)), data(std::move(other.data)) {
        std::cout << "Moving " << name << std::endl;
    }

    void setName(const std::string& n) { name = n; }
    const std::string& getName() const { return name; }
    size_t size() const { return data.size(); }
};

Resource createResource(const std::string& name) {
    return Resource(name, 1000);
}

int main() {
    // RVO (Return Value Optimization) - no move
    Resource r1 = createResource("r1");

    // Explicit move
    Resource r2 = std::move(r1);
    std::cout << "r1 size after move: " << r1.size() << std::endl;
    std::cout << "r2 size: " << r2.size() << std::endl;

    // Move into vector
    std::vector<Resource> resources;
    resources.push_back(std::move(r2));

    std::cout << "\nVector contains " << resources.size() << " resource(s)" << std::endl;

    return 0;
}
```

## Practical Example 1: Smart Buffer

```cpp
#include <iostream>
#include <algorithm>

template <typename T>
class Buffer {
private:
    T* data;
    size_t capacity;
    size_t count;

public:
    Buffer(size_t cap = 10)
        : data(new T[cap]), capacity(cap), count(0) {
        std::cout << "Buffer created with capacity " << capacity << std::endl;
    }

    // Copy constructor
    Buffer(const Buffer& other)
        : data(new T[other.capacity]),
          capacity(other.capacity),
          count(other.count) {
        std::copy(other.data, other.data + count, data);
        std::cout << "Buffer copied" << std::endl;
    }

    // Move constructor
    Buffer(Buffer&& other) noexcept
        : data(other.data),
          capacity(other.capacity),
          count(other.count) {
        other.data = nullptr;
        other.capacity = 0;
        other.count = 0;
        std::cout << "Buffer moved" << std::endl;
    }

    // Copy assignment
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            count = other.count;
            data = new T[capacity];
            std::copy(other.data, other.data + count, data);
            std::cout << "Buffer copy assigned" << std::endl;
        }
        return *this;
    }

    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            capacity = other.capacity;
            count = other.count;
            other.data = nullptr;
            other.capacity = 0;
            other.count = 0;
            std::cout << "Buffer move assigned" << std::endl;
        }
        return *this;
    }

    ~Buffer() {
        delete[] data;
    }

    void add(const T& value) {
        if (count < capacity) {
            data[count++] = value;
        }
    }

    void add(T&& value) {
        if (count < capacity) {
            data[count++] = std::move(value);
        }
    }

    size_t size() const { return count; }

    void print() const {
        std::cout << "Buffer[" << count << "/" << capacity << "]: ";
        for (size_t i = 0; i < count; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    Buffer<int> buf1(5);
    buf1.add(10);
    buf1.add(20);
    buf1.add(30);
    buf1.print();

    // Move construction
    Buffer<int> buf2 = std::move(buf1);
    buf2.print();
    std::cout << "buf1 size after move: " << buf1.size() << std::endl;

    // Move assignment
    Buffer<int> buf3(3);
    buf3 = std::move(buf2);
    buf3.print();

    return 0;
}
```

## Practical Example 2: Forwarding Wrapper

```cpp
#include <iostream>
#include <string>
#include <utility>
#include <chrono>

// Logger that perfectly forwards arguments
class Logger {
public:
    template <typename... Args>
    void log(Args&&... args) {
        auto now = std::chrono::system_clock::now();
        std::cout << "[LOG] ";
        logImpl(std::forward<Args>(args)...);
    }

private:
    void logImpl() {
        std::cout << std::endl;
    }

    template <typename T, typename... Args>
    void logImpl(T&& first, Args&&... rest) {
        std::cout << std::forward<T>(first) << " ";
        logImpl(std::forward<Args>(rest)...);
    }
};

int main() {
    Logger logger;

    logger.log("Application", "started");
    logger.log("Error", "code:", 404);

    std::string msg = "User logged in";
    logger.log(msg);  // Lvalue
    logger.log(std::string("Temporary message"));  // Rvalue

    return 0;
}
```

## Practical Example 3: Move-Only Type

```cpp
#include <iostream>
#include <vector>

class FileHandle {
private:
    int fd;
    std::string filename;

public:
    FileHandle(const std::string& fname, int descriptor)
        : filename(fname), fd(descriptor) {
        std::cout << "Opening file: " << filename << std::endl;
    }

    // Delete copy operations
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Move operations
    FileHandle(FileHandle&& other) noexcept
        : filename(std::move(other.filename)), fd(other.fd) {
        other.fd = -1;
        std::cout << "Moving file handle: " << filename << std::endl;
    }

    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            close();
            filename = std::move(other.filename);
            fd = other.fd;
            other.fd = -1;
        }
        return *this;
    }

    ~FileHandle() {
        close();
    }

    void close() {
        if (fd != -1) {
            std::cout << "Closing file: " << filename << std::endl;
            fd = -1;
        }
    }

    bool isOpen() const { return fd != -1; }
    const std::string& getFilename() const { return filename; }
};

int main() {
    std::vector<FileHandle> files;

    // Must use std::move because FileHandle is move-only
    files.push_back(FileHandle("file1.txt", 1));
    files.push_back(FileHandle("file2.txt", 2));

    FileHandle f3("file3.txt", 3);
    files.push_back(std::move(f3));

    std::cout << "\nFiles in vector:" << std::endl;
    for (const auto& file : files) {
        std::cout << "  " << file.getFilename()
                  << " (open: " << file.isOpen() << ")" << std::endl;
    }

    return 0;
}
```

## Practical Example 4: Efficient Container Operations

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Data {
private:
    std::string value;
    std::vector<int> numbers;

public:
    Data(const std::string& v, std::vector<int> nums)
        : value(v), numbers(std::move(nums)) {
        std::cout << "Data created: " << value << std::endl;
    }

    Data(const Data& other)
        : value(other.value), numbers(other.numbers) {
        std::cout << "Data copied: " << value << std::endl;
    }

    Data(Data&& other) noexcept
        : value(std::move(other.value)), numbers(std::move(other.numbers)) {
        std::cout << "Data moved: " << value << std::endl;
    }

    const std::string& getValue() const { return value; }
    size_t numbersCount() const { return numbers.size(); }
};

std::vector<Data> createDataset() {
    std::vector<Data> dataset;

    dataset.push_back(Data("A", {1, 2, 3}));
    dataset.push_back(Data("B", {4, 5, 6}));
    dataset.push_back(Data("C", {7, 8, 9}));

    return dataset;  // RVO
}

int main() {
    std::cout << "Creating dataset..." << std::endl;
    auto dataset = createDataset();

    std::cout << "\nDataset size: " << dataset.size() << std::endl;

    // Move within container
    std::cout << "\nMoving first element to back..." << std::endl;
    auto first = std::move(dataset[0]);
    dataset.push_back(std::move(first));

    std::cout << "\nFinal dataset:" << std::endl;
    for (const auto& data : dataset) {
        std::cout << "  " << data.getValue()
                  << " (" << data.numbersCount() << " numbers)" << std::endl;
    }

    return 0;
}
```

## Exercises

### Exercise 1: Implement Rule of Five
**Problem:** Create a class that manages a dynamic array and implements all five special member functions.

**Solution:**
```cpp
#include <iostream>
#include <algorithm>

template <typename T>
class DynamicArray {
private:
    T* data;
    size_t size;

public:
    // Constructor
    DynamicArray(size_t n = 0) : data(n ? new T[n] : nullptr), size(n) {
        std::cout << "Constructor: size=" << size << std::endl;
    }

    // Destructor
    ~DynamicArray() {
        std::cout << "Destructor: size=" << size << std::endl;
        delete[] data;
    }

    // Copy constructor
    DynamicArray(const DynamicArray& other)
        : data(other.size ? new T[other.size] : nullptr), size(other.size) {
        std::copy(other.data, other.data + size, data);
        std::cout << "Copy constructor: size=" << size << std::endl;
    }

    // Move constructor
    DynamicArray(DynamicArray&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        std::cout << "Move constructor: size=" << size << std::endl;
    }

    // Copy assignment
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = size ? new T[size] : nullptr;
            std::copy(other.data, other.data + size, data);
            std::cout << "Copy assignment: size=" << size << std::endl;
        }
        return *this;
    }

    // Move assignment
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            std::cout << "Move assignment: size=" << size << std::endl;
        }
        return *this;
    }

    size_t getSize() const { return size; }
};

int main() {
    DynamicArray<int> arr1(10);
    DynamicArray<int> arr2 = arr1;  // Copy
    DynamicArray<int> arr3 = std::move(arr1);  // Move

    std::cout << "arr1 size: " << arr1.getSize() << std::endl;
    std::cout << "arr3 size: " << arr3.getSize() << std::endl;

    return 0;
}
```

### Exercise 2: Perfect Forwarding Factory
**Problem:** Create a factory function that perfectly forwards arguments to constructors.

**Solution:**
```cpp
#include <iostream>
#include <memory>
#include <string>
#include <utility>

class Product {
private:
    int id;
    std::string name;
    double price;

public:
    Product(int i, const std::string& n, double p)
        : id(i), name(n), price(p) {
        std::cout << "Product(" << id << ", " << name << ", " << price << ")" << std::endl;
    }

    void display() const {
        std::cout << "ID: " << id << ", Name: " << name << ", Price: $" << price << std::endl;
    }
};

template <typename T, typename... Args>
std::unique_ptr<T> create(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int main() {
    auto p1 = create<Product>(1, "Widget", 19.99);
    p1->display();

    std::string name = "Gadget";
    auto p2 = create<Product>(2, name, 29.99);
    p2->display();

    auto p3 = create<Product>(3, std::string("Gizmo"), 39.99);
    p3->display();

    return 0;
}
```

### Exercise 3: Move-Aware Container
**Problem:** Create a simple vector-like container that efficiently handles move operations.

**Solution:**
```cpp
#include <iostream>
#include <algorithm>
#include <utility>

template <typename T>
class SimpleVector {
private:
    T* data;
    size_t capacity;
    size_t count;

public:
    SimpleVector() : data(nullptr), capacity(0), count(0) {}

    SimpleVector(size_t cap)
        : data(new T[cap]), capacity(cap), count(0) {}

    ~SimpleVector() {
        delete[] data;
    }

    SimpleVector(const SimpleVector& other)
        : data(new T[other.capacity]),
          capacity(other.capacity),
          count(other.count) {
        std::copy(other.data, other.data + count, data);
    }

    SimpleVector(SimpleVector&& other) noexcept
        : data(other.data),
          capacity(other.capacity),
          count(other.count) {
        other.data = nullptr;
        other.capacity = 0;
        other.count = 0;
    }

    SimpleVector& operator=(SimpleVector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            capacity = other.capacity;
            count = other.count;
            other.data = nullptr;
            other.capacity = 0;
            other.count = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        ensureCapacity();
        data[count++] = value;
    }

    void push_back(T&& value) {
        ensureCapacity();
        data[count++] = std::move(value);
    }

    size_t size() const { return count; }

private:
    void ensureCapacity() {
        if (count >= capacity) {
            size_t newCap = capacity == 0 ? 1 : capacity * 2;
            T* newData = new T[newCap];
            for (size_t i = 0; i < count; ++i) {
                newData[i] = std::move(data[i]);
            }
            delete[] data;
            data = newData;
            capacity = newCap;
        }
    }
};

int main() {
    SimpleVector<std::string> vec;
    vec.push_back("Hello");
    vec.push_back(std::string("World"));

    std::string temp = "Test";
    vec.push_back(temp);
    vec.push_back(std::move(temp));

    std::cout << "Vector size: " << vec.size() << std::endl;

    return 0;
}
```

### Exercise 4: Forwarding Logger
**Problem:** Create a logger that perfectly forwards all argument types.

**Solution:**
```cpp
#include <iostream>
#include <string>
#include <sstream>
#include <utility>

class Logger {
private:
    std::ostringstream buffer;

public:
    template <typename T>
    Logger& operator<<(T&& value) {
        buffer << std::forward<T>(value);
        return *this;
    }

    void flush() {
        std::cout << "[LOG] " << buffer.str() << std::endl;
        buffer.str("");
        buffer.clear();
    }

    template <typename... Args>
    void log(Args&&... args) {
        ((*this << std::forward<Args>(args)), ...);
        flush();
    }
};

int main() {
    Logger logger;

    logger.log("Error code: ", 404, " - Not found");
    logger.log("User: ", std::string("Alice"), " logged in");

    std::string msg = "System";
    logger.log(msg, " started successfully");

    return 0;
}
```

### Exercise 5: Swap with Move
**Problem:** Implement an efficient swap function using move semantics.

**Solution:**
```cpp
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template <typename T>
void mySwap(T& a, T& b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

class Heavy {
private:
    std::vector<int> data;
    std::string name;

public:
    Heavy(const std::string& n, size_t size)
        : name(n), data(size, 0) {
        std::cout << "Created " << name << " with " << size << " elements" << std::endl;
    }

    Heavy(const Heavy& other)
        : name(other.name), data(other.data) {
        std::cout << "Copied " << name << std::endl;
    }

    Heavy(Heavy&& other) noexcept
        : name(std::move(other.name)), data(std::move(other.data)) {
        std::cout << "Moved " << name << std::endl;
    }

    Heavy& operator=(Heavy&& other) noexcept {
        name = std::move(other.name);
        data = std::move(other.data);
        std::cout << "Move assigned " << name << std::endl;
        return *this;
    }

    const std::string& getName() const { return name; }
};

int main() {
    Heavy h1("Heavy1", 10000);
    Heavy h2("Heavy2", 20000);

    std::cout << "\nBefore swap:" << std::endl;
    std::cout << "h1: " << h1.getName() << std::endl;
    std::cout << "h2: " << h2.getName() << std::endl;

    std::cout << "\nSwapping..." << std::endl;
    mySwap(h1, h2);

    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "h1: " << h1.getName() << std::endl;
    std::cout << "h2: " << h2.getName() << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- Move semantics enable efficient resource transfer
- std::move casts to rvalue reference
- Perfect forwarding preserves value categories
- std::forward is essential in template functions
- Rule of Five for managing resources
- Move-only types for exclusive ownership

## Checklist

- [ ] Understand lvalue vs rvalue
- [ ] Can implement move constructor
- [ ] Can implement move assignment
- [ ] Know when to use std::move
- [ ] Understand perfect forwarding
- [ ] Can use std::forward correctly
- [ ] Completed all 5 exercises
- [ ] Understand reference collapsing
- [ ] Know the Rule of Five
- [ ] Ready to learn smart pointers

## Next Lesson

In Lesson 55, we'll explore **Smart Pointers** and modern memory management with unique_ptr, shared_ptr, and weak_ptr.
