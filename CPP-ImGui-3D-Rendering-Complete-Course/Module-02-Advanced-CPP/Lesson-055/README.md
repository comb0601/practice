# Lesson 55: Smart Pointers (unique_ptr, shared_ptr, weak_ptr)

**Duration:** 2.5 hours

## Learning Objectives
- Master std::unique_ptr for exclusive ownership
- Understand std::shared_ptr for shared ownership
- Learn std::weak_ptr to break circular references
- Implement RAII with smart pointers
- Avoid memory leaks and dangling pointers

## Introduction

Smart pointers are wrapper classes that manage dynamically allocated memory automatically. They follow RAII (Resource Acquisition Is Initialization) and ensure proper cleanup, eliminating manual delete calls and preventing memory leaks.

## std::unique_ptr - Exclusive Ownership

### Basic Usage

```cpp
#include <iostream>
#include <memory>
#include <string>

class Resource {
private:
    std::string name;

public:
    Resource(const std::string& n) : name(n) {
        std::cout << "Resource acquired: " << name << std::endl;
    }

    ~Resource() {
        std::cout << "Resource released: " << name << std::endl;
    }

    void use() {
        std::cout << "Using resource: " << name << std::endl;
    }
};

int main() {
    // Creating unique_ptr
    std::unique_ptr<Resource> ptr1(new Resource("Resource1"));
    ptr1->use();

    // Using make_unique (C++14, preferred)
    auto ptr2 = std::make_unique<Resource>("Resource2");
    ptr2->use();

    // Get raw pointer
    Resource* raw = ptr2.get();
    raw->use();

    // ptr1 and ptr2 automatically deleted when going out of scope
    return 0;
}
```

**Output:**
```
Resource acquired: Resource1
Using resource: Resource1
Resource acquired: Resource2
Using resource: Resource2
Using resource: Resource2
Resource released: Resource2
Resource released: Resource1
```

### Moving unique_ptr

```cpp
#include <iostream>
#include <memory>
#include <vector>

class Widget {
private:
    int id;

public:
    Widget(int i) : id(i) {
        std::cout << "Widget " << id << " created" << std::endl;
    }

    ~Widget() {
        std::cout << "Widget " << id << " destroyed" << std::endl;
    }

    int getId() const { return id; }
};

std::unique_ptr<Widget> createWidget(int id) {
    return std::make_unique<Widget>(id);
}

int main() {
    // Move from function return
    auto w1 = createWidget(1);

    // Move construction
    auto w2 = std::move(w1);
    std::cout << "w1 is " << (w1 ? "valid" : "null") << std::endl;
    std::cout << "w2 points to widget " << w2->getId() << std::endl;

    // Store in vector (requires move)
    std::vector<std::unique_ptr<Widget>> widgets;
    widgets.push_back(std::make_unique<Widget>(3));
    widgets.push_back(std::make_unique<Widget>(4));

    std::cout << "\nWidgets in vector: " << widgets.size() << std::endl;

    return 0;
}
```

### Custom Deleters

```cpp
#include <iostream>
#include <memory>
#include <cstdio>

// Custom deleter for FILE*
struct FileDeleter {
    void operator()(FILE* fp) const {
        if (fp) {
            std::cout << "Closing file" << std::endl;
            fclose(fp);
        }
    }
};

// Lambda deleter
auto arrayDeleter = [](int* p) {
    std::cout << "Deleting array" << std::endl;
    delete[] p;
};

int main() {
    // Custom deleter with struct
    std::unique_ptr<FILE, FileDeleter> file(fopen("test.txt", "w"));
    if (file) {
        fprintf(file.get(), "Hello, smart pointers!\n");
    }

    // Custom deleter with lambda
    std::unique_ptr<int[], decltype(arrayDeleter)> arr(new int[10], arrayDeleter);
    for (int i = 0; i < 10; ++i) {
        arr[i] = i * i;
    }

    // std::unique_ptr with array
    auto arr2 = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) {
        arr2[i] = i;
        std::cout << arr2[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## std::shared_ptr - Shared Ownership

### Basic Usage

```cpp
#include <iostream>
#include <memory>

class Data {
private:
    int value;

public:
    Data(int v) : value(v) {
        std::cout << "Data(" << value << ") created" << std::endl;
    }

    ~Data() {
        std::cout << "Data(" << value << ") destroyed" << std::endl;
    }

