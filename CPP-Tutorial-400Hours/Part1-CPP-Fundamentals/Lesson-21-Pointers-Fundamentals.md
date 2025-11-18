# Lesson 21: Pointers Fundamentals

**Duration**: 6 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Introduction to Pointers
2. Memory Addresses
3. Pointer Declaration and Initialization
4. Dereferencing Pointers
5. Pointers and Variables
6. Null Pointers
7. Pointer Best Practices
8. Exercises and Projects

---

## 1. Introduction to Pointers

### What is a Pointer?

A pointer is a variable that stores the memory address of another variable.

```cpp
#include <iostream>

int main()
{
    int number = 42;
    int* ptr = &number;  // ptr stores the address of number

    std::cout << "Value of number: " << number << "\n";
    std::cout << "Address of number: " << &number << "\n";
    std::cout << "Value of ptr (address): " << ptr << "\n";
    std::cout << "Value pointed to by ptr: " << *ptr << "\n";

    return 0;
}
```

**Output** (addresses will vary):
```
Value of number: 42
Address of number: 0x7ffd5c3e8a4c
Value of ptr (address): 0x7ffd5c3e8a4c
Value pointed to by ptr: 42
```

### Why Use Pointers?

```cpp
#include <iostream>

void modifyValue(int* ptr)
{
    *ptr = 100;  // Modify the value at the address
}

int main()
{
    int number = 42;

    std::cout << "Before: " << number << "\n";

    modifyValue(&number);

    std::cout << "After: " << number << "\n";

    return 0;
}
```

---

## 2. Memory Addresses

### Understanding Memory Layout:

```cpp
#include <iostream>

int main()
{
    int a = 10;
    int b = 20;
    int c = 30;

    std::cout << "Variable a: value=" << a << ", address=" << &a << "\n";
    std::cout << "Variable b: value=" << b << ", address=" << &b << "\n";
    std::cout << "Variable c: value=" << c << ", address=" << &c << "\n";

    std::cout << "\nSize of int: " << sizeof(int) << " bytes\n";
    std::cout << "Size of int*: " << sizeof(int*) << " bytes\n";

    return 0;
}
```

---

## 3. Pointer Declaration and Initialization

### Different Pointer Types:

```cpp
#include <iostream>

int main()
{
    // Integer pointer
    int num = 42;
    int* intPtr = &num;

    // Double pointer
    double pi = 3.14159;
    double* doublePtr = &pi;

    // Char pointer
    char letter = 'A';
    char* charPtr = &letter;

    // Boolean pointer
    bool flag = true;
    bool* boolPtr = &flag;

    std::cout << "int pointer points to: " << *intPtr << "\n";
    std::cout << "double pointer points to: " << *doublePtr << "\n";
    std::cout << "char pointer points to: " << *charPtr << "\n";
    std::cout << "bool pointer points to: " << *boolPtr << "\n";

    return 0;
}
```

### Pointer Initialization:

```cpp
#include <iostream>

int main()
{
    int value = 100;

    // Valid initializations
    int* ptr1 = &value;          // Point to existing variable
    int* ptr2 = nullptr;         // Initialize to null (C++11)
    int* ptr3 = NULL;            // Old style null (C)
    int* ptr4 = 0;               // Initialize to zero

    // Later assignment
    ptr2 = &value;

    std::cout << "ptr1: " << *ptr1 << "\n";
    std::cout << "ptr2: " << *ptr2 << "\n";

    return 0;
}
```

---

## 4. Dereferencing Pointers

### Accessing Values Through Pointers:

```cpp
#include <iostream>

int main()
{
    int number = 42;
    int* ptr = &number;

    // Dereference to get value
    std::cout << "Value: " << *ptr << "\n";

    // Modify through pointer
    *ptr = 100;
    std::cout << "Modified value: " << number << "\n";

    // Modify original affects pointer
    number = 200;
    std::cout << "Value through pointer: " << *ptr << "\n";

    return 0;
}
```

### Multiple Pointers to Same Variable:

```cpp
#include <iostream>

int main()
{
    int value = 50;
    int* ptr1 = &value;
    int* ptr2 = &value;

    std::cout << "Initial value: " << value << "\n";

    *ptr1 = 100;
    std::cout << "After *ptr1 = 100: " << value << "\n";
    std::cout << "Through ptr2: " << *ptr2 << "\n";

    *ptr2 = 200;
    std::cout << "After *ptr2 = 200: " << value << "\n";
    std::cout << "Through ptr1: " << *ptr1 << "\n";

    return 0;
}
```

