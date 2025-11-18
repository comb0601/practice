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
