# Lesson 45: Abstract Classes and Interfaces

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Pure Virtual Functions
2. Abstract Classes
3. Interfaces in C++
4. Abstract vs Concrete Classes
5. Design Patterns with Abstraction
6. Real-World Examples
7. Best Practices
8. Exercises
9. Practice Projects

---

## 1. Pure Virtual Functions

**Pure virtual function**: Virtual function with no implementation in base class.

Syntax: `virtual returnType functionName() = 0;`

```cpp
#include <iostream>
using namespace std;

class Shape {
public:
    // Pure virtual functions
    virtual double area() = 0;
    virtual double perimeter() = 0;
    virtual void draw() = 0;
    
    // Regular virtual function
    virtual void displayType() {
        cout << "This is a shape" << endl;
    }
};
```

---

## 2. Abstract Classes

**Abstract class**: Class with at least one pure virtual function. Cannot be instantiated.

```cpp
#include <iostream>
#include <cmath>
using namespace std;

// Abstract class
class Shape {
public:
    virtual double area() = 0;  // Pure virtual
    virtual void draw() = 0;    // Pure virtual
    
    void printArea() {  // Concrete function
        cout << "Area: " << area() << endl;
    }
    
    virtual ~Shape() {}  // Virtual destructor
};

// Concrete class
class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    double area() override {
        return 3.14159 * radius * radius;
    }
    
    void draw() override {
        cout << "Drawing circle with radius " << radius << endl;
    }
};

class Rectangle : public Shape {
private:
    double width, height;
    
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    
    double area() override {
        return width * height;
    }
    
    void draw() override {
        cout << "Drawing rectangle " << width << "x" << height << endl;
    }
};

int main() {
    // Shape s;  // ERROR: Cannot instantiate abstract class
    
    Shape* shapes[2];
    shapes[0] = new Circle(5.0);
    shapes[1] = new Rectangle(4.0, 6.0);
    
    for (int i = 0; i < 2; i++) {
        shapes[i]->draw();
        shapes[i]->printArea();
        cout << endl;
    }
    
    // Cleanup
    for (int i = 0; i < 2; i++) {
        delete shapes[i];
    }
    
    return 0;
}
```

---

## 3. Interfaces in C++

**Interface**: Abstract class with only pure virtual functions (no data members).

```cpp
#include <iostream>
#include <string>
using namespace std;

// Interface (pure abstract class)
class IDrawable {
public:
    virtual void draw() = 0;
    virtual ~IDrawable() {}
};

class IPrintable {
public:
    virtual void print() = 0;
    virtual ~IPrintable() {}
};

// Implementing multiple interfaces
class Document : public IDrawable, public IPrintable {
private:
    string content;
    
public:
    Document(const string& c) : content(c) {}
    
    void draw() override {
        cout << "Drawing document: " << content << endl;
    }
    
    void print() override {
        cout << "Printing document: " << content << endl;
    }
};

int main() {
    Document doc("Hello World");
    doc.draw();
    doc.print();
    
    // Polymorphism through interfaces
    IDrawable* drawable = &doc;
    drawable->draw();
    
    IPrintable* printable = &doc;
    printable->print();
    
    return 0;
}
```

---

## 4. Abstract vs Concrete Classes

### Comparison:

| Aspect | Abstract Class | Concrete Class |
|--------|---------------|----------------|
| Instantiation | Cannot instantiate | Can instantiate |
| Purpose | Define interface | Provide implementation |
| Virtual functions | Has pure virtual | All implemented |
| Use case | Base class | Derived class |

---

## 5. Design Patterns with Abstraction

### Factory Pattern Example:

```cpp
#include <iostream>
#include <string>
using namespace std;

// Abstract Product
class Animal {
public:
    virtual void makeSound() = 0;
    virtual string getType() = 0;
    virtual ~Animal() {}
};

// Concrete Products
class Dog : public Animal {
public:
    void makeSound() override { cout << "Woof!" << endl; }
    string getType() override { return "Dog"; }
};

class Cat : public Animal {
public:
    void makeSound() override { cout << "Meow!" << endl; }
    string getType() override { return "Cat"; }
};

// Factory
class AnimalFactory {
public:
    static Animal* createAnimal(const string& type) {
        if (type == "dog") return new Dog();
        if (type == "cat") return new Cat();
        return nullptr;
    }
};

int main() {
    Animal* pet1 = AnimalFactory::createAnimal("dog");
    Animal* pet2 = AnimalFactory::createAnimal("cat");
    
    pet1->makeSound();
    pet2->makeSound();
    
    delete pet1;
    delete pet2;
    
    return 0;
}
```

---

## Key Takeaways

1. **Pure virtual function**: `= 0` makes function pure
2. **Abstract class**: Has at least one pure virtual function
3. **Cannot instantiate**: Abstract classes
4. **Interface**: Pure abstract class (no implementation)
5. **Polymorphism**: Use base pointers to derived objects
6. **Virtual destructor**: Always in abstract base classes
7. **Design patterns**: Factory, Strategy, Observer use abstraction

---

## Next Lesson Preview

In **Lesson 46: Function Templates**, we'll learn:
- Template basics
- Function templates
- Template parameters
- Template specialization preview
- Generic programming

Excellent! You understand abstraction in C++!