    int getValue() const { return value; }
};

int main() {
    // Create shared_ptr
    auto ptr1 = std::make_shared<Data>(42);
    std::cout << "Reference count: " << ptr1.use_count() << std::endl;

    {
        // Share ownership
        auto ptr2 = ptr1;
        std::cout << "Reference count: " << ptr1.use_count() << std::endl;

        auto ptr3 = ptr1;
        std::cout << "Reference count: " << ptr1.use_count() << std::endl;

        std::cout << "Value: " << ptr2->getValue() << std::endl;
    }  // ptr2 and ptr3 destroyed

    std::cout << "Reference count: " << ptr1.use_count() << std::endl;

    return 0;
}  // ptr1 destroyed, Data destroyed
```

### Shared Resources

```cpp
#include <iostream>
#include <memory>
#include <vector>

class Image {
private:
    std::string filename;
    size_t width, height;

public:
    Image(const std::string& file, size_t w, size_t h)
        : filename(file), width(w), height(h) {
        std::cout << "Loading image: " << filename << std::endl;
    }

    ~Image() {
        std::cout << "Unloading image: " << filename << std::endl;
    }

    void display() const {
        std::cout << "Displaying " << filename
                  << " (" << width << "x" << height << ")" << std::endl;
    }
};

class Sprite {
private:
    std::shared_ptr<Image> image;
    int x, y;

public:
    Sprite(std::shared_ptr<Image> img, int posX, int posY)
        : image(img), x(posX), y(posY) {}

    void render() {
        std::cout << "Sprite at (" << x << ", " << y << "): ";
        image->display();
    }
};

int main() {
    // Multiple sprites share the same image
    auto texture = std::make_shared<Image>("hero.png", 64, 64);

    std::vector<Sprite> sprites;
    sprites.emplace_back(texture, 100, 100);
    sprites.emplace_back(texture, 200, 150);
    sprites.emplace_back(texture, 300, 200);

    std::cout << "Image reference count: " << texture.use_count() << std::endl;

    std::cout << "\nRendering sprites:" << std::endl;
    for (auto& sprite : sprites) {
        sprite.render();
    }

    return 0;
}
```

## std::weak_ptr - Breaking Circular References

### Observing without Owning

```cpp
#include <iostream>
#include <memory>

class Observer {
public:
    void update(int value) {
        std::cout << "Observer notified: " << value << std::endl;
    }
};

class Subject {
private:
    std::weak_ptr<Observer> observer;
    int state;

public:
    Subject() : state(0) {}

    void setObserver(std::shared_ptr<Observer> obs) {
        observer = obs;
    }

    void setState(int s) {
        state = s;
        notify();
    }

    void notify() {
        // Check if observer still exists
        if (auto obs = observer.lock()) {
            obs->update(state);
        } else {
            std::cout << "Observer no longer exists" << std::endl;
        }
    }
};

int main() {
    Subject subject;

    {
        auto observer = std::make_shared<Observer>();
        subject.setObserver(observer);

        subject.setState(10);  // Observer exists
        subject.setState(20);  // Observer exists
    }  // observer destroyed

    subject.setState(30);  // Observer doesn't exist

    return 0;
}
```

### Breaking Circular Dependencies

```cpp
#include <iostream>
#include <memory>
#include <string>

class Child;

class Parent {
private:
    std::string name;
    std::shared_ptr<Child> child;

public:
    Parent(const std::string& n) : name(n) {
        std::cout << "Parent " << name << " created" << std::endl;
    }

    ~Parent() {
        std::cout << "Parent " << name << " destroyed" << std::endl;
    }

    void setChild(std::shared_ptr<Child> c) {
        child = c;
    }

    const std::string& getName() const { return name; }
};

class Child {
private:
    std::string name;
    std::weak_ptr<Parent> parent;  // Use weak_ptr to break cycle!

public:
    Child(const std::string& n) : name(n) {
        std::cout << "Child " << name << " created" << std::endl;
    }

    ~Child() {
        std::cout << "Child " << name << " destroyed" << std::endl;
    }

    void setParent(std::shared_ptr<Parent> p) {
        parent = p;
    }

    void showParent() {
        if (auto p = parent.lock()) {
            std::cout << "My parent is: " << p->getName() << std::endl;
        } else {
            std::cout << "No parent" << std::endl;
        }
    }
};

