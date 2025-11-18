# Lesson 22: Pointer Arithmetic

**Duration**: 5 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Introduction to Pointer Arithmetic
2. Incrementing and Decrementing Pointers
3. Adding and Subtracting from Pointers
4. Pointer Comparisons
5. Pointer Differences
6. Traversing Arrays with Pointers
7. Pointer Arithmetic Rules
8. Exercises and Projects

---

## 1. Introduction to Pointer Arithmetic

### What is Pointer Arithmetic?

Pointer arithmetic allows you to perform mathematical operations on pointers to navigate through memory.

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int* ptr = arr;  // Points to first element

    std::cout << "arr[0] = " << *ptr << " at " << ptr << "\n";

    ptr++;  // Move to next element
    std::cout << "arr[1] = " << *ptr << " at " << ptr << "\n";

    ptr += 2;  // Move 2 elements forward
    std::cout << "arr[3] = " << *ptr << " at " << ptr << "\n";

    return 0;
}
```

---

## 2. Incrementing and Decrementing Pointers

### Pointer Increment/Decrement:

```cpp
#include <iostream>

int main()
{
    int numbers[] = {100, 200, 300, 400, 500};
    int* ptr = numbers;

    std::cout << "Initial: *ptr = " << *ptr << "\n";

    // Pre-increment
    ++ptr;
    std::cout << "After ++ptr: *ptr = " << *ptr << "\n";

    // Post-increment
    ptr++;
    std::cout << "After ptr++: *ptr = " << *ptr << "\n";

    // Decrement
    --ptr;
    std::cout << "After --ptr: *ptr = " << *ptr << "\n";

    return 0;
}
```

---

## 3. Adding and Subtracting from Pointers

### Arithmetic Operations:

```cpp
#include <iostream>

int main()
{
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int* ptr = arr;

    std::cout << "ptr[0] = " << *ptr << "\n";
    std::cout << "ptr[3] = " << *(ptr + 3) << "\n";
    std::cout << "ptr[7] = " << *(ptr + 7) << "\n";

    // Pointer subtraction
    ptr = &arr[5];
    std::cout << "\n*ptr = " << *ptr << " (arr[5])\n";
    std::cout << "*(ptr - 2) = " << *(ptr - 2) << " (arr[3])\n";

    return 0;
}
```

---

## 4. Pointer Comparisons

### Comparing Pointers:

```cpp
#include <iostream>

int main()
{
    int arr[5] = {1, 2, 3, 4, 5};
    int* ptr1 = &arr[0];
    int* ptr2 = &arr[4];

    if (ptr1 < ptr2)
    {
        std::cout << "ptr1 points to earlier element\n";
    }

    if (ptr2 > ptr1)
    {
        std::cout << "ptr2 points to later element\n";
    }

    ptr1 = ptr2;
    if (ptr1 == ptr2)
    {
        std::cout << "ptr1 and ptr2 now point to same location\n";
    }

    return 0;
}
```

---

## 5. Pointer Differences

### Subtracting Pointers:

```cpp
#include <iostream>

int main()
{
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    int* start = &arr[2];
    int* end = &arr[7];

    // Difference gives number of elements between pointers
    int diff = end - start;

    std::cout << "Difference: " << diff << " elements\n";
    std::cout << "From arr[2] to arr[7]\n";

    return 0;
}
```

---

## 6. Traversing Arrays with Pointers

### Using Pointers to Iterate:

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int* ptr = arr;

    std::cout << "Using pointer arithmetic:\n";
    for (int i = 0; i < 5; i++)
    {
        std::cout << "arr[" << i << "] = " << *(ptr + i) << "\n";
    }

    std::cout << "\nUsing pointer increment:\n";
    ptr = arr;  // Reset
    for (int i = 0; i < 5; i++)
    {
        std::cout << "Element: " << *ptr << "\n";
        ptr++;
    }

    return 0;
}
```

---

## 7. Pointer Arithmetic Rules

### Size-Aware Arithmetic:

```cpp
#include <iostream>

int main()
{
    int intArr[3] = {1, 2, 3};
    double doubleArr[3] = {1.1, 2.2, 3.3};

    int* intPtr = intArr;
    double* doublePtr = doubleArr;

    std::cout << "Integer pointer:\n";
    std::cout << "Address: " << intPtr << "\n";
    std::cout << "After +1: " << (intPtr + 1) << "\n";
    std::cout << "Difference: " << ((intPtr + 1) - intPtr) * sizeof(int) << " bytes\n\n";

    std::cout << "Double pointer:\n";
    std::cout << "Address: " << doublePtr << "\n";
    std::cout << "After +1: " << (doublePtr + 1) << "\n";
    std::cout << "Difference: " << ((doublePtr + 1) - doublePtr) * sizeof(double) << " bytes\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise: Array Sum Using Pointers

```cpp
#include <iostream>

int sumArray(int* arr, int size)
{
    int sum = 0;
    int* end = arr + size;

    for (int* ptr = arr; ptr < end; ptr++)
    {
        sum += *ptr;
    }

    return sum;
}

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};
    int size = 5;

    int total = sumArray(numbers, size);

    std::cout << "Sum: " << total << "\n";

    return 0;
}
```

---

## Key Takeaways

1. Pointer arithmetic moves by element size, not bytes
2. ptr++ moves to next element
3. ptr + n moves n elements forward
4. Pointer subtraction gives element count
5. Pointers can be compared
6. Understanding pointer arithmetic is key for arrays
7. Always stay within array bounds
8. Pointer arithmetic is type-aware

---

## Next Lesson Preview

In **Lesson 23**, we'll explore:
- Deep relationship between pointers and arrays
- Array names as pointers
- Pointer notation vs array notation
- Passing arrays to functions
- Multi-level pointers and arrays

**Pointer arithmetic mastered!**
