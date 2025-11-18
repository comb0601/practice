# Lesson 06: Pointers and References
**Estimated Time: 4-5 hours**

## Table of Contents
1. [Introduction to Pointers](#introduction-to-pointers)
2. [Pointer Basics](#pointer-basics)
3. [Pointer Arithmetic](#pointer-arithmetic)
4. [Pointers and Arrays](#pointers-and-arrays)
5. [Pointers and Functions](#pointers-and-functions)
6. [References](#references)
7. [Pointer to Pointer](#pointer-to-pointer)
8. [Const Pointers and Pointer to Const](#const-pointers-and-pointer-to-const)
9. [Null Pointers](#null-pointers)
10. [Complete Working Examples](#complete-working-examples)
11. [Common Mistakes](#common-mistakes)
12. [Exercises](#exercises)

## Introduction to Pointers

Pointers are variables that store memory addresses of other variables.

### Why Pointers?

1. **Direct memory access**: Manipulate memory directly
2. **Efficient parameter passing**: Avoid copying large data structures
3. **Dynamic memory allocation**: Create variables at runtime
4. **Data structures**: Build linked lists, trees, graphs
5. **Function pointers**: Callbacks and advanced techniques

### Memory Addresses

Every variable in memory has an address.

```
Variable:  int x = 42;

Memory visualization:
Address:  0x1000  0x1001  0x1002  0x1003
Value:       42      0       0       0
             |-------- x (4 bytes) -------|
```

## Pointer Basics

### Declaring Pointers

```cpp
#include <iostream>

int main()
{
    int x = 42;
    int* ptr;  // Pointer to int (uninitialized)

    // Different declaration styles (all equivalent):
    int* p1;   // Preferred
    int *p2;   // Also valid
    int * p3;  // Also valid

    // Multiple pointers
    int* p4, *p5;  // Both are pointers
    int* p6, p7;   // p6 is pointer, p7 is int (confusing!)

    return 0;
}
```

### Address-of Operator (&)

```cpp
#include <iostream>

int main()
{
    int x = 42;
    int* ptr = &x;  // Get address of x

    std::cout << "Value of x: " << x << "\n";
    std::cout << "Address of x: " << &x << "\n";
    std::cout << "Value of ptr: " << ptr << "\n";  // Same as &x
    std::cout << "Address of ptr: " << &ptr << "\n";

    return 0;
}
```

Output (addresses will vary):
```
Value of x: 42
Address of x: 0x7ffd5c8e4a1c
Value of ptr: 0x7ffd5c8e4a1c
Address of ptr: 0x7ffd5c8e4a20
```

### Dereference Operator (*)

```cpp
#include <iostream>

int main()
{
    int x = 42;
    int* ptr = &x;

    std::cout << "Value of x: " << x << "\n";
    std::cout << "Value via pointer: " << *ptr << "\n";  // Dereference

    // Modify x through pointer
    *ptr = 100;

    std::cout << "New value of x: " << x << "\n";
    std::cout << "New value via pointer: " << *ptr << "\n";

    return 0;
}
```

Output:
```
Value of x: 42
Value via pointer: 42
New value of x: 100
New value via pointer: 100
```

### Memory Diagram

```cpp
int x = 42;
int* ptr = &x;

Memory:
Address   Variable   Value
0x1000    x          42
0x2000    ptr        0x1000  (stores address of x)

*ptr means: "Go to address stored in ptr (0x1000) and get the value (42)"
&x means: "Get the address where x is stored (0x1000)"
```

### Complete Example

```cpp
#include <iostream>

int main()
{
    int a = 10;
    int b = 20;

    int* ptr1 = &a;  // ptr1 points to a
    int* ptr2 = &b;  // ptr2 points to b

    std::cout << "a = " << a << ", b = " << b << "\n";
    std::cout << "*ptr1 = " << *ptr1 << ", *ptr2 = " << *ptr2 << "\n";

    // Swap values using pointers
    int temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;

    std::cout << "After swap:\n";
    std::cout << "a = " << a << ", b = " << b << "\n";

    // Change what ptr1 points to
    ptr1 = &b;  // Now ptr1 points to b
    std::cout << "*ptr1 now = " << *ptr1 << "\n";

    return 0;
}
```

## Pointer Arithmetic

Pointers can be incremented, decremented, and compared.

### Basic Arithmetic

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int* ptr = arr;  // Points to first element

    std::cout << "ptr points to: " << *ptr << "\n";       // 10

    ptr++;  // Move to next element
    std::cout << "After ptr++: " << *ptr << "\n";         // 20

    ptr += 2;  // Move 2 elements forward
    std::cout << "After ptr+=2: " << *ptr << "\n";        // 40

    ptr--;  // Move back one element
    std::cout << "After ptr--: " << *ptr << "\n";         // 30

    return 0;
}
```

### Pointer Arithmetic Rules

```cpp
int arr[5] = {10, 20, 30, 40, 50};
int* ptr = arr;

// Legal operations:
ptr + 3      // Pointer to 4th element
ptr - 1      // Pointer to previous element
ptr++        // Increment to next element
ptr--        // Decrement to previous element
ptr2 - ptr1  // Distance between pointers (number of elements)

// Illegal operations:
ptr + ptr2   // Cannot add two pointers
ptr * 2      // Cannot multiply pointer
ptr / 2      // Cannot divide pointer
```

### Address Calculation

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int* ptr = arr;

    std::cout << "Size of int: " << sizeof(int) << " bytes\n\n";

    for (int i = 0; i < 5; i++)
    {
        std::cout << "Element " << i << ":\n";
        std::cout << "  Address: " << (ptr + i) << "\n";
        std::cout << "  Value: " << *(ptr + i) << "\n";
    }

    return 0;
}
```

**Note**: When you do `ptr + 1`, it doesn't add 1 byte, it adds `sizeof(type)` bytes!

```cpp
int* ptr;
ptr + 1  // Adds 4 bytes (sizeof(int))

double* dptr;
dptr + 1  // Adds 8 bytes (sizeof(double))
```

## Pointers and Arrays

Arrays and pointers are closely related in C++.

### Array Name as Pointer

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};

    // Array name is a pointer to first element
    std::cout << "arr: " << arr << "\n";
    std::cout << "&arr[0]: " << &arr[0] << "\n";  // Same address

    std::cout << "*arr: " << *arr << "\n";        // 10
    std::cout << "arr[0]: " << arr[0] << "\n";    // 10

    return 0;
}
```

### Array Access with Pointers

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};

    // These are equivalent:
    std::cout << "arr[2] = " << arr[2] << "\n";
    std::cout << "*(arr + 2) = " << *(arr + 2) << "\n";

    // Pointer variable
    int* ptr = arr;

    // These are also equivalent:
    std::cout << "ptr[2] = " << ptr[2] << "\n";
    std::cout << "*(ptr + 2) = " << *(ptr + 2) << "\n";

    return 0;
}
```

**Important**: `arr[i]` is equivalent to `*(arr + i)`

### Iterating Arrays with Pointers

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int* ptr = arr;

    // Method 1: Using pointer arithmetic
    std::cout << "Method 1:\n";
    for (int i = 0; i < 5; i++)
    {
        std::cout << *(ptr + i) << " ";
    }
    std::cout << "\n";

    // Method 2: Incrementing pointer
    std::cout << "Method 2:\n";
    ptr = arr;  // Reset pointer
    for (int i = 0; i < 5; i++)
    {
        std::cout << *ptr << " ";
        ptr++;
    }
    std::cout << "\n";

    // Method 3: Pointer comparison
    std::cout << "Method 3:\n";
    int* end = arr + 5;  // One past last element
    for (ptr = arr; ptr < end; ptr++)
    {
        std::cout << *ptr << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### Pointer vs Array Differences

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int* ptr = arr;

    // DIFFERENCE 1: sizeof
    std::cout << "sizeof(arr): " << sizeof(arr) << "\n";   // 20 (5 * 4)
    std::cout << "sizeof(ptr): " << sizeof(ptr) << "\n";   // 8 (on 64-bit)

    // DIFFERENCE 2: Assignment
    // arr = ptr;  // ERROR: Cannot assign to array
    ptr = arr;     // OK: Can reassign pointer

    // DIFFERENCE 3: Modification
    // arr++;  // ERROR: Cannot modify array name
    ptr++;     // OK: Can modify pointer

    return 0;
}
```

### Multidimensional Arrays and Pointers

```cpp
#include <iostream>

int main()
{
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // Pointer to array of 4 integers
    int (*ptr)[4] = matrix;

    // Access elements
    std::cout << "matrix[1][2] = " << matrix[1][2] << "\n";
    std::cout << "*(*(ptr + 1) + 2) = " << *(*(ptr + 1) + 2) << "\n";

    // Iterate using pointer
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << *(*(ptr + i) + j) << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

## Pointers and Functions

### Passing Pointers to Functions

```cpp
#include <iostream>

void modifyValue(int* ptr)
{
    *ptr = 100;  // Modifies original variable
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int x = 42;
    std::cout << "Before: x = " << x << "\n";

    modifyValue(&x);  // Pass address of x

    std::cout << "After: x = " << x << "\n";

    int p = 10, q = 20;
    std::cout << "Before swap: p = " << p << ", q = " << q << "\n";

    swap(&p, &q);

    std::cout << "After swap: p = " << p << ", q = " << q << "\n";

    return 0;
}
```

### Returning Pointers from Functions

```cpp
#include <iostream>

// Return pointer to larger value
int* max(int* a, int* b)
{
    if (*a > *b)
        return a;
    else
        return b;
}

// Return pointer to static variable
int* createCounter()
{
    static int counter = 0;
    counter++;
    return &counter;
}

int main()
{
    int x = 10, y = 20;
    int* maxPtr = max(&x, &y);

    std::cout << "Larger value: " << *maxPtr << "\n";

    int* ctr1 = createCounter();
    int* ctr2 = createCounter();

    std::cout << "*ctr1 = " << *ctr1 << "\n";  // 2 (same variable)
    std::cout << "*ctr2 = " << *ctr2 << "\n";  // 2

    return 0;
}
```

**Warning**: Never return pointer to local variable!

```cpp
// WRONG - Dangling pointer!
int* getBadPointer()
{
    int x = 42;
    return &x;  // x is destroyed when function returns
}

int main()
{
    int* ptr = getBadPointer();
    std::cout << *ptr << "\n";  // UNDEFINED BEHAVIOR!
}
```

### Passing Arrays to Functions

```cpp
#include <iostream>

// These declarations are equivalent:
void printArray1(int arr[], int size);
void printArray2(int* arr, int size);

void printArray1(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

void printArray2(int* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << *(arr + i) << " ";
    }
    std::cout << "\n";
}

void modifyArray(int* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] *= 2;  // Double each element
    }
}

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};
    int size = 5;

    printArray1(numbers, size);
    printArray2(numbers, size);

    modifyArray(numbers, size);
    std::cout << "After modification: ";
    printArray1(numbers, size);

    return 0;
}
```

## References

References are aliases for existing variables.

### Reference Basics

```cpp
#include <iostream>

int main()
{
    int x = 42;
    int& ref = x;  // ref is a reference to x

    std::cout << "x = " << x << "\n";
    std::cout << "ref = " << ref << "\n";

    // Modifying through reference
    ref = 100;

    std::cout << "After ref = 100:\n";
    std::cout << "x = " << x << "\n";       // 100
    std::cout << "ref = " << ref << "\n";   // 100

    // Both have same address
    std::cout << "Address of x: " << &x << "\n";
    std::cout << "Address of ref: " << &ref << "\n";  // Same!

    return 0;
}
```

### References vs Pointers

```cpp
#include <iostream>

int main()
{
    int x = 10;

    // POINTER
    int* ptr = &x;
    *ptr = 20;  // Need to dereference

    // REFERENCE
    int& ref = x;
    ref = 30;   // Direct access (cleaner syntax)

    std::cout << "x = " << x << "\n";  // 30

    return 0;
}
```

**Key Differences:**

| Feature | Pointer | Reference |
|---------|---------|-----------|
| Syntax | `int* ptr` | `int& ref` |
| Initialization | Can be uninitialized | Must be initialized |
| Reassignment | Can point to different variables | Cannot be rebound |
| Null value | Can be nullptr | Cannot be null |
| Dereference | Requires * operator | Automatic |
| Address-of | `&ptr` gives pointer's address | `&ref` gives original variable's address |

### Reference Rules

```cpp
// VALID
int x = 10;
int& ref1 = x;           // OK: Reference to variable

// INVALID
int& ref2;               // ERROR: Must be initialized
int& ref3 = 42;          // ERROR: Cannot bind to literal (non-const)
int& ref4 = x + 5;       // ERROR: Cannot bind to temporary

// CONST REFERENCE (can bind to temporary)
const int& ref5 = 42;    // OK: Const reference to literal
const int& ref6 = x + 5; // OK: Const reference to temporary
```

### References in Functions

```cpp
#include <iostream>

// Pass by reference
void increment(int& x)
{
    x++;  // Modifies original
}

void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

// Return reference (be careful!)
int& getElement(int arr[], int index)
{
    return arr[index];  // Returns reference to array element
}

int main()
{
    int num = 10;
    std::cout << "Before: " << num << "\n";
    increment(num);
    std::cout << "After: " << num << "\n";

    int x = 5, y = 15;
    std::cout << "Before swap: x = " << x << ", y = " << y << "\n";
    swap(x, y);
    std::cout << "After swap: x = " << x << ", y = " << y << "\n";

    int arr[5] = {10, 20, 30, 40, 50};
    getElement(arr, 2) = 300;  // Can modify through reference!
    std::cout << "arr[2] = " << arr[2] << "\n";  // 300

    return 0;
}
```

### Const References

```cpp
#include <iostream>
#include <string>

// Efficient: No copy, but cannot modify
void printString(const std::string& str)
{
    std::cout << str << "\n";
    // str = "Modified";  // ERROR: Cannot modify const reference
}

// Inefficient: Copies entire string
void printStringBad(std::string str)
{
    std::cout << str << "\n";
}

int max(const int& a, const int& b)
{
    return (a > b) ? a : b;
}

int main()
{
    std::string message = "Hello, World!";
    printString(message);  // No copy!

    std::cout << "Max: " << max(10, 20) << "\n";

    return 0;
}
```

## Pointer to Pointer

Double pointers store the address of another pointer.

### Basic Double Pointer

```cpp
#include <iostream>

int main()
{
    int x = 42;
    int* ptr = &x;      // Pointer to x
    int** pptr = &ptr;  // Pointer to pointer

    std::cout << "Value of x: " << x << "\n";
    std::cout << "Value via ptr: " << *ptr << "\n";
    std::cout << "Value via pptr: " << **pptr << "\n";

    std::cout << "\nAddresses:\n";
    std::cout << "Address of x: " << &x << "\n";
    std::cout << "Value of ptr (address of x): " << ptr << "\n";
    std::cout << "Address of ptr: " << &ptr << "\n";
    std::cout << "Value of pptr (address of ptr): " << pptr << "\n";

    // Modify through double pointer
    **pptr = 100;
    std::cout << "\nAfter **pptr = 100:\n";
    std::cout << "x = " << x << "\n";  // 100

    return 0;
}
```

### Memory Diagram

```
int x = 42;
int* ptr = &x;
int** pptr = &ptr;

Memory:
Address   Variable   Value
0x1000    x          42
0x2000    ptr        0x1000  (address of x)
0x3000    pptr       0x2000  (address of ptr)

*ptr = *(0x1000) = 42
**pptr = *(*(0x2000)) = *(0x1000) = 42
```

### Pointer to Pointer in Functions

```cpp
#include <iostream>

void modifyPointer(int** pptr, int* newAddress)
{
    *pptr = newAddress;  // Change what the pointer points to
}

void allocateArray(int** arr, int size)
{
    *arr = new int[size];  // Dynamic allocation (covered in next lesson)

    for (int i = 0; i < size; i++)
    {
        (*arr)[i] = i + 1;
    }
}

int main()
{
    int x = 10, y = 20;
    int* ptr = &x;

    std::cout << "*ptr = " << *ptr << "\n";  // 10

    modifyPointer(&ptr, &y);  // Change ptr to point to y

    std::cout << "*ptr = " << *ptr << "\n";  // 20

    int* dynamicArr = nullptr;
    allocateArray(&dynamicArr, 5);

    std::cout << "Dynamic array: ";
    for (int i = 0; i < 5; i++)
    {
        std::cout << dynamicArr[i] << " ";
    }
    std::cout << "\n";

    delete[] dynamicArr;  // Clean up

    return 0;
}
```

## Const Pointers and Pointer to Const

### Four Combinations

```cpp
#include <iostream>

int main()
{
    int x = 10, y = 20;

    // 1. Regular pointer (can change both)
    int* ptr1 = &x;
    *ptr1 = 15;    // OK: Change value
    ptr1 = &y;     // OK: Change address

    // 2. Pointer to const (cannot change value)
    const int* ptr2 = &x;
    // *ptr2 = 15; // ERROR: Cannot change value
    ptr2 = &y;     // OK: Can change address

    // 3. Const pointer (cannot change address)
    int* const ptr3 = &x;
    *ptr3 = 15;    // OK: Can change value
    // ptr3 = &y;  // ERROR: Cannot change address

    // 4. Const pointer to const (cannot change either)
    const int* const ptr4 = &x;
    // *ptr4 = 15; // ERROR: Cannot change value
    // ptr4 = &y;  // ERROR: Cannot change address

    return 0;
}
```

### Reading Pointer Declarations

**Rule**: Read from right to left

```cpp
int* ptr;              // ptr is a pointer to int
const int* ptr;        // ptr is a pointer to const int
int* const ptr;        // ptr is a const pointer to int
const int* const ptr;  // ptr is a const pointer to const int

// Alternative syntax (same meaning):
int const* ptr;        // Same as const int*
```

### Practical Example

```cpp
#include <iostream>

void printArray(const int* arr, int size)
{
    // Cannot modify array elements
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
        // arr[i] = 0;  // ERROR: arr points to const int
    }
    std::cout << "\n";
}

void resetArray(int* const arr, int size)
{
    // Cannot change where arr points, but can modify elements
    for (int i = 0; i < size; i++)
    {
        arr[i] = 0;  // OK: Modify elements
    }
    // arr = nullptr;  // ERROR: arr is const pointer
}

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};
    int size = 5;

    printArray(numbers, size);
    resetArray(numbers, size);
    printArray(numbers, size);

    return 0;
}
```

## Null Pointers

### nullptr (C++11)

```cpp
#include <iostream>

int main()
{
    int* ptr1 = nullptr;  // Modern way (C++11)
    int* ptr2 = NULL;     // Old C way (macro)
    int* ptr3 = 0;        // Also valid but discouraged

    // Check for null
    if (ptr1 == nullptr)
    {
        std::cout << "ptr1 is null\n";
    }

    // Before dereferencing, always check!
    int x = 42;
    int* ptr = &x;

    if (ptr != nullptr)
    {
        std::cout << "*ptr = " << *ptr << "\n";  // Safe
    }

    return 0;
}
```

### Null Pointer Pitfalls

```cpp
#include <iostream>

void dangerousFunction(int* ptr)
{
    // DANGEROUS: Assuming pointer is not null
    std::cout << *ptr << "\n";  // Crash if ptr is nullptr!
}

void safeFunction(int* ptr)
{
    if (ptr == nullptr)
    {
        std::cout << "Error: Null pointer!\n";
        return;
    }

    std::cout << *ptr << "\n";  // Safe
}

int main()
{
    int* ptr = nullptr;

    // dangerousFunction(ptr);  // Would crash!
    safeFunction(ptr);           // Handles null safely

    return 0;
}
```

## Complete Working Examples

### Example 1: Dynamic Array Utilities

```cpp
#include <iostream>

void fillArray(int* arr, int size, int value);
void printArray(const int* arr, int size);
int* findElement(int* arr, int size, int target);
void reverseArray(int* arr, int size);
int* copyArray(const int* source, int size);

int main()
{
    const int SIZE = 10;
    int numbers[SIZE];

    std::cout << "=== Array Utilities with Pointers ===\n\n";

    // Fill array
    fillArray(numbers, SIZE, 5);
    std::cout << "After fill with 5: ";
    printArray(numbers, SIZE);

    // Modify some elements
    numbers[3] = 42;
    numbers[7] = 99;
    std::cout << "After modifications: ";
    printArray(numbers, SIZE);

    // Find element
    int target = 42;
    int* found = findElement(numbers, SIZE, target);
    if (found != nullptr)
    {
        std::cout << "Found " << target << " at index "
                  << (found - numbers) << "\n";
    }

    // Reverse
    reverseArray(numbers, SIZE);
    std::cout << "After reverse: ";
    printArray(numbers, SIZE);

    // Copy
    int* copy = copyArray(numbers, SIZE);
    std::cout << "Copied array: ";
    printArray(copy, SIZE);

    delete[] copy;  // Clean up

    return 0;
}

void fillArray(int* arr, int size, int value)
{
    for (int i = 0; i < size; i++)
    {
        *(arr + i) = value;
    }
}

void printArray(const int* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int* findElement(int* arr, int size, int target)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == target)
        {
            return &arr[i];  // Return pointer to element
        }
    }
    return nullptr;  // Not found
}

void reverseArray(int* arr, int size)
{
    int* left = arr;
    int* right = arr + size - 1;

    while (left < right)
    {
        int temp = *left;
        *left = *right;
        *right = temp;

        left++;
        right--;
    }
}

int* copyArray(const int* source, int size)
{
    int* copy = new int[size];

    for (int i = 0; i < size; i++)
    {
        copy[i] = source[i];
    }

    return copy;
}
```

### Example 2: String Manipulation with Pointers

```cpp
#include <iostream>

int stringLength(const char* str);
void stringCopy(char* dest, const char* src);
void stringConcat(char* dest, const char* src);
int stringCompare(const char* str1, const char* str2);
void stringReverse(char* str);
char* stringFind(const char* str, const char* substr);

int main()
{
    char str1[100] = "Hello";
    char str2[100] = "World";
    char result[200];

    std::cout << "=== String Utilities with Pointers ===\n\n";

    std::cout << "Length of \"" << str1 << "\": "
              << stringLength(str1) << "\n";

    stringCopy(result, str1);
    std::cout << "After copy: " << result << "\n";

    stringConcat(result, " ");
    stringConcat(result, str2);
    std::cout << "After concat: " << result << "\n";

    std::cout << "Compare \"Hello\" and \"World\": "
              << stringCompare(str1, str2) << "\n";

    char test[] = "racecar";
    stringReverse(test);
    std::cout << "Reversed \"racecar\": " << test << "\n";

    char sentence[] = "The quick brown fox";
    char* found = stringFind(sentence, "quick");
    if (found != nullptr)
    {
        std::cout << "Found substring at position: "
                  << (found - sentence) << "\n";
        std::cout << "Remaining string: " << found << "\n";
    }

    return 0;
}

int stringLength(const char* str)
{
    int length = 0;
    while (*str != '\0')
    {
        length++;
        str++;
    }
    return length;
}

void stringCopy(char* dest, const char* src)
{
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

void stringConcat(char* dest, const char* src)
{
    // Move to end of dest
    while (*dest != '\0')
    {
        dest++;
    }

    // Copy src to end of dest
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

int stringCompare(const char* str1, const char* str2)
{
    while (*str1 != '\0' && *str2 != '\0')
    {
        if (*str1 < *str2)
            return -1;
        if (*str1 > *str2)
            return 1;
        str1++;
        str2++;
    }

    if (*str1 == '\0' && *str2 == '\0')
        return 0;
    return (*str1 == '\0') ? -1 : 1;
}

void stringReverse(char* str)
{
    char* end = str;

    // Find end
    while (*end != '\0')
    {
        end++;
    }
    end--;  // Move back to last character

    // Swap characters
    while (str < end)
    {
        char temp = *str;
        *str = *end;
        *end = temp;

        str++;
        end--;
    }
}

char* stringFind(const char* str, const char* substr)
{
    const char* s = str;

    while (*s != '\0')
    {
        const char* s1 = s;
        const char* s2 = substr;

        while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
        {
            s1++;
            s2++;
        }

        if (*s2 == '\0')
        {
            return const_cast<char*>(s);  // Found
        }

        s++;
    }

    return nullptr;  // Not found
}
```

### Example 3: Matrix Operations with Pointers

```cpp
#include <iostream>

void allocateMatrix(int**& matrix, int rows, int cols);
void freeMatrix(int** matrix, int rows);
void fillMatrix(int** matrix, int rows, int cols);
void printMatrix(int** matrix, int rows, int cols);
void transposeMatrix(int** source, int** dest, int rows, int cols);
void addMatrices(int** a, int** b, int** result, int rows, int cols);

int main()
{
    std::cout << "=== Dynamic Matrix Operations ===\n\n";

    int rows = 3, cols = 4;
    int** matrix1 = nullptr;
    int** matrix2 = nullptr;
    int** result = nullptr;

    // Allocate matrices
    allocateMatrix(matrix1, rows, cols);
    allocateMatrix(matrix2, rows, cols);
    allocateMatrix(result, rows, cols);

    // Fill matrices
    std::cout << "Matrix 1:\n";
    fillMatrix(matrix1, rows, cols);
    printMatrix(matrix1, rows, cols);

    std::cout << "\nMatrix 2:\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix2[i][j] = (i + 1) * (j + 1);
        }
    }
    printMatrix(matrix2, rows, cols);

    // Add matrices
    addMatrices(matrix1, matrix2, result, rows, cols);
    std::cout << "\nSum:\n";
    printMatrix(result, rows, cols);

    // Transpose
    int** transposed = nullptr;
    allocateMatrix(transposed, cols, rows);
    transposeMatrix(matrix1, transposed, rows, cols);
    std::cout << "\nTranspose of Matrix 1:\n";
    printMatrix(transposed, cols, rows);

    // Free memory
    freeMatrix(matrix1, rows);
    freeMatrix(matrix2, rows);
    freeMatrix(result, rows);
    freeMatrix(transposed, cols);

    return 0;
}

void allocateMatrix(int**& matrix, int rows, int cols)
{
    matrix = new int*[rows];
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = new int[cols];
    }
}

void freeMatrix(int** matrix, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void fillMatrix(int** matrix, int rows, int cols)
{
    int value = 1;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = value++;
        }
    }
}

void printMatrix(int** matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

void transposeMatrix(int** source, int** dest, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            dest[j][i] = source[i][j];
        }
    }
}

void addMatrices(int** a, int** b, int** result, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}
```

### Example 4: Swap Functions Comparison

```cpp
#include <iostream>

// Swap using pointers
void swapWithPointers(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Swap using references
void swapWithReferences(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

// Generic swap with templates and references
template<typename T>
void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

int main()
{
    std::cout << "=== Swap Function Comparison ===\n\n";

    // Test with pointers
    int x1 = 10, y1 = 20;
    std::cout << "Before swap (pointers): x = " << x1 << ", y = " << y1 << "\n";
    swapWithPointers(&x1, &y1);
    std::cout << "After swap (pointers): x = " << x1 << ", y = " << y1 << "\n\n";

    // Test with references
    int x2 = 10, y2 = 20;
    std::cout << "Before swap (references): x = " << x2 << ", y = " << y2 << "\n";
    swapWithReferences(x2, y2);
    std::cout << "After swap (references): x = " << x2 << ", y = " << y2 << "\n\n";

    // Test template version
    double d1 = 3.14, d2 = 2.71;
    std::cout << "Before swap (template): d1 = " << d1 << ", d2 = " << d2 << "\n";
    swap(d1, d2);
    std::cout << "After swap (template): d1 = " << d1 << ", d2 = " << d2 << "\n";

    return 0;
}
```

### Example 5: Pointer-Based Linked List (Preview)

```cpp
#include <iostream>

struct Node
{
    int data;
    Node* next;
};

void insertFront(Node*& head, int value);
void printList(const Node* head);
void deleteList(Node*& head);
int listLength(const Node* head);
bool search(const Node* head, int value);

int main()
{
    std::cout << "=== Simple Linked List ===\n\n";

    Node* head = nullptr;

    // Insert elements
    insertFront(head, 50);
    insertFront(head, 40);
    insertFront(head, 30);
    insertFront(head, 20);
    insertFront(head, 10);

    std::cout << "List: ";
    printList(head);

    std::cout << "Length: " << listLength(head) << "\n";

    int searchValue = 30;
    if (search(head, searchValue))
    {
        std::cout << "Found " << searchValue << " in list\n";
    }

    // Clean up
    deleteList(head);

    return 0;
}

void insertFront(Node*& head, int value)
{
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = head;
    head = newNode;
}

void printList(const Node* head)
{
    const Node* current = head;
    while (current != nullptr)
    {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "NULL\n";
}

void deleteList(Node*& head)
{
    while (head != nullptr)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int listLength(const Node* head)
{
    int count = 0;
    const Node* current = head;
    while (current != nullptr)
    {
        count++;
        current = current->next;
    }
    return count;
}

bool search(const Node* head, int value)
{
    const Node* current = head;
    while (current != nullptr)
    {
        if (current->data == value)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}
```

## Common Mistakes

### 1. Uninitialized Pointer

```cpp
// WRONG - pointer not initialized
int* ptr;
*ptr = 42;  // CRASH! ptr points to random memory

// CORRECT
int* ptr = nullptr;  // Initialize to null
int x = 42;
ptr = &x;  // Now safe to use
*ptr = 100;
```

### 2. Dangling Pointer

```cpp
// WRONG - returning address of local variable
int* getDangling()
{
    int x = 42;
    return &x;  // x is destroyed!
}

int main()
{
    int* ptr = getDangling();
    *ptr = 100;  // UNDEFINED BEHAVIOR!
}

// CORRECT - use static or dynamic allocation
int* getStatic()
{
    static int x = 42;
    return &x;  // OK: static persists
}
```

### 3. Dereferencing nullptr

```cpp
// WRONG - no null check
void printValue(int* ptr)
{
    std::cout << *ptr << "\n";  // Crash if ptr is null!
}

// CORRECT - check for null
void printValue(int* ptr)
{
    if (ptr == nullptr)
    {
        std::cout << "Error: null pointer\n";
        return;
    }
    std::cout << *ptr << "\n";
}
```

### 4. Pointer Arithmetic Errors

```cpp
// WRONG - going beyond array bounds
int arr[5] = {1, 2, 3, 4, 5};
int* ptr = arr;
ptr += 10;  // Points beyond array!
*ptr = 100;  // UNDEFINED BEHAVIOR!

// CORRECT - check bounds
int* ptr = arr;
int index = 2;
if (index >= 0 && index < 5)
{
    *(ptr + index) = 100;  // Safe
}
```

### 5. Memory Leak (Preview)

```cpp
// WRONG - allocated memory not freed
void leak()
{
    int* ptr = new int[100];
    // Forgot to delete[]!
}  // Memory leaked!

// CORRECT - always free allocated memory
void noLeak()
{
    int* ptr = new int[100];
    // Use ptr...
    delete[] ptr;  // Free memory
}
```

### 6. Confusing & and *

```cpp
int x = 42;
int* ptr;

// WRONG
ptr = *x;   // ERROR: *x is value, not address
ptr = &*x;  // ERROR: *x is int, &*x doesn't make sense

// CORRECT
ptr = &x;   // Get address of x
int y = *ptr;  // Get value via pointer
```

## Exercises

### Exercise 1: Pointer Basics
Write functions using pointers:
1. `void findMinMax(int* arr, int size, int** min, int** max)` - Find min/max via pointers
2. `void rotateLeft(int* arr, int size)` - Rotate array left by one
3. `void rotateRight(int* arr, int size)` - Rotate array right by one
4. `int* merge(int* arr1, int size1, int* arr2, int size2)` - Merge two arrays

### Exercise 2: String Operations
Implement using only pointers (no array indexing):
1. `bool isPalindrome(const char* str)` - Check palindrome
2. `char* substring(const char* str, int start, int length)` - Extract substring
3. `int countOccurrences(const char* str, char ch)` - Count character
4. `void removeChar(char* str, char ch)` - Remove all occurrences

### Exercise 3: Reference Practice
Create functions using references:
1. `void inputArray(int& arr[], int& size)` - Input array via reference
2. `void findMinMaxRef(int arr[], int size, int& min, int& max)` - Find via reference
3. `void sortThree(int& a, int& b, int& c)` - Sort three values
4. `void circularShift(int& a, int& b, int& c)` - Shift values circularly

### Exercise 4: Dynamic 2D Array
Implement matrix class using pointers:
1. Create dynamic matrix (rows × cols)
2. Fill with user input or pattern
3. Matrix addition, subtraction, multiplication
4. Find row/column sums
5. Properly deallocate memory

### Exercise 5: Pointer-Based Data Structure
Implement a simple dynamic array class:
1. `void push_back(int** arr, int& size, int& capacity, int value)` - Add element
2. `void pop_back(int** arr, int& size)` - Remove last element
3. `void insert(int** arr, int& size, int& capacity, int index, int value)` - Insert at position
4. `void remove(int** arr, int& size, int index)` - Remove at position
5. Handle resizing when capacity is exceeded

## Summary

You learned:
- ✅ Pointer basics: addresses, dereference, pointer arithmetic
- ✅ Relationship between pointers and arrays
- ✅ Passing pointers to functions
- ✅ References: syntax and usage
- ✅ Differences between pointers and references
- ✅ Double pointers (pointer to pointer)
- ✅ Const pointers and pointer to const
- ✅ Null pointers and safe practices
- ✅ Common pointer mistakes and how to avoid them

## Next Lesson Preview

In Lesson 07, we'll explore:
- Dynamic memory allocation with `new` and `delete`
- Dynamic arrays and memory management
- Memory leaks and how to prevent them
- RAII (Resource Acquisition Is Initialization) basics
- Smart pointers preview (unique_ptr, shared_ptr)
- Debugging common memory errors
- Valgrind and memory debugging tools

---

**Congratulations on completing Lesson 06!**

**Practice time**: Spend 4+ hours working with pointers and references. Pointers are one of the most powerful but challenging concepts in C++. Master them through extensive practice!
