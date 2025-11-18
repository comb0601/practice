# Lesson 09: Pointers and Memory Addresses

**Duration: 3.5 hours**

## Table of Contents
1. Introduction to Pointers
2. Memory Addresses and the Address-of Operator
3. Pointer Declaration and Initialization
4. Dereferencing Pointers
5. Pointer Arithmetic
6. Pointers and Arrays
7. Pointers and Functions
8. Common Pointer Pitfalls
9. Practical Exercises
10. Complete Projects

## 1. Introduction to Pointers

Pointers are one of the most powerful features in C++. They allow direct memory manipulation and are essential for dynamic memory allocation, data structures, and efficient programming.

### What is a Pointer?

A pointer is a variable that stores the memory address of another variable.

```cpp
#include <iostream>

int main() {
    int value = 42;
    int* ptr = &value;  // ptr stores the address of value

    std::cout << "Value: " << value << std::endl;
    std::cout << "Address of value: " << &value << std::endl;
    std::cout << "Pointer ptr: " << ptr << std::endl;
    std::cout << "Value pointed to by ptr: " << *ptr << std::endl;

    return 0;
}
```

**Output:**
```
Value: 42
Address of value: 0x7ffd8c3e4a5c
Pointer ptr: 0x7ffd8c3e4a5c
Value pointed to by ptr: 42
```

### Why Use Pointers?

```cpp
#include <iostream>

// Without pointers - pass by value (creates copy)
void incrementValue(int x) {
    x++;  // Only modifies local copy
}

// With pointers - can modify original
void incrementPointer(int* x) {
    (*x)++;  // Modifies original value
}

int main() {
    int num = 10;

    incrementValue(num);
    std::cout << "After incrementValue: " << num << std::endl;  // Still 10

    incrementPointer(&num);
    std::cout << "After incrementPointer: " << num << std::endl;  // Now 11

    return 0;
}
```

## 2. Memory Addresses and the Address-of Operator

### Understanding Memory Addresses

Every variable in memory has an address. The `&` operator gives us that address.

```cpp
#include <iostream>

int main() {
    int a = 10;
    int b = 20;
    int c = 30;

    std::cout << "Variable a: " << a << " at address: " << &a << std::endl;
    std::cout << "Variable b: " << b << " at address: " << &b << std::endl;
    std::cout << "Variable c: " << c << " at address: " << &c << std::endl;

    // Variables are typically stored in consecutive memory locations
    std::cout << "\nSize of int: " << sizeof(int) << " bytes" << std::endl;

    return 0;
}
```

### Address-of Operator (&)

```cpp
#include <iostream>

int main() {
    int number = 100;
    double decimal = 3.14;
    char letter = 'A';

    std::cout << "Integer address: " << &number << std::endl;
    std::cout << "Double address: " << &decimal << std::endl;
    std::cout << "Char address: " << static_cast<void*>(&letter) << std::endl;

    // Size of addresses (typically 8 bytes on 64-bit systems)
    std::cout << "\nSize of pointer: " << sizeof(&number) << " bytes" << std::endl;

    return 0;
}
```

## 3. Pointer Declaration and Initialization

### Basic Pointer Declaration

```cpp
#include <iostream>

int main() {
    int value = 42;

    // Pointer declaration and initialization
    int* ptr1 = &value;   // Style 1: asterisk near type
    int *ptr2 = &value;   // Style 2: asterisk near variable
    int * ptr3 = &value;  // Style 3: asterisk in middle

    // All three are equivalent, but Style 1 is recommended

    std::cout << "Value: " << value << std::endl;
    std::cout << "ptr1 points to: " << *ptr1 << std::endl;
    std::cout << "ptr2 points to: " << *ptr2 << std::endl;
    std::cout << "ptr3 points to: " << *ptr3 << std::endl;

    return 0;
}
```

### Null Pointers

```cpp
#include <iostream>

int main() {
    // Null pointer - points to nothing
    int* ptr1 = nullptr;     // C++11 (recommended)
    int* ptr2 = NULL;        // C-style (deprecated)
    int* ptr3 = 0;           // Old style (not recommended)

    // Always check before dereferencing
    if (ptr1 != nullptr) {
        std::cout << *ptr1 << std::endl;
    } else {
        std::cout << "ptr1 is null" << std::endl;
    }

    // Assign a value
    int value = 100;
    ptr1 = &value;

    if (ptr1 != nullptr) {
        std::cout << "ptr1 now points to: " << *ptr1 << std::endl;
    }

    return 0;
}
```