int main() {
    {
        auto parent = std::make_shared<Parent>("Alice");
        auto child = std::make_shared<Child>("Bob");

        parent->setChild(child);
        child->setParent(parent);

        child->showParent();
    }  // Both properly destroyed due to weak_ptr

    std::cout << "Scope ended" << std::endl;

    return 0;
}
```

## Practical Example 1: Resource Manager

```cpp
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>

template <typename T>
class ResourceManager {
private:
    std::unordered_map<std::string, std::shared_ptr<T>> resources;

public:
    std::shared_ptr<T> load(const std::string& key) {
        auto it = resources.find(key);
        if (it != resources.end()) {
            std::cout << "Resource '" << key << "' found in cache" << std::endl;
            return it->second;
        }

        std::cout << "Loading new resource '" << key << "'" << std::endl;
        auto resource = std::make_shared<T>(key);
        resources[key] = resource;
        return resource;
    }

    void unload(const std::string& key) {
        resources.erase(key);
        std::cout << "Resource '" << key << "' unloaded" << std::endl;
    }

    size_t count() const {
        return resources.size();
    }
};

class Texture {
private:
    std::string filename;

public:
    Texture(const std::string& file) : filename(file) {
        std::cout << "  Texture loaded: " << filename << std::endl;
    }

    ~Texture() {
        std::cout << "  Texture unloaded: " << filename << std::endl;
    }

    const std::string& getFilename() const { return filename; }
};

int main() {
    ResourceManager<Texture> texManager;

    auto tex1 = texManager.load("hero.png");
    auto tex2 = texManager.load("hero.png");  // Cached
    auto tex3 = texManager.load("enemy.png");

    std::cout << "Resources in cache: " << texManager.count() << std::endl;
    std::cout << "hero.png ref count: " << tex1.use_count() << std::endl;

    return 0;
}
```

## Practical Example 2: Factory Pattern

```cpp
#include <iostream>
#include <memory>
#include <string>

class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    void draw() const override {
        std::cout << "Circle with radius " << radius << std::endl;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    void draw() const override {
        std::cout << "Rectangle " << width << "x" << height << std::endl;
    }
};

class ShapeFactory {
public:
    static std::unique_ptr<Shape> createCircle(double radius) {
        return std::make_unique<Circle>(radius);
    }

    static std::unique_ptr<Shape> createRectangle(double w, double h) {
        return std::make_unique<Rectangle>(w, h);
    }

    static std::unique_ptr<Shape> createShape(const std::string& type) {
        if (type == "circle") {
            return createCircle(5.0);
        } else if (type == "rectangle") {
            return createRectangle(10.0, 20.0);
        }
        return nullptr;
    }
};

int main() {
    auto circle = ShapeFactory::createCircle(7.5);
    auto rect = ShapeFactory::createRectangle(15.0, 25.0);
    auto shape = ShapeFactory::createShape("circle");

    circle->draw();
    rect->draw();
    if (shape) {
        shape->draw();
    }

    return 0;
}
```

## Practical Example 3: Observer Pattern

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const std::string& message) = 0;
};

class Subject {
private:
    std::vector<std::weak_ptr<IObserver>> observers;

public:
    void attach(std::shared_ptr<IObserver> observer) {
        observers.push_back(observer);
    }

    void notify(const std::string& message) {
        // Remove expired observers and notify active ones
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [&](std::weak_ptr<IObserver>& weak) {
                    if (auto obs = weak.lock()) {
                        obs->update(message);
                        return false;
                    }
                    return true;  // Remove expired
                }),
            observers.end()
        );
    }
};

class ConcreteObserver : public IObserver {
private:
    std::string name;

public:
    ConcreteObserver(const std::string& n) : name(n) {}

    void update(const std::string& message) override {
        std::cout << "Observer " << name << " received: " << message << std::endl;
    }
};

int main() {
    Subject subject;

    auto obs1 = std::make_shared<ConcreteObserver>("Observer1");
    auto obs2 = std::make_shared<ConcreteObserver>("Observer2");

    subject.attach(obs1);
    subject.attach(obs2);

    subject.notify("Event 1");

    obs1.reset();  // Destroy observer1

    subject.notify("Event 2");  // Only observer2 receives

    return 0;
}
```

## Practical Example 4: Pooling Objects

