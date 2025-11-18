# Lesson 47: Class Templates

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Class Template Basics
2. Template Member Functions
3. Template Instantiation
4. Template Class Examples
5. Template Friends
6. Template Inheritance
7. Best Practices
8. Exercises
9. Practice Projects

---

## 1. Class Template Basics

```cpp
#include <iostream>
using namespace std;

template <typename T>
class Box {
private:
    T value;
    
public:
    Box(T v) : value(v) {}
    
    T getValue() const { return value; }
    void setValue(T v) { value = v; }
    
    void display() {
        cout << "Value: " << value << endl;
    }
};

int main() {
    Box<int> intBox(42);
    Box<double> doubleBox(3.14);
    Box<string> stringBox("Hello");
    
    intBox.display();
    doubleBox.display();
    stringBox.display();
    
    return 0;
}
```

---

## 2. Template Member Functions

```cpp
template <typename T>
class Array {
private:
    T* data;
    int size;
    
public:
    Array(int s);
    ~Array();
    T& operator[](int index);
    int getSize() const;
};

// Define outside class
template <typename T>
Array<T>::Array(int s) : size(s) {
    data = new T[size];
}

template <typename T>
Array<T>::~Array() {
    delete[] data;
}

template <typename T>
T& Array<T>::operator[](int index) {
    return data[index];
}

template <typename T>
int Array<T>::getSize() const {
    return size;
}
```

---

## 3. Complete Example: Stack Template

```cpp
#include <iostream>
using namespace std;

template <typename T>
class Stack {
private:
    T* arr;
    int capacity;
    int top;
    
public:
    Stack(int size = 100) : capacity(size), top(-1) {
        arr = new T[capacity];
    }
    
    ~Stack() {
        delete[] arr;
    }
    
    void push(T item) {
        if (top == capacity - 1) {
            cout << "Stack overflow!" << endl;
            return;
        }
        arr[++top] = item;
    }
    
    T pop() {
        if (top == -1) {
            cout << "Stack underflow!" << endl;
            return T();
        }
        return arr[top--];
    }
    
    T peek() const {
        if (top == -1) {
            cout << "Stack is empty!" << endl;
            return T();
        }
        return arr[top];
    }
    
    bool isEmpty() const {
        return top == -1;
    }
    
    int getSize() const {
        return top + 1;
    }
};

int main() {
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    
    cout << "Top: " << intStack.peek() << endl;
    cout << "Popped: " << intStack.pop() << endl;
    cout << "Size: " << intStack.getSize() << endl;
    
    Stack<string> stringStack;
    stringStack.push("Hello");
    stringStack.push("World");
    
    cout << "\nString stack top: " << stringStack.peek() << endl;
    
    return 0;
}
```

---

## 4. Multiple Template Parameters

```cpp
template <typename K, typename V>
class Pair {
private:
    K key;
    V value;
    
public:
    Pair(K k, V v) : key(k), value(v) {}
    
    K getKey() const { return key; }
    V getValue() const { return value; }
    
    void display() {
        cout << key << " : " << value << endl;
    }
};

int main() {
    Pair<int, string> p1(1, "One");
    Pair<string, double> p2("Pi", 3.14159);
    
    p1.display();
    p2.display();
    
    return 0;
}
```

---

## Key Takeaways

1. **Class templates**: Generic classes
2. **template <typename T>**: Before class declaration
3. **Instantiation**: `ClassName<Type> obj;`
4. **Member functions**: Need template prefix when defined outside
5. **Multiple parameters**: `template <typename T1, typename T2>`
6. **Type safety**: Compile-time type checking
7. **STL**: Built on class templates

---

## Next Lesson Preview

In **Lesson 48: Template Specialization**, we'll learn:
- Full template specialization
- Partial specialization
- When to specialize
- Advanced template techniques

Excellent! You understand class templates!
