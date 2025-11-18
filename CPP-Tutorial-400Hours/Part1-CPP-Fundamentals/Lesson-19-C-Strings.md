# Lesson 19: C-Strings (char arrays)

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to C-Strings
2. Declaration and Initialization
3. String Input and Output
4. C-String Functions
5. String Manipulation
6. Comparing C-Strings
7. Converting Between Types
8. Exercises and Projects

---

## 1. Introduction to C-Strings

### What are C-Strings?

C-strings are arrays of characters terminated by a null character (`'\0'`). They're the traditional way C handles strings.

```cpp
#include <iostream>

int main()
{
    // C-string: character array with null terminator
    char greeting[] = "Hello";  // Automatically adds '\0'

    // Equivalent manual declaration
    char greeting2[] = {'H', 'e', 'l', 'l', 'o', '\0'};

    // Display
    std::cout << "Greeting 1: " << greeting << "\n";
    std::cout << "Greeting 2: " << greeting2 << "\n";

    // Show individual characters including null terminator
    std::cout << "\nIndividual characters:\n";
    for (int i = 0; i <= 5; i++)
    {
        if (greeting[i] == '\0')
            std::cout << "Index " << i << ": '\\0' (null terminator)\n";
        else
            std::cout << "Index " << i << ": '" << greeting[i] << "'\n";
    }

    return 0;
}
```

### Memory Layout:

```cpp
#include <iostream>

int main()
{
    char str[] = "Hello";

    std::cout << "String: " << str << "\n";
    std::cout << "Size: " << sizeof(str) << " bytes\n";  // 6 (including '\0')

    // Memory representation
    std::cout << "\nMemory layout:\n";
    for (int i = 0; i < sizeof(str); i++)
    {
        std::cout << "str[" << i << "] = ";
        if (str[i] == '\0')
            std::cout << "'\\0' (ASCII 0)\n";
        else
            std::cout << "'" << str[i] << "' (ASCII " << (int)str[i] << ")\n";
    }

    return 0;
}
```

---

## 2. Declaration and Initialization

### Different Ways to Declare:

```cpp
#include <iostream>

int main()
{
    // String literal
    char str1[] = "Hello";

    // Character array with size
    char str2[20] = "World";

    // Incomplete initialization (rest filled with '\0')
    char str3[10] = "Hi";

    // Manual character assignment
    char str4[6];
    str4[0] = 'H';
    str4[1] = 'e';
    str4[2] = 'l';
    str4[3] = 'l';
    str4[4] = 'o';
    str4[5] = '\0';  // Don't forget!

    std::cout << "str1: " << str1 << "\n";
    std::cout << "str2: " << str2 << "\n";
    std::cout << "str3: " << str3 << "\n";
    std::cout << "str4: " << str4 << "\n";

    return 0;
}
```

---

## 3. String Input and Output

### Input Methods:

```cpp
#include <iostream>
#include <cstring>

int main()
{
    char name[50];
    char sentence[100];

    // Using cin (stops at whitespace)
    std::cout << "Enter your name: ";
    std::cin >> name;
    std::cout << "Hello, " << name << "!\n\n";

    // Clear input buffer
    std::cin.ignore(1000, '\n');

    // Using cin.getline() (reads entire line)
    std::cout << "Enter a sentence: ";
    std::cin.getline(sentence, 100);
    std::cout << "You said: " << sentence << "\n\n";

    // Using cin.get()
    char city[50];
    std::cout << "Enter your city: ";
    std::cin.get(city, 50);
    std::cout << "City: " << city << "\n";

    return 0;
}
```

---

## 4. C-String Functions

### strlen() - String Length:

```cpp
#include <iostream>
#include <cstring>

int main()
{
    char message[] = "Hello, World!";

    // strlen() returns length WITHOUT null terminator
    int length = strlen(message);

    std::cout << "Message: " << message << "\n";
    std::cout << "Length: " << length << "\n";
    std::cout << "Size in memory: " << sizeof(message) << " bytes\n";

    return 0;
}
```

