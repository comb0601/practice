# Lesson 68: Modern C++ Features (auto, range-based for)

**Duration**: 6 hours
**Difficulty**: Intermediate

Modern C++ (C++11 and later) introduced many convenient features.

## auto Keyword:

```cpp
#include <vector>
#include <map>
#include <iostream>

int main()
{
    // Type deduction
    auto x = 42;              // int
    auto y = 3.14;            // double
    auto s = "Hello";         // const char*
    auto str = std::string("World");  // std::string

    // With containers
    std::vector<int> v = {1, 2, 3};
    auto it = v.begin();      // std::vector<int>::iterator

    // With complex types
    std::map<std::string, int> m;
    auto pair = std::make_pair("key", 42);  // std::pair<const char*, int>

    return 0;
}
```

## Range-Based For Loop:

```cpp
#include <vector>
#include <map>
#include <iostream>

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Basic range-for
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // With auto
    for (auto x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // By reference (for modification)
    for (auto& x : v) {
        x *= 2;
    }

    // Const reference (efficient, no copy)
    for (const auto& x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // With map
    std::map<std::string, int> m = {{"Alice", 30}, {"Bob", 25}};
    for (const auto& [key, value] : m) {  // C++17 structured binding
        std::cout << key << ": " << value << "\n";
    }

    return 0;
}
```

## nullptr:

```cpp
#include <iostream>

void process(int* ptr) {
    if (ptr != nullptr) {
        std::cout << "Value: " << *ptr << "\n";
    }
}

int main()
{
    int* p1 = nullptr;  // Better than NULL or 0
    int x = 42;
    int* p2 = &x;

    process(p1);
    process(p2);

    return 0;
}
```

## Uniform Initialization:

```cpp
#include <vector>
#include <iostream>

struct Point {
    int x, y;
};

int main()
{
    // Uniform initialization syntax
    int a{42};
    double b{3.14};
    std::string s{"Hello"};

    std::vector<int> v{1, 2, 3, 4, 5};
    Point p{10, 20};

    // Prevents narrowing conversions
    // int x{3.14};  // Error: narrowing

    return 0;
}
```

## decltype:

```cpp
#include <iostream>

int main()
{
    int x = 42;
    decltype(x) y = 100;  // y is int

    auto add = [](int a, int b) { return a + b; };
    decltype(add(1, 2)) result = 10;  // result is int

    std::cout << "y: " << y << ", result: " << result << "\n";

    return 0;
}
```

**Key Takeaways:**
- auto for type deduction
- Range-for for cleaner iteration
- nullptr replaces NULL
- Uniform initialization with {}
- decltype for type inference
