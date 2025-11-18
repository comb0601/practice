# Lesson 39: Operator Overloading Part 2

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Stream Insertion and Extraction Operators
2. Subscript Operator []
3. Function Call Operator ()
4. Arrow Operator ->
5. Type Conversion Operators
6. Assignment Operators
7. Complete Examples
8. Exercises
9. Practice Projects

---

## 1. Stream Insertion and Extraction Operators

**<<** (insertion) and **>>** (extraction) must be overloaded as friend functions.

### Syntax:
```cpp
class MyClass {
    friend ostream& operator<<(ostream& out, const MyClass& obj);
    friend istream& operator>>(istream& in, MyClass& obj);
};
```

### Example 1: Basic Stream Operators

```cpp
#include <iostream>
#include <string>
using namespace std;

class Point {
private:
    double x, y;

public:
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    // Output operator
    friend ostream& operator<<(ostream& out, const Point& p) {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }

    // Input operator
    friend istream& operator>>(istream& in, Point& p) {
        cout << "Enter x y: ";
        in >> p.x >> p.y;
        return in;
    }
};

int main() {
    Point p1(3.5, 7.2);
    cout << "Point p1: " << p1 << endl;

    Point p2;
    cin >> p2;
    cout << "You entered: " << p2 << endl;

    // Can chain outputs
    Point p3(1, 2), p4(3, 4);
    cout << "Points: " << p3 << ", " << p4 << endl;

    return 0;
}
```

### Example 2: Formatted Output

```cpp
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int id;
    double gpa;

public:
    Student(const string& n = "", int i = 0, double g = 0.0)
        : name(n), id(i), gpa(g) {}

    friend ostream& operator<<(ostream& out, const Student& s) {
        out << "┌" << string(40, '─') << "┐" << endl;
        out << "│ Student Information" << string(20, ' ') << "│" << endl;
        out << "├" << string(40, '─') << "┤" << endl;
        out << "│ Name: " << left << setw(31) << s.name << "│" << endl;
        out << "│ ID:   " << setw(31) << s.id << "│" << endl;
        out << "│ GPA:  " << fixed << setprecision(2) << setw(31)
            << s.gpa << "│" << endl;
        out << "└" << string(40, '─') << "┘";
        return out;
    }

    friend istream& operator>>(istream& in, Student& s) {
        cout << "Enter name: ";
        getline(in, s.name);
        cout << "Enter ID: ";
        in >> s.id;
        cout << "Enter GPA: ";
        in >> s.gpa;
        in.ignore();  // Clear newline
        return in;
    }
};

int main() {
    Student student("Alice Johnson", 12345, 3.85);
    cout << student << endl;

    return 0;
}
```

---

## 2. Subscript Operator []

The **subscript operator** allows array-like access to objects.

### Syntax:
```cpp
class MyClass {
public:
    Type& operator[](int index);              // For modification
    const Type& operator[](int index) const;  // For const objects
};
```

### Example 1: Array Wrapper

```cpp
#include <iostream>
using namespace std;

class IntArray {
private:
    int* data;
    int size;

public:
    IntArray(int s) : size(s) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    ~IntArray() {
        delete[] data;
    }

    // Non-const version - allows modification
    int& operator[](int index) {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds!" << endl;
            return data[0];  // Return first element as fallback
        }
        return data[index];
    }

    // Const version - read-only
    const int& operator[](int index) const {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds!" << endl;
            return data[0];
        }
        return data[index];
    }

    int getSize() const { return size; }
};

int main() {
    IntArray arr(5);

    // Set values using []
    for (int i = 0; i < 5; i++) {
        arr[i] = (i + 1) * 10;
    }

    // Read values using []
    cout << "Array contents: ";
    for (int i = 0; i < arr.getSize(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Test bounds checking
    arr[10] = 999;  // Out of bounds

    return 0;
}
```

### Example 2: 2D Matrix

