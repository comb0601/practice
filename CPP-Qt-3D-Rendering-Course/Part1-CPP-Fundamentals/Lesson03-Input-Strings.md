# Lesson 03: Input Operations and String Handling

**Duration: 2.5 hours**

## Table of Contents
1. Basic Input with cin
2. Input Problems and Solutions
3. String Basics
4. String Input Methods
5. String Manipulation
6. Common String Operations
7. Practical Exercises

## 1. Basic Input with cin

### Simple Input

```cpp
#include <iostream>

int main() {
    int age;

    std::cout << "Enter your age: ";
    std::cin >> age;

    std::cout << "You are " << age << " years old." << std::endl;

    return 0;
}
```

### Multiple Inputs

```cpp
#include <iostream>

int main() {
    int x, y, z;

    std::cout << "Enter three numbers: ";
    std::cin >> x >> y >> z;

    std::cout << "You entered: " << x << ", " << y << ", " << z << std::endl;

    int sum = x + y + z;
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
```

### Input Different Types

```cpp
#include <iostream>

int main() {
    std::string name;
    int age;
    double height;
    char grade;

    std::cout << "Enter your name: ";
    std::cin >> name;  // Warning: only reads until first space!

    std::cout << "Enter your age: ";
    std::cin >> age;

    std::cout << "Enter your height (cm): ";
    std::cin >> height;

    std::cout << "Enter your grade: ";
    std::cin >> grade;

    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "Height: " << height << " cm" << std::endl;
    std::cout << "Grade: " << grade << std::endl;

    return 0;
}
```

## 2. Input Problems and Solutions

### Problem 1: Whitespace with cin

```cpp
#include <iostream>
#include <string>

int main() {
    std::string firstName;

    std::cout << "Enter full name: ";
    std::cin >> firstName;  // Only reads "John" from "John Doe"

    std::cout << "Name: " << firstName << std::endl;
    // Problem: Doesn't read full name!

    return 0;
}
```

### Solution: Using getline

```cpp
#include <iostream>
#include <string>

int main() {
    std::string fullName;

    std::cout << "Enter full name: ";
    std::getline(std::cin, fullName);

    std::cout << "Name: " << fullName << std::endl;
    // Now reads entire line including spaces

    return 0;
}
```

### Problem 2: Mixing cin and getline

```cpp
#include <iostream>
#include <string>

int main() {
    int age;
    std::string name;

    std::cout << "Enter age: ";
    std::cin >> age;

    std::cout << "Enter name: ";
    std::getline(std::cin, name);  // PROBLEM: Skips input!

    std::cout << "Age: " << age << std::endl;
    std::cout << "Name: " << name << std::endl;  // Empty!

    return 0;
}
```

### Solution: Clear Input Buffer

```cpp
#include <iostream>
#include <string>
#include <limits>

int main() {
    int age;
    std::string name;

    std::cout << "Enter age: ";
    std::cin >> age;

    // Clear the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter name: ";
    std::getline(std::cin, name);

    std::cout << "Age: " << age << std::endl;
    std::cout << "Name: " << name << std::endl;

    return 0;
}
```

### Input Validation

```cpp
#include <iostream>

int main() {
    int age;

    std::cout << "Enter age: ";

    while (!(std::cin >> age)) {
        std::cout << "Invalid input! Please enter a number: ";
        std::cin.clear();  // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Valid age: " << age << std::endl;

    return 0;
}
```

## 3. String Basics

### C-Style Strings (Character Arrays)

```cpp
#include <iostream>
#include <cstring>

int main() {
    // C-style string (null-terminated character array)
    char oldStyle[50] = "Hello, World!";

    std::cout << "C-style string: " << oldStyle << std::endl;
    std::cout << "Length: " << strlen(oldStyle) << std::endl;

    // Dangerous: no bounds checking!
    strcpy(oldStyle, "New string");
    std::cout << "After strcpy: " << oldStyle << std::endl;

    return 0;
}
```

### C++ Strings (std::string) - RECOMMENDED

```cpp
#include <iostream>
#include <string>

int main() {
    // Modern C++ string
    std::string modernString = "Hello, C++!";

    std::cout << "C++ string: " << modernString << std::endl;
    std::cout << "Length: " << modernString.length() << std::endl;

    // Safe operations
    modernString = "New string";
    std::cout << "After assignment: " << modernString << std::endl;

    return 0;
}
```

