# Lesson 69: Move Semantics and Rvalue References

**Duration**: 6 hours
**Difficulty**: Advanced

Move semantics enable efficient transfer of resources, avoiding unnecessary copies.

## Lvalues vs Rvalues:

```cpp
/*
Lvalue: Has a name, can take address
Rvalue: Temporary, cannot take address

int x = 10;     // x is lvalue, 10 is rvalue
int y = x + 5;  // y is lvalue, x+5 is rvalue
*/
```

## Rvalue References:

```cpp
#include <iostream>

void process(int& x) {
    std::cout << "Lvalue reference\n";
}

void process(int&& x) {
    std::cout << "Rvalue reference\n";
}

int main()
{
    int a = 10;
    process(a);      // Lvalue
    process(20);     // Rvalue
    process(a + 5);  // Rvalue

    return 0;
}
```

## Move Constructor and Assignment:

```cpp
#include <iostream>
#include <cstring>

class String {
private:
    char* data;
    size_t length;

public:
    // Constructor
    String(const char* str) {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
        std::cout << "Constructor\n";
    }

    // Copy constructor
    String(const String& other) {
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);
        std::cout << "Copy constructor\n";
    }

    // Move constructor
    String(String&& other) noexcept {
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
        std::cout << "Move constructor\n";
    }

    // Destructor
    ~String() {
        delete[] data;
    }

    const char* c_str() const { return data; }
};

int main()
{
    String s1("Hello");
    String s2 = s1;              // Copy
    String s3 = std::move(s1);   // Move

    return 0;
}
```

## std::move:

```cpp
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::cout << "v1 size: " << v1.size() << "\n";

    // Move v1 into v2
    std::vector<int> v2 = std::move(v1);
    std::cout << "After move:\n";
    std::cout << "v1 size: " << v1.size() << "\n";  // 0
    std::cout << "v2 size: " << v2.size() << "\n";  // 5

    return 0;
}
```

**Key Takeaways:**
- Move semantics avoid expensive copies
- Rvalue references (&&) bind to temporaries
- Move constructor transfers ownership
- std::move converts lvalue to rvalue
- Use noexcept on move operations
- Original object left in valid but unspecified state