### strcpy() - String Copy:

```cpp
#include <iostream>
#include <cstring>

int main()
{
    char source[] = "Hello";
    char destination[20];

    // Copy source to destination
    strcpy(destination, source);

    std::cout << "Source: " << source << "\n";
    std::cout << "Destination: " << destination << "\n";

    // strncpy - safer version (specify max chars)
    char dest2[10];
    strncpy(dest2, "Very long string", 9);
    dest2[9] = '\0';  // Ensure null termination

    std::cout << "Partial copy: " << dest2 << "\n";

    return 0;
}
```

### strcat() - String Concatenation:

```cpp
#include <iostream>
#include <cstring>

int main()
{
    char greeting[50] = "Hello";
    char name[] = " World";

    // Concatenate name to greeting
    strcat(greeting, name);

    std::cout << "Result: " << greeting << "\n";

    // strncat - safer version
    char msg[30] = "Good";
    strncat(msg, " morning", 8);
    std::cout << "Message: " << msg << "\n";

    return 0;
}
```

### strcmp() - String Comparison:

```cpp
#include <iostream>
#include <cstring>

int main()
{
    char str1[] = "apple";
    char str2[] = "banana";
    char str3[] = "apple";

    // strcmp returns:
    // 0 if equal
    // < 0 if str1 < str2
    // > 0 if str1 > str2

    int result1 = strcmp(str1, str2);
    int result2 = strcmp(str1, str3);
    int result3 = strcmp(str2, str1);

    std::cout << "Comparing 'apple' and 'banana': " << result1 << "\n";
    std::cout << "Comparing 'apple' and 'apple': " << result2 << "\n";
    std::cout << "Comparing 'banana' and 'apple': " << result3 << "\n";

    if (strcmp(str1, str3) == 0)
    {
        std::cout << "\nstr1 and str3 are equal\n";
    }

    return 0;
}
```

---

## 5. String Manipulation

### Converting Case:

```cpp
#include <iostream>
#include <cctype>
#include <cstring>

void toUpperCase(char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }
}

void toLowerCase(char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}

int main()
{
    char text1[] = "Hello World";
    char text2[] = "HELLO WORLD";

    std::cout << "Original: " << text1 << "\n";

    toUpperCase(text1);
    std::cout << "Uppercase: " << text1 << "\n";

    toLowerCase(text2);
    std::cout << "Lowercase: " << text2 << "\n";

    return 0;
}
```

### Reversing Strings:

```cpp
#include <iostream>
#include <cstring>

void reverseString(char str[])
{
    int length = strlen(str);

    for (int i = 0; i < length / 2; i++)
    {
        char temp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = temp;
    }
}

int main()
{
    char word[] = "Hello";

    std::cout << "Original: " << word << "\n";

    reverseString(word);

    std::cout << "Reversed: " << word << "\n";

    return 0;
}
```

### Counting Characters:

```cpp
#include <iostream>
#include <cstring>
#include <cctype>

void analyzeString(const char str[])
{
    int letters = 0, digits = 0, spaces = 0, others = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isalpha(str[i]))
            letters++;
        else if (isdigit(str[i]))
            digits++;
        else if (isspace(str[i]))
            spaces++;
        else
            others++;
    }

    std::cout << "Analysis of: \"" << str << "\"\n";
    std::cout << "Letters: " << letters << "\n";
    std::cout << "Digits: " << digits << "\n";
    std::cout << "Spaces: " << spaces << "\n";
    std::cout << "Others: " << others << "\n";
}

int main()
{
    char text[] = "Hello World 123!";

    analyzeString(text);

    return 0;
}
```

---

## 6. Comparing C-Strings

### Custom Comparison Functions:

