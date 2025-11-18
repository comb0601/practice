# Lesson 30: Smart Pointers Introduction

**Duration**: 6 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Smart Pointers
2. unique_ptr Basics
3. shared_ptr Basics
4. weak_ptr Basics
5. Choosing the Right Smart Pointer
6. Smart Pointers vs Raw Pointers
7. Best Practices
8. Exercises and Projects

---

## 1. Introduction to Smart Pointers

### What are Smart Pointers?

Smart pointers are template classes that manage dynamic memory automatically, preventing memory leaks and dangling pointers.

```cpp
#include <iostream>
#include <memory>

int main()
{
    // Raw pointer (manual management)
    int* raw = new int(42);
    std::cout << "Raw pointer: " << *raw << "\n";
    delete raw;  // Must remember to delete!

    // Smart pointer (automatic management)
    std::unique_ptr<int> smart(new int(42));
    std::cout << "Smart pointer: " << *smart << "\n";
    // Automatically deleted when smart goes out of scope!

    return 0;
}
```

---

## 2. unique_ptr Basics

### Exclusive Ownership:

```cpp
#include <iostream>
#include <memory>

int main()
{
    // Create unique_ptr
    std::unique_ptr<int> ptr1(new int(100));

    // Modern way (preferred)
    auto ptr2 = std::make_unique<int>(200);

    std::cout << "ptr1: " << *ptr1 << "\n";
    std::cout << "ptr2: " << *ptr2 << "\n";

    // Cannot copy unique_ptr
    // std::unique_ptr<int> ptr3 = ptr1;  // ERROR!

    // Can move ownership
    std::unique_ptr<int> ptr3 = std::move(ptr1);

    if (ptr1 == nullptr)
    {
        std::cout << "ptr1 is now null\n";
    }

    std::cout << "ptr3: " << *ptr3 << "\n";

    return 0;
}
```

### unique_ptr with Arrays:

```cpp
#include <iostream>
#include <memory>

int main()
{
    // Array of integers
    std::unique_ptr<int[]> arr(new int[5]);

    // Initialize
    for (int i = 0; i < 5; i++)
    {
        arr[i] = i * 10;
    }

    // Access
    for (int i = 0; i < 5; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Automatically calls delete[]

    return 0;
}
```

---

## 3. shared_ptr Basics

### Shared Ownership:

```cpp
#include <iostream>
#include <memory>

int main()
{
    // Create shared_ptr
    std::shared_ptr<int> ptr1 = std::make_shared<int>(42);

    std::cout << "ptr1: " << *ptr1 << "\n";
    std::cout << "Reference count: " << ptr1.use_count() << "\n\n";

    {
        // Share ownership
        std::shared_ptr<int> ptr2 = ptr1;

        std::cout << "ptr1: " << *ptr1 << "\n";
        std::cout << "ptr2: " << *ptr2 << "\n";
        std::cout << "Reference count: " << ptr1.use_count() << "\n\n";

    }  // ptr2 destroyed, but memory not freed yet

    std::cout << "After ptr2 destroyed:\n";
    std::cout << "Reference count: " << ptr1.use_count() << "\n";

    // Memory freed when last shared_ptr is destroyed

    return 0;
}
```

### Circular Reference Problem:

```cpp
#include <iostream>
#include <memory>

class Node
{
public:
    int value;
    std::shared_ptr<Node> next;

    Node(int v) : value(v)
    {
        std::cout << "Node " << value << " created\n";
    }

    ~Node()
    {
        std::cout << "Node " << value << " destroyed\n";
    }
};

int main()
{
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);

    node1->next = node2;
    node2->next = node1;  // Circular reference!

    // Memory leak! Neither will be destroyed

    return 0;
}
```

---

## 4. weak_ptr Basics

### Breaking Circular References:

```cpp
#include <iostream>
#include <memory>

class Node
{
public:
    int value;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // Use weak_ptr to break cycle

    Node(int v) : value(v)
    {
        std::cout << "Node " << value << " created\n";
    }

    ~Node()
    {
        std::cout << "Node " << value << " destroyed\n";
    }
};

int main()
{
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);

    node1->next = node2;
    node2->prev = node1;  // weak_ptr doesn't increase reference count

    std::cout << "Reference count of node1: " << node1.use_count() << "\n";
    std::cout << "Reference count of node2: " << node2.use_count() << "\n";

    // Both nodes will be properly destroyed

    return 0;
}
```

### Using weak_ptr:

```cpp
#include <iostream>
#include <memory>

int main()
{
    std::weak_ptr<int> weak;

    {
        auto shared = std::make_shared<int>(42);
        weak = shared;

        // Check if still valid
        if (auto locked = weak.lock())
        {
            std::cout << "Value: " << *locked << "\n";
        }

        std::cout << "Reference count: " << shared.use_count() << "\n";
    }

    // shared destroyed, memory freed

    // Check if still valid
    if (auto locked = weak.lock())
    {
        std::cout << "Value: " << *locked << "\n";
    }
    else
    {
        std::cout << "Object no longer exists\n";
    }

    return 0;
}
```

