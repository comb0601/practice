# Lesson 21: Move Semantics - Code Examples

This directory contains 15 comprehensive C++ programs demonstrating move semantics, one of the most important features introduced in C++11.

## Files Overview

### Basic Concepts
1. **01_lvalue_rvalue_basics.cpp** - Understanding lvalues and rvalues
   - Value categories and their differences
   - Function overload resolution
   - Reference binding rules
   - Lifetime extension

2. **02_rvalue_references.cpp** - Rvalue references (&&)
   - Binding rvalue references
   - Named rvalue references are lvalues
   - Forwarding references vs rvalue references
   - Reference collapsing

3. **03_move_constructor.cpp** - Move constructor implementation
   - Basic move constructor syntax
   - Performance comparison with copy
   - Return value optimization (RVO)
   - STL container integration

4. **04_move_assignment.cpp** - Move assignment operator
   - Move assignment implementation
   - Self-assignment check
   - Chain assignment
   - Moved-from state management

5. **05_std_move.cpp** - Understanding std::move
   - What std::move actually does (it's a cast!)
   - When to use std::move
   - Common mistakes with std::move
   - std::move with const objects

6. **06_perfect_forwarding.cpp** - Perfect forwarding with std::forward
   - Forwarding references (universal references)
   - Preserving value categories
   - std::forward implementation
   - Variadic perfect forwarding

### Advanced Implementation
7. **07_rule_of_five.cpp** - The Rule of Five
   - All five special member functions
   - Rule of Zero (preferred approach)
   - Move-only classes
   - Explicit default and delete

8. **08_move_stl_containers.cpp** - Move semantics with STL
   - Vector operations (push_back vs emplace_back)
   - Moving entire containers
   - std::move algorithm
   - Unique_ptr in containers

9. **09_performance_benchmark.cpp** - Performance measurements
   - Copy vs move benchmarks
   - Small String Optimization (SSO)
   - Large object performance
   - Return value optimization

10. **10_common_pitfalls.cpp** - Common mistakes
    - Using moved-from objects
    - Moving const objects
    - std::move on return values
    - Forgetting noexcept

### Practical Applications
11. **11_move_only_types.cpp** - Move-only types
    - unique_ptr and custom move-only classes
    - File handles and connections
    - Move-only types in containers
    - Ownership transfer patterns

12. **12_real_world_examples.cpp** - Real-world scenarios
    - Image processing pipelines
    - Document with history
    - Network message building
    - Database query results
    - Task queues and resource pools

13. **13_string_optimization.cpp** - String optimization
    - Small String Optimization (SSO)
    - Move performance for strings
    - String concatenation techniques
    - Building large strings efficiently

14. **14_custom_move_semantics.cpp** - Custom implementations
    - Dynamic arrays with move
    - Matrix class (2D arrays)
    - Linked list with move
    - Multiple resource management

15. **15_advanced_patterns.cpp** - Advanced patterns
    - Copy-and-swap idiom
    - Move-if-noexcept
    - Sink arguments
    - Ref-qualifiers (&& on member functions)
    - Builder pattern with move

## Compilation

All files can be compiled with:
```bash
g++ -std=c++17 -Wall -O2 filename.cpp -o filename
```

For performance benchmarks, use optimizations:
```bash
g++ -std=c++17 -Wall -O3 -DNDEBUG filename.cpp -o filename
```

## Key Concepts Covered

### Value Categories
- **Lvalues**: Named objects with persistent addresses
- **Rvalues**: Temporary objects about to be destroyed
- **Xvalues**: Expiring values (result of std::move)
- **Prvalues**: Pure rvalues (literals, temporaries)

### Move Operations
- **Move Constructor**: `T(T&& other) noexcept`
- **Move Assignment**: `T& operator=(T&& other) noexcept`
- **std::move**: Cast to rvalue reference
- **std::forward**: Conditional cast for perfect forwarding

### The Rule of Five
If you define any of these, consider all five:
1. Destructor
2. Copy constructor
3. Copy assignment operator
4. Move constructor
5. Move assignment operator

**Better**: Follow Rule of Zero when possible (use smart pointers and containers)

## Performance Impact

Move semantics provides significant performance improvements for:
- Large objects (vectors, strings, custom containers)
- Heap-allocated resources
- Containers of move-enabled types
- Temporary objects in expressions

**No benefit** for:
- Small POD types (int, float, small structs)
- Short strings (Small String Optimization)
- Objects without dynamic allocation

## Best Practices

1. **Always mark move operations noexcept**
   ```cpp
   T(T&&) noexcept;
   T& operator=(T&&) noexcept;
   ```

2. **Don't use std::move on return values**
   ```cpp
   // BAD
   return std::move(local);

   // GOOD
   return local;  // Enables RVO
   ```

3. **Use std::move for member initialization**
   ```cpp
   MyClass(std::string s) : name(std::move(s)) { }
   ```

4. **Prefer emplace over insert/push**
   ```cpp
   vec.emplace_back(args...);  // Better than push_back
   ```

5. **Check for self-assignment**
   ```cpp
   if (this != &other) { /* ... */ }
   ```

6. **Leave moved-from objects in valid state**
   ```cpp
   other.ptr = nullptr;
   other.size = 0;
   ```

7. **Follow Rule of Zero when possible**
   ```cpp
   class Good {
       std::vector<int> data;  // Handles everything
       std::unique_ptr<int> ptr;
   };
   ```

## Common Patterns

### Sink Arguments (Pass by Value)
```cpp
void setName(std::string name) {  // By value
    this->name = std::move(name);  // Move into member
}
```

### Copy-and-Swap
```cpp
T& operator=(T other) {  // Pass by value
    swap(other);
    return *this;
}
```

### Perfect Forwarding
```cpp
template<typename... Args>
void wrapper(Args&&... args) {
    target(std::forward<Args>(args)...);
}
```

### Builder Pattern
```cpp
Builder& append(std::string s) {
    data += std::move(s);
    return *this;
}
```

## Learning Path

**Beginners:**
1. Start with 01-06 (basics and std::move)
2. Understand lvalues vs rvalues
3. Learn when std::move helps

**Intermediate:**
1. Files 07-10 (Rule of Five, STL, pitfalls)
2. Implement move in your own classes
3. Understand performance implications

**Advanced:**
1. Files 11-15 (patterns and optimization)
2. Perfect forwarding and ref-qualifiers
3. Advanced patterns and idioms

## Testing Your Code

1. **Compile and run each example**
2. **Modify examples** to experiment
3. **Add print statements** to track operations
4. **Use debugger** to step through moves
5. **Profile** to measure performance gains
6. **Test with ASan/Valgrind** for memory safety

## Common Mistakes to Avoid

1. ❌ Using moved-from objects
2. ❌ std::move on return values
3. ❌ Forgetting noexcept on move operations
4. ❌ Moving from const objects
5. ❌ Not checking self-assignment
6. ❌ Not nullifying pointers after move
7. ❌ Expecting specific moved-from state
8. ❌ Moving in loops unnecessarily
9. ❌ Not implementing both move operations
10. ❌ Allocating in move constructor

## Additional Resources

- **C++ Reference**: https://en.cppreference.com/w/cpp/language/move_constructor
- **"Effective Modern C++"** by Scott Meyers (Items 23-30)
- **"C++ Move Semantics"** by Nicolai Josuttis
- **CppCon talks** on move semantics

## Practice Exercises

1. Implement a vector-like container with move semantics
2. Create a move-only file handle wrapper
3. Write a task queue using move semantics
4. Optimize legacy code by adding move operations
5. Implement a string class with SSO and move
6. Create a resource pool using move-only resources
7. Build a cache with move semantics
8. Profile copy vs move for different data sizes

## Summary

Move semantics is fundamental to modern C++:
- Eliminates unnecessary copying
- Enables unique ownership (unique_ptr)
- Improves performance significantly for large objects
- Provides zero-cost abstractions
- Essential for writing efficient C++ code

Master these concepts to write better, faster C++ code!