### Pointer to Different Types

```cpp
#include <iostream>

int main() {
    int intValue = 42;
    double doubleValue = 3.14159;
    char charValue = 'X';

    int* intPtr = &intValue;
    double* doublePtr = &doubleValue;
    char* charPtr = &charValue;

    std::cout << "Int pointer: " << *intPtr << std::endl;
    std::cout << "Double pointer: " << *doublePtr << std::endl;
    std::cout << "Char pointer: " << *charPtr << std::endl;

    // Size of different pointers (usually same on same architecture)
    std::cout << "\nSize of int*: " << sizeof(intPtr) << std::endl;
    std::cout << "Size of double*: " << sizeof(doublePtr) << std::endl;
    std::cout << "Size of char*: " << sizeof(charPtr) << std::endl;

    return 0;
}
```

## 4. Dereferencing Pointers

### The Dereference Operator (*)

```cpp
#include <iostream>

int main() {
    int value = 100;
    int* ptr = &value;

    std::cout << "Direct access: " << value << std::endl;
    std::cout << "Through pointer: " << *ptr << std::endl;

    // Modify through pointer
    *ptr = 200;

    std::cout << "\nAfter modification through pointer:" << std::endl;
    std::cout << "Direct access: " << value << std::endl;
    std::cout << "Through pointer: " << *ptr << std::endl;

    return 0;
}
```

### Multiple Pointers to Same Variable

```cpp
#include <iostream>

int main() {
    int value = 50;
    int* ptr1 = &value;
    int* ptr2 = &value;
    int* ptr3 = &value;

    std::cout << "Original value: " << value << std::endl;

    *ptr1 = 100;
    std::cout << "After *ptr1 = 100: " << value << std::endl;

    *ptr2 = 200;
    std::cout << "After *ptr2 = 200: " << value << std::endl;

    *ptr3 = 300;
    std::cout << "After *ptr3 = 300: " << value << std::endl;

    // All pointers point to same location
    std::cout << "\nptr1: " << ptr1 << std::endl;
    std::cout << "ptr2: " << ptr2 << std::endl;
    std::cout << "ptr3: " << ptr3 << std::endl;

    return 0;
}
```

### Pointers and const

```cpp
#include <iostream>

int main() {
    int value = 100;

    // Pointer to const int - cannot modify value through pointer
    const int* ptr1 = &value;
    // *ptr1 = 200;  // ERROR: cannot modify
    value = 200;     // OK: can modify directly
    std::cout << "ptr1 points to: " << *ptr1 << std::endl;

    // Const pointer to int - cannot change what it points to
    int* const ptr2 = &value;
    *ptr2 = 300;     // OK: can modify value
    // ptr2 = &another;  // ERROR: cannot change pointer
    std::cout << "ptr2 points to: " << *ptr2 << std::endl;

    // Const pointer to const int - cannot modify anything
    const int* const ptr3 = &value;
    // *ptr3 = 400;     // ERROR: cannot modify value
    // ptr3 = &another; // ERROR: cannot change pointer
    std::cout << "ptr3 points to: " << *ptr3 << std::endl;

    return 0;
}
```

## 5. Pointer Arithmetic

### Incrementing and Decrementing Pointers

```cpp
#include <iostream>

int main() {
    int arr[] = {10, 20, 30, 40, 50};
    int* ptr = arr;  // Points to first element

    std::cout << "Pointer arithmetic with array:" << std::endl;
    std::cout << "ptr points to: " << *ptr << std::endl;

    ptr++;  // Move to next int
    std::cout << "After ptr++: " << *ptr << std::endl;

    ptr++;
    std::cout << "After another ptr++: " << *ptr << std::endl;

    ptr--;  // Move back
    std::cout << "After ptr--: " << *ptr << std::endl;

    // Jump multiple elements
    ptr += 2;
    std::cout << "After ptr += 2: " << *ptr << std::endl;

    return 0;
}
```

### Pointer Arithmetic Details

```cpp
#include <iostream>

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int* ptr = arr;

    std::cout << "Array address: " << arr << std::endl;
    std::cout << "First element address: " << &arr[0] << std::endl;
    std::cout << "Pointer address: " << ptr << std::endl;

    std::cout << "\nPointer arithmetic:" << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << "ptr + " << i << " = " << (ptr + i)
                  << ", value = " << *(ptr + i) << std::endl;
    }

    // Difference between pointers
    int* start = arr;
    int* end = arr + 4;
    std::cout << "\nDifference (end - start): " << (end - start) << std::endl;

    return 0;
}
```

