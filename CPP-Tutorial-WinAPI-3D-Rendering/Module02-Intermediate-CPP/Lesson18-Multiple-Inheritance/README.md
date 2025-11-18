# Lesson 18: Multiple Inheritance in C++

**Estimated Time**: 4 hours | **Difficulty**: Advanced

## Table of Contents
1. [Introduction](#intro) 2. [Basic Syntax](#syntax) 3. [Diamond Problem](#diamond)
4. [Virtual Inheritance](#virtual) 5. [Interface Classes](#interfaces)
6. [Mix-in Pattern](#mixin) 7. [Best Practices](#practices)

## 1. Introduction {#intro}
Multiple inheritance allows a class to inherit from multiple base classes.

**Use cases**:
- Combining independent functionalities
- Interface implementation
- Mix-in patterns

**Challenges**:
- Diamond problem
- Name ambiguity
- Increased complexity

## 2. Basic Syntax {#syntax}
```cpp
class Base1 {
public:
    void function1() { }
};

class Base2 {
public:
    void function2() { }
};

class Derived : public Base1, public Base2 {
    // Has both function1() and function2()
};
```

## 3. Diamond Problem {#diamond}
```cpp
class Base { int value; };
class Derived1 : public Base { };
class Derived2 : public Base { };
class Final : public Derived1, public Derived2 { };
// Final has TWO copies of Base!
```

**Accessing ambiguous members**:
```cpp
Final f;
f.Derived1::value;  // First copy
f.Derived2::value;  // Second copy
```

## 4. Virtual Inheritance {#virtual}
Solves the diamond problem:
```cpp
class Base { int value; };
class Derived1 : virtual public Base { };
class Derived2 : virtual public Base { };
class Final : public Derived1, public Derived2 { };
// Final has ONE copy of Base!
```

**How it works**:
- Virtual base class shared by all paths
- Most derived class constructs virtual base
- Small performance cost

## 5. Interface Classes {#interfaces}
Pure abstract classes as interfaces:
```cpp
class Printable {
public:
    virtual void print() const = 0;
    virtual ~Printable() = default;
};

class Saveable {
public:
    virtual void save() const = 0;
    virtual ~Saveable() = default;
};

class Document : public Printable, public Saveable {
public:
    void print() const override { }
    void save() const override { }
};
```

## 6. Mix-in Pattern {#mixin}
Add functionality through inheritance:
```cpp
template<typename Base>
class Timestamped : public Base {
    time_t timestamp;
public:
    Timestamped() : timestamp(time(nullptr)) { }
    time_t getTimestamp() const { return timestamp; }
};

class Data { int value; };
Timestamped<Data> timestamped_data;
```

## 7. Best Practices {#practices}
1. **Prefer composition over multiple inheritance**
2. **Use interfaces (pure abstract classes) when possible**
3. **Use virtual inheritance for diamond scenarios**
4. **Be explicit about ambiguous names**
5. **Keep hierarchies shallow**
6. **Document inheritance structure clearly**
7. **Consider alternatives** (composition, templates)

**When to use multiple inheritance**:
- Implementing multiple interfaces
- Mix-in functionality
- Combining orthogonal concerns

**When to avoid**:
- Deep hierarchies
- Complex diamond scenarios
- When composition works better

**Next**: Lesson 19 - Friend Functions & Classes
