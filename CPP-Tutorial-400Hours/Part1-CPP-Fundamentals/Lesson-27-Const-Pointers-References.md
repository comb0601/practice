# Lesson 27: Const Pointers and References

**Duration**: 5 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Introduction to Const
2. Pointer to Const
3. Const Pointer
4. Const Pointer to Const
5. Const References
6. Const Correctness
7. Const with Functions
8. Exercises and Projects

---

## 1. Introduction to Const

### Const Variables:

```cpp
#include <iostream>

int main()
{
    const int MAX_SIZE = 100;
    // MAX_SIZE = 200;  // ERROR: Cannot modify const

    std::cout << "MAX_SIZE: " << MAX_SIZE << "\n";

    return 0;
}
```

---

## 2. Pointer to Const

### Cannot Modify Pointee:

```cpp
#include <iostream>

int main()
{
    int value = 42;
    const int* ptr = &value;

    std::cout << "*ptr = " << *ptr << "\n";

    // *ptr = 100;  // ERROR: Cannot modify through pointer to const

    value = 100;  // OK: Can modify value directly
    std::cout << "*ptr = " << *ptr << "\n";

    int other = 200;
    ptr = &other;  // OK: Can change what pointer points to
    std::cout << "*ptr = " << *ptr << "\n";

    return 0;
}
```

---

## 3. Const Pointer

### Cannot Change Pointer:

```cpp
#include <iostream>

int main()
{
    int value = 42;
    int* const ptr = &value;

    std::cout << "*ptr = " << *ptr << "\n";

    *ptr = 100;  // OK: Can modify value
    std::cout << "*ptr = " << *ptr << "\n";

    int other = 200;
    // ptr = &other;  // ERROR: Cannot change const pointer

    return 0;
}
```

---

## 4. Const Pointer to Const

### Both Immutable:

```cpp
#include <iostream>

int main()
{
    int value = 42;
    const int* const ptr = &value;

    std::cout << "*ptr = " << *ptr << "\n";

    // *ptr = 100;       // ERROR: Cannot modify value
    // int other = 200;
    // ptr = &other;     // ERROR: Cannot change pointer

    return 0;
}
```

---

## 5. Const References

### Efficient Parameter Passing:

```cpp
#include <iostream>
#include <string>

void printMessage(const std::string& msg)
{
    std::cout << msg << "\n";
    // msg = "new";  // ERROR: Cannot modify const reference
}

int main()
{
    std::string text = "Hello, World!";
    printMessage(text);

    return 0;
}
```

---

## 6. Const Correctness

### Member Functions:

```cpp
#include <iostream>

class Rectangle
{
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    // Const member function (doesn't modify object)
    double getArea() const
    {
        return width * height;
    }

    // Non-const member function (can modify object)
    void setWidth(double w)
    {
        width = w;
    }
};

int main()
{
    const Rectangle rect(5.0, 3.0);

    std::cout << "Area: " << rect.getArea() << "\n";  // OK

    // rect.setWidth(10.0);  // ERROR: Cannot call non-const function on const object

    return 0;
}
```

---

## Key Takeaways

1. const prevents modification
2. Pointer to const: cannot modify pointee
3. Const pointer: cannot change what it points to
4. Const pointer to const: both immutable
5. Const references for efficient parameter passing
6. Const member functions don't modify object
7. Const correctness improves code safety
8. Use const whenever possible

---

## Next Lesson Preview

In **Lesson 28**, we'll explore:
- Pointer vs reference comparison
- When to use pointers
- When to use references
- Performance considerations
- Best practices

**Const mastered!**
