# Lesson 26: References

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to References
2. Reference Declaration and Initialization
3. References vs Pointers
4. Reference Parameters
5. Reference Return Values
6. Const References
7. Reference Best Practices
8. Exercises and Projects

---

## 1. Introduction to References

### What is a Reference?

A reference is an alias for another variable. Once initialized, it acts as another name for the same variable.

```cpp
#include <iostream>

int main()
{
    int original = 42;
    int& ref = original;  // ref is a reference to original

    std::cout << "original: " << original << "\n";
    std::cout << "ref: " << ref << "\n";

    ref = 100;  // Modifies original
    std::cout << "After ref = 100:\n";
    std::cout << "original: " << original << "\n";
    std::cout << "ref: " << ref << "\n";

    return 0;
}
```

---

## 2. Reference Declaration and Initialization

### Reference Rules:

```cpp
#include <iostream>

int main()
{
    int value = 10;

    // Must be initialized when declared
    int& ref1 = value;

    // Cannot be reseated
    int other = 20;
    ref1 = other;  // This changes value, not what ref1 refers to!

    std::cout << "value: " << value << " (now 20)\n";
    std::cout << "ref1: " << ref1 << " (still refers to value)\n";

    return 0;
}
```

---

## 3. References vs Pointers

### Key Differences:

```cpp
#include <iostream>

int main()
{
    int num = 42;

    // Reference
    int& ref = num;

    // Pointer
    int* ptr = &num;

    std::cout << "Using reference: " << ref << "\n";
    std::cout << "Using pointer: " << *ptr << "\n";

    // Modify through reference
    ref = 100;
    std::cout << "num after ref = 100: " << num << "\n";

    // Modify through pointer
    *ptr = 200;
    std::cout << "num after *ptr = 200: " << num << "\n";

    return 0;
}
```

---

## 4. Reference Parameters

### Pass by Reference:

```cpp
#include <iostream>

void increment(int& value)
{
    value++;
}

void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

int main()
{
    int num = 10;
    std::cout << "Before: " << num << "\n";

    increment(num);
    std::cout << "After increment: " << num << "\n";

    int x = 5, y = 20;
    std::cout << "\nBefore swap: x=" << x << ", y=" << y << "\n";

    swap(x, y);
    std::cout << "After swap: x=" << x << ", y=" << y << "\n";

    return 0;
}
```

---

## 5. Reference Return Values

### Returning References:

```cpp
#include <iostream>

int& getElement(int arr[], int index)
{
    return arr[index];
}

int main()
{
    int numbers[] = {10, 20, 30, 40, 50};

    std::cout << "numbers[2] = " << numbers[2] << "\n";

    // Modify through reference
    getElement(numbers, 2) = 999;

    std::cout << "After modification: " << numbers[2] << "\n";

    return 0;
}
```

---

## 6. Const References

### Read-Only References:

```cpp
#include <iostream>
#include <string>

void printString(const std::string& str)
{
    std::cout << str << "\n";
    // str = "new value";  // ERROR: Cannot modify const reference
}

double calculateAverage(const int arr[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return static_cast<double>(sum) / size;
}

int main()
{
    std::string message = "Hello, World!";
    printString(message);

    int scores[] = {85, 92, 78, 95, 88};
    double avg = calculateAverage(scores, 5);
    std::cout << "Average: " << avg << "\n";

    return 0;
}
```

---

## Key Takeaways

1. References are aliases for variables
2. Must be initialized when declared
3. Cannot be reseated
4. No null references
5. Use for pass-by-reference
6. Const references for read-only access
7. References are safer than pointers
8. Prefer references over pointers when possible

---

## Next Lesson Preview

In **Lesson 27**, we'll explore:
- Const pointers
- Pointer to const
- Const pointer to const
- Const references in depth
- Const correctness

**References mastered!**
