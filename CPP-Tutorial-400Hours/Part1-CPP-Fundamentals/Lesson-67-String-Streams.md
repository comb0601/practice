# Lesson 67: String Streams

**Duration**: 5 hours
**Difficulty**: Intermediate

String streams allow string manipulation using stream operations.

## istringstream (Input):

```cpp
#include <sstream>
#include <iostream>
#include <string>

int main()
{
    std::string data = "42 3.14 Hello";
    std::istringstream iss(data);

    int num;
    double pi;
    std::string word;

    iss >> num >> pi >> word;

    std::cout << "Integer: " << num << "\n";
    std::cout << "Double: " << pi << "\n";
    std::cout << "String: " << word << "\n";

    return 0;
}
```

## ostringstream (Output):

```cpp
#include <sstream>
#include <iostream>

int main()
{
    std::ostringstream oss;

    oss << "Name: " << "Alice" << ", Age: " << 30;
    std::string result = oss.str();

    std::cout << result << "\n";

    return 0;
}
```

## String Parsing:

```cpp
#include <sstream>
#include <vector>
#include <iostream>

std::vector<int> parseNumbers(const std::string& str) {
    std::vector<int> numbers;
    std::istringstream iss(str);
    int num;

    while (iss >> num) {
        numbers.push_back(num);
    }

    return numbers;
}

int main()
{
    std::string input = "10 20 30 40 50";
    auto numbers = parseNumbers(input);

    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    return 0;
}
```

## Type Conversion:

```cpp
#include <sstream>
#include <iostream>
#include <string>

template<typename T>
std::string toString(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template<typename T>
T fromString(const std::string& str) {
    std::istringstream iss(str);
    T value;
    iss >> value;
    return value;
}

int main()
{
    // Convert to string
    std::string s1 = toString(42);
    std::string s2 = toString(3.14);

    // Convert from string
    int num = fromString<int>("123");
    double pi = fromString<double>("3.14159");

    std::cout << s1 << ", " << s2 << "\n";
    std::cout << num << ", " << pi << "\n";

    return 0;
}
```

**Key Takeaways:**
- istringstream for parsing strings
- ostringstream for building strings
- Great for type conversions
- No file I/O overhead
- Useful for formatting and parsing