### Traversing Arrays with Pointers

```cpp
#include <iostream>

int main() {
    int numbers[] = {10, 20, 30, 40, 50};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    // Method 1: Using index
    std::cout << "Method 1 - Index notation:" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    // Method 2: Using pointer arithmetic
    std::cout << "Method 2 - Pointer arithmetic:" << std::endl;
    for (int* ptr = numbers; ptr < numbers + size; ptr++) {
        std::cout << *ptr << " ";
    }
    std::cout << std::endl;

    // Method 3: Pointer with index
    std::cout << "Method 3 - Pointer with index:" << std::endl;
    int* ptr = numbers;
    for (int i = 0; i < size; i++) {
        std::cout << ptr[i] << " ";  // Same as *(ptr + i)
    }
    std::cout << std::endl;

    return 0;
}
```

## 6. Pointers and Arrays

### Array Name as Pointer

```cpp
#include <iostream>

int main() {
    int arr[] = {1, 2, 3, 4, 5};

    // Array name is a pointer to first element
    std::cout << "arr: " << arr << std::endl;
    std::cout << "&arr[0]: " << &arr[0] << std::endl;
    std::cout << "Are they equal? " << (arr == &arr[0]) << std::endl;

    // Access elements
    std::cout << "\nAccessing elements:" << std::endl;
    std::cout << "arr[0] = " << arr[0] << std::endl;
    std::cout << "*arr = " << *arr << std::endl;
    std::cout << "arr[2] = " << arr[2] << std::endl;
    std::cout << "*(arr + 2) = " << *(arr + 2) << std::endl;

    return 0;
}
```

### Dynamic Arrays

```cpp
#include <iostream>

int main() {
    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;

    // Dynamically allocate array
    int* arr = new int[size];

    // Initialize
    std::cout << "Enter " << size << " numbers:" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cin >> arr[i];
    }

    // Display
    std::cout << "You entered: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    // Free memory
    delete[] arr;

    return 0;
}
```

### 2D Arrays and Pointers

```cpp
#include <iostream>

int main() {
    const int ROWS = 3;
    const int COLS = 4;

    // Static 2D array
    int matrix[ROWS][COLS] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // Access using pointer arithmetic
    int* ptr = &matrix[0][0];

    std::cout << "2D array as 1D:" << std::endl;
    for (int i = 0; i < ROWS * COLS; i++) {
        std::cout << *(ptr + i) << " ";
        if ((i + 1) % COLS == 0) {
            std::cout << std::endl;
        }
    }

    return 0;
}
```

## 7. Pointers and Functions

### Passing Pointers to Functions

```cpp
#include <iostream>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void increment(int* value) {
    (*value)++;
}

void multiplyByTwo(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= 2;
    }
}

int main() {
    int x = 10, y = 20;
    std::cout << "Before swap: x = " << x << ", y = " << y << std::endl;
    swap(&x, &y);
    std::cout << "After swap: x = " << x << ", y = " << y << std::endl;

    int num = 5;
    std::cout << "\nBefore increment: " << num << std::endl;
    increment(&num);
    std::cout << "After increment: " << num << std::endl;

    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);

    std::cout << "\nBefore multiplyByTwo: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    multiplyByTwo(arr, size);

    std::cout << "After multiplyByTwo: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### Returning Pointers from Functions

```cpp
#include <iostream>

// DANGEROUS - returning pointer to local variable
int* dangerousFunction() {
    int localVar = 100;
    return &localVar;  // BAD: localVar is destroyed after function returns
}

// SAFE - returning pointer to dynamically allocated memory
int* safeFunction(int value) {
    int* ptr = new int;
    *ptr = value;
    return ptr;  // OK: memory persists after function returns
}

// SAFE - returning pointer to static variable
int* staticFunction() {
    static int staticVar = 200;
    return &staticVar;  // OK: static variable persists
}

int main() {
    // Safe usage
    int* ptr1 = safeFunction(42);
    std::cout << "Dynamically allocated: " << *ptr1 << std::endl;
    delete ptr1;  // Don't forget to free!

    int* ptr2 = staticFunction();
    std::cout << "Static variable: " << *ptr2 << std::endl;
    // No need to delete static variable

    return 0;
}
```

### Pointer Parameters for Output

```cpp
#include <iostream>
#include <cmath>

