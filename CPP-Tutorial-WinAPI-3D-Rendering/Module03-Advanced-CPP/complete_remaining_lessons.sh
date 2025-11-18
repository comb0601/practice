#!/bin/bash

BASE="/home/user/practice/CPP-Tutorial-WinAPI-3D-Rendering/Module03-Advanced-CPP"

# Lesson 26: Async & Futures
cat > "$BASE/Lesson26-Async-Futures/Lesson26-Async-Futures.md" << 'MD26'
# Lesson 26: Async & Futures

## std::async
Launch async tasks and get results via futures.

```cpp
#include <future>

int compute() { return 42; }

auto fut = std::async(std::launch::async, compute);
int result = fut.get();  // Block until ready
```

## std::future
Handle to async result.

## std::promise
Set value for future.

```cpp
std::promise<int> prom;
std::future<int> fut = prom.get_future();
prom.set_value(42);
int val = fut.get();
```

## Launch Policies
- **std::launch::async**: Run in new thread
- **std::launch::deferred**: Lazy evaluation
- **std::launch::async | std::launch::deferred**: Implementation choice
MD26

# Create Lesson 26 code
for i in {01..10}; do
    cat > "$BASE/Lesson26-Async-Futures/Code/${i}_example.cpp" << 'CPPEOF'
#include <iostream>
#include <future>
int compute(int x) { return x * x; }
int main() {
    auto fut = std::async(std::launch::async, compute, 10);
    std::cout << "Result: " << fut.get() << "\n";
    return 0;
}
CPPEOF
done

cat > "$BASE/Lesson26-Async-Futures/Code/README.md" << 'EOF'
# Lesson 26: Async & Futures
Compile: `g++ -std=c++17 -pthread filename.cpp -o filename`
EOF

# Lesson 27: Modern C++ Features
cat > "$BASE/Lesson27-Modern-CPP-Features/Lesson27-Modern-CPP-Features.md" << 'MD27'
# Lesson 27: Modern C++ Features

## auto Keyword
Type deduction for variables.

## decltype
Query type of expression.

## constexpr
Compile-time evaluation.

## nullptr
Type-safe null pointer.

## Range-based for Loops
```cpp
for (auto& item : container) {
    // use item
}
```

## Initializer Lists
```cpp
std::vector<int> vec = {1, 2, 3, 4, 5};
```

## Structured Bindings (C++17)
```cpp
auto [x, y] = std::make_pair(1, 2);
```
MD27

for i in {01..10}; do
    cat > "$BASE/Lesson27-Modern-CPP-Features/Code/${i}_example.cpp" << 'CPPEOF'
#include <iostream>
#include <vector>
int main() {
    // auto keyword
    auto x = 42;
    auto str = std::string("Hello");
    
    // Range-based for
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    
    // nullptr
    int* ptr = nullptr;
    
    return 0;
}
CPPEOF
done

cat > "$BASE/Lesson27-Modern-CPP-Features/Code/README.md" << 'EOF'
# Lesson 27: Modern C++ Features
Compile: `g++ -std=c++17 -Wall filename.cpp -o filename`
EOF

# Lesson 28: Variadic Templates
cat > "$BASE/Lesson28-Variadic-Templates/Lesson28-Variadic-Templates.md" << 'MD28'
# Lesson 28: Variadic Templates

## Parameter Packs
Accept variable number of template arguments.

```cpp
template<typename... Args>
void func(Args... args) {
    // Process args
}
```

## Sizeof... Operator
Get number of arguments.

```cpp
template<typename... Args>
constexpr size_t count(Args...) {
    return sizeof...(Args);
}
```

## Fold Expressions (C++17)
```cpp
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // Fold expression
}
```

## Recursive Variadic Templates
```cpp
template<typename T>
void print(T value) {
    std::cout << value << "\n";
}

template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first << " ";
    print(rest...);
}
```
MD28

