# Lesson 37: Friend Functions and Classes

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. What are Friend Functions?
2. Friend Function Syntax
3. Friend Classes
4. When to Use Friends
5. Friend vs Member Functions
6. Friend and Operator Overloading
7. Best Practices
8. Exercises
9. Practice Projects

---

## 1. What are Friend Functions?

**Friend functions** are non-member functions that have access to private and protected members of a class.

### Key Concepts:

- Not member functions
- Can access private/protected members
- Declared with `friend` keyword
- Break encapsulation (use sparingly!)
- Useful for operator overloading and helper functions

### Syntax:
```cpp
class ClassName {
private:
    int privateMember;

    friend void friendFunction(ClassName& obj);  // Friend declaration
};

void friendFunction(ClassName& obj) {
    obj.privateMember = 10;  // Can access private members!
}
```

---

## 2. Friend Function Syntax

### Example 1: Basic Friend Function

```cpp
#include <iostream>
using namespace std;

class Box {
private:
    double width;
    double height;
    double depth;

public:
    Box(double w, double h, double d)
        : width(w), height(h), depth(d) {}

    // Friend function declaration
    friend void printDimensions(const Box& box);
    friend double calculateVolume(const Box& box);
};

// Friend function definitions (not member functions)
void printDimensions(const Box& box) {
    // Can access private members directly
    cout << "Dimensions: " << box.width << " x "
         << box.height << " x " << box.depth << endl;
}

double calculateVolume(const Box& box) {
    return box.width * box.height * box.depth;
}

int main() {
    Box myBox(10.0, 5.0, 3.0);

    printDimensions(myBox);
    cout << "Volume: " << calculateVolume(myBox) << endl;

    return 0;
}
```

**Output**:
```
Dimensions: 10 x 5 x 3
Volume: 150
```

### Example 2: Friend Function with Multiple Classes

```cpp
#include <iostream>
#include <string>
using namespace std;

class Teacher;  // Forward declaration

class Student {
private:
    string name;
    double gpa;

public:
    Student(const string& n, double g) : name(n), gpa(g) {}

    // Friend function that accesses both Student and Teacher
    friend void displayPair(const Student& student, const Teacher& teacher);
};

class Teacher {
private:
    string name;
    string subject;

public:
    Teacher(const string& n, const string& s) : name(n), subject(s) {}

    friend void displayPair(const Student& student, const Teacher& teacher);
};

// Friend function can access private members of both classes
void displayPair(const Student& student, const Teacher& teacher) {
    cout << "Student: " << student.name
         << " (GPA: " << student.gpa << ")" << endl;
    cout << "Teacher: " << teacher.name
         << " (Subject: " << teacher.subject << ")" << endl;
}

int main() {
    Student student("Alice", 3.8);
    Teacher teacher("Dr. Smith", "Mathematics");

    displayPair(student, teacher);

    return 0;
}
```

---

## 3. Friend Classes

A **friend class** can access all private and protected members of another class.

### Syntax:
```cpp
class ClassA {
private:
    int privateData;

    friend class ClassB;  // ClassB is a friend
};

class ClassB {
public:
    void accessClassA(ClassA& obj) {
        obj.privateData = 10;  // OK: ClassB is friend
    }
};
```

### Example 1: Friend Class

```cpp
#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    string accountNumber;
    double balance;
    string pin;

    // BankManager is a friend class
    friend class BankManager;

public:
    BankAccount(const string& acc, double bal, const string& p)
        : accountNumber(acc), balance(bal), pin(p) {}
};

class BankManager {
public:
    void displayAccountDetails(const BankAccount& account) {
        // Can access all private members
        cout << "Account Number: " << account.accountNumber << endl;
        cout << "Balance: $" << account.balance << endl;
        // PIN would normally not be displayed, just for demo
        cout << "PIN: " << account.pin << endl;
    }

    void adjustBalance(BankAccount& account, double amount) {
        account.balance += amount;
        cout << "Balance adjusted by $" << amount << endl;
        cout << "New balance: $" << account.balance << endl;
    }

    bool verifyAccount(const BankAccount& account, const string& pin) {
        return account.pin == pin;
    }
};

int main() {
    BankAccount account("ACC12345", 5000.0, "1234");
    BankManager manager;

    manager.displayAccountDetails(account);

    if (manager.verifyAccount(account, "1234")) {
        cout << "\nAccount verified!" << endl;
        manager.adjustBalance(account, 1000.0);
    }

    return 0;
}
```

### Example 2: One-Way Friendship

