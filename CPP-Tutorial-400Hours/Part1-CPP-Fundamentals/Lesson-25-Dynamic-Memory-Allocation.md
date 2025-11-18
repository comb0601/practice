# Lesson 25: Dynamic Memory Allocation (new/delete)

**Duration**: 6 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Dynamic Memory
2. Stack vs Heap Memory
3. new and delete Operators
4. Dynamic Arrays
5. Memory Leaks and Prevention
6. RAII Principles
7. Best Practices
8. Exercises and Projects

---

## 1. Introduction to Dynamic Memory

### Why Dynamic Allocation?

Dynamic memory allocation allows you to allocate memory at runtime, enabling flexible data structures and efficient memory usage.

```cpp
#include <iostream>

int main()
{
    // Static allocation (compile-time size)
    int staticArray[5];

    // Dynamic allocation (runtime size)
    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;

    int* dynamicArray = new int[size];

    // Use the array
    for (int i = 0; i < size; i++)
    {
        dynamicArray[i] = i + 1;
        std::cout << dynamicArray[i] << " ";
    }
    std::cout << "\n";

    // Free memory
    delete[] dynamicArray;

    return 0;
}
```

---

## 2. Stack vs Heap Memory

### Understanding Memory Regions:

```cpp
#include <iostream>

int globalVar = 100;  // Global memory

void demonstrateMemory()
{
    int stackVar = 50;  // Stack memory

    int* heapVar = new int(25);  // Heap memory

    std::cout << "Global variable address: " << &globalVar << "\n";
    std::cout << "Stack variable address: " << &stackVar << "\n";
    std::cout << "Heap variable address: " << heapVar << "\n";

    delete heapVar;
}

int main()
{
    demonstrateMemory();
    return 0;
}
```

---

## 3. new and delete Operators

### Single Object Allocation:

```cpp
#include <iostream>

int main()
{
    // Allocate single integer
    int* ptr = new int;
    *ptr = 42;

    std::cout << "Value: " << *ptr << "\n";
    std::cout << "Address: " << ptr << "\n";

    delete ptr;  // Free memory

    // Allocate with initialization
    int* ptr2 = new int(100);
    std::cout << "Initialized value: " << *ptr2 << "\n";

    delete ptr2;

    // Modern C++ initialization
    int* ptr3 = new int{200};
    std::cout << "Modern init: " << *ptr3 << "\n";

    delete ptr3;

    return 0;
}
```

### Array Allocation:

```cpp
#include <iostream>

int main()
{
    // Dynamic array allocation
    int* arr = new int[5];

    // Initialize
    for (int i = 0; i < 5; i++)
    {
        arr[i] = i * 10;
    }

    // Use
    for (int i = 0; i < 5; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Free memory (note the [])
    delete[] arr;

    return 0;
}
```

---

## 4. Dynamic Arrays

### Dynamic 2D Arrays:

```cpp
#include <iostream>

int main()
{
    int rows = 3, cols = 4;

    // Allocate 2D array
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = new int[cols];
    }

    // Initialize
    int value = 1;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = value++;
        }
    }

    // Display
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << "\n";
    }

    // Free memory
    for (int i = 0; i < rows; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}
```

---

## 5. Memory Leaks and Prevention

### Common Memory Leak:

```cpp
#include <iostream>

void memoryLeak()
{
    int* ptr = new int(42);
    // Forgot to delete!
}  // Memory leaked!

void properCleanup()
{
    int* ptr = new int(42);
    std::cout << *ptr << "\n";
    delete ptr;  // Properly freed
}

int main()
{
    properCleanup();
    return 0;
}
```

### Double Delete Error:

```cpp
#include <iostream>

int main()
{
    int* ptr = new int(42);

    delete ptr;
    // delete ptr;  // ERROR: Double delete!

    ptr = nullptr;  // Safe practice
    delete ptr;     // OK (deleting nullptr is safe)

    return 0;
}
```

---

## 6. RAII Principles

### Resource Acquisition Is Initialization:

```cpp
#include <iostream>

class IntArray
{
private:
    int* data;
    int size;

public:
    IntArray(int s) : size(s)
    {
        data = new int[size];
        std::cout << "Array allocated\n";
    }

    ~IntArray()
    {
        delete[] data;
        std::cout << "Array deallocated\n";
    }

    int& operator[](int index)
    {
        return data[index];
    }
};

int main()
{
    IntArray arr(5);

    for (int i = 0; i < 5; i++)
    {
        arr[i] = i * 10;
    }

    for (int i = 0; i < 5; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Automatic cleanup when arr goes out of scope

    return 0;
}
```

---

## 7. Best Practices

### Safe Memory Management:

```cpp
#include <iostream>

class SafeIntArray
{
private:
    int* data;
    int size;

public:
    SafeIntArray(int s) : data(nullptr), size(s)
    {
        if (size > 0)
        {
            data = new int[size]();  // Zero-initialized
        }
    }

    ~SafeIntArray()
    {
        if (data != nullptr)
        {
            delete[] data;
            data = nullptr;
        }
    }

    // Prevent copying (or implement properly)
    SafeIntArray(const SafeIntArray&) = delete;
    SafeIntArray& operator=(const SafeIntArray&) = delete;

    int getSize() const { return size; }

    int& operator[](int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
};

int main()
{
    try
    {
        SafeIntArray arr(10);

        for (int i = 0; i < arr.getSize(); i++)
        {
            arr[i] = i * i;
        }

        for (int i = 0; i < arr.getSize(); i++)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
```

---

## Key Takeaways

1. new allocates memory on heap
2. delete frees allocated memory
3. Always pair new with delete
4. Use new[] and delete[] for arrays
5. Memory leaks occur when memory isn't freed
6. RAII helps prevent memory leaks
7. Modern C++ prefers smart pointers
8. Never delete the same pointer twice

---

## Next Lesson Preview

In **Lesson 26**, we'll explore:
- References fundamentals
- Reference vs pointer
- Reference parameters
- Reference return values
- Const references

**Dynamic memory mastered!**