void calculateCircle(double radius, double* area, double* circumference) {
    const double PI = 3.14159265359;
    *area = PI * radius * radius;
    *circumference = 2 * PI * radius;
}

void divideWithRemainder(int dividend, int divisor, int* quotient, int* remainder) {
    *quotient = dividend / divisor;
    *remainder = dividend % divisor;
}

int main() {
    double area, circumference;
    calculateCircle(5.0, &area, &circumference);
    std::cout << "Circle with radius 5:" << std::endl;
    std::cout << "Area: " << area << std::endl;
    std::cout << "Circumference: " << circumference << std::endl;

    int quotient, remainder;
    divideWithRemainder(17, 5, &quotient, &remainder);
    std::cout << "\n17 divided by 5:" << std::endl;
    std::cout << "Quotient: " << quotient << std::endl;
    std::cout << "Remainder: " << remainder << std::endl;

    return 0;
}
```

## 8. Common Pointer Pitfalls

### Dangling Pointers

```cpp
#include <iostream>

int main() {
    int* ptr = new int(42);
    std::cout << "Value: " << *ptr << std::endl;

    delete ptr;  // Memory is freed

    // ptr is now a dangling pointer
    // std::cout << *ptr << std::endl;  // UNDEFINED BEHAVIOR!

    // Good practice: set to nullptr after delete
    ptr = nullptr;

    if (ptr != nullptr) {
        std::cout << *ptr << std::endl;
    } else {
        std::cout << "Pointer is null" << std::endl;
    }

    return 0;
}
```

### Memory Leaks

```cpp
#include <iostream>

void memoryLeakExample() {
    int* ptr = new int(100);
    // Forgot to delete ptr!
    // Memory leak occurs when function returns
}

void properMemoryManagement() {
    int* ptr = new int(100);
    std::cout << "Value: " << *ptr << std::endl;
    delete ptr;  // Properly freed
}

int main() {
    properMemoryManagement();

    // Example of leak
    for (int i = 0; i < 10; i++) {
        int* leak = new int(i);
        // Never deleted - memory leak!
    }

    return 0;
}
```

### Wild Pointers

```cpp
#include <iostream>

int main() {
    int* wildPtr;  // Uninitialized pointer - contains garbage

    // DANGEROUS: dereferencing uninitialized pointer
    // std::cout << *wildPtr << std::endl;  // UNDEFINED BEHAVIOR!

    // Always initialize pointers
    int* safePtr = nullptr;  // Initialized to nullptr

    int value = 42;
    safePtr = &value;  // Now points to valid memory

    if (safePtr != nullptr) {
        std::cout << "Safe to use: " << *safePtr << std::endl;
    }

    return 0;
}
```

## 9. Practical Exercises

### Exercise 1: Pointer Basics

```cpp
#include <iostream>

int main() {
    // Create variables and pointers
    int a = 10, b = 20, c = 30;
    int* ptr1 = &a;
    int* ptr2 = &b;
    int* ptr3 = &c;

    // Display values and addresses
    std::cout << "Values:" << std::endl;
    std::cout << "a = " << a << " at " << ptr1 << std::endl;
    std::cout << "b = " << b << " at " << ptr2 << std::endl;
    std::cout << "c = " << c << " at " << ptr3 << std::endl;

    // Swap using pointers
    int temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = *ptr3;
    *ptr3 = temp;

    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;

    return 0;
}
```

### Exercise 2: Array Manipulation

```cpp
#include <iostream>

void reverseArray(int* arr, int size) {
    int* start = arr;
    int* end = arr + size - 1;

    while (start < end) {
        int temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void printArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << *(arr + i) << " ";
    }
    std::cout << std::endl;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(arr) / sizeof(arr[0]);

    std::cout << "Original array: ";
    printArray(arr, size);

    reverseArray(arr, size);

    std::cout << "Reversed array: ";
    printArray(arr, size);

    return 0;
}
```

### Exercise 3: Finding Max and Min

```cpp
#include <iostream>

