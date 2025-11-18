# Lesson 29: Null Pointers and Pointer Safety

**Duration**: 5 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Introduction to Null Pointers
2. nullptr vs NULL vs 0
3. Checking for Null
4. Defensive Programming
5. Common Pointer Errors
6. Safe Pointer Patterns
7. Modern C++ Safety Features
8. Exercises and Projects

---

## 1. Introduction to Null Pointers

### What is a Null Pointer?

A null pointer doesn't point to any valid memory location. Dereferencing it causes undefined behavior.

```cpp
#include <iostream>

int main()
{
    int* ptr = nullptr;  // Null pointer

    std::cout << "Pointer value: " << ptr << "\n";

    // ❌ DANGEROUS: Dereferencing null pointer
    // std::cout << *ptr;  // Undefined behavior!

    // ✅ SAFE: Check before dereferencing
    if (ptr != nullptr)
    {
        std::cout << "Value: " << *ptr << "\n";
    }
    else
    {
        std::cout << "Pointer is null\n";
    }

    return 0;
}
```

---

## 2. nullptr vs NULL vs 0

### Modern C++ Null:

```cpp
#include <iostream>

void function(int x)
{
    std::cout << "Called with int: " << x << "\n";
}

void function(int* ptr)
{
    std::cout << "Called with pointer\n";
}

int main()
{
    // Modern C++ (C++11 and later)
    int* ptr1 = nullptr;  // ✅ Type-safe null pointer

    // Old C style
    int* ptr2 = NULL;     // Defined as 0 (macro)
    int* ptr3 = 0;        // Integer literal

    // Ambiguity with old style:
    // function(NULL);    // Which function? Depends on compiler!
    // function(0);       // Calls function(int)

    function(nullptr);    // ✅ Always calls function(int*)

    return 0;
}
```

---

## 3. Checking for Null

### Proper Null Checks:

```cpp
#include <iostream>

void safeAccess(int* ptr)
{
    // Explicit check
    if (ptr != nullptr)
    {
        std::cout << "Value: " << *ptr << "\n";
    }
    else
    {
        std::cout << "Null pointer!\n";
    }
}

void implicitCheck(int* ptr)
{
    // Implicit check (pointer as condition)
    if (ptr)  // True if not null
    {
        std::cout << "Value: " << *ptr << "\n";
    }
    else
    {
        std::cout << "Null pointer!\n";
    }
}

int main()
{
    int value = 42;
    int* ptr1 = &value;
    int* ptr2 = nullptr;

    safeAccess(ptr1);
    safeAccess(ptr2);

    implicitCheck(ptr1);
    implicitCheck(ptr2);

    return 0;
}
```

---

## 4. Defensive Programming

### Safe Pointer Practices:

```cpp
#include <iostream>

class SafePointerExample
{
private:
    int* data;

public:
    SafePointerExample() : data(nullptr)
    {
        std::cout << "Constructor: data initialized to nullptr\n";
    }

    void allocate(int size)
    {
        // Free existing memory first
        if (data != nullptr)
        {
            delete[] data;
        }

        data = new int[size];
        std::cout << "Allocated array of size " << size << "\n";
    }

    void setValue(int index, int value)
    {
        if (data != nullptr)
        {
            data[index] = value;
        }
        else
        {
            std::cerr << "Error: Cannot set value, data is null\n";
        }
    }

    ~SafePointerExample()
    {
        if (data != nullptr)
        {
            delete[] data;
            data = nullptr;  // Good practice
            std::cout << "Destructor: memory freed\n";
        }
    }
};

int main()
{
    SafePointerExample example;
    example.allocate(10);
    example.setValue(0, 42);

    return 0;
}
```

---

## 5. Common Pointer Errors

### Dangling Pointers:

```cpp
#include <iostream>

int* dangerousFunction()
{
    int local = 42;
    return &local;  // ❌ ERROR: Returning pointer to local variable!
}  // local is destroyed here!

int* safeFunction()
{
    static int value = 42;
    return &value;  // ✅ OK: static variable persists
}

int* betterFunction()
{
    int* ptr = new int(42);
    return ptr;  // ✅ OK: dynamically allocated
}  // Caller must delete!

int main()
{
    // int* bad = dangerousFunction();  // Undefined behavior!

    int* good1 = safeFunction();
    std::cout << *good1 << "\n";

    int* good2 = betterFunction();
    std::cout << *good2 << "\n";
    delete good2;  // Don't forget!

    return 0;
}
```

