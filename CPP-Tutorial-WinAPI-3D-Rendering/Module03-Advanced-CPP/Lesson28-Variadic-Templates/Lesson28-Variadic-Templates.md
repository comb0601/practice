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
