# Lesson 22: Advanced Constructors and Destructors

## Overview
This lesson explores advanced constructor and destructor patterns including delegating constructors, constructor initialization lists, explicit constructors, and the nuances of object lifetime management in C++.

## Topics Covered
- Constructor types and overloading
- Delegating constructors (C++11)
- Initialization lists vs assignment
- Explicit constructors
- Default and delete constructors
- Destructor order and virtual destructors
- Construction and destruction order in inheritance
- Exception safety in constructors

---

## 1. Constructor Types

### Example 1: Different Constructor Types

```cpp
#include <iostream>
#include <string>
#include <vector>

class Person {
private:
    std::string name;
    int age;
    std::string email;

public:
    // 1. Default constructor
    Person()
        : name("Unknown"), age(0), email("none@example.com") {
        std::cout << "Default constructor called" << std::endl;
    }

    // 2. Parameterized constructor
    Person(const std::string& n, int a)
        : name(n), age(a), email("none@example.com") {
        std::cout << "Parameterized constructor (name, age) called" << std::endl;
    }

    // 3. Full constructor
    Person(const std::string& n, int a, const std::string& e)
        : name(n), age(a), email(e) {
        std::cout << "Full constructor called" << std::endl;
    }

    // 4. Copy constructor
    Person(const Person& other)
        : name(other.name), age(other.age), email(other.email) {
        std::cout << "Copy constructor called for " << name << std::endl;
    }

    // 5. Move constructor
    Person(Person&& other) noexcept
        : name(std::move(other.name)),
          age(other.age),
          email(std::move(other.email)) {
        std::cout << "Move constructor called" << std::endl;
        other.age = 0;
    }

    void print() const {
        std::cout << "Person{" << name << ", " << age << ", " << email << "}" << std::endl;
    }
};

int main() {
    std::cout << "=== Different Constructor Types ===" << std::endl;

    Person p1;                          // Default
    Person p2("Alice", 25);             // Parameterized
    Person p3("Bob", 30, "bob@example.com");  // Full
    Person p4 = p2;                     // Copy
    Person p5 = std::move(p3);          // Move

    p1.print();
    p2.print();
    p4.print();
    p5.print();

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example1.cpp -o example1`

---

## 2. Delegating Constructors (C++11)

### Example 2: Constructor Delegation

```cpp
#include <iostream>
#include <string>
#include <stdexcept>

class Rectangle {
private:
    double width;
    double height;
    std::string name;

    void validate(double w, double h) {
        if (w <= 0 || h <= 0) {
            throw std::invalid_argument("Dimensions must be positive");
        }
    }

public:
    // Main constructor with validation
    Rectangle(double w, double h, const std::string& n)
        : width(w), height(h), name(n) {
        validate(w, h);
        std::cout << "Main constructor: " << name << " ("
                  << width << "x" << height << ")" << std::endl;
    }

    // Delegating constructor (no name)
    Rectangle(double w, double h)
        : Rectangle(w, h, "Unnamed") {  // Delegates to main constructor
        std::cout << "Delegating constructor (w, h) called" << std::endl;
    }

    // Delegating constructor (square)
    Rectangle(double side)
        : Rectangle(side, side, "Square") {  // Delegates to main constructor
        std::cout << "Delegating constructor (square) called" << std::endl;
    }

    // Delegating constructor (default)
    Rectangle()
        : Rectangle(1.0, 1.0, "Default") {  // Delegates to main constructor
        std::cout << "Delegating constructor (default) called" << std::endl;
    }

    double area() const {
        return width * height;
    }

    void print() const {
        std::cout << name << ": " << width << "x" << height
                  << " (area: " << area() << ")" << std::endl;
    }
};

int main() {
    std::cout << "=== Delegating Constructors ===" << std::endl;

    try {
        std::cout << "\n1. Default constructor:" << std::endl;
        Rectangle r1;
        r1.print();

        std::cout << "\n2. Square constructor:" << std::endl;
        Rectangle r2(5.0);
        r2.print();

        std::cout << "\n3. Width/height constructor:" << std::endl;
        Rectangle r3(3.0, 4.0);
        r3.print();

        std::cout << "\n4. Full constructor:" << std::endl;
        Rectangle r4(2.0, 6.0, "Custom");
        r4.print();

        std::cout << "\n5. Invalid constructor (should throw):" << std::endl;
        Rectangle r5(-1.0, 5.0);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example2.cpp -o example2`

