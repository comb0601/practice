# Lesson 42: Types of Inheritance

**Duration**: 7 hours
**Difficulty**: Intermediate

## Table of Contents
1. Single Inheritance
2. Multiple Inheritance
3. Multilevel Inheritance
4. Hierarchical Inheritance
5. Hybrid Inheritance
6. Virtual Inheritance
7. Diamond Problem
8. Exercises
9. Practice Projects

---

## 1. Single Inheritance

**Single inheritance**: One derived class inherits from one base class.

```cpp
#include <iostream>
using namespace std;

class Vehicle {
protected:
    int speed;
public:
    Vehicle() : speed(0) {}
    void setSpeed(int s) { speed = s; }
    int getSpeed() { return speed; }
};

class Car : public Vehicle {
private:
    int doors;
public:
    Car(int d) : doors(d) {}
    void display() {
        cout << "Car with " << doors << " doors, speed: " << speed << endl;
    }
};

int main() {
    Car myCar(4);
    myCar.setSpeed(120);
    myCar.display();
    return 0;
}
```

---

## 2. Multiple Inheritance

**Multiple inheritance**: One class inherits from multiple base classes.

```cpp
#include <iostream>
using namespace std;

class Engine {
protected:
    int horsepower;
public:
    Engine(int hp) : horsepower(hp) {}
    void startEngine() {
        cout << "Engine started (" << horsepower << " HP)" << endl;
    }
};

class GPS {
protected:
    string location;
public:
    GPS() : location("Unknown") {}
    void setLocation(string loc) {
        location = loc;
        cout << "GPS location: " << location << endl;
    }
};

// Multiple inheritance from Engine and GPS
class ModernCar : public Engine, public GPS {
private:
    string model;
public:
    ModernCar(string m, int hp) : Engine(hp), model(m) {}
    
    void display() {
        cout << "Car Model: " << model << endl;
        cout << "Horsepower: " << horsepower << endl;
        cout << "Location: " << location << endl;
    }
};

int main() {
    ModernCar car("Tesla Model 3", 283);
    car.startEngine();
    car.setLocation("New York");
    car.display();
    return 0;
}
```

---

## 3. Multilevel Inheritance

**Multilevel inheritance**: Chain of inheritance (A → B → C).

```cpp
#include <iostream>
#include <string>
using namespace std;

// Level 1
class LivingBeing {
protected:
    bool isAlive;
public:
    LivingBeing() : isAlive(true) {}
    void breathe() {
        cout << "Breathing..." << endl;
    }
};

// Level 2
class Animal : public LivingBeing {
protected:
    int legs;
public:
    Animal(int l) : legs(l) {}
    void move() {
        cout << "Moving with " << legs << " legs" << endl;
    }
};

// Level 3
class Dog : public Animal {
private:
    string name;
public:
    Dog(string n) : Animal(4), name(n) {}
    void bark() {
        cout << name << " says: Woof!" << endl;
    }
    void showAll() {
        breathe();  // From LivingBeing
        move();     // From Animal
        bark();     // From Dog
    }
};

int main() {
    Dog myDog("Buddy");
    myDog.showAll();
    return 0;
}
```

---

## 4. Hierarchical Inheritance

**Hierarchical inheritance**: Multiple classes inherit from one base class.

```cpp
#include <iostream>
using namespace std;

class Shape {
protected:
    double dim1, dim2;
public:
    Shape(double d1, double d2) : dim1(d1), dim2(d2) {}
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : Shape(w, h) {}
    double area() { return dim1 * dim2; }
};

class Triangle : public Shape {
public:
    Triangle(double b, double h) : Shape(b, h) {}
    double area() { return 0.5 * dim1 * dim2; }
};

int main() {
    Rectangle rect(10, 5);
    Triangle tri(10, 5);
    cout << "Rectangle area: " << rect.area() << endl;
    cout << "Triangle area: " << tri.area() << endl;
    return 0;
}
```

---

## 5. Hybrid Inheritance

**Hybrid inheritance**: Combination of multiple types.

```cpp
#include <iostream>
using namespace std;

// Base class
class Device {
protected:
    string brand;
public:
    Device(string b) : brand(b) {}
};

// Multiple inheritance level 1
class Phone : public Device {
protected:
    string phoneNumber;
public:
    Phone(string b, string num) : Device(b), phoneNumber(num) {}
};

class Camera : public Device {
protected:
    int megapixels;
public:
    Camera(string b, int mp) : Device(b), megapixels(mp) {}
};

// Multiple inheritance level 2
class Smartphone : public Phone, public Camera {
public:
    Smartphone(string b, string num, int mp)
        : Phone(b, num), Camera(b, mp) {}
    
    void display() {
        cout << "Smartphone - Brand: " << Phone::brand << endl;
        cout << "Phone: " << phoneNumber << endl;
        cout << "Camera: " << megapixels << "MP" << endl;
    }
};

int main() {
    Smartphone phone("Apple", "555-1234", 12);
    phone.display();
    return 0;
}
```

---

## 6. Virtual Inheritance

**Virtual inheritance**: Solves the diamond problem.

```cpp
#include <iostream>
using namespace std;

class Base {
protected:
    int value;
public:
    Base() : value(100) {
        cout << "Base constructor" << endl;
    }
};

// Virtual inheritance
class Derived1 : virtual public Base {
public:
    Derived1() {
        cout << "Derived1 constructor" << endl;
    }
};

class Derived2 : virtual public Base {
public:
    Derived2() {
        cout << "Derived2 constructor" << endl;
    }
};

class Final : public Derived1, public Derived2 {
public:
    Final() {
        cout << "Final constructor" << endl;
        cout << "Value: " << value << endl;  // Only one copy!
    }
};

int main() {
    Final obj;
    return 0;
}
```

---

## Key Takeaways

1. **Single**: One base, one derived
2. **Multiple**: Multiple bases, one derived
3. **Multilevel**: Chain of inheritance
4. **Hierarchical**: One base, multiple derived
5. **Hybrid**: Combination of types
6. **Virtual inheritance**: Solves diamond problem
7. **Diamond problem**: Ambiguity in multiple inheritance

---

## Next Lesson Preview

In **Lesson 43: Multiple Inheritance**, we'll dive deeper into:
- Multiple inheritance details
- Ambiguity resolution
- Virtual base classes
- Best practices

Great progress on inheritance types!