### String Declaration and Initialization

```cpp
#include <iostream>
#include <string>

int main() {
    // Various initialization methods
    std::string s1;                          // Empty string
    std::string s2 = "Hello";                // From string literal
    std::string s3("World");                 // Constructor
    std::string s4{s2};                      // Copy
    std::string s5(5, 'A');                  // Repeat character: "AAAAA"
    std::string s6 = s2 + " " + s3;          // Concatenation

    std::cout << "s1: '" << s1 << "'" << std::endl;
    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s3: " << s3 << std::endl;
    std::cout << "s4: " << s4 << std::endl;
    std::cout << "s5: " << s5 << std::endl;
    std::cout << "s6: " << s6 << std::endl;

    return 0;
}
```

## 4. String Input Methods

### Method 1: cin (reads until whitespace)

```cpp
#include <iostream>
#include <string>

int main() {
    std::string word;

    std::cout << "Enter a word: ";
    std::cin >> word;

    std::cout << "You entered: " << word << std::endl;

    return 0;
}
```

### Method 2: getline (reads entire line)

```cpp
#include <iostream>
#include <string>

int main() {
    std::string line;

    std::cout << "Enter a sentence: ";
    std::getline(std::cin, line);

    std::cout << "You entered: " << line << std::endl;

    return 0;
}
```

### Method 3: getline with custom delimiter

```cpp
#include <iostream>
#include <string>

int main() {
    std::string text;

    std::cout << "Enter text (end with '#'): ";
    std::getline(std::cin, text, '#');

    std::cout << "You entered: " << text << std::endl;

    return 0;
}
```

## 5. String Manipulation

### String Concatenation

```cpp
#include <iostream>
#include <string>

int main() {
    std::string first = "Hello";
    std::string second = "World";

    // Method 1: Using +
    std::string result1 = first + " " + second;

    // Method 2: Using +=
    std::string result2 = first;
    result2 += " ";
    result2 += second;

    // Method 3: Using append()
    std::string result3 = first;
    result3.append(" ").append(second);

    std::cout << "Result 1: " << result1 << std::endl;
    std::cout << "Result 2: " << result2 << std::endl;
    std::cout << "Result 3: " << result3 << std::endl;

    return 0;
}
```

### Accessing Characters

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, C++!";

    // Method 1: Using []
    std::cout << "First character: " << str[0] << std::endl;
    std::cout << "Last character: " << str[str.length() - 1] << std::endl;

    // Method 2: Using at() - with bounds checking
    std::cout << "Character at 7: " << str.at(7) << std::endl;

    // Modifying characters
    str[0] = 'h';
    std::cout << "Modified: " << str << std::endl;

    // Iterating through string
    std::cout << "All characters: ";
    for (char c : str) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### String Length and Size

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, World!";

    std::cout << "length(): " << str.length() << std::endl;
    std::cout << "size(): " << str.size() << std::endl;        // Same as length()
    std::cout << "capacity(): " << str.capacity() << std::endl;
    std::cout << "max_size(): " << str.max_size() << std::endl;
    std::cout << std::boolalpha;
    std::cout << "empty(): " << str.empty() << std::endl;

    std::string empty;
    std::cout << "Empty string empty(): " << empty.empty() << std::endl;

    return 0;
}
```

## 6. Common String Operations

### Substring

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, World!";

    // substr(start_position, length)
    std::string sub1 = str.substr(0, 5);    // "Hello"
    std::string sub2 = str.substr(7, 5);    // "World"
    std::string sub3 = str.substr(7);       // "World!" (to end)

    std::cout << "Original: " << str << std::endl;
    std::cout << "Substring 1: " << sub1 << std::endl;
    std::cout << "Substring 2: " << sub2 << std::endl;
    std::cout << "Substring 3: " << sub3 << std::endl;

    return 0;
}
```

