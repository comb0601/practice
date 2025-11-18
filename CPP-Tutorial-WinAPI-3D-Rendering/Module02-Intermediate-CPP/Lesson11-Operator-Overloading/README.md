# Lesson 11: Operator Overloading in C++

**Estimated Time**: 4-5 hours
**Difficulty**: Intermediate
**Prerequisites**: Classes, Objects, Constructors, Destructors

---

## Table of Contents
1. [Introduction to Operator Overloading](#introduction)
2. [Why Operator Overloading?](#why-operator-overloading)
3. [Syntax and Rules](#syntax-and-rules)
4. [Arithmetic Operators (+, -, *, /)](#arithmetic-operators)
5. [Comparison Operators (==, !=, <, >, <=, >=)](#comparison-operators)
6. [Stream Operators (<<, >>)](#stream-operators)
7. [Increment/Decrement Operators (++, --)](#increment-decrement-operators)
8. [Subscript Operator ([])](#subscript-operator)
9. [Assignment Operators (=, +=, -=, etc.)](#assignment-operators)
10. [Function Call Operator ()](#function-call-operator)
11. [Type Conversion Operators](#type-conversion-operators)
12. [Complex Number Class Example](#complex-number-class)
13. [Vector Class Example](#vector-class)
14. [Matrix Class Example](#matrix-class)
15. [Best Practices and Guidelines](#best-practices)
16. [Common Pitfalls](#common-pitfalls)
17. [Exercises](#exercises)

---

## 1. Introduction to Operator Overloading {#introduction}

**Operator overloading** is a feature in C++ that allows you to redefine the behavior of operators (like +, -, *, /, etc.) for user-defined types (classes and structs). This makes your custom types behave like built-in types, resulting in more intuitive and readable code.

### What Can Be Overloaded?

Almost all C++ operators can be overloaded:
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Logical: `&&`, `||`, `!`
- Bitwise: `&`, `|`, `^`, `~`, `<<`, `>>`
- Assignment: `=`, `+=`, `-=`, `*=`, `/=`, etc.
- Increment/Decrement: `++`, `--`
- Subscript: `[]`
- Function call: `()`
- Member access: `->`
- Comma: `,`
- New/Delete: `new`, `delete`, `new[]`, `delete[]`

### What CANNOT Be Overloaded?

The following operators cannot be overloaded:
- Scope resolution: `::`
- Member selection: `.`
- Member pointer selector: `.*`
- Ternary conditional: `?:`
- `sizeof` operator
- `typeid` operator

---

## 2. Why Operator Overloading? {#why-operator-overloading}

### Benefits

1. **Intuitive Code**: Makes custom types behave like built-in types
2. **Readability**: `a + b` is more readable than `a.add(b)`
3. **Consistency**: Users expect operators to work on all types
4. **Integration**: Works seamlessly with algorithms and STL

### Example Comparison

```cpp
// Without operator overloading
ComplexNumber c1(3, 4);
ComplexNumber c2(1, 2);
ComplexNumber c3 = c1.add(c2);
if (c1.isEqual(c2)) { ... }

// With operator overloading
ComplexNumber c1(3, 4);
ComplexNumber c2(1, 2);
ComplexNumber c3 = c1 + c2;
if (c1 == c2) { ... }
```

---

## 3. Syntax and Rules {#syntax-and-rules}

### Member Function Syntax

```cpp
class ClassName {
public:
    ReturnType operator@(Parameters) {
        // Implementation
    }
};
```

### Non-Member Function Syntax

```cpp
ReturnType operator@(ClassName& obj1, ClassName& obj2) {
    // Implementation
}
```

### Important Rules

1. **Cannot Change Precedence**: Operator precedence and associativity cannot be changed
2. **Cannot Create New Operators**: You can only overload existing operators
3. **At Least One User-Defined Type**: At least one operand must be a user-defined type
4. **Cannot Change Arity**: Binary operators remain binary, unary remain unary
5. **Some Must Be Members**: `=`, `[]`, `()`, `->` must be member functions
6. **Return Types**: Usually return by value for arithmetic, by reference for assignment

---

## 4. Arithmetic Operators (+, -, *, /) {#arithmetic-operators}

### Binary Arithmetic Operators

These operators typically:
- Take two operands
- Return a new object by value
- Can be member or non-member functions
- Non-member functions are preferred for symmetry

```cpp
class Complex {
private:
    double real, imag;
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // Member function version
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // Non-member function version (preferred)
    friend Complex operator+(const Complex& c1, const Complex& c2) {
        return Complex(c1.real + c2.real, c1.imag + c2.imag);
    }
};
```

### Unary Arithmetic Operators

```cpp
class Number {
private:
    int value;
public:
    Number(int v = 0) : value(v) {}

    // Unary minus
    Number operator-() const {
        return Number(-value);
    }

    // Unary plus
    Number operator+() const {
        return Number(+value);
    }
};
```

---

## 5. Comparison Operators (==, !=, <, >, <=, >=) {#comparison-operators}

Comparison operators should:
- Return `bool`
- Be `const` member functions
- Implement `==` and `<`, then define others in terms of these
- Be consistent with each other

```cpp
class Point {
private:
    int x, y;
public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    // Equality
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    // Inequality
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    // Less than
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    // Greater than
    bool operator>(const Point& other) const {
        return other < *this;
    }

    // Less than or equal
    bool operator<=(const Point& other) const {
        return !(other < *this);
    }

    // Greater than or equal
    bool operator>=(const Point& other) const {
        return !(*this < other);
    }
};
```

### C++20 Spaceship Operator <=>

C++20 introduces the three-way comparison operator:

```cpp
#include <compare>

class Point {
private:
    int x, y;
public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    // Automatically generates all six comparison operators
    auto operator<=>(const Point& other) const = default;
};
```

---

## 6. Stream Operators (<<, >>) {#stream-operators}

Stream operators must be **non-member functions** to allow `cout << obj` syntax.

```cpp
class Complex {
private:
    double real, imag;
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // Friend function for output
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real;
        if (c.imag >= 0) os << " + " << c.imag << "i";
        else os << " - " << -c.imag << "i";
        return os;
    }

    // Friend function for input
    friend std::istream& operator>>(std::istream& is, Complex& c) {
        is >> c.real >> c.imag;
        return is;
    }
};
```

**Key Points**:
- Return reference to stream for chaining
- Output operator takes `const` reference
- Input operator takes non-const reference
- Must be non-member (usually friend)

---

## 7. Increment/Decrement Operators (++, --) {#increment-decrement-operators}

These operators have two forms:
- **Prefix** (`++x`): Returns reference to modified object
- **Postfix** (`x++`): Returns copy of original object

```cpp
class Counter {
private:
    int count;
public:
    Counter(int c = 0) : count(c) {}

    // Prefix increment: ++counter
    Counter& operator++() {
        ++count;
        return *this;
    }

    // Postfix increment: counter++
    Counter operator++(int) {  // int parameter distinguishes postfix
        Counter temp(*this);
        ++count;
        return temp;
    }

    // Prefix decrement: --counter
    Counter& operator--() {
        --count;
        return *this;
    }

    // Postfix decrement: counter--
    Counter operator--(int) {
        Counter temp(*this);
        --count;
        return temp;
    }

    int getCount() const { return count; }
};
```

**Performance Note**: Prefix is more efficient than postfix because postfix creates a copy.

---

## 8. Subscript Operator ([]) {#subscript-operator}

The subscript operator allows array-like access to objects. It **must be a member function**.

```cpp
class Array {
private:
    int* data;
    size_t size;
public:
    Array(size_t s) : size(s) {
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    ~Array() {
        delete[] data;
    }

    // Non-const version for modification
    int& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    // Const version for read-only access
    const int& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    size_t getSize() const { return size; }
};
```

**Usage**:
```cpp
Array arr(10);
arr[5] = 42;        // Uses non-const version
int val = arr[5];   // Uses non-const version

const Array& carr = arr;
int val2 = carr[5]; // Uses const version
// carr[5] = 10;    // Error: cannot modify const object
```

---

## 9. Assignment Operators (=, +=, -=, etc.) {#assignment-operators}

### Copy Assignment Operator

The copy assignment operator **must be a member function**.

```cpp
class String {
private:
    char* data;
    size_t length;
public:
    String(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }

    ~String() {
        delete[] data;
    }

    // Copy assignment operator
    String& operator=(const String& other) {
        if (this == &other) {  // Self-assignment check
            return *this;
        }

        // Release old memory
        delete[] data;

        // Allocate new memory and copy
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);

        return *this;
    }

    // Copy constructor
    String(const String& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
    }
};
```

### Compound Assignment Operators

```cpp
class Number {
private:
    int value;
public:
    Number(int v = 0) : value(v) {}

    // Compound addition
    Number& operator+=(const Number& other) {
        value += other.value;
        return *this;
    }

    // Compound subtraction
    Number& operator-=(const Number& other) {
        value -= other.value;
        return *this;
    }

    // Compound multiplication
    Number& operator*=(const Number& other) {
        value *= other.value;
        return *this;
    }

    // Compound division
    Number& operator/=(const Number& other) {
        if (other.value == 0) {
            throw std::runtime_error("Division by zero");
        }
        value /= other.value;
        return *this;
    }
};
```

---

## 10. Function Call Operator () {#function-call-operator}

The function call operator allows objects to be used like functions (creating **functors** or **function objects**).

```cpp
class Multiplier {
private:
    int factor;
public:
    Multiplier(int f) : factor(f) {}

    // Function call operator
    int operator()(int x) const {
        return x * factor;
    }

    // Can be overloaded with different parameters
    int operator()(int x, int y) const {
        return (x + y) * factor;
    }
};

// Usage
Multiplier times3(3);
int result1 = times3(10);        // 30
int result2 = times3(5, 7);      // 36
```

**Use Cases**:
- Callback functions
- STL algorithms (predicates, comparators)
- Lambda-like behavior in older C++ standards

---

## 11. Type Conversion Operators {#type-conversion-operators}

### Conversion Constructors

A constructor with a single parameter acts as an implicit type conversion:

```cpp
class Fraction {
private:
    int numerator, denominator;
public:
    // Conversion constructor: int -> Fraction
    Fraction(int num) : numerator(num), denominator(1) {}

    Fraction(int num, int denom) : numerator(num), denominator(denom) {}
};

// Implicit conversion
Fraction f = 5;  // Calls Fraction(5)
```

To prevent implicit conversion, use `explicit`:

```cpp
explicit Fraction(int num) : numerator(num), denominator(1) {}

// Fraction f = 5;  // Error: implicit conversion not allowed
Fraction f(5);      // OK: explicit construction
```

### Conversion Operators

Convert user-defined type to another type:

```cpp
class Fraction {
private:
    int numerator, denominator;
public:
    Fraction(int num, int denom) : numerator(num), denominator(denom) {}

    // Conversion to double
    operator double() const {
        return static_cast<double>(numerator) / denominator;
    }

    // Conversion to int (truncates)
    operator int() const {
        return numerator / denominator;
    }
};

// Usage
Fraction f(3, 4);
double d = f;        // 0.75
int i = f;           // 0
```

Use `explicit` to prevent implicit conversions:

```cpp
explicit operator double() const {
    return static_cast<double>(numerator) / denominator;
}

// double d = f;                     // Error
double d = static_cast<double>(f);   // OK
```

---

## 12. Complex Number Class Example {#complex-number-class}

A comprehensive example implementing a complex number class:

```cpp
#include <iostream>
#include <cmath>

class Complex {
private:
    double real;
    double imag;

public:
    // Constructors
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // Getters
    double getReal() const { return real; }
    double getImag() const { return imag; }

    // Arithmetic operators
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    Complex operator/(const Complex& other) const {
        double denom = other.real * other.real + other.imag * other.imag;
        if (denom == 0) {
            throw std::runtime_error("Division by zero");
        }
        return Complex(
            (real * other.real + imag * other.imag) / denom,
            (imag * other.real - real * other.imag) / denom
        );
    }

    // Unary operators
    Complex operator-() const {
        return Complex(-real, -imag);
    }

    // Compound assignment
    Complex& operator+=(const Complex& other) {
        real += other.real;
        imag += other.imag;
        return *this;
    }

    Complex& operator-=(const Complex& other) {
        real -= other.real;
        imag -= other.imag;
        return *this;
    }

    // Comparison
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    // Utility functions
    double magnitude() const {
        return sqrt(real * real + imag * imag);
    }

    Complex conjugate() const {
        return Complex(real, -imag);
    }

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real;
        if (c.imag >= 0) os << " + " << c.imag << "i";
        else os << " - " << -c.imag << "i";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Complex& c) {
        is >> c.real >> c.imag;
        return is;
    }
};
```

---

## 13. Vector Class Example {#vector-class}

A 3D vector class with overloaded operators:

```cpp
#include <iostream>
#include <cmath>

class Vector3D {
private:
    double x, y, z;

public:
    // Constructor
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    // Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    // Vector addition
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    // Vector subtraction
    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    // Scalar multiplication
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division
    Vector3D operator/(double scalar) const {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero");
        }
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }

    // Dot product
    double operator*(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Unary minus
    Vector3D operator-() const {
        return Vector3D(-x, -y, -z);
    }

    // Compound assignment
    Vector3D& operator+=(const Vector3D& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3D& operator-=(const Vector3D& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Comparison
    bool operator==(const Vector3D& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3D& other) const {
        return !(*this == other);
    }

    // Utility functions
    double magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3D normalize() const {
        double mag = magnitude();
        if (mag == 0) {
            throw std::runtime_error("Cannot normalize zero vector");
        }
        return *this / mag;
    }

    // Cross product
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Vector3D& v) {
        is >> v.x >> v.y >> v.z;
        return is;
    }
};

// Non-member scalar multiplication (scalar * vector)
Vector3D operator*(double scalar, const Vector3D& v) {
    return v * scalar;
}
```

---

## 14. Matrix Class Example {#matrix-class}

A 2D matrix class with operator overloading:

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

class Matrix {
private:
    std::vector<std::vector<double>> data;
    size_t rows, cols;

public:
    // Constructor
    Matrix(size_t r, size_t c, double init = 0.0) : rows(r), cols(c) {
        data.resize(rows, std::vector<double>(cols, init));
    }

    // Getters
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    // Subscript operator for access
    std::vector<double>& operator[](size_t row) {
        if (row >= rows) {
            throw std::out_of_range("Row index out of bounds");
        }
        return data[row];
    }

    const std::vector<double>& operator[](size_t row) const {
        if (row >= rows) {
            throw std::out_of_range("Row index out of bounds");
        }
        return data[row];
    }

    // Matrix addition
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result[i][j] = data[i][j] + other[i][j];
            }
        }
        return result;
    }

    // Matrix subtraction
    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result[i][j] = data[i][j] - other[i][j];
            }
        }
        return result;
    }

    // Matrix multiplication
    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Invalid matrix dimensions for multiplication");
        }
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < other.cols; j++) {
                for (size_t k = 0; k < cols; k++) {
                    result[i][j] += data[i][k] * other[k][j];
                }
            }
        }
        return result;
    }

    // Scalar multiplication
    Matrix operator*(double scalar) const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    // Compound assignment
    Matrix& operator+=(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match");
        }
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                data[i][j] += other[i][j];
            }
        }
        return *this;
    }

    // Comparison
    bool operator==(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                if (data[i][j] != other[i][j]) return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (size_t i = 0; i < m.rows; i++) {
            os << "[";
            for (size_t j = 0; j < m.cols; j++) {
                os << m.data[i][j];
                if (j < m.cols - 1) os << ", ";
            }
            os << "]";
            if (i < m.rows - 1) os << "\n";
        }
        return os;
    }
};

// Non-member scalar multiplication
Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar;
}
```

---

## 15. Best Practices and Guidelines {#best-practices}

### 1. Follow the Principle of Least Surprise
Operators should behave intuitively and consistently with built-in types.

```cpp
// Good: Intuitive behavior
Vector v1, v2;
Vector v3 = v1 + v2;  // Addition makes sense

// Bad: Counter-intuitive
Vector v3 = v1 + v2;  // Actually performs subtraction!
```

### 2. Maintain Consistency
Related operators should be consistent:

```cpp
// If you define ==, also define !=
// If you define <, also define >, <=, >=
// If you define +, also define +=
```

### 3. Return Types
- **Arithmetic operators**: Return by value
- **Assignment operators**: Return reference to `*this`
- **Comparison operators**: Return `bool`
- **Stream operators**: Return reference to stream

### 4. Const Correctness
- Mark operators that don't modify the object as `const`
- Take parameters by `const` reference when possible

```cpp
Complex operator+(const Complex& other) const {  // const method
    return Complex(real + other.real, imag + other.imag);
}
```

### 5. Prefer Non-Member Functions for Symmetry
```cpp
// Member function: only allows c + 5, not 5 + c
Complex Complex::operator+(double d) const { ... }

// Non-member: allows both c + 5 and 5 + c
friend Complex operator+(const Complex& c, double d) { ... }
friend Complex operator+(double d, const Complex& c) { ... }
```

### 6. Use Friend Functions When Needed
Friend functions can access private members while remaining non-members:

```cpp
friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.real << " + " << c.imag << "i";  // Access private members
    return os;
}
```

### 7. Avoid Overloading &&, ||, and Comma Operators
These have special evaluation rules that you can't preserve when overloading.

---

## 16. Common Pitfalls {#common-pitfalls}

### 1. Self-Assignment in operator=
Always check for self-assignment:

```cpp
String& operator=(const String& other) {
    if (this == &other) return *this;  // Self-assignment check
    // ... rest of implementation
}
```

### 2. Returning Local References
Never return references to local variables:

```cpp
// WRONG!
Complex& operator+(const Complex& other) const {
    Complex result(real + other.real, imag + other.imag);
    return result;  // Dangling reference!
}

// CORRECT
Complex operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}
```

### 3. Not Making Const Methods Const
```cpp
// WRONG
Complex operator+(const Complex& other) {  // Not const
    return Complex(real + other.real, imag + other.imag);
}

// CORRECT
Complex operator+(const Complex& other) const {  // Const
    return Complex(real + other.real, imag + other.imag);
}
```

### 4. Forgetting to Return *this in Assignment
```cpp
// WRONG
void operator=(const String& other) {  // Wrong return type
    // ...
}

// CORRECT
String& operator=(const String& other) {
    // ...
    return *this;
}
```

### 5. Not Providing Both Const and Non-Const Versions of operator[]
```cpp
class Array {
public:
    int& operator[](size_t index);              // For modification
    const int& operator[](size_t index) const;  // For const objects
};
```

---

## 17. Exercises {#exercises}

### Exercise 1: Fraction Class
Create a `Fraction` class that:
- Stores numerator and denominator
- Overloads `+`, `-`, `*`, `/` operators
- Automatically simplifies fractions
- Overloads `<<` and `>>` for I/O
- Overloads comparison operators

### Exercise 2: Time Class
Create a `Time` class that:
- Stores hours, minutes, and seconds
- Overloads `+` and `-` for time arithmetic
- Handles overflow/underflow correctly
- Overloads `++` and `--` to add/subtract seconds
- Overloads comparison operators

### Exercise 3: String Class
Create a `String` class that:
- Manages dynamic character array
- Overloads `+` for concatenation
- Overloads `+=` for append
- Overloads `==` and `!=` for comparison
- Overloads `[]` for character access
- Overloads `<<` for output

### Exercise 4: Polynomial Class
Create a `Polynomial` class that:
- Stores coefficients in a vector
- Overloads `+` and `-` for polynomial addition/subtraction
- Overloads `*` for polynomial multiplication
- Overloads `()` to evaluate polynomial at a given x
- Overloads `[]` to access coefficients

### Exercise 5: Money Class
Create a `Money` class that:
- Stores dollars and cents separately
- Overloads arithmetic operators
- Handles currency conversion
- Prevents negative amounts
- Formats output correctly

---

## Summary

Operator overloading is a powerful feature that allows you to:
- Make user-defined types behave like built-in types
- Write more intuitive and readable code
- Integrate seamlessly with STL and algorithms

**Key Takeaways**:
1. Not all operators can be overloaded
2. Some operators must be member functions (`=`, `[]`, `()`, `->`)
3. Follow the principle of least surprise
4. Maintain consistency between related operators
5. Use const correctness
6. Prefer non-member functions for symmetry
7. Always check for self-assignment in `operator=`
8. Return appropriate types (value, reference, or bool)

**Next Steps**:
- Practice implementing operator overloading for various classes
- Study existing C++ library code (like `std::string`, `std::complex`)
- Learn about move semantics and rvalue references (C++11+)
- Explore template operator overloading

---

**Continue to Lesson 12: Templates** to learn about generic programming in C++.