```cpp
#include <iostream>
#include <memory>
#include <vector>

class PooledObject {
private:
    static int instanceCount;
    int id;

public:
    PooledObject() : id(++instanceCount) {
        std::cout << "Object " << id << " created" << std::endl;
    }

    ~PooledObject() {
        std::cout << "Object " << id << " destroyed" << std::endl;
    }

    void use() {
        std::cout << "Using object " << id << std::endl;
    }
};

int PooledObject::instanceCount = 0;

class ObjectPool {
private:
    std::vector<std::unique_ptr<PooledObject>> pool;

public:
    std::shared_ptr<PooledObject> acquire() {
        if (pool.empty()) {
            // Create new object with custom deleter that returns to pool
            return std::shared_ptr<PooledObject>(
                new PooledObject(),
                [this](PooledObject* obj) {
                    std::cout << "Returning object to pool" << std::endl;
                    pool.push_back(std::unique_ptr<PooledObject>(obj));
                }
            );
        }

        // Reuse from pool
        std::cout << "Reusing object from pool" << std::endl;
        auto obj = std::shared_ptr<PooledObject>(
            pool.back().release(),
            [this](PooledObject* obj) {
                std::cout << "Returning object to pool" << std::endl;
                pool.push_back(std::unique_ptr<PooledObject>(obj));
            }
        );
        pool.pop_back();
        return obj;
    }

    size_t poolSize() const { return pool.size(); }
};

int main() {
    ObjectPool pool;

    {
        auto obj1 = pool.acquire();
        obj1->use();

        auto obj2 = pool.acquire();
        obj2->use();
    }  // Objects returned to pool

    std::cout << "Pool size: " << pool.poolSize() << std::endl;

    {
        auto obj3 = pool.acquire();  // Reused
        obj3->use();
    }

    std::cout << "Pool size: " << pool.poolSize() << std::endl;

    return 0;
}
```

## Exercises

### Exercise 1: Unique Pointer Practice
**Problem:** Create a class hierarchy and use unique_ptr for ownership.

**Solution:**
```cpp
#include <iostream>
#include <memory>
#include <vector>

class Animal {
protected:
    std::string name;

public:
    Animal(const std::string& n) : name(n) {}
    virtual ~Animal() = default;
    virtual void makeSound() const = 0;
};

class Dog : public Animal {
public:
    Dog(const std::string& n) : Animal(n) {}

    void makeSound() const override {
        std::cout << name << " says: Woof!" << std::endl;
    }
};

class Cat : public Animal {
public:
    Cat(const std::string& n) : Animal(n) {}

    void makeSound() const override {
        std::cout << name << " says: Meow!" << std::endl;
    }
};

int main() {
    std::vector<std::unique_ptr<Animal>> animals;

    animals.push_back(std::make_unique<Dog>("Buddy"));
    animals.push_back(std::make_unique<Cat>("Whiskers"));
    animals.push_back(std::make_unique<Dog>("Max"));

    for (const auto& animal : animals) {
        animal->makeSound();
    }

    return 0;
}
```

### Exercise 2: Shared Resource Manager
**Problem:** Implement a simple cache using shared_ptr.

**Solution:**
```cpp
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>

class CachedData {
private:
    std::string key;
    int value;

public:
    CachedData(const std::string& k, int v) : key(k), value(v) {
        std::cout << "Loading: " << key << std::endl;
    }

    ~CachedData() {
        std::cout << "Unloading: " << key << std::endl;
    }

    int getValue() const { return value; }
};

class Cache {
private:
    std::unordered_map<std::string, std::shared_ptr<CachedData>> data;

public:
    std::shared_ptr<CachedData> get(const std::string& key, int value) {
        auto it = data.find(key);
        if (it != data.end() && !it->second.expired()) {
            std::cout << "Cache hit: " << key << std::endl;
            return it->second;
        }

        std::cout << "Cache miss: " << key << std::endl;
        auto cached = std::make_shared<CachedData>(key, value);
        data[key] = cached;
        return cached;
    }
};

int main() {
    Cache cache;

    auto d1 = cache.get("data1", 100);
    auto d2 = cache.get("data1", 100);  // Cache hit

    std::cout << "Value: " << d1->getValue() << std::endl;

    return 0;
}
```

### Exercise 3: Weak Pointer Chain
**Problem:** Create a linked list using weak_ptr for back pointers.