### Find and Search

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, World! Hello, C++!";

    // find() - returns position of first occurrence
    size_t pos1 = str.find("World");
    size_t pos2 = str.find("Hello");
    size_t pos3 = str.find("xyz");  // Not found

    std::cout << "Position of 'World': " << pos1 << std::endl;
    std::cout << "Position of 'Hello': " << pos2 << std::endl;

    if (pos3 == std::string::npos) {
        std::cout << "'xyz' not found" << std::endl;
    }

    // Find from position
    size_t pos4 = str.find("Hello", 1);  // Find after position 1
    std::cout << "Second 'Hello' at: " << pos4 << std::endl;

    // rfind() - find from end
    size_t pos5 = str.rfind("Hello");
    std::cout << "Last 'Hello' at: " << pos5 << std::endl;

    return 0;
}
```

### Replace

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, World!";

    // replace(start, length, new_string)
    str.replace(7, 5, "C++");
    std::cout << "After replace: " << str << std::endl;

    // Find and replace
    std::string text = "I love Python. Python is great!";
    size_t pos = text.find("Python");
    while (pos != std::string::npos) {
        text.replace(pos, 6, "C++");
        pos = text.find("Python", pos + 3);
    }
    std::cout << "After find and replace: " << text << std::endl;

    return 0;
}
```

### Insert and Erase

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, World!";

    // Insert
    str.insert(7, "Beautiful ");
    std::cout << "After insert: " << str << std::endl;

    // Erase
    str.erase(7, 10);  // Remove "Beautiful "
    std::cout << "After erase: " << str << std::endl;

    // Clear
    std::string temp = "temporary";
    temp.clear();
    std::cout << "After clear: '" << temp << "'" << std::endl;

    return 0;
}
```

### Compare Strings

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str1 = "Hello";
    std::string str2 = "World";
    std::string str3 = "Hello";

    // Using == operator
    std::cout << std::boolalpha;
    std::cout << "str1 == str2: " << (str1 == str2) << std::endl;
    std::cout << "str1 == str3: " << (str1 == str3) << std::endl;

    // Using compare()
    int result = str1.compare(str2);
    if (result < 0) {
        std::cout << "str1 < str2" << std::endl;
    } else if (result > 0) {
        std::cout << "str1 > str2" << std::endl;
    } else {
        std::cout << "str1 == str2" << std::endl;
    }

    // Other comparisons
    std::cout << "str1 != str2: " << (str1 != str2) << std::endl;
    std::cout << "str1 < str2: " << (str1 < str2) << std::endl;
    std::cout << "str1 > str2: " << (str1 > str2) << std::endl;

    return 0;
}
```

### String to Number Conversion

```cpp
#include <iostream>
#include <string>

int main() {
    // String to number
    std::string strInt = "12345";
    std::string strDouble = "3.14159";

    int numInt = std::stoi(strInt);
    double numDouble = std::stod(strDouble);

    std::cout << "String to int: " << numInt << std::endl;
    std::cout << "String to double: " << numDouble << std::endl;

    // Number to string (C++11)
    int value = 42;
    std::string strValue = std::to_string(value);

    std::cout << "Int to string: " << strValue << std::endl;

    return 0;
}
```

## 7. Practical Exercises

### Exercise 1: Name Parser

```cpp
#include <iostream>
#include <string>

int main() {
    std::string fullName;

    std::cout << "Enter your full name: ";
    std::getline(std::cin, fullName);

    size_t spacePos = fullName.find(' ');

    if (spacePos != std::string::npos) {
        std::string firstName = fullName.substr(0, spacePos);
        std::string lastName = fullName.substr(spacePos + 1);

        std::cout << "First Name: " << firstName << std::endl;
        std::cout << "Last Name: " << lastName << std::endl;
    } else {
        std::cout << "Please enter both first and last name." << std::endl;
    }

    return 0;
}
```

### Exercise 2: String Reversal

```cpp
#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string str;

    std::cout << "Enter a string: ";
    std::getline(std::cin, str);

    // Method 1: Using reverse()
    std::string reversed = str;
    std::reverse(reversed.begin(), reversed.end());

    std::cout << "Original: " << str << std::endl;
    std::cout << "Reversed: " << reversed << std::endl;

    // Method 2: Manual reversal
    std::string manual;
    for (int i = str.length() - 1; i >= 0; i--) {
        manual += str[i];
    }
    std::cout << "Manual reverse: " << manual << std::endl;

    return 0;
}
```