---

## 3. Initialization Lists vs Assignment

### Example 3: Why Initialization Lists Matter

```cpp
#include <iostream>
#include <string>

class Expensive {
private:
    std::string data;

public:
    Expensive() {
        std::cout << "Expensive: default constructor" << std::endl;
    }

    Expensive(const std::string& d) : data(d) {
        std::cout << "Expensive: parameterized constructor (" << data << ")" << std::endl;
    }

    Expensive(const Expensive& other) : data(other.data) {
        std::cout << "Expensive: copy constructor (" << data << ")" << std::endl;
    }

    Expensive& operator=(const Expensive& other) {
        std::cout << "Expensive: copy assignment (" << other.data << ")" << std::endl;
        data = other.data;
        return *this;
    }

    ~Expensive() {
        std::cout << "Expensive: destructor (" << data << ")" << std::endl;
    }
};

// BAD: Using assignment in constructor body
class BadContainer {
private:
    Expensive member;

public:
    BadContainer(const std::string& value) {
        // member is first default-constructed, then assigned
        member = Expensive(value);  // Creates temporary, assigns, destroys temporary
        std::cout << "BadContainer: constructor body" << std::endl;
    }
};

// GOOD: Using initialization list
class GoodContainer {
private:
    Expensive member;

public:
    GoodContainer(const std::string& value)
        : member(value) {  // Direct construction, no temporary
        std::cout << "GoodContainer: constructor body" << std::endl;
    }
};

int main() {
    std::cout << "=== BAD: Assignment in constructor ===" << std::endl;
    {
        BadContainer bad("test");
    }

    std::cout << "\n=== GOOD: Initialization list ===" << std::endl;
    {
        GoodContainer good("test");
    }

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example3.cpp -o example3`

---

## 4. Explicit Constructors

### Example 4: Preventing Implicit Conversions

```cpp
#include <iostream>
#include <string>

// Without explicit: allows implicit conversions
class ImplicitArray {
private:
    int* data;
    size_t size;

public:
    ImplicitArray(size_t n) : data(new int[n]), size(n) {
        std::cout << "ImplicitArray constructor (size: " << size << ")" << std::endl;
    }

    ~ImplicitArray() {
        delete[] data;
    }

    size_t getSize() const { return size; }
};

// With explicit: prevents implicit conversions
class ExplicitArray {
private:
    int* data;
    size_t size;

public:
    explicit ExplicitArray(size_t n) : data(new int[n]), size(n) {
        std::cout << "ExplicitArray constructor (size: " << size << ")" << std::endl;
    }

    ~ExplicitArray() {
        delete[] data;
    }

    size_t getSize() const { return size; }
};

void processImplicit(ImplicitArray arr) {
    std::cout << "Processing implicit array of size " << arr.getSize() << std::endl;
}

void processExplicit(ExplicitArray arr) {
    std::cout << "Processing explicit array of size " << arr.getSize() << std::endl;
}

int main() {
    std::cout << "=== Implicit Conversions ===" << std::endl;

    // ImplicitArray allows implicit conversion from int
    ImplicitArray arr1(10);      // OK: direct initialization
    ImplicitArray arr2 = 20;     // OK: copy initialization with implicit conversion
    processImplicit(30);         // OK: implicit conversion in function call

    std::cout << "\n=== Explicit Constructors ===" << std::endl;

    // ExplicitArray prevents implicit conversion
    ExplicitArray arr3(10);      // OK: direct initialization
    // ExplicitArray arr4 = 20;  // ERROR: cannot use copy initialization
    // processExplicit(30);       // ERROR: cannot implicitly convert
    processExplicit(ExplicitArray(40));  // OK: explicit construction

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example4.cpp -o example4`

---