**Solution:**
```cpp
#include <iostream>
#include <memory>

class Node {
private:
    int value;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;

public:
    Node(int v) : value(v) {
        std::cout << "Node(" << value << ") created" << std::endl;
    }

    ~Node() {
        std::cout << "Node(" << value << ") destroyed" << std::endl;
    }

    void setNext(std::shared_ptr<Node> n) {
        next = n;
        if (n) {
            n->prev = shared_from_this();
        }
    }

    int getValue() const { return value; }

    std::shared_ptr<Node> getNext() const { return next; }

    std::shared_ptr<Node> getPrev() const { return prev.lock(); }
};

// Need to inherit from enable_shared_from_this
class SmartNode : public std::enable_shared_from_this<SmartNode> {
private:
    int value;
    std::shared_ptr<SmartNode> next;
    std::weak_ptr<SmartNode> prev;

public:
    SmartNode(int v) : value(v) {}

    void setNext(std::shared_ptr<SmartNode> n) {
        next = n;
        if (n) {
            n->prev = shared_from_this();
        }
    }

    int getValue() const { return value; }
};

int main() {
    auto n1 = std::make_shared<SmartNode>(1);
    auto n2 = std::make_shared<SmartNode>(2);
    auto n3 = std::make_shared<SmartNode>(3);

    n1->setNext(n2);
    n2->setNext(n3);

    std::cout << "List created" << std::endl;

    return 0;
}  // All nodes properly destroyed
```

### Exercise 4: Smart Pointer Factory
**Problem:** Create a factory that returns different smart pointer types based on ownership requirements.

**Solution:**
```cpp
#include <iostream>
#include <memory>

class Product {
private:
    int id;

public:
    Product(int i) : id(i) {
        std::cout << "Product " << id << " created" << std::endl;
    }

    ~Product() {
        std::cout << "Product " << id << " destroyed" << std::endl;
    }

    int getId() const { return id; }
};

class ProductFactory {
public:
    // Exclusive ownership
    static std::unique_ptr<Product> createUnique(int id) {
        return std::make_unique<Product>(id);
    }

    // Shared ownership
    static std::shared_ptr<Product> createShared(int id) {
        return std::make_shared<Product>(id);
    }
};

int main() {
    auto uniqueProd = ProductFactory::createUnique(1);
    std::cout << "Unique product: " << uniqueProd->getId() << std::endl;

    auto sharedProd1 = ProductFactory::createShared(2);
    auto sharedProd2 = sharedProd1;  // Share ownership

    std::cout << "Shared product ref count: "
              << sharedProd1.use_count() << std::endl;

    return 0;
}
```

### Exercise 5: RAII File Handler
**Problem:** Use unique_ptr with custom deleter for file handling.

**Solution:**
```cpp
#include <iostream>
#include <memory>
#include <cstdio>

class FileHandle {
private:
    struct FileDeleter {
        void operator()(FILE* fp) const {
            if (fp) {
                std::cout << "Closing file" << std::endl;
                fclose(fp);
            }
        }
    };

    std::unique_ptr<FILE, FileDeleter> file;
    std::string filename;

public:
    FileHandle(const std::string& fname, const char* mode)
        : filename(fname), file(fopen(fname.c_str(), mode)) {
        if (!file) {
            throw std::runtime_error("Failed to open file");
        }
        std::cout << "Opened file: " << filename << std::endl;
    }

    void write(const std::string& data) {
        if (file) {
            fprintf(file.get(), "%s\n", data.c_str());
        }
    }

    bool isOpen() const {
        return file != nullptr;
    }
};

int main() {
    try {
        FileHandle fh("output.txt", "w");
        fh.write("Hello, smart pointers!");
        fh.write("RAII makes life easier");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "File automatically closed" << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- std::unique_ptr for exclusive ownership
- std::shared_ptr for shared ownership with reference counting
- std::weak_ptr to observe without owning
- Custom deleters for special cleanup needs
- RAII pattern for resource management
- Breaking circular dependencies

## Checklist

- [ ] Understand unique_ptr usage
- [ ] Can use make_unique and make_shared
- [ ] Know when to use shared_ptr
- [ ] Understand weak_ptr purpose
- [ ] Can implement custom deleters
- [ ] Completed all 5 exercises
- [ ] Understand reference counting
- [ ] Can break circular references
- [ ] Know RAII principles
- [ ] Ready to learn lambda expressions

## Next Lesson

In Lesson 56, we'll explore **Lambda Expressions** and functional programming features in C++.