```cpp
#include <iostream>
using namespace std;

class Matrix {
private:
    double** data;
    int rows, cols;

public:
    class Row {
    private:
        double* rowData;
        int cols;

    public:
        Row(double* row, int c) : rowData(row), cols(c) {}

        double& operator[](int col) {
            if (col < 0 || col >= cols) {
                cout << "Column out of bounds!" << endl;
                return rowData[0];
            }
            return rowData[col];
        }
    };

    Matrix(int r, int c) : rows(r), cols(c) {
        data = new double*[rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
            for (int j = 0; j < cols; j++) {
                data[i][j] = 0.0;
            }
        }
    }

    ~Matrix() {
        for (int i = 0; i < rows; i++) {
            delete[] data[i];
        }
        delete[] data;
    }

    Row operator[](int row) {
        if (row < 0 || row >= rows) {
            cout << "Row out of bounds!" << endl;
            return Row(data[0], cols);
        }
        return Row(data[row], cols);
    }

    void display() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << data[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    Matrix m(3, 3);

    // Use like 2D array
    m[0][0] = 1.0;
    m[0][1] = 2.0;
    m[1][0] = 3.0;
    m[1][1] = 4.0;
    m[2][2] = 5.0;

    m.display();

    return 0;
}
```

---

## 3. Function Call Operator ()

The **function call operator** allows objects to be called like functions (functors).

### Example 1: Simple Functor

```cpp
#include <iostream>
using namespace std;

class Multiplier {
private:
    int factor;

public:
    Multiplier(int f) : factor(f) {}

    // Function call operator
    int operator()(int value) const {
        return value * factor;
    }
};

class Adder {
private:
    int base;

public:
    Adder(int b) : base(b) {}

    int operator()(int value) const {
        return value + base;
    }
};

int main() {
    Multiplier times3(3);
    Adder plus10(10);

    cout << "times3(5) = " << times3(5) << endl;
    cout << "plus10(5) = " << plus10(5) << endl;

    // Can be passed to functions
    int arr[] = {1, 2, 3, 4, 5};
    cout << "Transformed array: ";
    for (int x : arr) {
        cout << times3(x) << " ";
    }
    cout << endl;

    return 0;
}
```

### Example 2: Function with State

```cpp
#include <iostream>
using namespace std;

class Counter {
private:
    mutable int count;  // mutable allows modification in const method

public:
    Counter() : count(0) {}

    int operator()() const {
        return ++count;
    }

    int getCount() const {
        return count;
    }
};

class Generator {
private:
    int start;
    int step;
    mutable int current;

public:
    Generator(int s = 0, int st = 1)
        : start(s), step(st), current(s) {}

    int operator()() const {
        int value = current;
        current += step;
        return value;
    }

    void reset() {
        current = start;
    }
};

int main() {
    Counter counter;

    cout << "Calling counter 5 times:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "Call " << (i + 1) << ": " << counter() << endl;
    }

    cout << "\nGenerator (start=10, step=5):" << endl;
    Generator gen(10, 5);
    for (int i = 0; i < 5; i++) {
        cout << gen() << " ";
    }
    cout << endl;

    return 0;
}
```

---

## 4. Arrow Operator ->

The **arrow operator** is used for smart pointers.

### Example: Smart Pointer

```cpp
#include <iostream>
#include <string>
using namespace std;

class Resource {
public:
    void doSomething() {
        cout << "Resource doing something!" << endl;
    }

    void display() {
        cout << "Resource display" << endl;
    }
};

class SmartPointer {
private:
    Resource* ptr;

public:
    SmartPointer(Resource* p = nullptr) : ptr(p) {}

    ~SmartPointer() {
        delete ptr;
        cout << "Resource deleted" << endl;
    }

    // Dereference operator
    Resource& operator*() {
        return *ptr;
    }

    // Arrow operator
    Resource* operator->() {
        return ptr;
    }

    bool isNull() const {
        return ptr == nullptr;
    }
};

int main() {
    SmartPointer sp(new Resource());

    // Use arrow operator
    sp->doSomething();
    sp->display();

    // Use dereference operator
    (*sp).doSomething();

    return 0;  // Resource automatically deleted
}
```

