# Module 03: Advanced C++

Welcome to Module 03 of the C++ Tutorial - WinAPI 3D Rendering course. This module covers advanced C++ features introduced in C++11 and beyond, essential for modern C++ programming.

## Module Overview

This module contains **10 comprehensive lessons** (Lessons 21-30) covering advanced C++ topics. Each lesson includes detailed theory documentation and multiple executable code examples.

## Lessons

### Lesson 21: Move Semantics
**Master efficient resource transfer in modern C++**
- Lvalues and Rvalues
- Rvalue references (&&)
- Move constructor and move assignment
- std::move and std::forward
- Perfect forwarding
- Rule of Five
- 15 code examples

### Lesson 22: Smart Pointers
**Automatic memory management with RAII**
- std::unique_ptr (exclusive ownership)
- std::shared_ptr (shared ownership)
- std::weak_ptr (breaking cycles)
- Custom deleters
- make_unique and make_shared
- 5+ code examples

### Lesson 23: Lambda Expressions
**Anonymous functions and functional programming**
- Lambda syntax and captures
- Mutable lambdas
- Generic lambdas (C++14)
- Lambda with STL algorithms
- 3+ code examples

### Lesson 24: Multithreading Basics
**Concurrent programming with std::thread**
- Creating and managing threads
- Thread functions and arguments
- join() and detach()
- Thread IDs
- 10+ code examples

### Lesson 25: Synchronization
**Thread-safe programming**
- std::mutex and lock guards
- std::unique_lock
- Condition variables
- std::atomic operations
- Deadlock prevention
- 10+ code examples

### Lesson 26: Async & Futures
**Asynchronous task execution**
- std::async
- std::future and std::promise
- std::packaged_task
- Launch policies
- 10 code examples

### Lesson 27: Modern C++ Features
**New C++11/14/17 features**
- auto keyword
- decltype
- constexpr
- nullptr
- Range-based for loops
- Structured bindings
- 10 code examples

### Lesson 28: Variadic Templates
**Variable argument templates**
- Parameter packs
- Fold expressions (C++17)
- sizeof...
- Recursive variadic templates
- Perfect forwarding
- 10 code examples

### Lesson 29: Design Patterns
**Classic patterns in modern C++**
- Singleton, Factory, Observer
- Strategy, Decorator, Command
- Adapter, Facade, Proxy
- Template Method
- 10+ pattern implementations

### Lesson 30: CRTP & Advanced Templates
**Template metaprogramming**
- Curiously Recurring Template Pattern
- Static polymorphism
- Type traits
- SFINAE
- enable_if
- Tag dispatching
- 10 code examples

## Quick Start

### Compilation

All examples can be compiled with:

```bash
# Standard compilation
g++ -std=c++17 -Wall filename.cpp -o filename

# With threading (Lessons 24-26)
g++ -std=c++17 -pthread -Wall filename.cpp -o filename

# With optimization
g++ -std=c++17 -O2 -Wall filename.cpp -o filename
```

### Example Usage

```bash
cd Lesson21-Move-Semantics/Code
g++ -std=c++17 -Wall 01_lvalue_rvalue_basics.cpp -o 01_example
./01_example
```

## Learning Path

### Recommended Order for Beginners
1. Lesson 21 (Move Semantics) ⭐ Essential
2. Lesson 22 (Smart Pointers) ⭐ Essential
3. Lesson 23 (Lambda Expressions)
4. Lesson 27 (Modern C++ Features)
5. Lesson 24-26 (Concurrency)
6. Lesson 28-30 (Advanced Templates)
7. Lesson 29 (Design Patterns)

### For Concurrent Programming
1. Lesson 24 (Multithreading Basics)
2. Lesson 25 (Synchronization)
3. Lesson 26 (Async & Futures)

### For Template Experts
1. Lesson 28 (Variadic Templates)
2. Lesson 30 (CRTP & Advanced Templates)

## Prerequisites

Before starting Module 03, you should be comfortable with:
- C++ basics (variables, functions, classes)
- Object-oriented programming
- Basic memory management
- STL containers (vector, map, etc.)
- STL algorithms

## What You'll Learn

By completing this module, you will:
- ✅ Master move semantics for efficient code
- ✅ Use smart pointers for automatic memory management
- ✅ Write concurrent programs with threads
- ✅ Apply modern C++ features and idioms
- ✅ Implement advanced template techniques
- ✅ Use design patterns effectively
- ✅ Write production-quality C++ code

## Requirements

- **Compiler**: GCC 7+, Clang 5+, or MSVC 2017+ with C++17 support
- **Libraries**: Standard C++ Library, pthread (for threading)
- **OS**: Linux, macOS, or Windows

## Module Statistics

- **Total Lessons**: 10
- **Code Examples**: 100+ files
- **Theory Content**: 40,000+ words
- **Topics**: C++11, C++14, C++17
- **Difficulty**: Intermediate to Advanced

## File Structure

```
Module03-Advanced-CPP/
├── Lesson21-Move-Semantics/
│   ├── Lesson21-Move-Semantics.md
│   └── Code/ (15 .cpp files + README.md)
├── Lesson22-Smart-Pointers/
│   ├── Lesson22-Smart-Pointers.md
│   └── Code/ (5+ .cpp files + README.md)
├── Lesson23-Lambda-Expressions/
├── Lesson24-Multithreading-Basics/
├── Lesson25-Synchronization/
├── Lesson26-Async-Futures/
├── Lesson27-Modern-CPP-Features/
├── Lesson28-Variadic-Templates/
├── Lesson29-Design-Patterns/
├── Lesson30-CRTP-Advanced-Templates/
├── MODULE03-COMPLETION-SUMMARY.md
└── README.md (this file)
```

## How to Use This Module

### For Self-Study
1. Read the lesson theory file (Lesson*.md)
2. Study code examples in the Code/ directory
3. Compile and run examples
4. Modify examples to experiment
5. Complete exercises if provided

### For Teaching
- Use theory files as lecture slides
- Demonstrate code in class
- Assign code modifications as homework
- Use as reference material

### For Reference
- Quick lookup of C++ features
- Copy-paste production-ready patterns
- Performance benchmarking code
- Best practices examples

## Key Features

### Comprehensive Theory
- Detailed concept explanations
- Practical examples
- Best practices
- Common pitfalls
- Performance tips

### Executable Code
- Complete, compilable programs
- Extensive comments
- Progressive difficulty
- Real-world scenarios

### Modern C++
- C++11/14/17 features
- Industry best practices
- Thread-safe code
- Exception-safe patterns
- Memory-safe techniques

## Additional Resources

Each lesson includes:
- Further reading recommendations
- C++ reference links
- Book suggestions
- Video tutorial references

## Getting Help

If you encounter issues:
1. Check the README.md in each Code/ directory
2. Review compilation commands
3. Ensure C++17 compiler support
4. Verify pthread library installation (for threading)

## Next Module

After completing Module 03, proceed to:
- **Module 04**: WinAPI Fundamentals
- **Module 06**: 3D Graphics Mathematics
- **Module 07**: DirectX Rendering

## Contributing

This module is part of a comprehensive C++ tutorial series. For updates, corrections, or suggestions, please refer to the main course repository.

## License

Educational use. Please maintain attribution when sharing or modifying.

---

**Course**: C++ Tutorial - WinAPI 3D Rendering
**Module**: 03 - Advanced C++
**Lessons**: 21-30
**Status**: ✅ Complete
**Last Updated**: November 2025

**Happy Learning! 🚀**
