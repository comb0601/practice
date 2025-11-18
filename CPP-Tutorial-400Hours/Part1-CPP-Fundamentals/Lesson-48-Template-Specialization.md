# Lesson 48: Template Specialization

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Full Template Specialization
2. Partial Template Specialization
3. Function Template Specialization
4. Class Template Specialization
5. When to Use Specialization
6. Examples and Use Cases
7. Best Practices
8. Exercises
9. Practice Projects

---

## 1. Full Template Specialization

Provide specific implementation for a particular type.

```cpp
#include <iostream>
#include <cstring>
using namespace std;

// Generic template
template <typename T>
class Container {
private:
    T value;
public:
    Container(T v) : value(v) {}
    void display() { cout << value << endl; }
};

// Specialization for const char*
template <>
class Container<const char*> {
private:
    char* value;
public:
    Container(const char* v) {
        value = new char[strlen(v) + 1];
        strcpy(value, v);
    }
    ~Container() { delete[] value; }
    void display() { cout << value << " (string)" << endl; }
};

int main() {
    Container<int> c1(42);
    Container<const char*> c2("Hello");
    
    c1.display();
    c2.display();
    
    return 0;
}
```

---

## 2. Partial Template Specialization

Specialize for subset of template parameters.

```cpp
template <typename T, typename U>
class Pair {
public:
    void info() { cout << "Generic Pair" << endl; }
};

// Partial specialization: both same type
template <typename T>
class Pair<T, T> {
public:
    void info() { cout << "Pair of same types" << endl; }
};

// Partial specialization: second is pointer
template <typename T, typename U>
class Pair<T, U*> {
public:
    void info() { cout << "Second is pointer" << endl; }
};

int main() {
    Pair<int, double> p1;       // Generic
    Pair<int, int> p2;          // Same type specialization
    Pair<int, double*> p3;      // Pointer specialization
    
    p1.info();
    p2.info();
    p3.info();
    
    return 0;
}
```

---

## Key Takeaways

1. **Full specialization**: Complete custom implementation
2. **Partial specialization**: Specialize subset of parameters
3. **Function templates**: Can only be fully specialized
4. **Class templates**: Can be partially specialized
5. **Syntax**: `template <>` for full, `template <remaining>` for partial
6. **Use cases**: Optimization, special handling
7. **Prefer generic**: Only specialize when needed

---

## Next Lesson Preview

In **Lesson 49: Variadic Templates**, we'll learn:
- Variable number of template arguments
- Parameter packs
- Fold expressions
- Advanced template techniques

Excellent work on template specialization!