---

## 5. Type Conversion Operators

**Type conversion operators** allow implicit or explicit conversion to other types.

### Example 1: Basic Conversion

```cpp
#include <iostream>
using namespace std;

class Fraction {
private:
    int numerator;
    int denominator;

public:
    Fraction(int n, int d = 1) : numerator(n), denominator(d) {}

    // Conversion to double
    operator double() const {
        return static_cast<double>(numerator) / denominator;
    }

    // Conversion to int
    operator int() const {
        return numerator / denominator;
    }

    // Conversion to bool (for if statements)
    operator bool() const {
        return numerator != 0;
    }

    void display() const {
        cout << numerator << "/" << denominator;
    }
};

int main() {
    Fraction f(3, 4);

    cout << "Fraction: ";
    f.display();
    cout << endl;

    // Implicit conversion to double
    double d = f;
    cout << "As double: " << d << endl;

    // Implicit conversion to int
    int i = f;
    cout << "As int: " << i << endl;

    // Conversion to bool
    if (f) {
        cout << "Fraction is non-zero" << endl;
    }

    Fraction zero(0, 1);
    if (!zero) {
        cout << "Zero fraction is false" << endl;
    }

    return 0;
}
```

### Example 2: Explicit Conversion

```cpp
#include <iostream>
#include <string>
using namespace std;

class Temperature {
private:
    double celsius;

public:
    Temperature(double c) : celsius(c) {}

    // Explicit conversion to Fahrenheit
    explicit operator double() const {
        return celsius * 9.0 / 5.0 + 32.0;
    }

    // Implicit conversion to string
    operator string() const {
        return to_string(celsius) + "°C";
    }

    double getCelsius() const { return celsius; }
};

int main() {
    Temperature temp(25.0);

    // Implicit conversion to string
    string s = temp;
    cout << "As string: " << s << endl;

    // Explicit conversion to double (Fahrenheit)
    double f = static_cast<double>(temp);
    cout << "In Fahrenheit: " << f << "°F" << endl;

    // This would cause error (explicit):
    // double f2 = temp;  // ERROR

    return 0;
}
```

---

## 6. Assignment Operators

### Example: Complete Assignment Operators

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    char* data;
    int length;