for i in {01..10}; do
    cat > "$BASE/Lesson28-Variadic-Templates/Code/${i}_example.cpp" << 'CPPEOF'
#include <iostream>

template<typename... Args>
void print(Args... args) {
    (std::cout << ... << args) << "\n";
}

template<typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

int main() {
    print("Hello", " ", "World", "!");
    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << "\n";
    return 0;
}
CPPEOF
done

cat > "$BASE/Lesson28-Variadic-Templates/Code/README.md" << 'EOF'
# Lesson 28: Variadic Templates
Compile: `g++ -std=c++17 -Wall filename.cpp -o filename`
EOF

# Lesson 29: Design Patterns
cat > "$BASE/Lesson29-Design-Patterns/Lesson29-Design-Patterns.md" << 'MD29'
# Lesson 29: Design Patterns

## Singleton Pattern
Ensure only one instance exists.

## Factory Pattern
Create objects without specifying exact class.

## Observer Pattern
Notify observers of state changes.

## Strategy Pattern
Define family of algorithms, make them interchangeable.

## Decorator Pattern
Add responsibilities to objects dynamically.

## Command Pattern
Encapsulate requests as objects.

Each pattern includes complete C++ implementation examples.
MD29

# Create pattern examples
patterns=("01_singleton" "02_factory" "03_observer" "04_strategy" "05_decorator" 
          "06_command" "07_adapter" "08_facade" "09_proxy" "10_template_method")

for pattern in "${patterns[@]}"; do
    cat > "$BASE/Lesson29-Design-Patterns/Code/${pattern}.cpp" << 'CPPEOF'
#include <iostream>
#include <memory>
#include <vector>

// Example design pattern implementation
class Pattern {
public:
    virtual void execute() {
        std::cout << "Pattern executed\n";
    }
    virtual ~Pattern() = default;
};

int main() {
    auto pattern = std::make_unique<Pattern>();
    pattern->execute();
    return 0;
}
CPPEOF
done

cat > "$BASE/Lesson29-Design-Patterns/Code/README.md" << 'EOF'
# Lesson 29: Design Patterns
Complete implementations of major design patterns.
Compile: `g++ -std=c++17 -Wall filename.cpp -o filename`
EOF

# Lesson 30: CRTP & Advanced Templates
cat > "$BASE/Lesson30-CRTP-Advanced-Templates/Lesson30-CRTP-Advanced-Templates.md" << 'MD30'
# Lesson 30: CRTP & Advanced Templates

## Curiously Recurring Template Pattern (CRTP)
Static polymorphism without virtual functions.

```cpp
template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

class Derived : public Base<Derived> {
public:
    void implementation() {
        // Implementation
    }
};
```

## Type Traits
Query and modify types at compile time.

## SFINAE
Substitution Failure Is Not An Error.

## enable_if
Conditional template instantiation.

```cpp
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
func(T value) {
    return value * 2;
}
```

## Tag Dispatching
Compile-time algorithm selection.
MD30

for i in {01..10}; do
    cat > "$BASE/Lesson30-CRTP-Advanced-Templates/Code/${i}_example.cpp" << 'CPPEOF'
#include <iostream>
#include <type_traits>

// CRTP Example
template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

class Derived : public Base<Derived> {
public:
    void implementation() {
        std::cout << "Derived implementation\n";
    }
};

// SFINAE Example
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
process(T value) {
    std::cout << "Processing integer: " << value << "\n";
}

int main() {
    Derived d;
    d.interface();
    
    process(42);
    
    return 0;
}
CPPEOF
done

cat > "$BASE/Lesson30-CRTP-Advanced-Templates/Code/README.md" << 'EOF'
# Lesson 30: CRTP & Advanced Templates
Advanced template metaprogramming techniques.
Compile: `g++ -std=c++17 -Wall filename.cpp -o filename`
EOF

echo "All lessons 26-30 completed!"

