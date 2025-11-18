# Lesson 44: Polymorphism and Virtual Functions

**Duration**: 9 hours
**Difficulty**: Advanced

## Table of Contents
1. What is Polymorphism?
2. Compile-Time vs Runtime Polymorphism
3. Virtual Functions
4. Virtual Destructors
5. Override and Final Keywords
6. VTable and VPointer
7. Pure Virtual Functions Preview
8. Exercises
9. Practice Projects

---

## 1. What is Polymorphism?

**Polymorphism** = "many forms". Objects of different types can be accessed through the same interface.

Types:
1. **Compile-time** (Static): Function/Operator overloading
2. **Runtime** (Dynamic): Virtual functions

---

## 2. Compile-Time vs Runtime Polymorphism

### Compile-Time (Function Overloading):
```cpp
class Math {
public:
    int add(int a, int b) { return a + b; }
    double add(double a, double b) { return a + b; }
};
```

### Runtime (Virtual Functions):
```cpp
class Shape {
public:
    virtual void draw() { cout << "Drawing shape" << endl; }
};

class Circle : public Shape {
public:
    void draw() override { cout << "Drawing circle" << endl; }
};
```

---

## 3. Virtual Functions

**Virtual functions** enable runtime polymorphism.

```cpp
#include <iostream>
using namespace std;

class Animal {
public:
    virtual void makeSound() {
        cout << "Generic animal sound" << endl;
    }
    
    virtual void move() {
        cout << "Animal moves" << endl;
    }
};

class Dog : public Animal {
public:
    void makeSound() override {
        cout << "Woof! Woof!" << endl;
    }
    
    void move() override {
        cout << "Dog runs" << endl;
    }
};

class Cat : public Animal {
public:
    void makeSound() override {
        cout << "Meow!" << endl;
    }
    
    void move() override {
        cout << "Cat walks silently" << endl;
    }
};

void animalActions(Animal* animal) {
    animal->makeSound();  // Polymorphic call!
    animal->move();
}

int main() {
    Dog dog;
    Cat cat;
    
    animalActions(&dog);  // Calls Dog's functions
    cout << endl;
    animalActions(&cat);  // Calls Cat's functions
    
    return 0;
}
```

Output:
```
Woof! Woof!
Dog runs

Meow!
Cat walks silently
```

---

## 4. Virtual Destructors

**Always make destructors virtual** in base classes with virtual functions!

```cpp
#include <iostream>
using namespace std;

class Base {
public:
    Base() { cout << "Base constructor" << endl; }
    
    // Virtual destructor - IMPORTANT!
    virtual ~Base() { cout << "Base destructor" << endl; }
};

class Derived : public Base {
private:
    int* data;
    
public:
    Derived() {
        data = new int[10];
        cout << "Derived constructor" << endl;
    }
    
    ~Derived() {
        delete[] data;
        cout << "Derived destructor" << endl;
    }
};

int main() {
    Base* ptr = new Derived();
    delete ptr;  // Calls both destructors correctly!
    return 0;
}
```

---

## 5. Override and Final Keywords (C++11)

```cpp
class Base {
public:
    virtual void func1() {}
    virtual void func2() {}
};

class Derived : public Base {
public:
    void func1() override {}  // Explicitly override
    // void func3() override {}  // ERROR: func3 not in base
    
    void func2() final {}  // Cannot be overridden further
};

class MoreDerived : public Derived {
    // void func2() override {}  // ERROR: func2 is final
};
```

---

## 6. VTable and VPointer

Virtual functions work through:
- **VTable** (Virtual Table): Array of function pointers
- **VPointer**: Pointer to VTable in each object

```
Object with virtual functions:
┌─────────────┐
│  vptr   ────┼──> VTable
├─────────────┤
│  data members
└─────────────┘

VTable:
┌────────────────┐
│ &func1()       │
│ &func2()       │
│ &func3()       │
└────────────────┘
```

---

## 7. Pure Virtual Functions Preview

```cpp
class Shape {
public:
    // Pure virtual function
    virtual double area() = 0;
    virtual void draw() = 0;
};
// Shape is now abstract - cannot instantiate
```

---

## Key Takeaways

1. **Polymorphism**: Many forms, one interface
2. **Virtual functions**: Enable runtime polymorphism
3. **override**: Explicitly mark overridden functions
4. **final**: Prevent further overriding
5. **Virtual destructor**: Required for proper cleanup
6. **VTable**: Mechanism for virtual functions
7. **Base pointer**: Can point to derived objects

---

## Next Lesson Preview

In **Lesson 45: Abstract Classes and Interfaces**, we'll learn:
- Pure virtual functions
- Abstract classes
- Interfaces in C++
- Design patterns with abstractions

Fantastic! You understand polymorphism!