public:
    String(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }

    ~String() {
        delete[] data;
    }

    // Copy assignment
    String& operator=(const String& other) {
        if (this != &other) {  // Self-assignment check
            delete[] data;

            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    // Compound assignment operators
    String& operator+=(const String& other) {
        int newLength = length + other.length;
        char* newData = new char[newLength + 1];

        strcpy(newData, data);
        strcat(newData, other.data);

        delete[] data;
        data = newData;
        length = newLength;

        return *this;
    }

    // Addition operator using +=
    String operator+(const String& other) const {
        String result = *this;
        result += other;
        return result;
    }

    void display() const {
        cout << data;
    }
};

int main() {
    String s1("Hello");
    String s2(" World");

    s1.display();
    cout << " + ";
    s2.display();
    cout << " = ";

    String s3 = s1 + s2;
    s3.display();
    cout << endl;

    s1 += s2;
    cout << "After s1 += s2: ";
    s1.display();
    cout << endl;

    return 0;
}
```

---

## 7. Complete Examples

### Example: Comprehensive Complex Number Class

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Complex {
private:
    double real, imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // Arithmetic operators
    Complex operator+(const Complex& c) const {
        return Complex(real + c.real, imag + c.imag);
    }

    Complex operator-(const Complex& c) const {
        return Complex(real - c.real, imag - c.imag);
    }

    Complex operator*(const Complex& c) const {
        return Complex(real * c.real - imag * c.imag,
                       real * c.imag + imag * c.real);
    }

    Complex operator/(const Complex& c) const {
        double denominator = c.real * c.real + c.imag * c.imag;
        return Complex((real * c.real + imag * c.imag) / denominator,
                       (imag * c.real - real * c.imag) / denominator);
    }

    // Compound assignment
    Complex& operator+=(const Complex& c) {
        real += c.real;
        imag += c.imag;
        return *this;
    }

    // Comparison
    bool operator==(const Complex& c) const {
        return fabs(real - c.real) < 0.0001 &&
               fabs(imag - c.imag) < 0.0001;
    }

    // Unary operators
    Complex operator-() const {
        return Complex(-real, -imag);
    }

    Complex operator+() const {
        return *this;
    }

    // Stream operators
    friend ostream& operator<<(ostream& out, const Complex& c) {
        if (c.imag >= 0)
            out << c.real << " + " << c.imag << "i";
        else
            out << c.real << " - " << -c.imag << "i";
        return out;
    }

    friend istream& operator>>(istream& in, Complex& c) {
        cout << "Enter real and imaginary parts: ";
        in >> c.real >> c.imag;
        return in;
    }

    // Conversion operators
    operator double() const {
        return sqrt(real * real + imag * imag);  // Magnitude
    }

    double magnitude() const {
        return static_cast<double>(*this);
    }
};

int main() {
    Complex c1(3, 4);
    Complex c2(1, 2);

    cout << "c1 = " << c1 << endl;
    cout << "c2 = " << c2 << endl;

    cout << "c1 + c2 = " << (c1 + c2) << endl;
    cout << "c1 - c2 = " << (c1 - c2) << endl;
    cout << "c1 * c2 = " << (c1 * c2) << endl;
    cout << "c1 / c2 = " << (c1 / c2) << endl;

    cout << "|c1| = " << c1.magnitude() << endl;

    c1 += c2;
    cout << "After c1 += c2: " << c1 << endl;

    return 0;
}
```

---

## 8. Exercises

### Exercise 1: Date Class with Operators
```cpp
class Date {
private:
    int day, month, year;

public:
    Date(int d, int m, int y);

    Date operator+(int days) const;  // Add days
    int operator-(const Date& other) const;  // Days between
    Date& operator++();  // Next day (prefix)
    Date operator++(int);  // Next day (postfix)
    bool operator<(const Date& other) const;
    friend ostream& operator<<(ostream& out, const Date& d);
    friend istream& operator>>(istream& in, Date& d);
};
```

### Exercise 2: BitSet Class
```cpp
class BitSet {
private:
    unsigned int bits;

public:
    BitSet(unsigned int b = 0);

    BitSet operator&(const BitSet& other) const;  // AND
    BitSet operator|(const BitSet& other) const;  // OR
    BitSet operator^(const BitSet& other) const;  // XOR
    BitSet operator~() const;  // NOT
    bool operator[](int index) const;  // Get bit
};
```

### Exercise 3: Polynomial Class
```cpp
class Polynomial {
private:
    double* coefficients;
    int degree;

public:
    Polynomial(int deg);

    Polynomial operator+(const Polynomial& p) const;
    Polynomial operator*(const Polynomial& p) const;
    double operator()(double x) const;  // Evaluate
    double& operator[](int index);  // Set coefficient
};
```

---

## 9. Practice Projects

### Project: Complete Vector Class

```cpp
#include <iostream>
#include <cmath>
#include <initializer_list>
using namespace std;

class Vector {
private:
    double* data;
    int size;

public:
    Vector(int s = 0) : size(s) {
        data = new double[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0.0;
        }
    }

    Vector(initializer_list<double> list) : size(list.size()) {
        data = new double[size];
        int i = 0;
        for (double val : list) {
            data[i++] = val;
        }
    }

    Vector(const Vector& v) : size(v.size) {
        data = new double[size];
        for (int i = 0; i < size; i++) {
            data[i] = v.data[i];
        }
    }

    ~Vector() {
        delete[] data;
    }

    Vector& operator=(const Vector& v) {
        if (this != &v) {
            delete[] data;
            size = v.size;
            data = new double[size];
            for (int i = 0; i < size; i++) {
                data[i] = v.data[i];
            }
        }
        return *this;
    }

    double& operator[](int index) {
        return data[index];
    }

    const double& operator[](int index) const {
        return data[index];
    }

    Vector operator+(const Vector& v) const {
        if (size != v.size) {
            cout << "Size mismatch!" << endl;
            return *this;
        }
        Vector result(size);
        for (int i = 0; i < size; i++) {
            result.data[i] = data[i] + v.data[i];
        }
        return result;
    }

    Vector operator-(const Vector& v) const {
        if (size != v.size) {
            cout << "Size mismatch!" << endl;
            return *this;
        }
        Vector result(size);
        for (int i = 0; i < size; i++) {
            result.data[i] = data[i] - v.data[i];
        }
        return result;
    }

    Vector operator*(double scalar) const {
        Vector result(size);
        for (int i = 0; i < size; i++) {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }

    double operator*(const Vector& v) const {  // Dot product
        if (size != v.size) {
            cout << "Size mismatch!" << endl;
            return 0.0;
        }
        double result = 0.0;
        for (int i = 0; i < size; i++) {
            result += data[i] * v.data[i];
        }
        return result;
    }

    friend Vector operator*(double scalar, const Vector& v) {
        return v * scalar;
    }

    double magnitude() const {
        return sqrt(*this * *this);
    }

    Vector normalize() const {
        double mag = magnitude();
        if (mag == 0) return *this;
        return *this * (1.0 / mag);
    }

    friend ostream& operator<<(ostream& out, const Vector& v) {
        out << "[";
        for (int i = 0; i < v.size; i++) {
            out << v.data[i];
            if (i < v.size - 1) out << ", ";
        }
        out << "]";
        return out;
    }

    int getSize() const { return size; }
};

int main() {
    Vector v1 = {1, 2, 3};
    Vector v2 = {4, 5, 6};

    cout << "v1 = " << v1 << endl;
    cout << "v2 = " << v2 << endl;

    cout << "v1 + v2 = " << (v1 + v2) << endl;
    cout << "v1 - v2 = " << (v1 - v2) << endl;
    cout << "v1 * 2 = " << (v1 * 2) << endl;
    cout << "2 * v1 = " << (2 * v1) << endl;
    cout << "v1 . v2 = " << (v1 * v2) << endl;
    cout << "|v1| = " << v1.magnitude() << endl;
    cout << "normalized v1 = " << v1.normalize() << endl;

    return 0;
}
```

---

## Key Takeaways

1. **Stream operators** must be friend functions
2. **Subscript operator** enables array-like access
3. **Function call operator** creates functors
4. **Arrow operator** used for smart pointers
5. **Type conversion** allows implicit/explicit conversion
6. **Assignment operators** must handle self-assignment
7. **Return *this** from compound assignments
8. **const correctness** is crucial

---

## Summary Checklist

Before moving to Lesson 40, ensure you can:
- [ ] Overload << and >> operators
- [ ] Implement subscript operator []
- [ ] Create functors with ()
- [ ] Understand arrow operator
- [ ] Implement type conversions
- [ ] Overload assignment operators
- [ ] Handle self-assignment
- [ ] Use const correctly

---

## Next Lesson Preview

In **Lesson 40: Copy Constructor and Assignment Operator**, we'll learn:
- Deep vs shallow copy
- Copy constructor in detail
- Copy assignment operator
- Rule of Three/Five
- Move semantics introduction

Fantastic progress on operator overloading!
