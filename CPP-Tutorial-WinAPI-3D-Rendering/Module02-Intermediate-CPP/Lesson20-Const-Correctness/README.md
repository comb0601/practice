# Lesson 20: Const Correctness in C++

**Estimated Time**: 4 hours | **Difficulty**: Intermediate

## Table of Contents
1. [Introduction](#intro) 2. [Const Variables](#variables) 3. [Const Pointers](#pointers)
4. [Const Member Functions](#members) 5. [Const Parameters](#parameters)
6. [Const Return Values](#returns) 7. [Mutable](#mutable) 8. [Best Practices](#practices)

## 1. Introduction {#intro}
Const correctness is using `const` keyword properly to document immutability and enable compiler optimizations.

**Benefits**:
- Prevents accidental modifications
- Documents intent
- Enables compiler optimizations
- Catches bugs at compile time

## 2. Const Variables {#variables}
```cpp
const int x = 10;
// x = 20;  // Error: cannot modify

const double PI = 3.14159;
const std::string name = "Alice";
```

## 3. Const Pointers {#pointers}
```cpp
int value = 10;

const int* ptr1 = &value;     // Pointer to const int
// *ptr1 = 20;                // Error
ptr1 = &other;                // OK

int* const ptr2 = &value;     // Const pointer to int
*ptr2 = 20;                   // OK
// ptr2 = &other;             // Error

const int* const ptr3 = &value;  // Const pointer to const int
// *ptr3 = 20;                // Error
// ptr3 = &other;             // Error
```

**Tip**: Read right-to-left: `const int* const` = "const pointer to const int"

## 4. Const Member Functions {#members}
```cpp
class MyClass {
    int value;
public:
    MyClass(int v) : value(v) { }
    
    int getValue() const {  // Doesn't modify object
        return value;
    }
    
    void setValue(int v) {  // Modifies object
        value = v;
    }
};

const MyClass obj(10);
obj.getValue();    // OK
// obj.setValue(20);  // Error: non-const function on const object
```

## 5. Const Parameters {#parameters}
```cpp
// Pass by const reference - efficient and safe
void process(const std::vector<int>& data) {
    // Cannot modify data
    for (int val : data) {
        std::cout << val << " ";
    }
}

// Small types can be const by value
void compute(const int value) {
    // Local copy is const
}
```

## 6. Const Return Values {#returns}
```cpp
class String {
    char* data;
public:
    // Return const reference for read-only access
    const char* getData() const {
        return data;
    }
    
    // Return non-const reference for modification
    char* getData() {
        return data;
    }
};
```

## 7. Mutable Keyword {#mutable}
Allows modification of const objects:
```cpp
class Cache {
    mutable int accessCount;  // Can be modified even in const functions
    int value;
public:
    Cache(int v) : value(v), accessCount(0) { }
    
    int getValue() const {
        accessCount++;  // OK: mutable member
        return value;
    }
};
```

**Use cases for mutable**:
- Caching
- Lazy initialization
- Debugging counters
- Mutexes

## 8. Best Practices {#practices}
1. **Use const by default, remove only when necessary**
2. **Mark member functions const if they don't modify state**
3. **Pass large objects by const reference**
4. **Return const references when appropriate**
5. **Use const iterators when not modifying**
6. **Const correctness is contagious - embrace it!**
7. **const_cast is a code smell - avoid it**

**Const Overloading**:
```cpp
class Array {
    int data[10];
public:
    // Non-const version for modification
    int& operator[](int index) {
        return data[index];
    }
    
    // Const version for read-only
    const int& operator[](int index) const {
        return data[index];
    }
};
```

**Summary**: Const correctness makes code safer, more maintainable, and self-documenting. Start using const everywhere possible - your future self will thank you!

**Congratulations!** You've completed Module 2: Intermediate C++!
