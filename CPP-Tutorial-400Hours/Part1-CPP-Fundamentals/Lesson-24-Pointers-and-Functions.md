# Lesson 24: Pointers and Functions

**Duration**: 5 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Passing Pointers to Functions
2. Returning Pointers from Functions
3. Function Pointers
4. Callback Functions
5. Pointer Parameters Best Practices
6. Common Patterns
7. Memory Safety
8. Exercises and Projects

---

## 1. Passing Pointers to Functions

### Modifying Values Through Pointers:

```cpp
#include <iostream>

void increment(int* ptr)
{
    (*ptr)++;
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int num = 10;
    std::cout << "Before: " << num << "\n";

    increment(&num);
    std::cout << "After increment: " << num << "\n";

    int x = 5, y = 20;
    std::cout << "\nBefore swap: x=" << x << ", y=" << y << "\n";

    swap(&x, &y);
    std::cout << "After swap: x=" << x << ", y=" << y << "\n";

    return 0;
}
```

---

## 2. Returning Pointers from Functions

### Safe Return of Pointers:

```cpp
#include <iostream>

// Returning pointer to static variable (safe)
int* getStaticValue()
{
    static int value = 100;
    return &value;
}

// Returning pointer to dynamically allocated memory (safe if handled properly)
int* createArray(int size)
{
    int* arr = new int[size];
    for (int i = 0; i < size; i++)
    {
        arr[i] = i + 1;
    }
    return arr;
}

int main()
{
    int* ptr1 = getStaticValue();
    std::cout << "Static value: " << *ptr1 << "\n";

    int* arr = createArray(5);
    for (int i = 0; i < 5; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    delete[] arr;  // Important!

    return 0;
}
```

---

## 3. Function Pointers

### Declaring and Using Function Pointers:

```cpp
#include <iostream>

int add(int a, int b)
{
    return a + b;
}

int multiply(int a, int b)
{
    return a * b;
}

int main()
{
    // Function pointer declaration
    int (*operation)(int, int);

    // Point to add function
    operation = add;
    std::cout << "5 + 3 = " << operation(5, 3) << "\n";

    // Point to multiply function
    operation = multiply;
    std::cout << "5 * 3 = " << operation(5, 3) << "\n";

    return 0;
}
```

---

## 4. Callback Functions

### Using Callbacks:

```cpp
#include <iostream>

void processArray(int arr[], int size, void (*callback)(int))
{
    for (int i = 0; i < size; i++)
    {
        callback(arr[i]);
    }
}

void printValue(int value)
{
    std::cout << value << " ";
}

void printSquare(int value)
{
    std::cout << value * value << " ";
}

int main()
{
    int numbers[] = {1, 2, 3, 4, 5};

    std::cout << "Values: ";
    processArray(numbers, 5, printValue);
    std::cout << "\n";

    std::cout << "Squares: ";
    processArray(numbers, 5, printSquare);
    std::cout << "\n";

    return 0;
}
```

---

## Key Takeaways

1. Pointers enable pass-by-reference
2. Functions can return pointers
3. Be careful returning pointers to local variables
4. Function pointers enable callbacks
5. Callbacks provide flexible design
6. Always validate pointer parameters
7. Document ownership of returned pointers
8. Consider using references instead when appropriate

---

## Next Lesson Preview

In **Lesson 25**, we'll explore:
- Dynamic memory allocation with new/delete
- Memory management
- Heap vs stack
- Memory leaks prevention
- RAII principles

**Pointers and functions mastered!**