```cpp
#include <iostream>
#include <cctype>

bool isEqual(const char str1[], const char str2[])
{
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i])
            return false;
        i++;
    }
    return str1[i] == str2[i];  // Both must be '\0'
}

bool isEqualIgnoreCase(const char str1[], const char str2[])
{
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (tolower(str1[i]) != tolower(str2[i]))
            return false;
        i++;
    }
    return str1[i] == str2[i];
}

int main()
{
    char str1[] = "Hello";
    char str2[] = "hello";
    char str3[] = "Hello";

    std::cout << "str1: " << str1 << "\n";
    std::cout << "str2: " << str2 << "\n";
    std::cout << "str3: " << str3 << "\n\n";

    std::cout << "str1 == str2 (case-sensitive): "
              << (isEqual(str1, str2) ? "true" : "false") << "\n";

    std::cout << "str1 == str3 (case-sensitive): "
              << (isEqual(str1, str3) ? "true" : "false") << "\n";

    std::cout << "str1 == str2 (ignore case): "
              << (isEqualIgnoreCase(str1, str2) ? "true" : "false") << "\n";

    return 0;
}
```

---

## 7. Converting Between Types

### String to Number:

```cpp
#include <iostream>
#include <cstdlib>
#include <cstring>

int main()
{
    char numStr1[] = "12345";
    char numStr2[] = "3.14159";
    char numStr3[] = "42 is the answer";

    // atoi - string to integer
    int num1 = atoi(numStr1);
    std::cout << "Integer: " << num1 << "\n";

    // atof - string to double
    double num2 = atof(numStr2);
    std::cout << "Double: " << num2 << "\n";

    // atoi stops at first non-digit
    int num3 = atoi(numStr3);
    std::cout << "Parsed from mixed string: " << num3 << "\n";

    return 0;
}
```

### Number to String:

```cpp
#include <iostream>
#include <cstdio>

int main()
{
    int number = 12345;
    double pi = 3.14159;

    char buffer1[20];
    char buffer2[20];

    // sprintf - formatted print to string
    sprintf(buffer1, "%d", number);
    sprintf(buffer2, "%.2f", pi);

    std::cout << "Integer as string: " << buffer1 << "\n";
    std::cout << "Double as string: " << buffer2 << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: String Utilities Library

```cpp
#include <iostream>
#include <cstring>
#include <cctype>

class StringUtils
{
public:
    static bool isPalindrome(const char str[])
    {
        int length = strlen(str);

        for (int i = 0; i < length / 2; i++)
        {
            if (tolower(str[i]) != tolower(str[length - 1 - i]))
                return false;
        }
        return true;
    }

    static int countWords(const char str[])
    {
        int count = 0;
        bool inWord = false;

        for (int i = 0; str[i] != '\0'; i++)
        {
            if (isspace(str[i]))
            {
                inWord = false;
            }
            else if (!inWord)
            {
                inWord = true;
                count++;
            }
        }

        return count;
    }

    static void trim(char str[])
    {
        int start = 0;
        while (isspace(str[start]))
            start++;

        int end = strlen(str) - 1;
        while (end >= 0 && isspace(str[end]))
            end--;

        int j = 0;
        for (int i = start; i <= end; i++)
        {
            str[j++] = str[i];
        }
        str[j] = '\0';
    }

    static void replaceChar(char str[], char oldChar, char newChar)
    {
        for (int i = 0; str[i] != '\0'; i++)
        {
            if (str[i] == oldChar)
            {
                str[i] = newChar;
            }
        }
    }
};

int main()
{
    char word[] = "racecar";
    std::cout << "Is 'racecar' a palindrome? "
              << (StringUtils::isPalindrome(word) ? "Yes" : "No") << "\n";

    char sentence[] = "Hello World from C++";
    std::cout << "Word count in '" << sentence << "': "
              << StringUtils::countWords(sentence) << "\n";

    char text[] = "   spaces   ";
    std::cout << "Before trim: '" << text << "'\n";
    StringUtils::trim(text);
    std::cout << "After trim: '" << text << "'\n";

    char msg[] = "Hello World";
    StringUtils::replaceChar(msg, 'o', '0');
    std::cout << "After replacing 'o' with '0': " << msg << "\n";

    return 0;
}
```

---

## Practice Project: Text Editor Functions

```cpp
#include <iostream>
#include <cstring>
#include <cctype>

