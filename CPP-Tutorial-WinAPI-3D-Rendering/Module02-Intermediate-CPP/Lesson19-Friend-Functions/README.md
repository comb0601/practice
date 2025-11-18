# Lesson 19: Friend Functions & Classes in C++

**Estimated Time**: 3-4 hours | **Difficulty**: Intermediate

## Table of Contents
1. [Introduction](#intro) 2. [Friend Functions](#functions) 3. [Friend Classes](#classes)
4. [Operator Overloading](#operators) 5. [When to Use](#when) 6. [Best Practices](#practices)

## 1. Introduction {#intro}
Friend functions and classes can access private and protected members of a class.

**Purpose**:
- Operator overloading
- Utility functions
- Tight coupling between classes

**Caution**: Breaks encapsulation if overused!

## 2. Friend Functions {#functions}
```cpp
class Box {
private:
    int width;
    friend void printWidth(Box& b);
public:
    Box(int w) : width(w) { }
};

void printWidth(Box& b) {
    std::cout << b.width << std::endl;  // Can access private members
}
```

## 3. Friend Classes {#classes}
```cpp
class Box {
private:
    int width;
    friend class BoxPrinter;
public:
    Box(int w) : width(w) { }
};

class BoxPrinter {
public:
    void print(const Box& b) {
        std::cout << b.width << std::endl;
    }
};
```

**Note**: Friendship is not transitive or inherited!

## 4. Operator Overloading with Friends {#operators}
Friends enable symmetric binary operators:

```cpp
class Complex {
    double real, imag;
public:
    Complex(double r, double i) : real(r), imag(i) { }
    
    // Friend allows: complex + int AND int + complex
    friend Complex operator+(const Complex& c, int n) {
        return Complex(c.real + n, c.imag);
    }
    
    friend Complex operator+(int n, const Complex& c) {
        return Complex(n + c.real, c.imag);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real << " + " << c.imag << "i";
        return os;
    }
};
```

## 5. When to Use Friends {#when}
**Good uses**:
1. **Stream operators** (`<<`, `>>`) - must be non-members
2. **Symmetric binary operators** - allows both `obj + int` and `int + obj`
3. **Tightly coupled classes** - when two classes work together closely
4. **Utility functions** - needing private access without being members

**Bad uses**:
- As a shortcut to avoid proper encapsulation
- For functions that don't need private access
- Creating complex friend webs

## 6. Best Practices {#practices}
1. **Minimize friend declarations**
2. **Prefer member functions when possible**
3. **Use friends for operators that need symmetry**
4. **Document why friendship is necessary**
5. **Consider if composition might be better**
6. **Keep friend functions in same file as class**
7. **Friends are part of the class interface**

**Alternatives to friends**:
- Public accessor methods
- Member functions
- Nested classes
- Composition

**Summary**: Friends are powerful but should be used sparingly. They're most appropriate for operator overloading and tightly coupled designs.

**Next**: Lesson 20 - Const Correctness
