# Lesson 20: String Class Deep Dive

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to std::string
2. String Construction and Assignment
3. String Operations
4. String Access and Modification
5. String Searching and Finding
6. String Comparison
7. String Conversion and Formatting
8. Exercises and Projects

---

## 1. Introduction to std::string

### Why Use std::string?

std::string is the C++ Standard Library class for handling strings. It's safer, more convenient, and more powerful than C-strings.

```cpp
#include <iostream>
#include <string>

int main()
{
    // Easy declaration and initialization
    std::string greeting = "Hello, World!";

    // Automatic memory management
    std::string longString = "This is a very long string that would require careful memory management with C-strings";

    // Easy concatenation
    std::string firstName = "John";
    std::string lastName = "Doe";
    std::string fullName = firstName + " " + lastName;

    std::cout << greeting << "\n";
    std::cout << fullName << "\n";

    // No need to worry about null terminators or buffer sizes!

    return 0;
}
```

---

## 2. String Construction and Assignment

### Different Ways to Create Strings:

```cpp
#include <iostream>
#include <string>

int main()
{
    // Default constructor (empty string)
    std::string str1;

    // From C-string
    std::string str2 = "Hello";
    std::string str3("World");

    // From another string
    std::string str4 = str2;
    std::string str5(str3);

    // From substring
    std::string str6(str2, 0, 3);  // "Hel"

    // Repeated characters
    std::string str7(5, '*');  // "*****"

    // From character array
    char chars[] = {'C', '+', '+'};
    std::string str8(chars, 3);

    std::cout << "str1: '" << str1 << "'\n";
    std::cout << "str2: " << str2 << "\n";
    std::cout << "str3: " << str3 << "\n";
    std::cout << "str4: " << str4 << "\n";
    std::cout << "str5: " << str5 << "\n";
    std::cout << "str6: " << str6 << "\n";
    std::cout << "str7: " << str7 << "\n";
    std::cout << "str8: " << str8 << "\n";

    return 0;
}
```

---

## 3. String Operations

### Concatenation:

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string str1 = "Hello";
    std::string str2 = "World";

    // Using + operator
    std::string result1 = str1 + " " + str2;

    // Using += operator
    std::string result2 = str1;
    result2 += " ";
    result2 += str2;

    // Using append()
    std::string result3 = str1;
    result3.append(" ").append(str2);

    std::cout << "result1: " << result1 << "\n";
    std::cout << "result2: " << result2 << "\n";
    std::cout << "result3: " << result3 << "\n";

    return 0;
}
```

---

## 4. String Access and Modification

### Accessing Characters:

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string str = "Hello";

    // Using [] operator
    std::cout << "First char: " << str[0] << "\n";

    // Using at() method (bounds checking)
    std::cout << "Second char: " << str.at(1) << "\n";

    // Modify characters
    str[0] = 'h';
    std::cout << "Modified: " << str << "\n";

    // Front and back
    std::cout << "Front: " << str.front() << "\n";
    std::cout << "Back: " << str.back() << "\n";

    return 0;
}
```

---

## 5. String Searching and Finding

### Find Methods:

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string text = "Hello World, Hello C++";

    // find() - first occurrence
    size_t pos1 = text.find("Hello");
    std::cout << "First 'Hello' at: " << pos1 << "\n";

    // find from position
    size_t pos2 = text.find("Hello", 5);
    std::cout << "Next 'Hello' at: " << pos2 << "\n";

    // rfind() - last occurrence
    size_t pos3 = text.rfind("Hello");
    std::cout << "Last 'Hello' at: " << pos3 << "\n";

    // find_first_of() - any character from set
    size_t pos4 = text.find_first_of("aeiou");
    std::cout << "First vowel at: " << pos4 << "\n";

    // Check if found
    if (text.find("Python") == std::string::npos)
    {
        std::cout << "'Python' not found\n";
    }

    return 0;
}
```

---

## 6. String Comparison

### Comparison Operations:

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string str1 = "apple";
    std::string str2 = "banana";
    std::string str3 = "apple";

    // Using operators
    if (str1 == str3)
        std::cout << "str1 equals str3\n";

    if (str1 != str2)
        std::cout << "str1 not equal to str2\n";

    if (str1 < str2)
        std::cout << "str1 comes before str2\n";

    // Using compare()
    int result = str1.compare(str2);
    if (result < 0)
        std::cout << "str1 < str2 (lexicographically)\n";

    return 0;
}
```

---

## 7. String Conversion and Formatting

### Number Conversion:

```cpp
#include <iostream>
#include <string>

int main()
{
    // String to number
    std::string numStr = "12345";
    int num = std::stoi(numStr);
    std::cout << "String to int: " << num << "\n";

    std::string doubleStr = "3.14159";
    double d = std::stod(doubleStr);
    std::cout << "String to double: " << d << "\n";

    // Number to string
    int value = 42;
    std::string str = std::to_string(value);
    std::cout << "Int to string: " << str << "\n";

    return 0;
}
```

---

## Key Takeaways

1. std::string manages memory automatically
2. Use std::string instead of C-strings when possible
3. String operations are intuitive and safe
4. Many useful member functions available
5. Automatic conversions between string types
6. No need to manage null terminators
7. Better performance through optimization
8. Exception safety built-in

---

## Next Lesson Preview

In **Lesson 21**, we'll explore:
- Pointer fundamentals
- Memory addresses
- Pointer declaration and initialization
- Dereferencing pointers
- Pointer arithmetic basics

**Strings mastered - pointers await!**
