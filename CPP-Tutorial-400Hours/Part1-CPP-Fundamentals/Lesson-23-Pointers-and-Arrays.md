# Lesson 23: Pointers and Arrays

**Duration**: 5 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Arrays and Pointers Relationship
2. Array Names as Pointers
3. Pointer Notation vs Array Notation
4. Passing Arrays to Functions
5. Pointer Arrays
6. Array of Pointers
7. Common Pitfalls
8. Exercises and Projects

---

## 1. Arrays and Pointers Relationship

### Understanding the Connection:

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};

    // Array name is a pointer to first element
    std::cout << "arr = " << arr << "\n";
    std::cout << "&arr[0] = " << &arr[0] << "\n";

    // They're the same!
    std::cout << "*arr = " << *arr << "\n";
    std::cout << "arr[0] = " << arr[0] << "\n";

    return 0;
}
```

---

## 2. Array Names as Pointers

### Array Decay:

```cpp
#include <iostream>

void printArray(int* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int main()
{
    int numbers[] = {1, 2, 3, 4, 5};

    // Array decays to pointer when passed
    printArray(numbers, 5);

    return 0;
}
```

---

## 3. Pointer Notation vs Array Notation

### Equivalent Access Methods:

```cpp
#include <iostream>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int* ptr = arr;

    // All these are equivalent:
    std::cout << "arr[2] = " << arr[2] << "\n";
    std::cout << "*(arr + 2) = " << *(arr + 2) << "\n";
    std::cout << "ptr[2] = " << ptr[2] << "\n";
    std::cout << "*(ptr + 2) = " << *(ptr + 2) << "\n";

    return 0;
}
```

---

## 4. Passing Arrays to Functions

### Different Ways to Pass Arrays:

```cpp
#include <iostream>

// Method 1: Array notation
void printArray1(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

// Method 2: Pointer notation
void printArray2(int* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << *(arr + i) << " ";
    }
    std::cout << "\n";
}

int main()
{
    int numbers[] = {1, 2, 3, 4, 5};

    printArray1(numbers, 5);
    printArray2(numbers, 5);

    return 0;
}
```

---

## 5. Pointer Arrays

### Working with 2D Arrays:

```cpp
#include <iostream>

int main()
{
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // Pointer to first row
    int (*ptr)[4] = matrix;

    std::cout << "Using pointer to array:\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << ptr[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

---

## 6. Array of Pointers

### Pointer Arrays:

```cpp
#include <iostream>
#include <cstring>

int main()
{
    // Array of pointers to strings
    const char* names[] = {
        "Alice",
        "Bob",
        "Charlie",
        "Diana"
    };

    std::cout << "Names:\n";
    for (int i = 0; i < 4; i++)
    {
        std::cout << i + 1 << ". " << names[i] << "\n";
    }

    return 0;
}
```

---

## Key Takeaways

1. Array names decay to pointers
2. arr[i] is equivalent to *(arr + i)
3. Pointers and arrays are closely related
4. Array size information is lost when passed to functions
5. Always pass size along with array
6. Array of pointers is different from pointer to array
7. Understanding this relationship is crucial
8. Be careful with pointer arithmetic bounds

---

## Next Lesson Preview

In **Lesson 24**, we'll explore:
- Passing pointers to functions
- Returning pointers from functions
- Function pointers
- Callback functions
- Advanced pointer techniques

**Pointers and arrays mastered!**