## 5. Virtual Destructors

### Example 5: Importance of Virtual Destructors

```cpp
#include <iostream>
#include <memory>

// WITHOUT virtual destructor (DANGEROUS)
class BaseNonVirtual {
public:
    BaseNonVirtual() {
        std::cout << "BaseNonVirtual constructor" << std::endl;
    }

    ~BaseNonVirtual() {  // NOT virtual
        std::cout << "BaseNonVirtual destructor" << std::endl;
    }
};

class DerivedNonVirtual : public BaseNonVirtual {
private:
    int* data;

public:
    DerivedNonVirtual() : data(new int[100]) {
        std::cout << "DerivedNonVirtual constructor (allocated memory)" << std::endl;
    }

    ~DerivedNonVirtual() {
        std::cout << "DerivedNonVirtual destructor (freeing memory)" << std::endl;
        delete[] data;
    }
};

// WITH virtual destructor (CORRECT)
class BaseVirtual {
public:
    BaseVirtual() {
        std::cout << "BaseVirtual constructor" << std::endl;
    }

    virtual ~BaseVirtual() {  // Virtual destructor
        std::cout << "BaseVirtual destructor" << std::endl;
    }
};

class DerivedVirtual : public BaseVirtual {
private:
    int* data;

public:
    DerivedVirtual() : data(new int[100]) {
        std::cout << "DerivedVirtual constructor (allocated memory)" << std::endl;
    }

    ~DerivedVirtual() override {
        std::cout << "DerivedVirtual destructor (freeing memory)" << std::endl;
        delete[] data;
    }
};

int main() {
    std::cout << "=== WITHOUT Virtual Destructor (MEMORY LEAK!) ===" << std::endl;
    {
        BaseNonVirtual* ptr = new DerivedNonVirtual();
        delete ptr;  // Only calls BaseNonVirtual destructor!
                     // DerivedNonVirtual destructor NOT called -> MEMORY LEAK!
    }

    std::cout << "\n=== WITH Virtual Destructor (CORRECT) ===" << std::endl;
    {
        BaseVirtual* ptr = new DerivedVirtual();
        delete ptr;  // Calls both destructors correctly
    }

    std::cout << "\n=== Using Smart Pointers (SAFE) ===" << std::endl;
    {
        std::unique_ptr<BaseVirtual> ptr = std::make_unique<DerivedVirtual>();
        // Automatically calls correct destructor
    }

    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall example5.cpp -o example5`

---

## Project: Object Pool with Custom Allocator

A complete object pool implementation demonstrating advanced constructor/destructor patterns.

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

template<typename T>
class ObjectPool {
private:
    struct Slot {
        alignas(T) unsigned char storage[sizeof(T)];
        bool in_use;

        Slot() : in_use(false) {}
    };

    std::vector<Slot> pool;
    size_t capacity;

public:
    explicit ObjectPool(size_t size) : pool(size), capacity(size) {
        std::cout << "ObjectPool created with capacity " << capacity << std::endl;
    }

    ~ObjectPool() {
        // Destroy all active objects
        for (auto& slot : pool) {
            if (slot.in_use) {
                T* obj = reinterpret_cast<T*>(slot.storage);
                obj->~T();
            }
        }
        std::cout << "ObjectPool destroyed" << std::endl;
    }

    // Prevent copying
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    // Allocate and construct object
    template<typename... Args>
    T* construct(Args&&... args) {
        // Find free slot
        for (auto& slot : pool) {
            if (!slot.in_use) {
                // Construct object in-place
                T* obj = new (slot.storage) T(std::forward<Args>(args)...);
                slot.in_use = true;
                return obj;
            }
        }
        throw std::runtime_error("Object pool exhausted");
    }

    // Destroy object and return to pool
    void destroy(T* obj) {
        if (!obj) return;

        // Find the slot
        for (auto& slot : pool) {
            if (reinterpret_cast<T*>(slot.storage) == obj) {
                if (slot.in_use) {
                    obj->~T();  // Explicitly call destructor
                    slot.in_use = false;
                    return;
                }
            }
        }
        throw std::invalid_argument("Object not from this pool");
    }