### Exercise 3: Word Counter

```cpp
#include <iostream>
#include <string>
#include <sstream>

int main() {
    std::string sentence;

    std::cout << "Enter a sentence: ";
    std::getline(std::cin, sentence);

    std::istringstream iss(sentence);
    std::string word;
    int wordCount = 0;

    while (iss >> word) {
        wordCount++;
    }

    std::cout << "Word count: " << wordCount << std::endl;

    return 0;
}
```

### Exercise 4: Email Validator (Simple)

```cpp
#include <iostream>
#include <string>

int main() {
    std::string email;

    std::cout << "Enter email address: ";
    std::getline(std::cin, email);

    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);

    if (atPos != std::string::npos &&
        dotPos != std::string::npos &&
        atPos > 0 &&
        dotPos > atPos + 1 &&
        dotPos < email.length() - 1) {
        std::cout << "Valid email format" << std::endl;
    } else {
        std::cout << "Invalid email format" << std::endl;
    }

    return 0;
}
```

### Exercise 5: String Statistics

```cpp
#include <iostream>
#include <string>
#include <cctype>

int main() {
    std::string text;

    std::cout << "Enter text: ";
    std::getline(std::cin, text);

    int letters = 0, digits = 0, spaces = 0, others = 0;

    for (char c : text) {
        if (std::isalpha(c)) {
            letters++;
        } else if (std::isdigit(c)) {
            digits++;
        } else if (std::isspace(c)) {
            spaces++;
        } else {
            others++;
        }
    }

    std::cout << "\n=== Statistics ===" << std::endl;
    std::cout << "Total characters: " << text.length() << std::endl;
    std::cout << "Letters: " << letters << std::endl;
    std::cout << "Digits: " << digits << std::endl;
    std::cout << "Spaces: " << spaces << std::endl;
    std::cout << "Others: " << others << std::endl;

    return 0;
}
```

## Complete Project: Text Processor

```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

void displayMenu() {
    std::cout << "\n=== Text Processor ===" << std::endl;
    std::cout << "1. Convert to uppercase" << std::endl;
    std::cout << "2. Convert to lowercase" << std::endl;
    std::cout << "3. Reverse text" << std::endl;
    std::cout << "4. Count words" << std::endl;
    std::cout << "5. Find and replace" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "Choice: ";
}

int main() {
    std::string text;

    std::cout << "Enter text: ";
    std::getline(std::cin, text);

    int choice;

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::string upper = text;
                std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
                std::cout << "Uppercase: " << upper << std::endl;
                break;
            }
            case 2: {
                std::string lower = text;
                std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                std::cout << "Lowercase: " << lower << std::endl;
                break;
            }
            case 3: {
                std::string reversed = text;
                std::reverse(reversed.begin(), reversed.end());
                std::cout << "Reversed: " << reversed << std::endl;
                break;
            }
            case 4: {
                int count = 0;
                bool inWord = false;
                for (char c : text) {
                    if (std::isspace(c)) {
                        inWord = false;
                    } else if (!inWord) {
                        inWord = true;
                        count++;
                    }
                }
                std::cout << "Word count: " << count << std::endl;
                break;
            }
            case 5: {
                std::string find, replace;
                std::cout << "Find: ";
                std::getline(std::cin, find);
                std::cout << "Replace with: ";
                std::getline(std::cin, replace);

                std::string result = text;
                size_t pos = result.find(find);
                while (pos != std::string::npos) {
                    result.replace(pos, find.length(), replace);
                    pos = result.find(find, pos + replace.length());
                }
                std::cout << "Result: " << result << std::endl;
                break;
            }
            case 6:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 6);

    return 0;
}
```

## Summary

In this lesson, you learned:
- Basic input with std::cin
- Input validation and error handling
- C++ strings (std::string)
- String input methods (cin, getline)
- String manipulation and operations
- Common string functions
- Practical string processing

## Next Lesson Preview

In Lesson 04, we'll cover:
- Arithmetic operators
- Relational operators
- Logical operators
- Bitwise operators
- Operator precedence

## Practice Problems

1. Create a palindrome checker
2. Build a simple password validator
3. Make a text-based calculator with string input
4. Create a string encryption/decryption program
5. Build a simple find-and-replace utility
