# Lesson 70: Smart Pointers Deep Dive

**Duration**: 6 hours
**Difficulty**: Intermediate

Smart pointers automatically manage memory, preventing leaks and dangling pointers.

## unique_ptr:

```cpp
#include <memory>
#include <iostream>

class Resource {
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource released\n"; }
    void use() { std::cout << "Using resource\n"; }
};

int main()
{
    // Create unique_ptr
    std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>();
    ptr1->use();

    // Cannot copy, only move
    // std::unique_ptr<Resource> ptr2 = ptr1;  // Error!
    std::unique_ptr<Resource> ptr2 = std::move(ptr1);

    if (!ptr1) {
        std::cout << "ptr1 is null\n";
    }

    return 0;  // Resource automatically deleted
}
```

## shared_ptr:

```cpp
#include <memory>
#include <iostream>

int main()
{
    std::shared_ptr<int> ptr1 = std::make_shared<int>(42);
    std::cout << "Count: " << ptr1.use_count() << "\n";  // 1

    {
        std::shared_ptr<int> ptr2 = ptr1;  // Copy allowed
        std::cout << "Count: " << ptr1.use_count() << "\n";  // 2
        std::cout << "Value: " << *ptr2 << "\n";
    }  // ptr2 destroyed

    std::cout << "Count: " << ptr1.use_count() << "\n";  // 1

    return 0;  // Last shared_ptr destroyed, memory freed
}
```

## weak_ptr:

```cpp
#include <memory>
#include <iostream>

int main()
{
    std::shared_ptr<int> shared = std::make_shared<int>(100);
    std::weak_ptr<int> weak = shared;  // Doesn't increase count

    std::cout << "Shared count: " << shared.use_count() << "\n";  // 1

    // Access through weak_ptr
    if (auto temp = weak.lock()) {
        std::cout << "Value: " << *temp << "\n";
    }

    shared.reset();  // Release shared_ptr

    if (weak.expired()) {
        std::cout << "Object deleted\n";
    }

    return 0;
}
```

## Custom Deleters:

```cpp
#include <memory>
#include <iostream>

void customDeleter(int* p) {
    std::cout << "Custom deleter called\n";
    delete p;
}

int main()
{
    std::unique_ptr<int, decltype(&customDeleter)> ptr(new int(42), customDeleter);

    std::shared_ptr<int> ptr2(new int(100), customDeleter);

    return 0;
}
```

**Key Takeaways:**
- unique_ptr for exclusive ownership
- shared_ptr for shared ownership
- weak_ptr to break circular references
- Always use make_unique/make_shared
- Smart pointers prevent memory leaks
- Custom deleters for special cleanup