### Wild Pointers:

```cpp
#include <iostream>

int main()
{
    int* wild;  // ❌ Uninitialized pointer (dangerous!)

    // std::cout << *wild;  // Undefined behavior!

    int* safe = nullptr;  // ✅ Always initialize

    if (safe != nullptr)
    {
        std::cout << *safe << "\n";
    }

    return 0;
}
```

---

## 6. Safe Pointer Patterns

### RAII Pattern:

```cpp
#include <iostream>

class SafeArray
{
private:
    int* data;
    int size;

public:
    SafeArray(int s) : size(s)
    {
        data = new int[size]();
    }

    ~SafeArray()
    {
        delete[] data;
    }

    // Prevent copying
    SafeArray(const SafeArray&) = delete;
    SafeArray& operator=(const SafeArray&) = delete;

    int& operator[](int index)
    {
        if (index >= 0 && index < size)
        {
            return data[index];
        }
        throw std::out_of_range("Index out of bounds");
    }

    int getSize() const { return size; }
};

int main()
{
    SafeArray arr(10);

    for (int i = 0; i < arr.getSize(); i++)
    {
        arr[i] = i * i;
    }

    for (int i = 0; i < arr.getSize(); i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Automatic cleanup when arr goes out of scope

    return 0;
}
```

---

## 7. Modern C++ Safety Features

### Smart Pointers Preview:

```cpp
#include <iostream>
#include <memory>

int main()
{
    // unique_ptr: Exclusive ownership
    std::unique_ptr<int> ptr1(new int(42));
    std::cout << "ptr1: " << *ptr1 << "\n";

    // Automatic cleanup (no need for delete!)

    // shared_ptr: Shared ownership
    std::shared_ptr<int> ptr2 = std::make_shared<int>(100);
    std::cout << "ptr2: " << *ptr2 << "\n";

    // Multiple pointers can share ownership
    std::shared_ptr<int> ptr3 = ptr2;
    std::cout << "ptr3: " << *ptr3 << "\n";

    // Memory freed when last shared_ptr is destroyed

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise: Safe Pointer Manager

```cpp
#include <iostream>

template<typename T>
class SafePointer
{
private:
    T* ptr;

public:
    SafePointer() : ptr(nullptr) {}

    SafePointer(T* p) : ptr(p) {}

    ~SafePointer()
    {
        if (ptr != nullptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }

    T& operator*()
    {
        if (ptr == nullptr)
        {
            throw std::runtime_error("Dereferencing null pointer");
        }
        return *ptr;
    }

    T* operator->()
    {
        return ptr;
    }

    bool isNull() const
    {
        return ptr == nullptr;
    }

    void reset(T* p = nullptr)
    {
        if (ptr != nullptr)
        {
            delete ptr;
        }
        ptr = p;
    }

    // Prevent copying
    SafePointer(const SafePointer&) = delete;
    SafePointer& operator=(const SafePointer&) = delete;
};

int main()
{
    SafePointer<int> ptr(new int(42));

    std::cout << "Value: " << *ptr << "\n";

    if (!ptr.isNull())
    {
        std::cout << "Pointer is valid\n";
    }

    ptr.reset(new int(100));
    std::cout << "New value: " << *ptr << "\n";

    // Automatic cleanup

    return 0;
}
```

---

## Key Takeaways

1. Always initialize pointers (use nullptr)
2. Check for null before dereferencing
3. Use nullptr instead of NULL or 0
4. Avoid returning pointers to local variables
5. Set pointers to nullptr after delete
6. Use RAII for automatic cleanup
7. Consider smart pointers for safety
8. Defensive programming prevents crashes

---

## Summary Checklist

Before moving to Lesson 30, ensure you can:
- [ ] Initialize pointers properly
- [ ] Check for null before dereferencing
- [ ] Use nullptr correctly
- [ ] Avoid dangling and wild pointers
- [ ] Implement safe pointer patterns
- [ ] Understand common pointer errors
- [ ] Apply defensive programming
- [ ] Recognize when to use smart pointers

---

## Next Lesson Preview

In **Lesson 30**, we'll explore:
- Smart pointers in detail
- unique_ptr
- shared_ptr
- weak_ptr
- Modern C++ memory management

**Pointer safety mastered!**
