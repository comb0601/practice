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