class TextEditor
{
private:
    char buffer[1000];

public:
    TextEditor()
    {
        buffer[0] = '\0';
    }

    void setText(const char text[])
    {
        strncpy(buffer, text, 999);
        buffer[999] = '\0';
    }

    void display()
    {
        std::cout << "Text: " << buffer << "\n";
    }

    void toUpper()
    {
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            buffer[i] = toupper(buffer[i]);
        }
    }

    void toLower()
    {
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            buffer[i] = tolower(buffer[i]);
        }
    }

    int find(const char searchTerm[])
    {
        const char* result = strstr(buffer, searchTerm);
        if (result != nullptr)
        {
            return result - buffer;
        }
        return -1;
    }

    void replace(const char oldStr[], const char newStr[])
    {
        char temp[1000];
        char* pos = strstr(buffer, oldStr);

        if (pos != nullptr)
        {
            int index = pos - buffer;

            // Copy before match
            strncpy(temp, buffer, index);
            temp[index] = '\0';

            // Add replacement
            strcat(temp, newStr);

            // Add after match
            strcat(temp, buffer + index + strlen(oldStr));

            strcpy(buffer, temp);
        }
    }

    int countOccurrences(char ch)
    {
        int count = 0;
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (buffer[i] == ch)
                count++;
        }
        return count;
    }

    void statistics()
    {
        int chars = strlen(buffer);
        int words = 0;
        int lines = 1;

        bool inWord = false;
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (buffer[i] == '\n')
                lines++;

            if (isspace(buffer[i]))
            {
                inWord = false;
            }
            else if (!inWord)
            {
                inWord = true;
                words++;
            }
        }

        std::cout << "\n=== STATISTICS ===\n";
        std::cout << "Characters: " << chars << "\n";
        std::cout << "Words: " << words << "\n";
        std::cout << "Lines: " << lines << "\n";
    }
};

int main()
{
    TextEditor editor;

    editor.setText("Hello World! This is a test.");
    editor.display();
    editor.statistics();

    std::cout << "\nConverting to uppercase...\n";
    editor.toUpper();
    editor.display();

    std::cout << "\nSearching for 'WORLD'...\n";
    int pos = editor.find("WORLD");
    if (pos != -1)
    {
        std::cout << "Found at position: " << pos << "\n";
    }

    std::cout << "\nReplacing 'TEST' with 'EXAMPLE'...\n";
    editor.replace("TEST", "EXAMPLE");
    editor.display();

    std::cout << "\nCount of 'O': " << editor.countOccurrences('O') << "\n";

    return 0;
}
```

---

## Key Takeaways

1. C-strings are null-terminated char arrays
2. Always account for '\0' in size calculations
3. Use cstring functions for manipulation
4. Be careful with buffer sizes to avoid overflow
5. strcmp() returns 0 for equal strings
6. strncpy() and strncat() are safer alternatives
7. Convert between strings and numbers using stdlib functions
8. Modern C++ std::string is usually preferred

---

## Summary Checklist

Before moving to Lesson 20, ensure you can:
- [ ] Declare and initialize C-strings
- [ ] Use cstring functions
- [ ] Manipulate C-strings safely
- [ ] Compare strings correctly
- [ ] Convert between strings and numbers
- [ ] Handle null terminators properly
- [ ] Understand buffer overflow risks
- [ ] Know when to use C-strings vs std::string

---

## Next Lesson Preview

In **Lesson 20**, we'll explore:
- std::string class in detail
- String member functions
- String operations and algorithms
- Performance considerations
- When to use std::string vs C-strings

**C-strings mastered - modern strings next!**