```cpp
#include <iostream>
using namespace std;

class Engine {
private:
    int horsepower;
    double fuelLevel;

    friend class Car;  // Car can access Engine's private members

public:
    Engine(int hp) : horsepower(hp), fuelLevel(100.0) {}
};

class Car {
private:
    string model;
    Engine engine;

public:
    Car(const string& m, int engineHP)
        : model(m), engine(engineHP) {}

    void displayInfo() {
        cout << "Car Model: " << model << endl;
        // Can access Engine's private members
        cout << "Engine Horsepower: " << engine.horsepower << endl;
        cout << "Fuel Level: " << engine.fuelLevel << "%" << endl;
    }

    void drive(double distance) {
        double fuelUsed = distance * 0.1;
        engine.fuelLevel -= fuelUsed;  // Can modify private member
        cout << "Drove " << distance << " km. Fuel: "
             << engine.fuelLevel << "%" << endl;
    }
};

int main() {
    Car myCar("Tesla Model 3", 283);

    myCar.displayInfo();
    myCar.drive(50);
    myCar.displayInfo();

    return 0;
}
```

---

## 4. When to Use Friends

### Use Case 1: Operator Overloading

```cpp
#include <iostream>
using namespace std;

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // Friend function for operator overloading
    friend Complex operator+(const Complex& c1, const Complex& c2);
    friend ostream& operator<<(ostream& out, const Complex& c);
};

Complex operator+(const Complex& c1, const Complex& c2) {
    return Complex(c1.real + c2.real, c1.imag + c2.imag);
}

ostream& operator<<(ostream& out, const Complex& c) {
    out << c.real;
    if (c.imag >= 0)
        out << " + " << c.imag << "i";
    else
        out << " - " << -c.imag << "i";
    return out;
}

int main() {
    Complex c1(3, 4);
    Complex c2(1, 2);

    cout << "c1 = " << c1 << endl;
    cout << "c2 = " << c2 << endl;
    cout << "c1 + c2 = " << (c1 + c2) << endl;

    return 0;
}
```

### Use Case 2: Accessing Multiple Classes

```cpp
#include <iostream>
#include <string>
using namespace std;

class Doctor;

class Patient {
private:
    string name;
    string illness;
    bool isAdmitted;

public:
    Patient(const string& n, const string& i)
        : name(n), illness(i), isAdmitted(false) {}

    friend void performDiagnosis(Patient& patient, Doctor& doctor);
};

class Doctor {
private:
    string name;
    string specialization;
    int patientsToday;

public:
    Doctor(const string& n, const string& spec)
        : name(n), specialization(spec), patientsToday(0) {}

    friend void performDiagnosis(Patient& patient, Doctor& doctor);
};

void performDiagnosis(Patient& patient, Doctor& doctor) {
    cout << "\n=== Diagnosis Session ===" << endl;
    cout << "Doctor: " << doctor.name
         << " (" << doctor.specialization << ")" << endl;
    cout << "Patient: " << patient.name << endl;
    cout << "Illness: " << patient.illness << endl;

    doctor.patientsToday++;
    patient.isAdmitted = true;

    cout << "Patient admitted. Doctor's patients today: "
         << doctor.patientsToday << endl;
}

int main() {
    Patient patient("John Doe", "Fever");
    Doctor doctor("Dr. Smith", "General Medicine");

    performDiagnosis(patient, doctor);

    return 0;
}
```

---

## 5. Friend vs Member Functions

### Comparison:

```cpp
#include <iostream>
using namespace std;

class Rectangle {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    // Member function
    double area() const {
        return width * height;
    }

    // Friend function declaration
    friend double perimeter(const Rectangle& rect);
};

// Friend function definition
double perimeter(const Rectangle& rect) {
    return 2 * (rect.width + rect.height);
}

int main() {
    Rectangle rect(10.0, 5.0);

    // Member function called with dot operator
    cout << "Area: " << rect.area() << endl;

    // Friend function called as regular function
    cout << "Perimeter: " << perimeter(rect) << endl;

    return 0;
}
```

### Key Differences:

| Aspect | Member Function | Friend Function |
|--------|----------------|-----------------|
| Declaration | Inside class | Inside class with `friend` |
| Definition | Inside or outside class | Outside class |
| Calling | `object.function()` | `function(object)` |
| `this` pointer | Yes | No |
| Inheritance | Inherited | Not inherited |

---

## 6. Friend and Operator Overloading

### Complete Example: Vector Class

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Vector2D {
private:
    double x, y;

public:
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    // Friend functions for operator overloading
    friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
    friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
    friend Vector2D operator*(const Vector2D& v, double scalar);
    friend Vector2D operator*(double scalar, const Vector2D& v);
    friend double operator*(const Vector2D& v1, const Vector2D& v2);  // Dot product
    friend ostream& operator<<(ostream& out, const Vector2D& v);
    friend istream& operator>>(istream& in, Vector2D& v);
    friend bool operator==(const Vector2D& v1, const Vector2D& v2);
};