---

## 5. Choosing the Right Smart Pointer

### Decision Guide:

```cpp
#include <iostream>
#include <memory>
#include <vector>

class Resource
{
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource released\n"; }
};

int main()
{
    // Use unique_ptr for exclusive ownership
    std::unique_ptr<Resource> exclusive(new Resource());

    // Use shared_ptr for shared ownership
    std::vector<std::shared_ptr<Resource>> shared_resources;
    auto resource = std::make_shared<Resource>();
    shared_resources.push_back(resource);
    shared_resources.push_back(resource);  // Multiple owners

    std::cout << "Reference count: " << resource.use_count() << "\n";

    // Use weak_ptr to observe without owning
    std::weak_ptr<Resource> observer = resource;

    return 0;
}
```

---

## 6. Smart Pointers vs Raw Pointers

### Comparison:

```cpp
#include <iostream>
#include <memory>

// ❌ Raw pointer version
void rawPointerExample()
{
    int* ptr = new int(42);

    // Risk of forgetting to delete
    // Risk of double delete
    // Risk of using after delete

    delete ptr;
}

// ✅ Smart pointer version
void smartPointerExample()
{
    auto ptr = std::make_unique<int>(42);

    // Automatic cleanup
    // Cannot be copied accidentally
    // Type-safe
}

int main()
{
    rawPointerExample();
    smartPointerExample();

    return 0;
}
```

---

## 7. Best Practices

### Modern C++ Memory Management:

```cpp
#include <iostream>
#include <memory>
#include <vector>

class DataManager
{
private:
    std::vector<std::unique_ptr<int>> data;

public:
    void addValue(int value)
    {
        data.push_back(std::make_unique<int>(value));
    }

    void display() const
    {
        for (const auto& ptr : data)
        {
            std::cout << *ptr << " ";
        }
        std::cout << "\n";
    }

    // No need for destructor - automatic cleanup!
};

int main()
{
    DataManager manager;

    manager.addValue(10);
    manager.addValue(20);
    manager.addValue(30);

    manager.display();

    // All memory automatically freed

    return 0;
}
```

---

## 8. Exercises and Projects

### Practice Project: Resource Manager

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class File
{
private:
    std::string name;

public:
    File(const std::string& n) : name(n)
    {
        std::cout << "Opening file: " << name << "\n";
    }

    ~File()
    {
        std::cout << "Closing file: " << name << "\n";
    }

    void write(const std::string& data)
    {
        std::cout << "Writing to " << name << ": " << data << "\n";
    }
};

class FileManager
{
private:
    std::vector<std::shared_ptr<File>> openFiles;

public:
    std::shared_ptr<File> openFile(const std::string& name)
    {
        auto file = std::make_shared<File>(name);
        openFiles.push_back(file);
        return file;
    }

    void displayOpenFiles() const
    {
        std::cout << "\nOpen files:\n";
        for (const auto& file : openFiles)
        {
            std::cout << "  Reference count: " << file.use_count() << "\n";
        }
    }
};

int main()
{
    FileManager manager;

    {
        auto file1 = manager.openFile("data.txt");
        auto file2 = manager.openFile("config.txt");

        file1->write("Hello");
        file2->write("Settings");

        manager.displayOpenFiles();

    }  // file1 and file2 go out of scope

    std::cout << "\nAfter local scope:\n";
    manager.displayOpenFiles();

    // All files automatically closed

    return 0;
}
```

---

## Key Takeaways

1. Smart pointers manage memory automatically
2. unique_ptr for exclusive ownership
3. shared_ptr for shared ownership
4. weak_ptr to break circular references
5. Prefer make_unique and make_shared
6. Smart pointers prevent memory leaks
7. Choose based on ownership semantics
8. Modern C++ prefers smart pointers over raw pointers

---

## Summary Checklist

Congratulations on completing Part 1: C++ Fundamentals!

Before moving forward, ensure you can:
- [ ] Use unique_ptr for exclusive ownership
- [ ] Use shared_ptr for shared ownership
- [ ] Use weak_ptr to break cycles
- [ ] Choose appropriate smart pointer type
- [ ] Understand reference counting
- [ ] Avoid circular references
- [ ] Follow modern C++ best practices
- [ ] Write memory-safe code

---

## Course Completion

**Congratulations!** You've completed all 30 lessons of C++ Fundamentals!

You've mastered:
- Variables, data types, and operators
- Control flow and loops
- Functions and recursion
- Arrays and strings
- Pointers and references
- Memory management
- Modern C++ features

**Next Steps:**
- Part 2: Object-Oriented Programming
- Part 3: Advanced C++ Features
- Part 4: Standard Template Library
- Real-world projects and applications

**Keep coding and building amazing things with C++!**
