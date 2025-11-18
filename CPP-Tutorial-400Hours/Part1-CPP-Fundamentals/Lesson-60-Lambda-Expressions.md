# Lesson 60: Lambda Expressions

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Lambdas
2. Lambda Syntax
3. Capture Clauses
4. Lambda Parameters
5. Return Type Deduction
6. Generic Lambdas
7. std::function
8. Exercises and Projects

---

## 1. Introduction to Lambdas

Lambdas are anonymous functions that can be defined inline. Introduced in C++11.

### Basic Lambda:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Lambda as predicate
    std::for_each(v.begin(), v.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "\n";

    // Count even numbers
    int count = std::count_if(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
    });
    std::cout << "Even count: " << count << "\n";

    return 0;
}
```

---

## 2. Lambda Syntax

### Full Syntax:

```cpp
// [capture](parameters) -> return_type { body }

auto lambda = [](int x, int y) -> int {
    return x + y;
};
```

### Minimal Syntax:

```cpp
auto lambda = [](int x) { return x * 2; };
```

---

## 3. Capture Clauses

### Capture by Value:

```cpp
#include <iostream>

int main()
{
    int multiplier = 3;

    auto triple = [multiplier](int x) {
        return x * multiplier;
    };

    std::cout << triple(5) << "\n";  // 15

    return 0;
}
```

### Capture by Reference:

```cpp
#include <iostream>

int main()
{
    int count = 0;

    auto increment = [&count]() {
        count++;
    };

    increment();
    increment();
    std::cout << "Count: " << count << "\n";  // 2

    return 0;
}
```

### Capture All:

```cpp
#include <iostream>

int main()
{
    int a = 10, b = 20;

    // Capture all by value
    auto lambda1 = [=]() { return a + b; };

    // Capture all by reference
    auto lambda2 = [&]() { a++; b++; };

    std::cout << lambda1() << "\n";
    lambda2();
    std::cout << "a=" << a << ", b=" << b << "\n";

    return 0;
}
```

---

## 4. Lambda Parameters

### Multiple Parameters:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v1 = {1, 2, 3};
    std::vector<int> v2 = {10, 20, 30};
    std::vector<int> result(3);

    std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(),
                  [](int a, int b) { return a + b; });

    for (int x : result) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 5. Return Type Deduction

### Auto Return Type:

```cpp
#include <iostream>

int main()
{
    // Return type deduced as int
    auto lambda1 = [](int x) { return x * 2; };

    // Explicit return type
    auto lambda2 = [](int x) -> double { return x * 2.5; };

    std::cout << lambda1(5) << "\n";
    std::cout << lambda2(5) << "\n";

    return 0;
}
```

---

## 6. Generic Lambdas

### Auto Parameters (C++14):

```cpp
#include <iostream>
#include <string>

int main()
{
    auto print = [](auto x) {
        std::cout << x << "\n";
    };

    print(42);
    print(3.14);
    print(std::string("Hello"));

    return 0;
}
```

---

## 7. std::function

### Storing Lambdas:

```cpp
#include <functional>
#include <iostream>

int main()
{
    std::function<int(int, int)> add = [](int a, int b) {
        return a + b;
    };

    std::cout << add(3, 4) << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Project: Event Handler System

```cpp
#include <functional>
#include <vector>
#include <string>
#include <iostream>

class EventSystem {
private:
    std::vector<std::function<void(const std::string&)>> handlers;

public:
    void subscribe(std::function<void(const std::string&)> handler) {
        handlers.push_back(handler);
    }

    void notify(const std::string& event) {
        for (auto& handler : handlers) {
            handler(event);
        }
    }
};

int main()
{
    EventSystem events;

    events.subscribe([](const std::string& event) {
        std::cout << "Logger: " << event << "\n";
    });

    events.subscribe([](const std::string& event) {
        std::cout << "Handler 2: " << event << "\n";
    });

    events.notify("User logged in");

    return 0;
}
```

---

## Key Takeaways

1. ✅ Lambdas are anonymous inline functions
2. ✅ Capture by value [=] or reference [&]
3. ✅ Can capture specific variables [x, &y]
4. ✅ Return type often auto-deduced
5. ✅ Generic lambdas use auto parameters
6. ✅ std::function stores any callable
7. ✅ Great for STL algorithms and callbacks

---

## Next Lesson Preview

In **Lesson 61**, we'll learn about:
- Exception handling basics
- try-catch blocks
- Throwing exceptions
- Standard exception types

**Excellent work on lambdas!**