---

## 5. Pointers and Variables

### Pointer to Pointer:

```cpp
#include <iostream>

int main()
{
    int value = 42;
    int* ptr = &value;
    int** ptrPtr = &ptr;  // Pointer to pointer

    std::cout << "value: " << value << "\n";
    std::cout << "*ptr: " << *ptr << "\n";
    std::cout << "**ptrPtr: " << **ptrPtr << "\n\n";

    std::cout << "Address of value: " << &value << "\n";
    std::cout << "ptr: " << ptr << "\n";
    std::cout << "*ptrPtr: " << *ptrPtr << "\n\n";

    std::cout << "Address of ptr: " << &ptr << "\n";
    std::cout << "ptrPtr: " << ptrPtr << "\n";

    // Modify through pointer to pointer
    **ptrPtr = 100;
    std::cout << "\nAfter **ptrPtr = 100:\n";
    std::cout << "value: " << value << "\n";

    return 0;
}
```

---

## 6. Null Pointers

### Working with nullptr:

```cpp
#include <iostream>

int main()
{
    int* ptr = nullptr;

    // Always check before dereferencing
    if (ptr != nullptr)
    {
        std::cout << "Value: " << *ptr << "\n";
    }
    else
    {
        std::cout << "Pointer is null, cannot dereference\n";
    }

    // Assign address
    int value = 42;
    ptr = &value;

    if (ptr != nullptr)
    {
        std::cout << "Value: " << *ptr << "\n";
    }

    return 0;
}
```

---

## 7. Pointer Best Practices

### Safe Pointer Usage:

```cpp
#include <iostream>

void safePointerUsage()
{
    int* ptr = nullptr;  // Always initialize

    int value = 100;
    ptr = &value;

    // Check before use
    if (ptr != nullptr)
    {
        std::cout << "Value: " << *ptr << "\n";
    }

    // Set to nullptr after use if needed
    ptr = nullptr;
}

int main()
{
    safePointerUsage();
    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Pointer Basics

```cpp
#include <iostream>

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int x = 10, y = 20;

    std::cout << "Before swap: x=" << x << ", y=" << y << "\n";

    swap(&x, &y);

    std::cout << "After swap: x=" << x << ", y=" << y << "\n";

    return 0;
}
```

### Practice Project: Pointer Calculator

```cpp
#include <iostream>

class PointerCalculator
{
public:
    static void add(int* a, int* b, int* result)
    {
        *result = *a + *b;
    }

    static void subtract(int* a, int* b, int* result)
    {
        *result = *a - *b;
    }

    static void multiply(int* a, int* b, int* result)
    {
        *result = *a * *b;
    }

    static void divide(int* a, int* b, int* result)
    {
        if (*b != 0)
        {
            *result = *a / *b;
        }
        else
        {
            *result = 0;
        }
    }
};

int main()
{
    int num1 = 10, num2 = 5, result;

    PointerCalculator::add(&num1, &num2, &result);
    std::cout << num1 << " + " << num2 << " = " << result << "\n";

    PointerCalculator::multiply(&num1, &num2, &result);
    std::cout << num1 << " * " << num2 << " = " << result << "\n";

    return 0;
}
```

---

## Key Takeaways

1. Pointers store memory addresses
2. Use & to get address, * to dereference
3. Always initialize pointers
4. Check for nullptr before dereferencing
5. Pointers enable pass-by-reference
6. Understanding pointers is crucial for C++
7. Use nullptr instead of NULL or 0
8. Be careful with pointer arithmetic

---

## Summary Checklist

Before moving to Lesson 22, ensure you can:
- [ ] Declare and initialize pointers
- [ ] Understand memory addresses
- [ ] Dereference pointers correctly
- [ ] Use nullptr properly
- [ ] Pass pointers to functions
- [ ] Understand pointer-to-pointer
- [ ] Follow pointer best practices
- [ ] Debug pointer-related issues

---

## Next Lesson Preview

In **Lesson 22**, we'll explore:
- Pointer arithmetic operations
- Incrementing and decrementing pointers
- Pointer comparisons
- Pointer and array relationship
- Advanced pointer manipulations

**Pointers fundamentals mastered!**
