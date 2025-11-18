# Lesson 43: Multiple Inheritance

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Understanding Multiple Inheritance
2. Syntax and Basics
3. Ambiguity and Resolution
4. Diamond Problem
5. Virtual Base Classes
6. Constructor Order in Multiple Inheritance
7. Best Practices
8. Exercises
9. Practice Projects

---

## 1. Understanding Multiple Inheritance

**Multiple inheritance** allows a class to inherit from more than one base class.

```
    Base1    Base2
       \      /
        \    /
       Derived
```

### Advantages:
- Code reuse from multiple sources
- Model complex relationships
- Combine functionalities

### Disadvantages:
- Complexity
- Ambiguity issues
- Diamond problem

---

## 2. Syntax and Basics

```cpp
#include <iostream>
using namespace std;

class Printable {
public:
    void print() {
        cout << "Printing..." << endl;
    }
};

class Scannable {
public:
    void scan() {
        cout << "Scanning..." << endl;
    }
};

// Multiple inheritance
class Printer : public Printable, public Scannable {
public:
    void process() {
        print();
        scan();
    }
};

int main() {
    Printer device;
    device.print();
    device.scan();
    device.process();
    return 0;
}
```

---

## 3. Ambiguity and Resolution

```cpp
#include <iostream>
using namespace std;

class Base1 {
public:
    void display() {
        cout << "Base1 display" << endl;
    }
};

class Base2 {
public:
    void display() {
        cout << "Base2 display" << endl;
    }
};

class Derived : public Base1, public Base2 {
public:
    void show() {
        // Ambiguous: display()
        Base1::display();  // Resolve using scope
        Base2::display();
    }
};

int main() {
    Derived obj;
    obj.show();
    return 0;
}
```

---

## 4. Diamond Problem

```cpp
#include <iostream>
using namespace std;

class Device {
protected:
    string brand;
public:
    Device() { cout << "Device constructor" << endl; }
};

class Phone : public Device {
public:
    Phone() { cout << "Phone constructor" << endl; }
};

class Camera : public Device {
public:
    Camera() { cout << "Camera constructor" << endl; }
};

// Diamond: Device appears twice!
class Smartphone : public Phone, public Camera {
public:
    Smartphone() { cout << "Smartphone constructor" << endl; }
};

int main() {
    Smartphone phone;
    // phone.brand = "Apple";  // ERROR: Ambiguous!
    return 0;
}
```

---

## 5. Virtual Base Classes

**Solution to diamond problem**: Virtual inheritance

```cpp
#include <iostream>
using namespace std;

class Device {
protected:
    string brand;
public:
    Device() : brand("Unknown") {
        cout << "Device constructor" << endl;
    }
};

// Virtual inheritance
class Phone : virtual public Device {
public:
    Phone() { cout << "Phone constructor" << endl; }
};

class Camera : virtual public Device {
public:
    Camera() { cout << "Camera constructor" << endl; }
};

class Smartphone : public Phone, public Camera {
public:
    Smartphone() {
        cout << "Smartphone constructor" << endl;
        brand = "Apple";  // Now unambiguous!
    }
    
    void display() {
        cout << "Brand: " << brand << endl;
    }
};

int main() {
    Smartphone phone;
    phone.display();
    return 0;
}
```

---

## Key Takeaways

1. Multiple inheritance: inherit from multiple classes
2. Ambiguity: resolve with scope operator
3. Diamond problem: virtual inheritance solves it
4. Use sparingly: prefer composition
5. Virtual base: only one copy of base
6. Constructor order: left to right in declaration
7. Consider interfaces instead

---

## Next Lesson Preview

In **Lesson 44: Polymorphism and Virtual Functions**, we'll learn:
- Runtime polymorphism
- Virtual functions
- Virtual destructors
- Pure virtual functions

Excellent work on multiple inheritance!