void findMinMax(int* arr, int size, int* min, int* max) {
    if (size == 0) return;

    *min = *max = arr[0];

    for (int i = 1; i < size; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}

int main() {
    int numbers[] = {45, 23, 78, 12, 90, 34, 67, 5, 89, 11};
    int size = sizeof(numbers) / sizeof(numbers[0]);
    int min, max;

    findMinMax(numbers, size, &min, &max);

    std::cout << "Array: ";
    for (int i = 0; i < size; i++) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Minimum: " << min << std::endl;
    std::cout << "Maximum: " << max << std::endl;

    return 0;
}
```

## 10. Complete Projects

### Project 1: Dynamic Array Manager

```cpp
#include <iostream>

class DynamicArray {
private:
    int* arr;
    int size;
    int capacity;

public:
    DynamicArray(int initialCapacity = 10) {
        capacity = initialCapacity;
        size = 0;
        arr = new int[capacity];
    }

    ~DynamicArray() {
        delete[] arr;
    }

    void add(int value) {
        if (size >= capacity) {
            resize();
        }
        arr[size++] = value;
    }

    void resize() {
        capacity *= 2;
        int* newArr = new int[capacity];
        for (int i = 0; i < size; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }

    void display() {
        std::cout << "Array (" << size << "/" << capacity << "): ";
        for (int i = 0; i < size; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }

    int getSize() { return size; }
    int getCapacity() { return capacity; }
};

int main() {
    DynamicArray arr(5);

    std::cout << "Adding elements..." << std::endl;
    for (int i = 1; i <= 12; i++) {
        arr.add(i * 10);
        arr.display();
    }

    return 0;
}
```

### Project 2: Simple String Manager

```cpp
#include <iostream>
#include <cstring>

class String {
private:
    char* data;
    int length;

public:
    String(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }

    ~String() {
        delete[] data;
    }

    void display() {
        std::cout << data << std::endl;
    }

    int getLength() {
        return length;
    }

    void append(const char* str) {
        int newLength = length + strlen(str);
        char* newData = new char[newLength + 1];

        strcpy(newData, data);
        strcat(newData, str);

        delete[] data;
        data = newData;
        length = newLength;
    }
};

int main() {
    String s1("Hello");
    std::cout << "String: ";
    s1.display();
    std::cout << "Length: " << s1.getLength() << std::endl;

    s1.append(" World");
    std::cout << "After append: ";
    s1.display();
    std::cout << "Length: " << s1.getLength() << std::endl;

    s1.append("!!!");
    std::cout << "After another append: ";
    s1.display();

    return 0;
}
```

### Project 3: Pointer-Based Linked List

```cpp
#include <iostream>

struct Node {
    int data;
    Node* next;
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insertAtBeginning(int value) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = head;
        head = newNode;
    }

    void insertAtEnd(int value) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }

    void display() {
        std::cout << "List: ";
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << "NULL" << std::endl;
    }

    int count() {
        int count = 0;
        Node* current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }
};

int main() {
    LinkedList list;

    std::cout << "Inserting at beginning:" << std::endl;
    list.insertAtBeginning(10);
    list.display();
    list.insertAtBeginning(20);
    list.display();
    list.insertAtBeginning(30);
    list.display();

    std::cout << "\nInserting at end:" << std::endl;
    list.insertAtEnd(40);
    list.display();
    list.insertAtEnd(50);
    list.display();

    std::cout << "\nTotal nodes: " << list.count() << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- ✅ What pointers are and why they're important
- ✅ Memory addresses and the address-of operator (&)
- ✅ Pointer declaration, initialization, and dereferencing
- ✅ Pointer arithmetic and array traversal
- ✅ Relationship between pointers and arrays
- ✅ Passing pointers to functions
- ✅ Common pointer pitfalls (dangling, wild, memory leaks)
- ✅ Dynamic memory management with pointers
- ✅ Building data structures with pointers

## Key Takeaways

1. **Pointers store memory addresses** - They reference locations in memory
2. **& gives address** - The address-of operator returns a variable's address
3. **\* dereferences** - Access the value at the address the pointer holds
4. **Always initialize** - Never use uninitialized pointers
5. **nullptr for safety** - Use nullptr instead of NULL
6. **Delete what you new** - Free dynamically allocated memory
7. **Pointers enable dynamic data structures** - Essential for linked lists, trees, etc.

## Next Lesson

Lesson 10: Dynamic Memory Allocation (new and delete)

## Practice Problems

1. Write a function that uses pointers to swap three variables
2. Implement a function to find the second largest element using pointers
3. Create a function that removes duplicates from an array using pointers
4. Build a simple stack using pointers and dynamic memory
5. Implement pointer-based string reversal function

## Additional Resources

- [cppreference.com - Pointers](https://en.cppreference.com/w/cpp/language/pointer)
- [Learn CPP - Pointers](https://www.learncpp.com/)
- Practice pointer problems on coding platforms
