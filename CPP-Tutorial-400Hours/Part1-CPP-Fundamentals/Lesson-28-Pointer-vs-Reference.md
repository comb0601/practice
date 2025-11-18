# Lesson 28: Pointer vs Reference

**Duration**: 4 hours
**Difficulty**: Intermediate

## Table of Contents
1. Comparing Pointers and References
2. When to Use Pointers
3. When to Use References
4. Performance Considerations
5. Syntax Comparison
6. Common Patterns
7. Best Practices
8. Exercises and Projects

---

## 1. Comparing Pointers and References

### Side-by-Side Comparison:

```cpp
#include <iostream>

int main()
{
    int value = 42;

    // Pointer
    int* ptr = &value;
    std::cout << "Pointer: " << *ptr << "\n";
    *ptr = 100;

    // Reference
    int& ref = value;
    std::cout << "Reference: " << ref << "\n";
    ref = 200;

    std::cout << "Final value: " << value << "\n";

    return 0;
}
```

---

## 2. When to Use Pointers

### Pointer Use Cases:

```cpp
#include <iostream>

// 1. When you need to represent "no value"
int* findValue(int arr[], int size, int target)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == target)
        {
            return &arr[i];
        }
    }
    return nullptr;  // Not found
}

// 2. When you need to reassign
void demonstrateReassignment()
{
    int a = 10, b = 20;
    int* ptr = &a;

    std::cout << "*ptr = " << *ptr << "\n";

    ptr = &b;  // Reassign to different variable
    std::cout << "*ptr = " << *ptr << "\n";
}

// 3. Dynamic memory
int* createArray(int size)
{
    return new int[size];
}

int main()
{
    int numbers[] = {10, 20, 30};
    int* found = findValue(numbers, 3, 20);

    if (found != nullptr)
    {
        std::cout << "Found: " << *found << "\n";
    }

    demonstrateReassignment();

    int* arr = createArray(5);
    delete[] arr;

    return 0;
}
```

---

## 3. When to Use References

### Reference Use Cases:

```cpp
#include <iostream>
#include <string>

// 1. Function parameters (avoid copying)
void printString(const std::string& str)
{
    std::cout << str << "\n";
}

// 2. Modify function arguments
void increment(int& value)
{
    value++;
}

// 3. Return from operator overloading
class Counter
{
private:
    int value;

public:
    Counter() : value(0) {}

    Counter& operator++()
    {
        value++;
        return *this;  // Return reference for chaining
    }

    int getValue() const { return value; }
};

int main()
{
    std::string message = "Hello";
    printString(message);

    int num = 10;
    increment(num);
    std::cout << "num: " << num << "\n";

    Counter c;
    ++c;
    ++c;
    ++c;
    std::cout << "Counter: " << c.getValue() << "\n";

    return 0;
}
```

---

## 4. Performance Considerations

### Efficiency Comparison:

```cpp
#include <iostream>
#include <string>
#include <vector>

// ❌ Inefficient: Copies large object
void processByValue(std::vector<int> vec)
{
    // Work with copy
}

// ✅ Efficient: No copy with const reference
void processByConstRef(const std::vector<int>& vec)
{
    // Work with original (read-only)
}

// ✅ Efficient: No copy with reference
void processByRef(std::vector<int>& vec)
{
    // Work with original (can modify)
}

// Pointer version (also efficient but more complex)
void processByPtr(std::vector<int>* vec)
{
    if (vec != nullptr)
    {
        // Work with original
    }
}

int main()
{
    std::vector<int> data(1000000);  // Large vector

    // All these avoid copying:
    processByConstRef(data);
    processByRef(data);
    processByPtr(&data);

    return 0;
}
```

---

## 5. Syntax Comparison

### Usage Patterns:

```cpp
#include <iostream>

void compareUsage()
{
    int value = 42;

    // Pointer syntax
    int* ptr = &value;      // Need & to get address
    std::cout << *ptr;      // Need * to dereference
    ptr = nullptr;          // Can be null
    if (ptr != nullptr)     // Need to check
    {
        *ptr = 100;
    }

    // Reference syntax
    int& ref = value;       // Direct binding
    std::cout << ref;       // Direct access (cleaner)
    // ref = nullptr;       // Cannot be null
    ref = 100;              // Direct assignment
}

int main()
{
    compareUsage();
    return 0;
}
```

---

## 6. Common Patterns

### Decision Matrix:

```cpp
#include <iostream>
#include <string>

// Pattern 1: Optional return value → Use pointer
int* findFirst(int arr[], int size, int target)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == target)
            return &arr[i];
    }
    return nullptr;
}

// Pattern 2: Large object parameter → Use const reference
void displayInfo(const std::string& info)
{
    std::cout << info << "\n";
}

// Pattern 3: Modify parameter → Use reference
void doubleValue(int& value)
{
    value *= 2;
}

// Pattern 4: Array access → Use pointer or reference
int& at(int arr[], int index)
{
    return arr[index];
}

int main()
{
    int numbers[] = {10, 20, 30};

    int* found = findFirst(numbers, 3, 20);
    if (found != nullptr)
    {
        std::cout << "Found: " << *found << "\n";
    }

    std::string message = "Hello, World!";
    displayInfo(message);

    int value = 50;
    doubleValue(value);
    std::cout << "Doubled: " << value << "\n";

    at(numbers, 0) = 999;
    std::cout << "numbers[0]: " << numbers[0] << "\n";

    return 0;
}
```

---

## 7. Best Practices

### Guidelines:

```cpp
#include <iostream>
#include <vector>

// ✅ Good: Use const reference for large read-only parameters
void process(const std::vector<int>& data)
{
    for (int val : data)
    {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

// ✅ Good: Use reference for modification
void modify(std::vector<int>& data)
{
    for (int& val : data)
    {
        val *= 2;
    }
}

// ✅ Good: Use pointer when nullptr is valid
int* find(std::vector<int>& data, int target)
{
    for (int& val : data)
    {
        if (val == target)
            return &val;
    }
    return nullptr;
}

// ❌ Bad: Pass by value for large objects
void inefficient(std::vector<int> data)  // Creates copy!
{
    // ...
}

int main()
{
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    process(numbers);
    modify(numbers);
    process(numbers);

    return 0;
}
```

---

## Key Takeaways

1. Pointers can be null, references cannot
2. Pointers can be reassigned, references cannot
3. References are safer and cleaner
4. Use references for parameters when possible
5. Use pointers when nullptr is meaningful
6. Use const reference for large read-only parameters
7. Performance is similar for both
8. Choose based on semantics, not performance

---

## Summary Checklist

Before moving to Lesson 29, ensure you can:
- [ ] Understand pointer vs reference differences
- [ ] Choose appropriate type for each situation
- [ ] Use const references for efficiency
- [ ] Know when pointers are necessary
- [ ] Follow best practices
- [ ] Understand performance implications
- [ ] Write clean, safe code
- [ ] Recognize common patterns

---

## Next Lesson Preview

In **Lesson 29**, we'll explore:
- Null pointer issues
- nullptr vs NULL vs 0
- Pointer safety techniques
- Defensive programming
- Modern C++ safety features

**Pointer vs Reference mastered!**