// Addition
Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

// Subtraction
Vector2D operator-(const Vector2D& v1, const Vector2D& v2) {
    return Vector2D(v1.x - v2.x, v1.y - v2.y);
}

// Scalar multiplication
Vector2D operator*(const Vector2D& v, double scalar) {
    return Vector2D(v.x * scalar, v.y * scalar);
}

Vector2D operator*(double scalar, const Vector2D& v) {
    return Vector2D(v.x * scalar, v.y * scalar);
}

// Dot product
double operator*(const Vector2D& v1, const Vector2D& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Output stream
ostream& operator<<(ostream& out, const Vector2D& v) {
    out << "(" << v.x << ", " << v.y << ")";
    return out;
}

// Input stream
istream& operator>>(istream& in, Vector2D& v) {
    cout << "Enter x y: ";
    in >> v.x >> v.y;
    return in;
}

// Equality
bool operator==(const Vector2D& v1, const Vector2D& v2) {
    const double EPSILON = 0.0001;
    return fabs(v1.x - v2.x) < EPSILON && fabs(v1.y - v2.y) < EPSILON;
}

int main() {
    Vector2D v1(3, 4);
    Vector2D v2(1, 2);

    cout << "v1 = " << v1 << endl;
    cout << "v2 = " << v2 << endl;

    Vector2D v3 = v1 + v2;
    cout << "v1 + v2 = " << v3 << endl;

    Vector2D v4 = v1 - v2;
    cout << "v1 - v2 = " << v4 << endl;

    Vector2D v5 = v1 * 2;
    cout << "v1 * 2 = " << v5 << endl;

    Vector2D v6 = 3 * v2;
    cout << "3 * v2 = " << v6 << endl;

    double dot = v1 * v2;
    cout << "v1 . v2 = " << dot << endl;

    cout << "v1 == v2: " << (v1 == v2 ? "true" : "false") << endl;

    return 0;
}
```

---

## 7. Best Practices

### Practice 1: Use Friends Sparingly

```cpp
// GOOD: Friend only when necessary
class GoodExample {
private:
    int data;

public:
    int getData() const { return data; }  // Prefer public interface

    // Only make friend when absolutely needed
    friend ostream& operator<<(ostream& out, const GoodExample& obj);
};
```

### Practice 2: Document Why Friend is Needed

```cpp
class Account {
private:
    double balance;

    // Friend for symmetric operator overloading
    // Reason: Need access to both operands' private data
    friend bool transfer(Account& from, Account& to, double amount);

public:
    Account(double bal) : balance(bal) {}
};

bool transfer(Account& from, Account& to, double amount) {
    if (from.balance >= amount) {
        from.balance -= amount;
        to.balance += amount;
        return true;
    }
    return false;
}
```

### Practice 3: Limit Scope of Friendship

```cpp
// BAD: Entire class as friend when only one function needed
class Bad {
    friend class Helper;  // Gives Helper full access
};

// GOOD: Only specific function as friend
class Good {
    friend void helperFunction(Good& obj);  // Limited access
};
```

---

## 8. Exercises

### Exercise 1: Distance Calculator
Create classes for Point and Line with friend functions.

```cpp
class Point {
private:
    double x, y;

public:
    Point(double x, double y);
    friend double distance(const Point& p1, const Point& p2);
};
```

### Exercise 2: Matrix Operations
Create a Matrix class with friend operators.

```cpp
class Matrix {
private:
    double data[2][2];

public:
    Matrix();
    friend Matrix operator+(const Matrix& m1, const Matrix& m2);
    friend Matrix operator*(const Matrix& m, double scalar);
    friend ostream& operator<<(ostream& out, const Matrix& m);
};
```

### Exercise 3: Temperature Comparison
Create Temperature class with friend comparison functions.

```cpp
class Temperature {
private:
    double celsius;

public:
    Temperature(double c);
    friend bool operator>(const Temperature& t1, const Temperature& t2);
    friend bool operator<(const Temperature& t1, const Temperature& t2);
    friend Temperature average(const Temperature& t1, const Temperature& t2);
};
```

---

## 9. Practice Projects

### Project: Complete Fraction Class

```cpp
#include <iostream>
using namespace std;

class Fraction {
private:
    int numerator;
    int denominator;

    // Private helper function
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

        int divisor = gcd(abs(numerator), abs(denominator));
        numerator /= divisor;
        denominator /= divisor;
    }

public:
    Fraction(int num = 0, int den = 1)
        : numerator(num), denominator(den) {
        if (denominator == 0) {
            denominator = 1;
        }
        simplify();
    }

    // Friend operators
    friend Fraction operator+(const Fraction& f1, const Fraction& f2);
    friend Fraction operator-(const Fraction& f1, const Fraction& f2);
    friend Fraction operator*(const Fraction& f1, const Fraction& f2);
    friend Fraction operator/(const Fraction& f1, const Fraction& f2);
    friend bool operator==(const Fraction& f1, const Fraction& f2);
    friend bool operator<(const Fraction& f1, const Fraction& f2);
    friend ostream& operator<<(ostream& out, const Fraction& f);
    friend istream& operator>>(istream& in, Fraction& f);
};