    size_t available() const {
        size_t count = 0;
        for (const auto& slot : pool) {
            if (!slot.in_use) ++count;
        }
        return count;
    }

    size_t getCapacity() const { return capacity; }
};

// Example class to use with pool
class GameObject {
private:
    int id;
    std::string name;
    double x, y;

public:
    GameObject(int i, const std::string& n, double px, double py)
        : id(i), name(n), x(px), y(py) {
        std::cout << "GameObject " << id << " constructed at (" << x << ", " << y << ")" << std::endl;
    }

    ~GameObject() {
        std::cout << "GameObject " << id << " destroyed" << std::endl;
    }

    void update(double dx, double dy) {
        x += dx;
        y += dy;
    }

    void print() const {
        std::cout << "GameObject[" << id << "]: " << name
                  << " at (" << x << ", " << y << ")" << std::endl;
    }

    int getId() const { return id; }
};

int main() {
    std::cout << "=== Object Pool Demo ===" << std::endl;

    ObjectPool<GameObject> pool(5);

    std::cout << "\nAvailable slots: " << pool.available() << "/" << pool.getCapacity() << std::endl;

    std::cout << "\n--- Creating objects ---" << std::endl;
    GameObject* obj1 = pool.construct(1, "Player", 0.0, 0.0);
    GameObject* obj2 = pool.construct(2, "Enemy1", 10.0, 5.0);
    GameObject* obj3 = pool.construct(3, "Enemy2", -5.0, 8.0);

    std::cout << "\nAvailable slots: " << pool.available() << "/" << pool.getCapacity() << std::endl;

    std::cout << "\n--- Using objects ---" << std::endl;
    obj1->print();
    obj2->print();
    obj3->print();

    obj1->update(5.0, 3.0);
    obj1->print();

    std::cout << "\n--- Destroying object 2 ---" << std::endl;
    pool.destroy(obj2);

    std::cout << "\nAvailable slots: " << pool.available() << "/" << pool.getCapacity() << std::endl;

    std::cout << "\n--- Creating new object (reuses slot) ---" << std::endl;
    GameObject* obj4 = pool.construct(4, "PowerUp", 3.0, 7.0);
    obj4->print();

    std::cout << "\nAvailable slots: " << pool.available() << "/" << pool.getCapacity() << std::endl;

    std::cout << "\n--- Testing pool exhaustion ---" << std::endl;
    try {
        pool.construct(5, "Item1", 1.0, 1.0);
        pool.construct(6, "Item2", 2.0, 2.0);
        pool.construct(7, "Item3", 3.0, 3.0);  // This should throw
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n--- Cleaning up remaining objects ---" << std::endl;
    pool.destroy(obj1);
    pool.destroy(obj3);
    pool.destroy(obj4);

    std::cout << "\nAvailable slots: " << pool.available() << "/" << pool.getCapacity() << std::endl;

    std::cout << "\n--- Pool destructor will be called ---" << std::endl;
    return 0;
}
```

**Compile:** `g++ -std=c++20 -Wall project.cpp -o project`

---

## Summary

In this lesson, you learned about:
- Different constructor types (default, parameterized, copy, move)
- Delegating constructors to reduce code duplication
- Initialization lists vs assignment in constructors
- Explicit constructors to prevent implicit conversions
- Virtual destructors for polymorphic classes
- Construction and destruction order
- Object pool implementation with placement new

## Key Takeaways

1. **Always use initialization lists** for member initialization
2. **Use `explicit`** for single-argument constructors to prevent implicit conversions
3. **Virtual destructors** are mandatory for polymorphic base classes
4. **Delegating constructors** reduce code duplication
5. **Constructor exceptions** leave the object in an invalid state

## Exercises

1. Implement a `String` class with all five special member functions
2. Create a smart pointer class with custom deleter support
3. Build a factory pattern using constructor delegation
4. Implement a RAII wrapper for file handles with proper error handling
5. Create a memory pool with alignment requirements

## Next Lesson
Lesson 23: Encapsulation and Access Control
