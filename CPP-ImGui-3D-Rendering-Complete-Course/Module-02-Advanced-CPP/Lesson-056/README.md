# Lesson 56: Lambda Expressions and Functional Programming

**Duration:** 2 hours

## Learning Objectives
- Master lambda expression syntax
- Understand capture modes (by value, by reference)
- Learn mutable lambdas and return type deduction
- Use lambdas with STL algorithms
- Implement closures and functors

## Introduction

Lambda expressions (introduced in C++11) allow you to write anonymous functions inline. They're essential for modern C++ and work seamlessly with STL algorithms, making code more concise and expressive.

## Basic Lambda Syntax

### Simple Lambdas

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // Basic lambda
    auto hello = []() {
        std::cout << "Hello, Lambda!" << std::endl;
    };
    hello();

    // Lambda with parameters
    auto add = [](int a, int b) {
        return a + b;
    };
    std::cout << "3 + 4 = " << add(3, 4) << std::endl;

    // Lambda with explicit return type
    auto divide = [](double a, double b) -> double {
        return b != 0 ? a / b : 0.0;
    };
    std::cout << "10 / 3 = " << divide(10.0, 3.0) << std::endl;

    return 0;
}
```

**Output:**
```
Hello, Lambda!
3 + 4 = 7
10 / 3 = 3.33333
```

### Using Lambdas with Algorithms

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // Sort with lambda
    std::sort(numbers.begin(), numbers.end(),
              [](int a, int b) { return a < b; });

    std::cout << "Sorted: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    // Count even numbers
    int evenCount = std::count_if(numbers.begin(), numbers.end(),
                                   [](int n) { return n % 2 == 0; });
    std::cout << "Even numbers: " << evenCount << std::endl;

    // Transform
    std::vector<int> squared(numbers.size());
    std::transform(numbers.begin(), numbers.end(), squared.begin(),
                   [](int n) { return n * n; });

    std::cout << "Squared: ";
    for (int n : squared) std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}
```

## Capture Modes

### Capture by Value

```cpp
#include <iostream>

int main() {
    int x = 10;
    int y = 20;

    // Capture x by value
    auto lambda1 = [x]() {
        std::cout << "x = " << x << std::endl;
        // x = 5; // Error: cannot modify captured value
    };

    // Capture all by value
    auto lambda2 = [=]() {
        std::cout << "x = " << x << ", y = " << y << std::endl;
    };

    x = 100;  // Original modified
    lambda1();  // Still prints 10
    lambda2();  // Still prints 10, 20

    return 0;
}
```

### Capture by Reference

```cpp
#include <iostream>

int main() {
    int counter = 0;

    // Capture by reference
    auto increment = [&counter]() {
        ++counter;
    };

    increment();
    increment();
    increment();

    std::cout << "Counter: " << counter << std::endl;

    // Capture all by reference
    int a = 1, b = 2;
    auto modify = [&]() {
        a *= 2;
        b *= 3;
    };

    modify();
    std::cout << "a = " << a << ", b = " << b << std::endl;

    return 0;
}
```

### Mixed Captures

```cpp
#include <iostream>

int main() {
    int x = 10, y = 20, z = 30;

    // Capture x by value, y and z by reference
    auto lambda = [x, &y, &z]() {
        std::cout << "x = " << x << std::endl;
        y += 10;
        z += 10;
    };

    // Capture all by value except y by reference
    auto lambda2 = [=, &y]() {
        std::cout << "x = " << x << ", z = " << z << std::endl;
        y += 5;
    };

    lambda();
    std::cout << "After lambda: y = " << y << ", z = " << z << std::endl;

    lambda2();
    std::cout << "After lambda2: y = " << y << std::endl;

    return 0;
}
```

## Mutable Lambdas

```cpp
#include <iostream>

int main() {
    int x = 10;

    // Mutable lambda - can modify captured values
    auto lambda = [x]() mutable {
        x += 5;
        std::cout << "Inside lambda: " << x << std::endl;
        return x;
    };

    lambda();
    lambda();

    std::cout << "Original x: " << x << std::endl;  // Still 10

    // Mutable with reference capture
    auto lambda2 = [&x]() mutable {
        x += 5;
    };

    lambda2();
    std::cout << "After lambda2: " << x << std::endl;  // 15

    return 0;
}
```

## Generic Lambdas (C++14)

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    // Generic lambda with auto parameters
    auto print = [](const auto& value) {
        std::cout << value << std::endl;
    };

    print(42);
    print(3.14);
    print(std::string("Hello"));

    // Generic lambda for comparison
    auto isGreater = [](const auto& a, const auto& b) {
        return a > b;
    };

    std::cout << "5 > 3: " << isGreater(5, 3) << std::endl;
    std::cout << "2.5 > 3.7: " << isGreater(2.5, 3.7) << std::endl;

    return 0;
}
```

## Practical Example 1: Event System

```cpp
#include <iostream>
#include <vector>
#include <functional>
#include <string>

class EventSystem {
private:
    using Callback = std::function<void(const std::string&)>;
    std::vector<Callback> listeners;

public:
    void subscribe(Callback callback) {
        listeners.push_back(callback);
    }

    void trigger(const std::string& event) {
        for (auto& listener : listeners) {
            listener(event);
        }
    }
};

int main() {
    EventSystem events;

    // Subscribe with lambda
    events.subscribe([](const std::string& event) {
        std::cout << "Listener 1: " << event << std::endl;
    });

    int eventCount = 0;
    events.subscribe([&eventCount](const std::string& event) {
        ++eventCount;
        std::cout << "Listener 2: Event #" << eventCount
                  << " - " << event << std::endl;
    });

    events.trigger("User logged in");
    events.trigger("Data saved");
    events.trigger("User logged out");

    return 0;
}
```

## Exercises

### Exercise 1: Filter and Transform
**Problem:** Use lambdas to filter and transform a vector.

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Filter even numbers
    std::vector<int> evens;
    std::copy_if(numbers.begin(), numbers.end(),
                 std::back_inserter(evens),
                 [](int n) { return n % 2 == 0; });

    std::cout << "Even: ";
    for (int n : evens) std::cout << n << " ";
    std::cout << std::endl;

    // Transform to squares
    std::transform(evens.begin(), evens.end(), evens.begin(),
                   [](int n) { return n * n; });

    std::cout << "Squared: ";
    for (int n : evens) std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- Lambda expression syntax and usage
- Capture modes (value, reference, mixed)
- Mutable lambdas
- Generic lambdas with auto
- Using lambdas with STL algorithms
- Practical applications of lambdas

## Checklist

- [ ] Understand lambda syntax
- [ ] Can use capture modes correctly
- [ ] Know when to use mutable
- [ ] Can write generic lambdas
- [ ] Understand std::function
- [ ] Completed all exercises
- [ ] Can use lambdas with algorithms

## Next Lesson

In Lesson 57, we'll explore **STL Algorithms** in depth with advanced usage patterns.
