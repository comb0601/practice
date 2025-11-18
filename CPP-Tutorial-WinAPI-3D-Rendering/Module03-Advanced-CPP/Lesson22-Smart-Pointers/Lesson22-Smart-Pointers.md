# Lesson 22: Smart Pointers

## Table of Contents
1. [Introduction to Smart Pointers](#introduction-to-smart-pointers)
2. [Problems with Raw Pointers](#problems-with-raw-pointers)
3. [RAII Principle](#raii-principle)
4. [unique_ptr](#unique_ptr)
5. [shared_ptr](#shared_ptr)
6. [weak_ptr](#weak_ptr)
7. [make_unique and make_shared](#make_unique-and-make_shared)
8. [Custom Deleters](#custom-deleters)
9. [Smart Pointers and Arrays](#smart-pointers-and-arrays)
10. [Performance Considerations](#performance-considerations)
11. [Best Practices](#best-practices)
12. [Common Pitfalls](#common-pitfalls)

## Introduction to Smart Pointers

Smart pointers are objects that act like pointers but provide automatic memory management. They were introduced to solve the problems of manual memory management with raw pointers.

### What Are Smart Pointers?

A smart pointer is a class template that wraps a raw pointer and manages the lifetime of the dynamically allocated object it points to.

```cpp
// Raw pointer (manual management)
int* rawPtr = new int(42);
// ... use rawPtr ...
delete rawPtr;  // Must remember to delete!

// Smart pointer (automatic management)
std::unique_ptr<int> smartPtr = std::make_unique<int>(42);
// ... use smartPtr ...
// Automatically deleted when smartPtr goes out of scope
```

### Types of Smart Pointers

C++11 introduced three main types of smart pointers in the `<memory>` header:

1. **`std::unique_ptr`**: Exclusive ownership, non-copyable
2. **`std::shared_ptr`**: Shared ownership, reference counted
3. **`std::weak_ptr`**: Non-owning observer, breaks cycles

### Benefits

- **Automatic memory management**: No need for manual `delete`
- **Exception safety**: Resources cleaned up even if exceptions thrown
- **Clear ownership semantics**: Express intent in code
- **Prevent memory leaks**: Automatic cleanup
- **Prevent double deletion**: Smart pointers handle this
- **Prevent dangling pointers**: Reduced risk of use-after-free

## Problems with Raw Pointers

Before smart pointers, C++ programmers faced numerous issues with raw pointers.

### Memory Leaks

```cpp
void processData() {
    int* data = new int[1000];
    // ... processing ...
    if (errorCondition) {
        return;  // LEAK! Forgot to delete
    }
    delete[] data;
}
```

### Double Deletion

```cpp
int* ptr = new int(42);
delete ptr;
delete ptr;  // UNDEFINED BEHAVIOR!
```

### Dangling Pointers

```cpp
int* ptr = new int(42);
delete ptr;
*ptr = 100;  // UNDEFINED BEHAVIOR! Use after free
```

### Exception Safety

```cpp
void function() {
    Resource* res = new Resource();
    riskyOperation();  // If this throws, res leaks!
    delete res;
}
```

### Unclear Ownership

```cpp
void process(Widget* widget) {
    // Do I own this? Should I delete it?
    // Is it valid for how long?
}
```

### Smart Pointers Solve These

Smart pointers use RAII (Resource Acquisition Is Initialization) to automatically manage resources, eliminating these problems.

## RAII Principle

**RAII** (Resource Acquisition Is Initialization) is a programming idiom where resource lifetime is tied to object lifetime.

### Core Idea

1. **Acquire resources in constructor**
2. **Release resources in destructor**
3. **Rely on automatic object lifetime**

### Example

```cpp
class FileHandle {
    FILE* file;
public:
    FileHandle(const char* name) {
        file = fopen(name, "r");  // Acquire in constructor
        if (!file) throw std::runtime_error("Failed to open");
    }

    ~FileHandle() {
        if (file) fclose(file);  // Release in destructor
    }

    // Prevent copying
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
};

void processFile() {
    FileHandle file("data.txt");
    // ... use file ...
    // Automatically closed when file goes out of scope
    // Even if exception is thrown!
}
```

### Smart Pointers Use RAII

Smart pointers apply RAII to dynamic memory:
- Constructor acquires memory (takes ownership of pointer)
- Destructor releases memory (calls `delete`)
- Object lifetime determines memory lifetime

## unique_ptr

`std::unique_ptr` represents **exclusive ownership** of a dynamically allocated object.

### Key Characteristics

- **Exclusive ownership**: Only one `unique_ptr` can own an object
- **Move-only**: Cannot be copied, only moved
- **Zero overhead**: Same size as raw pointer, no reference counting
- **Automatic deletion**: Deletes object when destroyed

### Basic Usage

```cpp
#include <memory>

// Create unique_ptr
std::unique_ptr<int> ptr = std::make_unique<int>(42);

// Access value
int value = *ptr;

// Access through pointer
ptr.reset();  // Delete object and set to nullptr

// Check if owns object
if (ptr) {
    // ptr points to valid object
}
```

### Creation

```cpp
// Modern way (C++14)
auto ptr1 = std::make_unique<int>(42);
auto ptr2 = std::make_unique<Widget>("args", 123);

// Old way (C++11)
std::unique_ptr<int> ptr3(new int(42));

// From raw pointer
int* raw = new int(42);
std::unique_ptr<int> ptr4(raw);  // Takes ownership
```

### Ownership Transfer

```cpp
std::unique_ptr<int> ptr1 = std::make_unique<int>(42);

// std::unique_ptr<int> ptr2 = ptr1;  // ERROR: Can't copy

std::unique_ptr<int> ptr2 = std::move(ptr1);  // OK: Move ownership

// Now ptr1 is nullptr, ptr2 owns the object
```

### Member Functions

```cpp
std::unique_ptr<int> ptr = std::make_unique<int>(42);

// Get raw pointer (doesn't release ownership)
int* raw = ptr.get();

// Release ownership (caller responsible for deletion)
int* released = ptr.release();
delete released;

// Replace owned object
ptr.reset(new int(100));

// Delete object and set to nullptr
ptr.reset();

// Swap two unique_ptrs
std::unique_ptr<int> other = std::make_unique<int>(99);
ptr.swap(other);
```

### Passing unique_ptr

```cpp
// By value (takes ownership)
void takeOwnership(std::unique_ptr<Widget> ptr) {
    // Function owns ptr, deletes on return
}

// By const reference (borrows, doesn't take ownership)
void borrow(const std::unique_ptr<Widget>& ptr) {
    // Function can use ptr but doesn't own it
}

// By raw pointer (when ownership is irrelevant)
void observe(Widget* ptr) {
    // Function just uses ptr
}

// Usage
auto widget = std::make_unique<Widget>();
takeOwnership(std::move(widget));  // Transfer ownership
// widget is now nullptr

auto widget2 = std::make_unique<Widget>();
borrow(widget2);  // widget2 still valid

auto widget3 = std::make_unique<Widget>();
observe(widget3.get());  // widget3 still valid
```

### Returning unique_ptr

```cpp
std::unique_ptr<Widget> createWidget() {
    auto widget = std::make_unique<Widget>();
    // ... initialize widget ...
    return widget;  // Automatically moved, no std::move needed
}

// Usage
auto myWidget = createWidget();  // Receives ownership
```

### unique_ptr with Custom Types

```cpp
class MyClass {
public:
    MyClass() { std::cout << "MyClass created\n"; }
    ~MyClass() { std::cout << "MyClass destroyed\n"; }
    void doSomething() { std::cout << "Doing something\n"; }
};

void example() {
    auto obj = std::make_unique<MyClass>();
    obj->doSomething();
    // obj automatically deleted here
}
```

## shared_ptr

`std::shared_ptr` implements **shared ownership** through reference counting.

### Key Characteristics

- **Shared ownership**: Multiple `shared_ptr`s can own same object
- **Reference counting**: Tracks number of owners
- **Automatic deletion**: Deletes object when last owner destroyed
- **Thread-safe counting**: Reference count is atomic
- **Overhead**: Two pointers size (object + control block)

### Basic Usage

```cpp
#include <memory>

// Create shared_ptr
std::shared_ptr<int> ptr1 = std::make_shared<int>(42);

// Copy creates another owner
std::shared_ptr<int> ptr2 = ptr1;

// Check reference count
long count = ptr1.use_count();  // Returns 2

// Both ptr1 and ptr2 are destroyed before object is deleted
```

### Reference Counting

```cpp
{
    auto ptr1 = std::make_shared<int>(42);
    std::cout << "Count: " << ptr1.use_count() << "\n";  // 1

    {
        auto ptr2 = ptr1;  // Copy, increment count
        std::cout << "Count: " << ptr1.use_count() << "\n";  // 2

        auto ptr3 = ptr1;  // Another copy
        std::cout << "Count: " << ptr1.use_count() << "\n";  // 3
    }  // ptr2 and ptr3 destroyed, count decrements

    std::cout << "Count: " << ptr1.use_count() << "\n";  // 1
}  // ptr1 destroyed, count becomes 0, object deleted
```

### Creation

```cpp
// Preferred way (efficient, single allocation)
auto ptr1 = std::make_shared<Widget>("arg", 42);

// Alternative (two allocations)
std::shared_ptr<Widget> ptr2(new Widget("arg", 42));

// From unique_ptr (transfers ownership)
std::unique_ptr<Widget> uptr = std::make_unique<Widget>();
std::shared_ptr<Widget> sptr = std::move(uptr);  // uptr becomes nullptr
```

### Member Functions

```cpp
std::shared_ptr<int> ptr = std::make_shared<int>(42);

// Get raw pointer
int* raw = ptr.get();

// Reset (decrement count, possibly delete)
ptr.reset();

// Reset with new object
ptr.reset(new int(100));

// Get reference count
long count = ptr.use_count();

// Check if unique owner
bool unique = ptr.unique();  // true if use_count() == 1

// Check if owns object
if (ptr) {
    // ptr points to valid object
}
```

### Passing shared_ptr

```cpp
// By value (shares ownership, increment/decrement count)
void shareOwnership(std::shared_ptr<Widget> ptr) {
    // Reference count incremented on entry, decremented on exit
}

// By const reference (observes, no count change)
void observe(const std::shared_ptr<Widget>& ptr) {
    // No reference count change
}

// By raw pointer (when ownership is irrelevant)
void useWidget(Widget* ptr) {
    // No smart pointer overhead
}

// Usage
auto widget = std::make_shared<Widget>();
shareOwnership(widget);  // Count temporarily increases
observe(widget);  // No count change
useWidget(widget.get());  // No count change
```

### Aliasing Constructor

```cpp
struct Pair {
    int first;
    int second;
};

auto pair = std::make_shared<Pair>();
pair->first = 10;
pair->second = 20;

// Aliasing: shared_ptr to member that shares ownership of whole
std::shared_ptr<int> firstPtr(pair, &pair->first);

// firstPtr points to first, but keeps entire Pair alive
// Pair deleted only when both pair and firstPtr destroyed
```

## weak_ptr

`std::weak_ptr` is a non-owning observer of an object managed by `shared_ptr`.

### Key Characteristics

- **Non-owning**: Doesn't affect reference count
- **Breaks cycles**: Prevents circular references
- **Safe observer**: Can detect if object still exists
- **Converts to shared_ptr**: To access object

### Purpose

The main purpose of `weak_ptr` is to break reference cycles that would cause memory leaks with `shared_ptr`.

### Basic Usage

```cpp
std::shared_ptr<int> sptr = std::make_shared<int>(42);
std::weak_ptr<int> wptr = sptr;  // wptr observes object

// weak_ptr doesn't affect reference count
std::cout << sptr.use_count() << "\n";  // 1

// To access object, convert to shared_ptr
if (auto locked = wptr.lock()) {
    // locked is shared_ptr, object exists
    std::cout << *locked << "\n";
}

// Check if object still exists
if (wptr.expired()) {
    std::cout << "Object deleted\n";
}
```

### Breaking Circular References

```cpp
// WITHOUT weak_ptr (MEMORY LEAK)
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;  // Circular reference!
};

auto node1 = std::make_shared<Node>();
auto node2 = std::make_shared<Node>();
node1->next = node2;
node2->prev = node1;  // Cycle! Both never deleted

// WITH weak_ptr (NO LEAK)
struct NodeFixed {
    std::shared_ptr<NodeFixed> next;
    std::weak_ptr<NodeFixed> prev;  // Weak breaks cycle
};

auto n1 = std::make_shared<NodeFixed>();
auto n2 = std::make_shared<NodeFixed>();
n1->next = n2;
n2->prev = n1;  // No cycle, proper cleanup
```

### Observer Pattern

```cpp
class Subject;

class Observer {
private:
    std::weak_ptr<Subject> subject;  // Doesn't keep Subject alive

public:
    void setSubject(std::weak_ptr<Subject> s) {
        subject = s;
    }

    void update() {
        if (auto s = subject.lock()) {
            // Subject still exists, use it
        } else {
            // Subject has been deleted
        }
    }
};
```

### Member Functions

```cpp
std::shared_ptr<int> sptr = std::make_shared<int>(42);
std::weak_ptr<int> wptr = sptr;

// Lock: convert to shared_ptr (nullptr if object deleted)
std::shared_ptr<int> locked = wptr.lock();

// Expired: check if object deleted
bool gone = wptr.expired();

// Use count: get current shared_ptr count
long count = wptr.use_count();

// Reset: stop observing
wptr.reset();
```

## make_unique and make_shared

`std::make_unique` and `std::make_shared` are factory functions that create smart pointers.

### make_unique (C++14)

```cpp
// Preferred
auto ptr = std::make_unique<Widget>(arg1, arg2);

// Instead of
std::unique_ptr<Widget> ptr(new Widget(arg1, arg2));
```

**Advantages:**
- Exception safe
- Less typing
- Prevents raw `new` in code
- More readable

### make_shared

```cpp
// Preferred
auto ptr = std::make_shared<Widget>(arg1, arg2);

// Instead of
std::shared_ptr<Widget> ptr(new Widget(arg1, arg2));
```

**Advantages:**
- Single allocation (object + control block together)
- More efficient (fewer allocations)
- Better cache locality
- Exception safe

### Exception Safety

```cpp
// EXCEPTION UNSAFE
void function(std::unique_ptr<Widget>(new Widget()),
              riskyFunction());

// If riskyFunction() throws before unique_ptr constructed,
// Widget leaks!

// EXCEPTION SAFE
void function(std::make_unique<Widget>(),
              riskyFunction());

// Widget lifetime tied to unique_ptr, no leak
```

### Performance

```cpp
// Two allocations: one for object, one for control block
std::shared_ptr<Widget> ptr1(new Widget());

// One allocation: object and control block together
auto ptr2 = std::make_shared<Widget>();
```

### When NOT to Use make_shared

1. **Custom deleters**: `make_shared` doesn't support custom deleters
   ```cpp
   std::shared_ptr<FILE> file(fopen("file.txt", "r"), fclose);
   ```

2. **Braced initializers**: Ambiguity with initializer lists
   ```cpp
   auto vec = std::make_shared<std::vector<int>>(10, 20);  // OK
   // vs
   std::shared_ptr<std::vector<int>> vec2(new std::vector<int>{10, 20});
   ```

3. **Large objects with weak_ptr**: Control block not freed until all weak_ptrs gone
   ```cpp
   auto large = std::make_shared<LargeObject>();
   std::weak_ptr<LargeObject> weak = large;
   large.reset();  // Object deleted, but control block remains for weak
   ```

## Custom Deleters

Smart pointers can use custom deleters for resources that aren't deleted with `delete`.

### With unique_ptr

```cpp
// File handle deleter
auto fileDeleter = [](FILE* f) {
    if (f) fclose(f);
    std::cout << "File closed\n";
};

std::unique_ptr<FILE, decltype(fileDeleter)> file(
    fopen("data.txt", "r"),
    fileDeleter
);

// Lambda deleter (C++17 with auto)
auto file2 = std::unique_ptr<FILE, decltype([](FILE* f) {
    if (f) fclose(f);
})>(fopen("data.txt", "r"));
```

### With shared_ptr

```cpp
// shared_ptr deleter is part of constructor, not type
std::shared_ptr<FILE> file(
    fopen("data.txt", "r"),
    [](FILE* f) {
        if (f) fclose(f);
        std::cout << "File closed\n";
    }
);

// Works with function pointers too
std::shared_ptr<FILE> file2(fopen("data.txt", "r"), fclose);
```

### Custom Deleters for Arrays (Pre-C++17)

```cpp
// unique_ptr for array (C++11)
std::unique_ptr<int[]> arr(new int[10]);  // Uses delete[]

// shared_ptr for array (needs custom deleter before C++17)
std::shared_ptr<int> arr2(new int[10], [](int* p) { delete[] p; });

// C++17: shared_ptr for arrays
std::shared_ptr<int[]> arr3(new int[10]);  // Automatic delete[]
```

### Common Custom Deleters

```cpp
// SDL resource
std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>
    window(SDL_CreateWindow(...), SDL_DestroyWindow);

// OpenGL buffer
auto glDeleter = [](GLuint* buf) {
    glDeleteBuffers(1, buf);
    delete buf;
};
std::unique_ptr<GLuint, decltype(glDeleter)> buffer(
    new GLuint,
    glDeleter
);

// Database connection
std::shared_ptr<Database> db(
    openDatabase(),
    [](Database* d) { closeDatabase(d); }
);
```

## Smart Pointers and Arrays

### unique_ptr with Arrays

```cpp
// Array specialization (uses delete[])
std::unique_ptr<int[]> arr = std::make_unique<int[]>(10);

// Access elements
arr[0] = 42;
arr[5] = 100;

// Bounds checking not provided (like raw arrays)
```

### shared_ptr with Arrays (C++17)

```cpp
// C++17: array specialization
std::shared_ptr<int[]> arr = std::shared_ptr<int[]>(new int[10]);

// Access elements
arr[0] = 42;

// C++20: make_shared for arrays
auto arr2 = std::make_shared<int[]>(10);
```

### Prefer std::vector

For most cases, prefer `std::vector` over smart pointer arrays:

```cpp
// Better than unique_ptr<int[]>
std::vector<int> vec(10);

// Advantages:
// - Bounds checking (at())
// - Size tracking
// - Dynamic resizing
// - Iterator support
// - Standard algorithms
```

## Performance Considerations

### unique_ptr Performance

- **Zero overhead**: Same size as raw pointer
- **No reference counting**: No atomic operations
- **Inlined**: Most operations can be inlined
- **Fast**: Effectively same as raw pointer

```cpp
sizeof(int*) == sizeof(std::unique_ptr<int>);  // Usually true
```

### shared_ptr Performance

- **Overhead**: Two pointers (object + control block)
- **Reference counting**: Atomic increment/decrement
- **Allocation**: Control block allocation
- **Thread-safe**: Atomic operations have cost

```cpp
sizeof(std::shared_ptr<int>) == 2 * sizeof(void*);  // Typically
```

### Comparison

```cpp
// Fastest: Raw pointer (but manual management)
Widget* raw = new Widget();

// Fast: unique_ptr (minimal overhead)
auto unique = std::make_unique<Widget>();

// Moderate: shared_ptr (reference counting overhead)
auto shared = std::make_shared<Widget>();

// For single ownership, prefer unique_ptr
// For shared ownership, use shared_ptr
// Avoid raw pointers for ownership
```

### Optimization Tips

1. **Prefer unique_ptr** for single ownership
2. **Use make_shared** to reduce allocations
3. **Pass by const reference** to avoid count changes
4. **Use raw pointers** for non-owning parameters
5. **Avoid excessive copying** of shared_ptr
6. **Reserve space** in containers of smart pointers

## Best Practices

### 1. Prefer Smart Pointers Over Raw Pointers

```cpp
// BAD
Widget* widget = new Widget();
// ... risk of leak ...
delete widget;

// GOOD
auto widget = std::make_unique<Widget>();
// Automatic cleanup
```

### 2. Use make_unique and make_shared

```cpp
// GOOD
auto ptr = std::make_unique<Widget>();
auto sptr = std::make_shared<Widget>();

// AVOID
std::unique_ptr<Widget> ptr(new Widget());
```

### 3. Express Ownership with Type

```cpp
// Unique ownership
std::unique_ptr<Widget> widget;

// Shared ownership
std::shared_ptr<Widget> widget;

// Non-owning observer
std::weak_ptr<Widget> widget;

// Non-owning, function parameter
void process(Widget* widget);
```

### 4. Return by Value

```cpp
std::unique_ptr<Widget> createWidget() {
    return std::make_unique<Widget>();
    // Automatically moved, efficient
}
```

### 5. Pass Smart Pointers Appropriately

```cpp
// Sink (takes ownership)
void takeOwnership(std::unique_ptr<Widget> widget);

// Observer (doesn't take ownership)
void useWidget(Widget* widget);

// Shared ownership
void shareWidget(std::shared_ptr<Widget> widget);

// Observe shared_ptr
void observeShared(const std::shared_ptr<Widget>& widget);
```

### 6. Use unique_ptr by Default

Use `unique_ptr` unless you need shared ownership:
```cpp
// Start with unique_ptr
auto widget = std::make_unique<Widget>();

// Convert to shared_ptr if needed later
std::shared_ptr<Widget> shared = std::move(widget);
```

### 7. Avoid Cycles with weak_ptr

```cpp
struct Node {
    std::shared_ptr<Node> next;      // Owns next
    std::weak_ptr<Node> prev;        // Observes previous
};
```

### 8. Don't Mix Raw and Smart Pointers

```cpp
// BAD
Widget* raw = new Widget();
std::unique_ptr<Widget> smart(raw);  // Who owns it?

// GOOD
auto smart = std::make_unique<Widget>();
```

## Common Pitfalls

### 1. Creating shared_ptr from Same Raw Pointer Twice

```cpp
// BAD: Double deletion!
Widget* raw = new Widget();
std::shared_ptr<Widget> ptr1(raw);
std::shared_ptr<Widget> ptr2(raw);  // Second control block!
// Both will try to delete raw
```

### 2. Circular References

```cpp
// MEMORY LEAK
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;  // Cycle!
};

// FIX: Use weak_ptr
struct NodeFixed {
    std::shared_ptr<NodeFixed> next;
    std::weak_ptr<NodeFixed> prev;  // Breaks cycle
};
```

### 3. Forgetting to Use make_unique/make_shared

```cpp
// Less efficient, exception unsafe
std::shared_ptr<Widget> ptr(new Widget());

// Better
auto ptr = std::make_shared<Widget>();
```

### 4. Passing Ownership When Not Needed

```cpp
// BAD: Unnecessary ownership transfer
void process(std::shared_ptr<Widget> widget);

// GOOD: Just observe
void process(Widget* widget);
void process(const Widget& widget);
```

### 5. Using get() Incorrectly

```cpp
auto ptr = std::make_unique<Widget>();

// BAD: Creating another unique_ptr from same raw pointer
std::unique_ptr<Widget> ptr2(ptr.get());  // Double deletion!

// GOOD: Move ownership
std::unique_ptr<Widget> ptr2 = std::move(ptr);
```

### 6. Dangling weak_ptr Dereference

```cpp
std::weak_ptr<Widget> weak;
{
    auto shared = std::make_shared<Widget>();
    weak = shared;
}  // shared destroyed, object deleted

// BAD: Using weak after object deleted
auto locked = weak.lock();  // Returns nullptr
// Must check before using!
```

### 7. Inefficient shared_ptr Copying

```cpp
// BAD: Copies shared_ptr many times (atomic ops!)
void process(std::shared_ptr<Widget> w);  // Copy on call
std::shared_ptr<Widget> widget = w;        // Copy in function

// GOOD: Pass by const reference
void process(const std::shared_ptr<Widget>& w);
```

## Summary

Smart pointers are essential for modern C++ memory management:

### Quick Reference

| Pointer Type | Ownership | Copyable | Use Case |
|-------------|-----------|----------|----------|
| `unique_ptr` | Exclusive | No | Single owner, move-only |
| `shared_ptr` | Shared | Yes | Multiple owners |
| `weak_ptr` | None | Yes | Observer, break cycles |
| Raw pointer | Depends | Yes | Non-owning observer |

### When to Use

- **unique_ptr**: Default choice for single ownership
- **shared_ptr**: When multiple owners needed
- **weak_ptr**: To observe shared_ptr without owning
- **Raw pointer**: For non-owning function parameters

### Key Benefits

1. Automatic memory management
2. Exception safety
3. Clear ownership semantics
4. Prevention of memory leaks
5. Prevention of dangling pointers
6. Thread-safe reference counting (shared_ptr)

### Remember

- Prefer `make_unique` and `make_shared`
- Express ownership in types
- Avoid raw `new`/`delete` in modern C++
- Use `unique_ptr` by default
- Use `weak_ptr` to break cycles
- Pass by raw pointer for observers
- Smart pointers are essential for RAII

Smart pointers, combined with RAII, make C++ memory management safe, clear, and efficient!

## Exercises

1. Convert legacy code using raw pointers to smart pointers
2. Implement a resource pool using smart pointers
3. Create a tree structure avoiding memory leaks
4. Build an observer pattern with weak_ptr
5. Optimize code by choosing appropriate smart pointer type
6. Implement custom deleters for various resource types
7. Profile unique_ptr vs shared_ptr performance
8. Design a cache with weak_ptr cleanup
9. Implement a smart pointer from scratch
10. Debug circular reference memory leak

## Further Reading

- "Effective Modern C++" by Scott Meyers (Items 18-22)
- C++ Core Guidelines on smart pointers
- cppreference.com documentation
- "C++ Move Semantics" by Nicolai Josuttis
- CppCon talks on memory management

---

**Next Lesson**: [Lesson 23: Lambda Expressions](../Lesson23-Lambda-Expressions/Lesson23-Lambda-Expressions.md)

**Previous Lesson**: [Lesson 21: Move Semantics](../Lesson21-Move-Semantics/Lesson21-Move-Semantics.md)

**Course Home**: [C++ Tutorial - WinAPI 3D Rendering](../../README.md)
