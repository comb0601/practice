# Lesson 38: Operator Overloading Part 1

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Operator Overloading
2. Overloading Arithmetic Operators
3. Overloading Comparison Operators
4. Overloading Increment/Decrement
5. Member vs Non-Member Overloading
6. Returning Values from Operators
7. Common Patterns
8. Exercises
9. Practice Projects

---

## 1. Introduction to Operator Overloading

**Operator overloading** allows you to redefine how operators work with user-defined types (classes).

### Why Operator Overloading?

Makes code more intuitive and natural:

```cpp
// Without operator overloading
Complex c3 = c1.add(c2);

// With operator overloading
Complex c3 = c1 + c2;  // Natural and intuitive!
```

### Rules for Operator Overloading:

1. Cannot create new operators
2. Cannot change operator precedence
3. Cannot change number of operands
4. At least one operand must be user-defined type
5. Some operators cannot be overloaded (::, ., .*, ?:, sizeof)

### Operators That Can Be Overloaded:

```
Arithmetic: +  -  *  /  %
Comparison: ==  !=  <  >  <=  >=
Logical:    &&  ||  !
Bitwise:    &  |  ^  ~  <<  >>
Assignment: =  +=  -=  *=  /=  %=  etc.
Increment:  ++  --
Access:     []  ->  ()
Other:      ,  new  delete
```

---

## 2. Overloading Arithmetic Operators

### Example 1: Complex Number Class

```cpp
#include <iostream>
using namespace std;

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // Overload + operator (member function)
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // Overload - operator
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    // Overload * operator
    Complex operator*(const Complex& other) const {
        // (a + bi)(c + di) = (ac - bd) + (ad + bc)i
        double r = real * other.real - imag * other.imag;
        double i = real * other.imag + imag * other.real;
        return Complex(r, i);
    }

    void display() const {
        if (imag >= 0)
            cout << real << " + " << imag << "i";
        else
            cout << real << " - " << -imag << "i";
    }
};

int main() {
    Complex c1(3, 4);
    Complex c2(1, 2);

    cout << "c1 = "; c1.display(); cout << endl;
    cout << "c2 = "; c2.display(); cout << endl;

    Complex c3 = c1 + c2;
    cout << "c1 + c2 = "; c3.display(); cout << endl;

    Complex c4 = c1 - c2;
    cout << "c1 - c2 = "; c4.display(); cout << endl;

    Complex c5 = c1 * c2;
    cout << "c1 * c2 = "; c5.display(); cout << endl;

    return 0;
}
```

**Output**:
```
c1 = 3 + 4i
c2 = 1 + 2i
c1 + c2 = 4 + 6i
c1 - c2 = 2 + 2i
c1 * c2 = -5 + 10i
```

