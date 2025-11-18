# Lesson 76: Namespaces

**Duration**: 5 hours
**Difficulty**: Intermediate

Namespaces prevent name collisions and organize code.

## Basic Namespace:

```cpp
#include <iostream>

namespace Math {
    int add(int a, int b) {
        return a + b;
    }

    int multiply(int a, int b) {
        return a * b;
    }
}

int main()
{
    std::cout << Math::add(5, 3) << "\n";
    std::cout << Math::multiply(5, 3) << "\n";

    return 0;
}
```

## Nested Namespaces:

```cpp
#include <iostream>

namespace Company {
    namespace Product {
        namespace Version {
            void info() {
                std::cout << "Company::Product::Version::info()\n";
            }
        }
    }
}

// C++17 nested namespace
namespace Company::Product::Version2 {
    void info() {
        std::cout << "Nested namespace syntax\n";
    }
}

int main()
{
    Company::Product::Version::info();
    Company::Product::Version2::info();

    return 0;
}
```

## Using Declarations:

```cpp
#include <iostream>
#include <vector>

namespace MyLib {
    void function() {
        std::cout << "MyLib function\n";
    }
}

int main()
{
    // using declaration
    using MyLib::function;
    function();

    // using directive
    using namespace std;
    vector<int> v = {1, 2, 3};
    cout << "Size: " << v.size() << "\n";

    return 0;
}
```

## Anonymous Namespaces:

```cpp
#include <iostream>

namespace {
    // Internal linkage (like static)
    void helper() {
        std::cout << "Anonymous namespace helper\n";
    }
}

int main()
{
    helper();  // Accessible in this file only
    return 0;
}
```

## Namespace Aliases:

```cpp
#include <iostream>

namespace VeryLongNamespaceName {
    void function() {
        std::cout << "Long namespace\n";
    }
}

namespace Short = VeryLongNamespaceName;

int main()
{
    Short::function();
    return 0;
}
```

**Key Takeaways:**
- Namespaces prevent name collisions
- Use :: to access namespace members
- Nested namespaces for organization
- using declarations import specific names
- using directives import entire namespace
- Anonymous namespaces for file-local scope
- Namespace aliases for convenience