Fraction operator+(const Fraction& f1, const Fraction& f2) {
    int num = f1.numerator * f2.denominator + f2.numerator * f1.denominator;
    int den = f1.denominator * f2.denominator;
    return Fraction(num, den);
}

Fraction operator-(const Fraction& f1, const Fraction& f2) {
    int num = f1.numerator * f2.denominator - f2.numerator * f1.denominator;
    int den = f1.denominator * f2.denominator;
    return Fraction(num, den);
}

Fraction operator*(const Fraction& f1, const Fraction& f2) {
    return Fraction(f1.numerator * f2.numerator,
                    f1.denominator * f2.denominator);
}

Fraction operator/(const Fraction& f1, const Fraction& f2) {
    return Fraction(f1.numerator * f2.denominator,
                    f1.denominator * f2.numerator);
}

bool operator==(const Fraction& f1, const Fraction& f2) {
    return f1.numerator == f2.numerator &&
           f1.denominator == f2.denominator;
}

bool operator<(const Fraction& f1, const Fraction& f2) {
    return f1.numerator * f2.denominator < f2.numerator * f1.denominator;
}

ostream& operator<<(ostream& out, const Fraction& f) {
    if (f.denominator == 1) {
        out << f.numerator;
    } else {
        out << f.numerator << "/" << f.denominator;
    }
    return out;
}

istream& operator>>(istream& in, Fraction& f) {
    char slash;
    in >> f.numerator >> slash >> f.denominator;
    if (f.denominator == 0) f.denominator = 1;
    f.simplify();
    return in;
}

int main() {
    Fraction f1(1, 2);
    Fraction f2(1, 3);

    cout << "f1 = " << f1 << endl;
    cout << "f2 = " << f2 << endl;

    cout << "f1 + f2 = " << (f1 + f2) << endl;
    cout << "f1 - f2 = " << (f1 - f2) << endl;
    cout << "f1 * f2 = " << (f1 * f2) << endl;
    cout << "f1 / f2 = " << (f1 / f2) << endl;

    cout << "f1 == f2: " << (f1 == f2 ? "true" : "false") << endl;
    cout << "f1 < f2: " << (f1 < f2 ? "true" : "false") << endl;

    return 0;
}
```

---

## Key Takeaways

1. **Friend functions** access private/protected members
2. **Not member functions** - different calling syntax
3. **Use sparingly** - breaks encapsulation
4. **Friend classes** have full access
5. **Friendship is not inherited**
6. **One-way relationship** - not mutual
7. **Useful for** operator overloading
8. **Document why** friendship is necessary

---

## Common Mistakes

### Mistake 1: Thinking Friendship is Mutual
```cpp
class A {
    friend class B;  // B can access A
};

class B {
    // A cannot access B unless explicitly declared friend
};
```

### Mistake 2: Thinking Friendship is Inherited
```cpp
class Base {
    friend class Friend;
};

class Derived : public Base {
    // Friend cannot access Derived's private members
};
```

### Mistake 3: Overusing Friends
```cpp
// BAD: Too many friends breaks encapsulation
class Bad {
    friend class Helper1;
    friend class Helper2;
    friend class Helper3;
    // ... consider redesign
};
```

---

## Summary Checklist

Before moving to Lesson 38, ensure you can:
- [ ] Declare and define friend functions
- [ ] Use friend classes appropriately
- [ ] Understand friend vs member functions
- [ ] Use friends for operator overloading
- [ ] Know when to use friends
- [ ] Understand friendship limitations
- [ ] Apply best practices for friends

---

## Next Lesson Preview

In **Lesson 38: Operator Overloading Part 1**, we'll learn:
- What operator overloading is
- Overloading arithmetic operators
- Overloading comparison operators
- Overloading as member vs friend
- Rules for operator overloading

Excellent work! You now understand friend functions and classes!