### Example 2: Vector Class

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Vector3D {
private:
    double x, y, z;

public:
    Vector3D(double x = 0, double y = 0, double z = 0)
        : x(x), y(y), z(z) {}

    // Addition
    Vector3D operator+(const Vector3D& v) const {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }

    // Subtraction
    Vector3D operator-(const Vector3D& v) const {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    // Scalar multiplication
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    // Dot product (different result type)
    double dot(const Vector3D& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Magnitude
    double magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    void display() const {
        cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    Vector3D v1(1, 2, 3);
    Vector3D v2(4, 5, 6);

    cout << "v1 = "; v1.display(); cout << endl;
    cout << "v2 = "; v2.display(); cout << endl;

    Vector3D v3 = v1 + v2;
    cout << "v1 + v2 = "; v3.display(); cout << endl;

    Vector3D v4 = v1 - v2;
    cout << "v1 - v2 = "; v4.display(); cout << endl;

    Vector3D v5 = v1 * 2;
    cout << "v1 * 2 = "; v5.display(); cout << endl;

    cout << "v1 . v2 = " << v1.dot(v2) << endl;
    cout << "|v1| = " << v1.magnitude() << endl;

    return 0;
}
```

---

## 3. Overloading Comparison Operators

### Example 1: Comparison Operators

```cpp
#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    string name;
    double gpa;

public:
    Student(const string& n, double g) : name(n), gpa(g) {}

    // Equality operator
    bool operator==(const Student& other) const {
        return gpa == other.gpa;
    }

    // Inequality operator
    bool operator!=(const Student& other) const {
        return gpa != other.gpa;
    }

    // Less than operator
    bool operator<(const Student& other) const {
        return gpa < other.gpa;
    }

    // Greater than operator
    bool operator>(const Student& other) const {
        return gpa > other.gpa;
    }

    // Less than or equal
    bool operator<=(const Student& other) const {
        return gpa <= other.gpa;
    }

    // Greater than or equal
    bool operator>=(const Student& other) const {
        return gpa >= other.gpa;
    }

    void display() const {
        cout << name << " (GPA: " << gpa << ")";
    }
};

int main() {
    Student alice("Alice", 3.8);
    Student bob("Bob", 3.5);
    Student charlie("Charlie", 3.8);

    alice.display();
    cout << " == ";
    bob.display();
    cout << ": " << (alice == bob ? "true" : "false") << endl;

    alice.display();
    cout << " > ";
    bob.display();
    cout << ": " << (alice > bob ? "true" : "false") << endl;

    alice.display();
    cout << " == ";
    charlie.display();
    cout << ": " << (alice == charlie ? "true" : "false") << endl;

    return 0;
}
```

### Example 2: Money Class

```cpp
#include <iostream>
using namespace std;

class Money {
private:
    double amount;

public:
    Money(double amt = 0.0) : amount(amt) {}

    // Arithmetic operators
    Money operator+(const Money& m) const {
        return Money(amount + m.amount);
    }

    Money operator-(const Money& m) const {
        return Money(amount - m.amount);
    }

    Money operator*(double factor) const {
        return Money(amount * factor);
    }

    // Comparison operators
    bool operator==(const Money& m) const {
        return fabs(amount - m.amount) < 0.01;  // Account for floating point
    }

    bool operator!=(const Money& m) const {
        return !(*this == m);
    }

    bool operator<(const Money& m) const {
        return amount < m.amount;
    }

    bool operator>(const Money& m) const {
        return amount > m.amount;
    }

    bool operator<=(const Money& m) const {
        return amount <= m.amount;
    }

    bool operator>=(const Money& m) const {
        return amount >= m.amount;
    }

    void display() const {
        cout << "$" << amount;
    }
};

int main() {
    Money price1(19.99);
    Money price2(29.99);

    cout << "Price 1: "; price1.display(); cout << endl;
    cout << "Price 2: "; price2.display(); cout << endl;

    Money total = price1 + price2;
    cout << "Total: "; total.display(); cout << endl;

    Money discount = price2 * 0.2;
    cout << "20% of Price 2: "; discount.display(); cout << endl;

    if (price1 < price2) {
        cout << "Price 1 is cheaper!" << endl;
    }

    return 0;
}
```

---

## 4. Overloading Increment/Decrement

Prefix (++x) and postfix (x++) are overloaded differently.

### Syntax:
```cpp
// Prefix: ++x
Type& operator++() {
    // Increment first
    return *this;
}

// Postfix: x++
Type operator++(int) {  // int parameter distinguishes postfix
    Type temp = *this;
    // Increment
    return temp;  // Return old value
}
```

### Example: Counter Class

```cpp
#include <iostream>
using namespace std;

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
    Counter operator++(int) {
        Counter temp = *this;
        count++;
        return temp;
    }

    // Prefix decrement: --counter
    Counter& operator--() {
        --count;
        return *this;
    }

    // Postfix decrement: counter--
    Counter operator--(int) {
        Counter temp = *this;
        count--;
        return temp;
    }

    int getValue() const {
        return count;
    }

    void display() const {
        cout << "Count: " << count << endl;
    }
};

int main() {
    Counter c(10);

    cout << "Initial: ";
    c.display();

    ++c;  // Prefix
    cout << "After ++c: ";
    c.display();

    c++;  // Postfix
    cout << "After c++: ";
    c.display();

    --c;
    cout << "After --c: ";
    c.display();

    // Demonstrate difference
    Counter c2(5);
    cout << "\nc2 initial value: " << c2.getValue() << endl;

    Counter c3 = ++c2;
    cout << "After c3 = ++c2:" << endl;
    cout << "  c2 = " << c2.getValue() << endl;
    cout << "  c3 = " << c3.getValue() << endl;

    Counter c4 = c2++;
    cout << "After c4 = c2++:" << endl;
    cout << "  c2 = " << c2.getValue() << endl;
    cout << "  c4 = " << c4.getValue() << endl;

    return 0;
}
```

**Output**:
```
Initial: Count: 10
After ++c: Count: 11
After c++: Count: 12
After --c: Count: 11

c2 initial value: 5
After c3 = ++c2:
  c2 = 6
  c3 = 6
After c4 = c2++:
  c2 = 7
  c4 = 6
```

---

## 5. Member vs Non-Member Overloading

### Member Function:
```cpp
class MyClass {
    Type operator+(const MyClass& other) const;
    // Called as: obj1 + obj2
    // obj1 is *this, obj2 is other
};
```

### Non-Member (Friend) Function:
```cpp
class MyClass {
    friend Type operator+(const MyClass& lhs, const MyClass& rhs);
};

Type operator+(const MyClass& lhs, const MyClass& rhs) {
    // Both operands as parameters
}
```

### When to Use Each?

**Use Member Function:**
- When left operand is always your class
- For =, [], (), -> operators (must be member)

**Use Non-Member Function:**
- When left operand might not be your class
- For symmetric operators (allows type conversion)
- For << and >> operators

### Example: Mixed Types

```cpp
#include <iostream>
using namespace std;

class Distance {
private:
    double meters;

public:
    Distance(double m = 0.0) : meters(m) {}

    // Member: Distance + Distance
    Distance operator+(const Distance& d) const {
        return Distance(meters + d.meters);
    }

    // Friend: Distance + double and double + Distance
    friend Distance operator+(const Distance& d, double m);
    friend Distance operator+(double m, const Distance& d);

    void display() const {
        cout << meters << " meters";
    }
};

// Non-member implementations
Distance operator+(const Distance& d, double m) {
    return Distance(d.meters + m);
}

Distance operator+(double m, const Distance& d) {
    return Distance(m + d.meters);
}

int main() {
    Distance d1(10.5);
    Distance d2(5.5);

    Distance d3 = d1 + d2;        // Distance + Distance
    Distance d4 = d1 + 3.0;       // Distance + double
    Distance d5 = 2.5 + d1;       // double + Distance

    cout << "d1 = "; d1.display(); cout << endl;
    cout << "d2 = "; d2.display(); cout << endl;
    cout << "d1 + d2 = "; d3.display(); cout << endl;
    cout << "d1 + 3.0 = "; d4.display(); cout << endl;
    cout << "2.5 + d1 = "; d5.display(); cout << endl;

    return 0;
}
```

---

## 6. Returning Values from Operators

### Return by Value vs Reference:

```cpp
class MyClass {
public:
    // Return by value - creates new object
    MyClass operator+(const MyClass& other) const {
        return MyClass(/* ... */);
    }

    // Return by reference - for chaining
    MyClass& operator+=(const MyClass& other) {
        // Modify this object
        return *this;
    }

    // Return by const reference - for getters
    const MyClass& operator[](int index) const {
        return data[index];
    }
};
```

### Example: String Builder

```cpp
#include <iostream>
#include <string>
using namespace std;

class StringBuilder {
private:
    string text;

public:
    StringBuilder(const string& s = "") : text(s) {}

    // operator+= returns reference for chaining
    StringBuilder& operator+=(const string& s) {
        text += s;
        return *this;
    }

    StringBuilder& operator+=(char c) {
        text += c;
        return *this;
    }

    // operator+ returns new object
    StringBuilder operator+(const string& s) const {
        StringBuilder result = *this;
        result.text += s;
        return result;
    }

    string toString() const {
        return text;
    }
};

int main() {
    StringBuilder sb("Hello");

    // Method chaining with +=
    sb += " " += "World" += "!";
    cout << sb.toString() << endl;

    // operator+ creates new object
    StringBuilder sb2 = sb + " How are you?";
    cout << "sb: " << sb.toString() << endl;
    cout << "sb2: " << sb2.toString() << endl;

    return 0;
}
```

---

## 7. Common Patterns

### Pattern 1: Implementing Compound Assignment

```cpp
class Number {
private:
    int value;

public:
    Number(int v = 0) : value(v) {}

    // Implement += first
    Number& operator+=(const Number& other) {
        value += other.value;
        return *this;
    }

    // Implement + using +=
    Number operator+(const Number& other) const {
        Number result = *this;
        result += other;
        return result;
    }

    // Similarly for other operators
    Number& operator-=(const Number& other) {
        value -= other.value;
        return *this;
    }

    Number operator-(const Number& other) const {
        Number result = *this;
        result -= other;
        return result;
    }
};
```

### Pattern 2: Implementing All Comparisons from <

```cpp
class Comparable {
private:
    int value;

public:
    Comparable(int v) : value(v) {}

    // Implement < only
    bool operator<(const Comparable& other) const {
        return value < other.value;
    }

    // Implement others using <
    bool operator>(const Comparable& other) const {
        return other < *this;
    }

    bool operator<=(const Comparable& other) const {
        return !(other < *this);
    }

    bool operator>=(const Comparable& other) const {
        return !(*this < other);
    }

    bool operator==(const Comparable& other) const {
        return !(*this < other) && !(other < *this);
    }

    bool operator!=(const Comparable& other) const {
        return !(*this == other);
    }
};
```

---

## 8. Exercises

### Exercise 1: Time Class
Create a Time class with operator overloading.

```cpp
class Time {
private:
    int hours;
    int minutes;
    int seconds;

public:
    Time(int h = 0, int m = 0, int s = 0);

    Time operator+(const Time& t) const;
    Time operator-(const Time& t) const;
    bool operator==(const Time& t) const;
    bool operator<(const Time& t) const;
    Time& operator++();      // Add one second
    Time operator++(int);    // Add one second
};
```

### Exercise 2: Fraction Class
Implement full arithmetic for fractions.

```cpp
class Fraction {
private:
    int numerator;
    int denominator;

public:
    Fraction(int n, int d);

    Fraction operator+(const Fraction& f) const;
    Fraction operator-(const Fraction& f) const;
    Fraction operator*(const Fraction& f) const;
    Fraction operator/(const Fraction& f) const;
    bool operator==(const Fraction& f) const;
};
```

### Exercise 3: Matrix Class
Create 2x2 matrix with operations.

```cpp
class Matrix2x2 {
private:
    double data[2][2];

public:
    Matrix2x2();

    Matrix2x2 operator+(const Matrix2x2& m) const;
    Matrix2x2 operator*(const Matrix2x2& m) const;
    Matrix2x2 operator*(double scalar) const;
};
```

---

## 9. Practice Projects

### Project: Complete Rational Number Class

```cpp
#include <iostream>
using namespace std;

class Rational {
private:
    int numerator;
    int denominator;

    int gcd(int a, int b) const {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    void simplify() {
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
        int g = gcd(abs(numerator), abs(denominator));
        numerator /= g;
        denominator /= g;
    }

public:
    Rational(int n = 0, int d = 1) : numerator(n), denominator(d) {
        if (denominator == 0) denominator = 1;
        simplify();
    }

    // Arithmetic operators
    Rational operator+(const Rational& r) const {
        return Rational(numerator * r.denominator + r.numerator * denominator,
                        denominator * r.denominator);
    }

    Rational operator-(const Rational& r) const {
        return Rational(numerator * r.denominator - r.numerator * denominator,
                        denominator * r.denominator);
    }

    Rational operator*(const Rational& r) const {
        return Rational(numerator * r.numerator,
                        denominator * r.denominator);
    }

    Rational operator/(const Rational& r) const {
        return Rational(numerator * r.denominator,
                        denominator * r.numerator);
    }

    // Comparison operators
    bool operator==(const Rational& r) const {
        return numerator == r.numerator && denominator == r.denominator;
    }

    bool operator!=(const Rational& r) const {
        return !(*this == r);
    }

    bool operator<(const Rational& r) const {
        return numerator * r.denominator < r.numerator * denominator;
    }

    bool operator>(const Rational& r) const {
        return r < *this;
    }

    bool operator<=(const Rational& r) const {
        return !(r < *this);
    }

    bool operator>=(const Rational& r) const {
        return !(*this < r);
    }

    // Compound assignment
    Rational& operator+=(const Rational& r) {
        *this = *this + r;
        return *this;
    }

    Rational& operator-=(const Rational& r) {
        *this = *this - r;
        return *this;
    }

    void display() const {
        if (denominator == 1)
            cout << numerator;
        else
            cout << numerator << "/" << denominator;
    }
};

int main() {
    Rational r1(1, 2);
    Rational r2(1, 3);
    Rational r3(2, 4);  // Will simplify to 1/2

    cout << "r1 = "; r1.display(); cout << endl;
    cout << "r2 = "; r2.display(); cout << endl;
    cout << "r3 = "; r3.display(); cout << endl;

    cout << "\nArithmetic:" << endl;
    Rational sum = r1 + r2;
    cout << "r1 + r2 = "; sum.display(); cout << endl;

    Rational diff = r1 - r2;
    cout << "r1 - r2 = "; diff.display(); cout << endl;

    Rational prod = r1 * r2;
    cout << "r1 * r2 = "; prod.display(); cout << endl;

    Rational quot = r1 / r2;
    cout << "r1 / r2 = "; quot.display(); cout << endl;

    cout << "\nComparisons:" << endl;
    cout << "r1 == r3: " << (r1 == r3 ? "true" : "false") << endl;
    cout << "r1 < r2: " << (r1 < r2 ? "true" : "false") << endl;
    cout << "r1 > r2: " << (r1 > r2 ? "true" : "false") << endl;

    return 0;
}
```

---

## Key Takeaways

1. **Operator overloading** makes code intuitive
2. **Cannot create** new operators
3. **At least one operand** must be user-defined
4. **Member functions** for unary and binary where left is your type
5. **Friend functions** for symmetric operations
6. **Return by value** for arithmetic operators
7. **Return by reference** for compound assignment
8. **Prefix vs postfix** uses int parameter

---

## Summary Checklist

Before moving to Lesson 39, ensure you can:
- [ ] Overload arithmetic operators
- [ ] Overload comparison operators
- [ ] Overload increment/decrement
- [ ] Choose member vs non-member
- [ ] Return appropriate types
- [ ] Implement compound operators
- [ ] Apply common patterns

---

## Next Lesson Preview

In **Lesson 39: Operator Overloading Part 2**, we'll learn:
- Stream operators (<< and >>)
- Subscript operator []
- Function call operator ()
- Assignment operators
- Type conversion operators

Excellent! You're mastering operator overloading!
