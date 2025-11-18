# Lesson 07: Dynamic Memory Management
**Estimated Time: 4-5 hours**

## Table of Contents
1. [Introduction to Dynamic Memory](#introduction-to-dynamic-memory)
2. [Stack vs Heap Memory](#stack-vs-heap-memory)
3. [The new Operator](#the-new-operator)
4. [The delete Operator](#the-delete-operator)
5. [Dynamic Arrays](#dynamic-arrays)
6. [Memory Leaks](#memory-leaks)
7. [Dangling Pointers](#dangling-pointers)
8. [RAII Pattern](#raii-pattern)
9. [Smart Pointers Preview](#smart-pointers-preview)
10. [Complete Working Examples](#complete-working-examples)
11. [Common Mistakes](#common-mistakes)
12. [Exercises](#exercises)

## Introduction to Dynamic Memory

Dynamic memory allows you to allocate memory at runtime, whose size isn't known at compile time.

### Why Dynamic Memory?

**Static allocation (compile-time):**
```cpp
int arr[100];  // Size must be known at compile time
```

**Dynamic allocation (runtime):**
```cpp
int size;
std::cin >> size;
int* arr = new int[size];  // Size determined at runtime!
```

### Benefits:
1. **Flexible size**: Allocate exactly what you need
2. **Lifetime control**: Variables persist until explicitly deleted
3. **Large allocations**: Heap has more space than stack
4. **Data structures**: Build linked lists, trees, graphs

### Drawbacks:
1. **Manual management**: Must remember to free memory
2. **Slower**: Heap allocation slower than stack
3. **Fragmentation**: Can lead to memory fragmentation
4. **Memory leaks**: Forgot to delete = memory leak

## Stack vs Heap Memory

### Stack Memory

```cpp
void function()
{
    int x = 42;        // Stack allocation
    int arr[10];       // Stack allocation

    // Automatically destroyed when function returns
}
```

**Characteristics:**
- Fast allocation/deallocation
- Limited size (typically 1-8 MB)
- Automatic lifetime management
- LIFO (Last In, First Out)
- Variables destroyed when scope ends

### Heap Memory

```cpp
void function()
{
    int* ptr = new int(42);      // Heap allocation
    int* arr = new int[10];      // Heap allocation

    // Must manually delete!
    delete ptr;
    delete[] arr;
}
```

**Characteristics:**
- Slower allocation/deallocation
- Large size (limited by system RAM)
- Manual lifetime management
- Can persist beyond function scope
- Requires explicit deallocation

### Memory Diagram

```
High Address
┌─────────────────┐
│   Stack         │  Local variables, function calls
│   ↓             │  Grows downward
├─────────────────┤
│   (Free)        │
├─────────────────┤
│   Heap          │  Dynamic allocations (new/delete)
│   ↑             │  Grows upward
├─────────────────┤
│   Static/Global │  Global variables, static variables
├─────────────────┤
│   Code          │  Program instructions
└─────────────────┘
Low Address
```

## The new Operator

### Allocating Single Variables

```cpp
#include <iostream>

int main()
{
    // Allocate single integer (uninitialized)
    int* ptr1 = new int;

    // Allocate and initialize
    int* ptr2 = new int(42);

    // Allocate and initialize (C++11)
    int* ptr3 = new int{99};

    std::cout << "*ptr1: " << *ptr1 << " (garbage)\n";
    std::cout << "*ptr2: " << *ptr2 << "\n";
    std::cout << "*ptr3: " << *ptr3 << "\n";

    // Don't forget to delete!
    delete ptr1;
    delete ptr2;
    delete ptr3;

    return 0;
}
```

### Allocating Different Types

```cpp
#include <iostream>

int main()
{
    // Integer
    int* pInt = new int(10);

    // Double
    double* pDouble = new double(3.14);

    // Character
    char* pChar = new char('A');

    // Boolean
    bool* pBool = new bool(true);

    std::cout << "*pInt: " << *pInt << "\n";
    std::cout << "*pDouble: " << *pDouble << "\n";
    std::cout << "*pChar: " << *pChar << "\n";
    std::cout << "*pBool: " << *pBool << "\n";

    delete pInt;
    delete pDouble;
    delete pChar;
    delete pBool;

    return 0;
}
```

### new Can Fail

```cpp
#include <iostream>
#include <new>

int main()
{
    try
    {
        // Try to allocate huge amount
        long long* huge = new long long[1000000000000];
        delete[] huge;
    }
    catch (std::bad_alloc& e)
    {
        std::cout << "Allocation failed: " << e.what() << "\n";
    }

    // Alternative: nothrow version (returns nullptr on failure)
    int* ptr = new(std::nothrow) int[1000000000000];
    if (ptr == nullptr)
    {
        std::cout << "Allocation failed (nothrow)\n";
    }
    else
    {
        delete[] ptr;
    }

    return 0;
}
```

## The delete Operator

### Deleting Single Variables

```cpp
#include <iostream>

int main()
{
    int* ptr = new int(42);

    std::cout << "*ptr: " << *ptr << "\n";

    delete ptr;  // Free the memory

    // ptr still holds the address (dangling pointer!)
    // ptr = nullptr;  // Good practice

    return 0;
}
```

### delete vs delete[]

```cpp
#include <iostream>

int main()
{
    // Single object: use delete
    int* single = new int(42);
    delete single;  // Correct

    // Array: use delete[]
    int* arr = new int[10];
    delete[] arr;   // Correct

    // WRONG COMBINATIONS:
    // delete[] single;  // Undefined behavior!
    // delete arr;       // Undefined behavior!

    return 0;
}
```

**Critical Rule**: Match allocation with deallocation!
- `new` → `delete`
- `new[]` → `delete[]`

### Multiple Deletes

```cpp
#include <iostream>

int main()
{
    int* ptr = new int(42);

    delete ptr;   // OK: Free memory
    // delete ptr;   // ERROR: Double delete! Undefined behavior!

    ptr = nullptr;  // Good practice
    delete ptr;     // OK: Deleting nullptr is safe (does nothing)

    return 0;
}
```

## Dynamic Arrays

### Allocating Arrays

```cpp
#include <iostream>

int main()
{
    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;

    // Allocate array (uninitialized)
    int* arr = new int[size];

    // Fill array
    for (int i = 0; i < size; i++)
    {
        arr[i] = i + 1;
    }

    // Print array
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Free array
    delete[] arr;

    return 0;
}
```

### Initializing Dynamic Arrays

```cpp
#include <iostream>

int main()
{
    // Uninitialized (garbage values)
    int* arr1 = new int[5];

    // Initialize to zero (C++11)
    int* arr2 = new int[5]();

    // Initialize with values (C++11)
    int* arr3 = new int[5]{10, 20, 30, 40, 50};

    std::cout << "arr1 (uninitialized): ";
    for (int i = 0; i < 5; i++)
        std::cout << arr1[i] << " ";
    std::cout << "\n";

    std::cout << "arr2 (zero-initialized): ";
    for (int i = 0; i < 5; i++)
        std::cout << arr2[i] << " ";
    std::cout << "\n";

    std::cout << "arr3 (value-initialized): ";
    for (int i = 0; i < 5; i++)
        std::cout << arr3[i] << " ";
    std::cout << "\n";

    delete[] arr1;
    delete[] arr2;
    delete[] arr3;

    return 0;
}
```

### Resizing Arrays

```cpp
#include <iostream>

int* resizeArray(int* oldArray, int oldSize, int newSize)
{
    // Allocate new array
    int* newArray = new int[newSize];

    // Copy old data
    int copySize = (oldSize < newSize) ? oldSize : newSize;
    for (int i = 0; i < copySize; i++)
    {
        newArray[i] = oldArray[i];
    }

    // Initialize remaining elements (if growing)
    for (int i = oldSize; i < newSize; i++)
    {
        newArray[i] = 0;
    }

    // Delete old array
    delete[] oldArray;

    return newArray;
}

int main()
{
    int* arr = new int[5]{1, 2, 3, 4, 5};

    std::cout << "Original array: ";
    for (int i = 0; i < 5; i++)
        std::cout << arr[i] << " ";
    std::cout << "\n";

    // Resize to 10
    arr = resizeArray(arr, 5, 10);

    std::cout << "Resized array: ";
    for (int i = 0; i < 10; i++)
        std::cout << arr[i] << " ";
    std::cout << "\n";

    delete[] arr;

    return 0;
}
```

### 2D Dynamic Arrays

```cpp
#include <iostream>

int main()
{
    int rows = 3, cols = 4;

    // Allocate array of pointers
    int** matrix = new int*[rows];

    // Allocate each row
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = new int[cols];
    }

    // Fill matrix
    int value = 1;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = value++;
        }
    }

    // Print matrix
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << "\n";
    }

    // Delete each row
    for (int i = 0; i < rows; i++)
    {
        delete[] matrix[i];
    }

    // Delete array of pointers
    delete[] matrix;

    return 0;
}
```

## Memory Leaks

A memory leak occurs when allocated memory is not freed.

### Simple Memory Leak

```cpp
void leakExample()
{
    int* ptr = new int(42);
    // Forgot to delete!
}  // Memory leaked! ptr is destroyed but memory still allocated

int main()
{
    for (int i = 0; i < 1000000; i++)
    {
        leakExample();  // Leaks 4 bytes each iteration!
    }
    // After loop: 4 MB leaked!

    return 0;
}
```

### Common Leak Scenarios

```cpp
// Scenario 1: Lost pointer
void scenario1()
{
    int* ptr = new int(42);
    ptr = new int(99);  // LEAK! Lost reference to first allocation
    delete ptr;         // Only frees second allocation
}

// Scenario 2: Early return
void scenario2(bool condition)
{
    int* ptr = new int(42);

    if (condition)
    {
        return;  // LEAK! Forgot to delete before return
    }

    delete ptr;  // Only reached if condition is false
}

// Scenario 3: Exception thrown
void scenario3()
{
    int* ptr = new int(42);

    // If this throws exception, delete is never reached
    riskyOperation();  // LEAK if exception thrown!

    delete ptr;
}
```

### Detecting Memory Leaks

```cpp
#include <iostream>

class LeakDetector
{
private:
    static int allocCount;
    static int deleteCount;

public:
    static void* allocate(size_t size)
    {
        allocCount++;
        return ::operator new(size);
    }

    static void deallocate(void* ptr)
    {
        deleteCount++;
        ::operator delete(ptr);
    }

    static void report()
    {
        std::cout << "Allocations: " << allocCount << "\n";
        std::cout << "Deallocations: " << deleteCount << "\n";
        std::cout << "Leaked: " << (allocCount - deleteCount) << "\n";
    }
};

int LeakDetector::allocCount = 0;
int LeakDetector::deleteCount = 0;

// Usage example (simplified)
int main()
{
    int* p1 = new int(42);
    int* p2 = new int(99);

    delete p1;
    // Forgot to delete p2!

    LeakDetector::report();  // Will show 1 leak

    return 0;
}
```

### Preventing Leaks

```cpp
#include <iostream>

// Good: Always delete in same scope
void good1()
{
    int* ptr = new int(42);
    // Use ptr...
    delete ptr;  // Always paired with new
}

// Good: Use smart pointers (covered later)
#include <memory>
void good2()
{
    std::unique_ptr<int> ptr(new int(42));
    // Automatically deleted when ptr goes out of scope
}

// Good: RAII pattern
class SafeArray
{
private:
    int* data;
    int size;

public:
    SafeArray(int n) : size(n)
    {
        data = new int[n];
    }

    ~SafeArray()
    {
        delete[] data;  // Automatic cleanup
    }

    int& operator[](int index)
    {
        return data[index];
    }
};

int main()
{
    SafeArray arr(10);  // Automatic cleanup when arr goes out of scope
    arr[0] = 42;

    return 0;
}  // arr destroyed, memory freed automatically
```

## Dangling Pointers

A dangling pointer points to memory that has been freed.

### Creating Dangling Pointers

```cpp
#include <iostream>

int main()
{
    int* ptr = new int(42);

    delete ptr;  // Memory freed

    // ptr is now dangling!
    // std::cout << *ptr << "\n";  // UNDEFINED BEHAVIOR!
    // *ptr = 100;                  // UNDEFINED BEHAVIOR!

    ptr = nullptr;  // Good practice: nullify after delete

    return 0;
}
```

### Multiple Pointers to Same Memory

```cpp
#include <iostream>

int main()
{
    int* ptr1 = new int(42);
    int* ptr2 = ptr1;  // Both point to same memory

    delete ptr1;  // Memory freed
    ptr1 = nullptr;

    // ptr2 is now dangling!
    // std::cout << *ptr2 << "\n";  // UNDEFINED BEHAVIOR!

    ptr2 = nullptr;  // Should also nullify ptr2

    return 0;
}
```

### Preventing Dangling Pointers

```cpp
#include <iostream>

// Pattern 1: Nullify immediately after delete
void pattern1()
{
    int* ptr = new int(42);
    delete ptr;
    ptr = nullptr;  // Cannot accidentally use dangling pointer
}

// Pattern 2: Use smart pointers
#include <memory>
void pattern2()
{
    std::unique_ptr<int> ptr(new int(42));
    // Cannot manually delete, so no dangling pointer possible
}

// Pattern 3: Check before use
void pattern3(int* ptr)
{
    if (ptr != nullptr)
    {
        std::cout << *ptr << "\n";
    }
}
```

## RAII Pattern

**RAII**: Resource Acquisition Is Initialization

### Basic RAII Principle

Resources (memory, files, locks) are:
1. **Acquired** in constructor
2. **Released** in destructor
3. **Automatically managed** by object lifetime

### Simple RAII Example

```cpp
#include <iostream>

class IntArray
{
private:
    int* data;
    int size;

public:
    // Constructor: Acquire resource
    IntArray(int n) : size(n)
    {
        std::cout << "Allocating array of " << n << " integers\n";
        data = new int[n];
    }

    // Destructor: Release resource
    ~IntArray()
    {
        std::cout << "Deleting array\n";
        delete[] data;
    }

    // Prevent copying (for now)
    IntArray(const IntArray&) = delete;
    IntArray& operator=(const IntArray&) = delete;

    int& operator[](int index)
    {
        return data[index];
    }

    int getSize() const
    {
        return size;
    }
};

int main()
{
    {
        IntArray arr(10);  // Acquire
        arr[0] = 42;
        arr[1] = 99;
        std::cout << "Using array...\n";
    }  // Automatic release when arr goes out of scope

    std::cout << "Array automatically deleted!\n";

    return 0;
}
```

### RAII for File Handling

```cpp
#include <iostream>
#include <fstream>

class FileHandler
{
private:
    std::ofstream file;

public:
    FileHandler(const std::string& filename)
    {
        std::cout << "Opening file: " << filename << "\n";
        file.open(filename);
    }

    ~FileHandler()
    {
        std::cout << "Closing file\n";
        if (file.is_open())
        {
            file.close();
        }
    }

    void write(const std::string& text)
    {
        file << text;
    }
};

int main()
{
    {
        FileHandler fh("test.txt");
        fh.write("Hello, RAII!\n");
    }  // File automatically closed

    std::cout << "File closed automatically!\n";

    return 0;
}
```

### Benefits of RAII

1. **Exception safety**: Resources freed even if exception thrown
2. **No memory leaks**: Automatic cleanup
3. **Clear ownership**: Resource tied to object lifetime
4. **Less error-prone**: No manual delete needed

```cpp
#include <iostream>

class SafeResource
{
private:
    int* data;

public:
    SafeResource(int value)
    {
        data = new int(value);
        std::cout << "Resource acquired\n";
    }

    ~SafeResource()
    {
        delete data;
        std::cout << "Resource released\n";
    }

    void riskyOperation()
    {
        throw std::runtime_error("Something went wrong!");
    }
};

int main()
{
    try
    {
        SafeResource res(42);
        res.riskyOperation();  // Throws exception
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
    // Resource still properly destroyed despite exception!

    return 0;
}
```

## Smart Pointers Preview

Smart pointers automatically manage memory.

### unique_ptr

```cpp
#include <iostream>
#include <memory>

int main()
{
    // Create unique_ptr
    std::unique_ptr<int> ptr1(new int(42));

    // Or use make_unique (C++14)
    auto ptr2 = std::make_unique<int>(99);

    std::cout << "*ptr1: " << *ptr1 << "\n";
    std::cout << "*ptr2: " << *ptr2 << "\n";

    // Automatically deleted when ptr goes out of scope
    return 0;
}  // No manual delete needed!
```

### unique_ptr with Arrays

```cpp
#include <iostream>
#include <memory>

int main()
{
    // Dynamic array
    std::unique_ptr<int[]> arr(new int[5]{1, 2, 3, 4, 5});

    for (int i = 0; i < 5; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Automatically uses delete[] for arrays
    return 0;
}
```

### shared_ptr

```cpp
#include <iostream>
#include <memory>

int main()
{
    // Multiple pointers can share ownership
    std::shared_ptr<int> ptr1 = std::make_shared<int>(42);

    {
        std::shared_ptr<int> ptr2 = ptr1;  // Share ownership
        std::cout << "Reference count: " << ptr1.use_count() << "\n";  // 2
        std::cout << "*ptr2: " << *ptr2 << "\n";
    }  // ptr2 destroyed, but memory not freed yet

    std::cout << "Reference count: " << ptr1.use_count() << "\n";  // 1

    return 0;
}  // Memory freed when last shared_ptr destroyed
```

### Why Smart Pointers?

```cpp
// OLD WAY (manual memory management)
void oldWay()
{
    int* ptr = new int(42);
    // Must remember to delete!
    delete ptr;
}

// NEW WAY (automatic memory management)
void newWay()
{
    std::unique_ptr<int> ptr = std::make_unique<int>(42);
    // Automatically deleted!
}
```

## Complete Working Examples

### Example 1: Dynamic Vector Class

```cpp
#include <iostream>
#include <stdexcept>

class Vector
{
private:
    int* data;
    int size;
    int capacity;

    void resize(int newCapacity)
    {
        int* newData = new int[newCapacity];

        for (int i = 0; i < size; i++)
        {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Vector() : data(nullptr), size(0), capacity(0) {}

    ~Vector()
    {
        delete[] data;
    }

    void push_back(int value)
    {
        if (size == capacity)
        {
            int newCapacity = (capacity == 0) ? 1 : capacity * 2;
            resize(newCapacity);
        }

        data[size++] = value;
    }

    void pop_back()
    {
        if (size > 0)
        {
            size--;
        }
    }

    int& operator[](int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }

    void print() const
    {
        std::cout << "[";
        for (int i = 0; i < size; i++)
        {
            std::cout << data[i];
            if (i < size - 1)
                std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

int main()
{
    Vector vec;

    std::cout << "=== Dynamic Vector ===\n\n";

    // Add elements
    for (int i = 1; i <= 10; i++)
    {
        vec.push_back(i * 10);
        std::cout << "After push_back(" << (i * 10) << "): ";
        std::cout << "size=" << vec.getSize()
                  << ", capacity=" << vec.getCapacity() << "\n";
    }

    std::cout << "\nVector contents: ";
    vec.print();

    // Remove elements
    vec.pop_back();
    vec.pop_back();
    std::cout << "After 2 pop_back: ";
    vec.print();

    // Access elements
    vec[0] = 999;
    std::cout << "After vec[0] = 999: ";
    vec.print();

    return 0;
}
```

### Example 2: Dynamic String Class

```cpp
#include <iostream>
#include <cstring>

class String
{
private:
    char* data;
    int length;

public:
    // Default constructor
    String() : data(nullptr), length(0)
    {
        data = new char[1];
        data[0] = '\0';
    }

    // Constructor from C-string
    String(const char* str)
    {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }

    // Copy constructor
    String(const String& other)
    {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
    }

    // Destructor
    ~String()
    {
        delete[] data;
    }

    // Assignment operator
    String& operator=(const String& other)
    {
        if (this != &other)
        {
            delete[] data;

            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    // Get length
    int getLength() const
    {
        return length;
    }

    // Get C-string
    const char* c_str() const
    {
        return data;
    }

    // Concatenation
    String operator+(const String& other) const
    {
        String result;
        delete[] result.data;

        result.length = length + other.length;
        result.data = new char[result.length + 1];

        strcpy(result.data, data);
        strcat(result.data, other.data);

        return result;
    }

    // Print
    friend std::ostream& operator<<(std::ostream& os, const String& str)
    {
        os << str.data;
        return os;
    }
};

int main()
{
    std::cout << "=== Dynamic String Class ===\n\n";

    String s1("Hello");
    String s2("World");

    std::cout << "s1: " << s1 << "\n";
    std::cout << "s2: " << s2 << "\n";
    std::cout << "s1 length: " << s1.getLength() << "\n";

    String s3 = s1 + " " + s2;
    std::cout << "s1 + \" \" + s2: " << s3 << "\n";

    String s4 = s3;  // Copy constructor
    std::cout << "Copy of s3: " << s4 << "\n";

    s1 = s2;  // Assignment operator
    std::cout << "After s1 = s2: " << s1 << "\n";

    return 0;
}
```

### Example 3: Memory Pool Allocator

```cpp
#include <iostream>

class MemoryPool
{
private:
    struct Block
    {
        Block* next;
    };

    void* memory;
    Block* freeList;
    size_t blockSize;
    size_t poolSize;

public:
    MemoryPool(size_t blockSize, size_t numBlocks)
        : blockSize(blockSize), poolSize(numBlocks)
    {
        // Allocate entire pool
        memory = ::operator new(blockSize * numBlocks);

        // Initialize free list
        freeList = static_cast<Block*>(memory);
        Block* current = freeList;

        for (size_t i = 0; i < numBlocks - 1; i++)
        {
            current->next = reinterpret_cast<Block*>(
                reinterpret_cast<char*>(current) + blockSize
            );
            current = current->next;
        }
        current->next = nullptr;

        std::cout << "Memory pool created: " << numBlocks
                  << " blocks of " << blockSize << " bytes\n";
    }

    ~MemoryPool()
    {
        ::operator delete(memory);
        std::cout << "Memory pool destroyed\n";
    }

    void* allocate()
    {
        if (freeList == nullptr)
        {
            std::cout << "Pool exhausted!\n";
            return nullptr;
        }

        Block* block = freeList;
        freeList = freeList->next;

        std::cout << "Allocated block at " << block << "\n";
        return block;
    }

    void deallocate(void* ptr)
    {
        if (ptr == nullptr)
            return;

        Block* block = static_cast<Block*>(ptr);
        block->next = freeList;
        freeList = block;

        std::cout << "Deallocated block at " << ptr << "\n";
    }
};

int main()
{
    std::cout << "=== Memory Pool Example ===\n\n";

    MemoryPool pool(sizeof(int), 5);

    int* p1 = static_cast<int*>(pool.allocate());
    int* p2 = static_cast<int*>(pool.allocate());
    int* p3 = static_cast<int*>(pool.allocate());

    *p1 = 10;
    *p2 = 20;
    *p3 = 30;

    std::cout << "\nValues: " << *p1 << ", " << *p2 << ", " << *p3 << "\n\n";

    pool.deallocate(p2);
    pool.deallocate(p1);

    int* p4 = static_cast<int*>(pool.allocate());
    *p4 = 40;

    std::cout << "\nValue: " << *p4 << "\n\n";

    pool.deallocate(p3);
    pool.deallocate(p4);

    return 0;
}
```

### Example 4: Smart Pointer Implementation

```cpp
#include <iostream>

template<typename T>
class UniquePtr
{
private:
    T* ptr;

public:
    // Constructor
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}

    // Destructor
    ~UniquePtr()
    {
        delete ptr;
    }

    // Delete copy constructor and assignment
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move constructor
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    // Move assignment
    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (this != &other)
        {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Dereference
    T& operator*() const
    {
        return *ptr;
    }

    // Arrow operator
    T* operator->() const
    {
        return ptr;
    }

    // Get raw pointer
    T* get() const
    {
        return ptr;
    }

    // Release ownership
    T* release()
    {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // Reset pointer
    void reset(T* p = nullptr)
    {
        delete ptr;
        ptr = p;
    }

    // Bool conversion
    explicit operator bool() const
    {
        return ptr != nullptr;
    }
};

int main()
{
    std::cout << "=== Custom Smart Pointer ===\n\n";

    UniquePtr<int> ptr1(new int(42));
    std::cout << "*ptr1: " << *ptr1 << "\n";

    *ptr1 = 99;
    std::cout << "After *ptr1 = 99: " << *ptr1 << "\n";

    UniquePtr<int> ptr2(std::move(ptr1));  // Move ownership
    // ptr1 is now nullptr

    if (ptr2)
    {
        std::cout << "*ptr2: " << *ptr2 << "\n";
    }

    if (!ptr1)
    {
        std::cout << "ptr1 is null\n";
    }

    return 0;
}  // Automatic cleanup
```

### Example 5: Memory Leak Detector

```cpp
#include <iostream>
#include <map>
#include <string>

class MemoryTracker
{
private:
    struct AllocationInfo
    {
        size_t size;
        std::string file;
        int line;
    };

    static std::map<void*, AllocationInfo> allocations;
    static size_t totalAllocated;
    static size_t totalFreed;

public:
    static void* allocate(size_t size, const char* file, int line)
    {
        void* ptr = malloc(size);
        allocations[ptr] = {size, file, line};
        totalAllocated += size;

        std::cout << "ALLOC: " << size << " bytes at " << ptr
                  << " (" << file << ":" << line << ")\n";

        return ptr;
    }

    static void deallocate(void* ptr)
    {
        if (ptr == nullptr)
            return;

        auto it = allocations.find(ptr);
        if (it != allocations.end())
        {
            totalFreed += it->second.size;
            std::cout << "FREE: " << it->second.size << " bytes at " << ptr << "\n";
            allocations.erase(it);
        }
        else
        {
            std::cout << "ERROR: Freeing untracked pointer " << ptr << "\n";
        }

        free(ptr);
    }

    static void report()
    {
        std::cout << "\n=== Memory Report ===\n";
        std::cout << "Total allocated: " << totalAllocated << " bytes\n";
        std::cout << "Total freed: " << totalFreed << " bytes\n";
        std::cout << "Leaked: " << (totalAllocated - totalFreed) << " bytes\n";

        if (!allocations.empty())
        {
            std::cout << "\nMemory leaks detected:\n";
            for (const auto& pair : allocations)
            {
                std::cout << "  " << pair.second.size << " bytes at "
                          << pair.first << " (" << pair.second.file
                          << ":" << pair.second.line << ")\n";
            }
        }
        else
        {
            std::cout << "\nNo memory leaks detected!\n";
        }
    }
};

std::map<void*, MemoryTracker::AllocationInfo> MemoryTracker::allocations;
size_t MemoryTracker::totalAllocated = 0;
size_t MemoryTracker::totalFreed = 0;

#define TRACKED_NEW(type) \
    (type*)MemoryTracker::allocate(sizeof(type), __FILE__, __LINE__)

#define TRACKED_DELETE(ptr) \
    MemoryTracker::deallocate(ptr)

int main()
{
    std::cout << "=== Memory Leak Detector ===\n\n";

    int* p1 = TRACKED_NEW(int);
    int* p2 = TRACKED_NEW(int);
    int* p3 = TRACKED_NEW(int);

    *p1 = 10;
    *p2 = 20;
    *p3 = 30;

    TRACKED_DELETE(p1);
    TRACKED_DELETE(p2);
    // Forgot to delete p3!

    MemoryTracker::report();

    return 0;
}
```

## Common Mistakes

### 1. Forgetting to Delete

```cpp
// WRONG
void leak()
{
    int* ptr = new int(42);
    // Forgot to delete!
}

// CORRECT
void noLeak()
{
    int* ptr = new int(42);
    delete ptr;
}
```

### 2. Mismatched new/delete

```cpp
// WRONG
int* arr = new int[10];
delete arr;  // Should be delete[]!

// WRONG
int* ptr = new int(42);
delete[] ptr;  // Should be delete!

// CORRECT
int* arr = new int[10];
delete[] arr;

int* ptr = new int(42);
delete ptr;
```

### 3. Double Delete

```cpp
// WRONG
int* ptr = new int(42);
delete ptr;
delete ptr;  // CRASH!

// CORRECT
int* ptr = new int(42);
delete ptr;
ptr = nullptr;  // Prevent double delete
```

### 4. Using After Delete

```cpp
// WRONG
int* ptr = new int(42);
delete ptr;
std::cout << *ptr << "\n";  // Dangling pointer!

// CORRECT
int* ptr = new int(42);
int value = *ptr;  // Save value first
delete ptr;
ptr = nullptr;
std::cout << value << "\n";
```

### 5. Returning Local Array

```cpp
// WRONG
int* getBadArray()
{
    int arr[10];  // Stack array
    return arr;   // ERROR!
}

// CORRECT
int* getGoodArray()
{
    int* arr = new int[10];  // Heap array
    return arr;
}

// Even better: Use smart pointer or vector
std::unique_ptr<int[]> getBestArray()
{
    return std::make_unique<int[]>(10);
}
```

### 6. Shallow Copy Issues

```cpp
// WRONG - Shallow copy
class Bad
{
    int* data;
public:
    Bad(int n) { data = new int[n]; }
    ~Bad() { delete[] data; }
    // Default copy constructor does shallow copy!
};

void problem()
{
    Bad a(10);
    Bad b = a;  // Shallow copy!
}  // CRASH: Both destructors delete same memory!

// CORRECT - Deep copy
class Good
{
    int* data;
    int size;
public:
    Good(int n) : size(n) { data = new int[n]; }
    ~Good() { delete[] data; }

    // Copy constructor (deep copy)
    Good(const Good& other) : size(other.size)
    {
        data = new int[size];
        for (int i = 0; i < size; i++)
            data[i] = other.data[i];
    }
};
```

## Exercises

### Exercise 1: Dynamic Matrix Class
Implement a complete matrix class:
- Constructor: allocate rows × cols
- Destructor: free memory
- Copy constructor and assignment operator
- Methods: fill, print, transpose, add, multiply
- Proper memory management

### Exercise 2: Dynamic String Array
Create a dynamic array of strings:
- Add string (resize if needed)
- Remove string at index
- Find string
- Sort strings alphabetically
- Deallocate all memory properly

### Exercise 3: Memory Pool
Implement a simple memory pool:
- Fixed-size blocks
- Fast allocation/deallocation
- Track used/free blocks
- Prevent fragmentation
- Report pool statistics

### Exercise 4: Smart Pointer Array
Create `UniqueArray<T>` smart pointer for arrays:
- Constructor with size
- Array access operator[]
- Automatic cleanup
- Move semantics
- Bounds checking

### Exercise 5: Garbage Collector (Simple)
Implement a simple reference-counting collector:
- Track all allocations
- Reference counting
- Automatic deallocation when count reaches 0
- Detect circular references
- Memory leak report

## Summary

You learned:
- ✅ Stack vs heap memory
- ✅ new and delete operators
- ✅ Dynamic arrays and resizing
- ✅ Memory leaks and detection
- ✅ Dangling pointers
- ✅ RAII pattern for automatic cleanup
- ✅ Smart pointers (unique_ptr, shared_ptr)
- ✅ Common memory management mistakes
- ✅ Best practices for safe memory management

## Next Lesson Preview

In Lesson 08, we'll explore:
- Object-Oriented Programming basics
- Classes and objects
- Member variables and functions
- Access specifiers (public, private, protected)
- Encapsulation principles
- Getters and setters
- Complete class examples (BankAccount, Student, etc.)

---

**Congratulations on completing Lesson 07!**

**Practice time**: Spend 4+ hours implementing dynamic data structures. Memory management is crucial in C++. Practice until you can allocate and deallocate memory safely without leaks!
