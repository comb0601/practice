# Lesson 46: Function Templates

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Templates
2. Function Template Syntax
3. Template Parameters
4. Template Instantiation
5. Template Specialization
6. Multiple Template Parameters
7. Template Best Practices
8. Exercises
9. Practice Projects

---

## 1. Introduction to Templates

**Templates** enable generic programming - write code once, use with multiple types.

### Why Templates?
- Code reuse
- Type safety
- Performance (no runtime overhead)
- Generic algorithms

---

## 2. Function Template Syntax

```cpp
#include <iostream>
using namespace std;

// Function template
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    cout << maximum(10, 20) << endl;        // int
    cout << maximum(3.14, 2.71) << endl;    // double
    cout << maximum('a', 'z') << endl;      // char
    
    return 0;
}
```

### Alternative Syntax:
```cpp
template <class T>  // 'class' same as 'typename'
T minimum(T a, T b) {
    return (a < b) ? a : b;
}
```

---

## 3. Template Parameters

```cpp
#include <iostream>
using namespace std;

template <typename T>
void printArray(T arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int intArr[] = {1, 2, 3, 4, 5};
    double doubleArr[] = {1.1, 2.2, 3.3};
    
    printArray(intArr, 5);
    printArray(doubleArr, 3);
    
    return 0;
}
```

---

## 4. Template Instantiation

Compiler generates specific version for each type used.

```cpp
template <typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    add(5, 10);        // Instantiates add<int>
    add(3.14, 2.71);   // Instantiates add<double>
    
    // Explicit instantiation
    add<float>(1.0f, 2.0f);
    
    return 0;
}
```

---

## 5. Template Specialization

```cpp
#include <iostream>
#include <cstring>
using namespace std;

// Generic template
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// Specialization for const char*
template <>
const char* maximum<const char*>(const char* a, const char* b) {
    return (strcmp(a, b) > 0) ? a : b;
}

int main() {
    cout << maximum(10, 20) << endl;
    cout << maximum("apple", "banana") << endl;  // Uses specialization
    
    return 0;
}
```

---

## 6. Multiple Template Parameters

```cpp
#include <iostream>
using namespace std;

template <typename T1, typename T2>
void printPair(T1 first, T2 second) {
    cout << "(" << first << ", " << second << ")" << endl;
}

template <typename T>
T sum(T arr[], int size) {
    T result = T();  // Initialize to default value
    for (int i = 0; i < size; i++) {
        result += arr[i];
    }
    return result;
}

int main() {
    printPair(10, "Hello");
    printPair(3.14, true);
    
    int numbers[] = {1, 2, 3, 4, 5};
    cout << "Sum: " << sum(numbers, 5) << endl;
    
    return 0;
}
```

---

## 7. Template Best Practices

1. **Use meaningful names**: `template <typename T>` not `template <typename X>`
2. **Document constraints**: What operations must T support?
3. **Test with multiple types**
4. **Consider concepts** (C++20)
5. **Avoid template bloat**: Each type instantiates code

---

## Complete Example: Generic Sorting

```cpp
#include <iostream>
using namespace std;

template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void bubbleSort(T arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

template <typename T>
void printArray(T arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int intArr[] = {64, 34, 25, 12, 22, 11, 90};
    int n1 = sizeof(intArr) / sizeof(intArr[0]);
    
    cout << "Before sorting: ";
    printArray(intArr, n1);
    
    bubbleSort(intArr, n1);
    
    cout << "After sorting: ";
    printArray(intArr, n1);
    
    // Works with doubles too!
    double doubleArr[] = {3.14, 1.41, 2.71, 0.577};
    int n2 = sizeof(doubleArr) / sizeof(doubleArr[0]);
    
    cout << "\nDouble array before: ";
    printArray(doubleArr, n2);
    
    bubbleSort(doubleArr, n2);
    
    cout << "Double array after: ";
    printArray(doubleArr, n2);
    
    return 0;
}
```

---

## Key Takeaways

1. **Templates**: Generic programming in C++
2. **template <typename T>**: Function template syntax
3. **Type inference**: Compiler deduces T from arguments
4. **Explicit instantiation**: `func<int>()`
5. **Specialization**: Custom implementation for specific type
6. **Multiple parameters**: `template <typename T1, typename T2>`
7. **Compile-time**: Template instantiation at compile time

---

## Next Lesson Preview

In **Lesson 47: Class Templates**, we'll learn:
- Class templates syntax
- Template member functions
- Template class instantiation
- Generic containers

Great! You understand function templates!
