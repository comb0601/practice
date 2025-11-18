# Lesson 49: Variadic Templates

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. What are Variadic Templates?
2. Parameter Packs
3. Pack Expansion
4. Recursive Processing
5. Fold Expressions (C++17)
6. Practical Examples
7. Use Cases
8. Exercises
9. Practice Projects

---

## 1. What are Variadic Templates?

**Variadic templates** accept variable number of template arguments.

```cpp
#include <iostream>
using namespace std;

// Variadic template
template <typename... Args>
void print(Args... args) {
    // args is a parameter pack
}

int main() {
    print(1);
    print(1, 2.5);
    print(1, 2.5, "Hello", 'c');
    
    return 0;
}
```

---

## 2. Parameter Packs

```cpp
#include <iostream>
using namespace std;

// Base case
void print() {
    cout << endl;
}

// Recursive case
template <typename T, typename... Args>
void print(T first, Args... rest) {
    cout << first << " ";
    print(rest...);  // Recursive call
}

int main() {
    print(1, 2, 3, 4, 5);
    print("Hello", "World", 2024);
    
    return 0;
}
```

---

## 3. Pack Expansion

```cpp
template <typename... Args>
int sum(Args... args) {
    return (args + ...);  // C++17 fold expression
}

int main() {
    cout << sum(1, 2, 3, 4, 5) << endl;  // 15
    cout << sum(1.5, 2.5, 3.5) << endl;  // 7.5
    
    return 0;
}
```

---

## 4. Practical Example: Tuple Printer

```cpp
#include <iostream>
#include <tuple>
using namespace std;

template <typename Tuple, size_t... Is>
void printTupleImpl(const Tuple& t, index_sequence<Is...>) {
    ((cout << (Is == 0 ? "" : ", ") << get<Is>(t)), ...);
}

template <typename... Args>
void printTuple(const tuple<Args...>& t) {
    printTupleImpl(t, index_sequence_for<Args...>{});
    cout << endl;
}

int main() {
    tuple<int, double, string> t(42, 3.14, "Hello");
    printTuple(t);
    
    return 0;
}
```

---

## Key Takeaways

1. **Variadic templates**: Variable arguments
2. **typename... Args**: Parameter pack declaration
3. **Args... args**: Pack expansion
4. **Recursive**: Common pattern for processing
5. **Fold expressions**: C++17 simplification
6. **Use cases**: printf-like functions, tuples
7. **Powerful**: Generic, type-safe interfaces

---

## Next Lesson Preview

In **Lesson 50: STL Overview**, we'll learn:
- Standard Template Library
- Containers, iterators, algorithms
- Vector, list, map, set
- Common STL algorithms
- STL in practice

Great! You understand variadic templates!
