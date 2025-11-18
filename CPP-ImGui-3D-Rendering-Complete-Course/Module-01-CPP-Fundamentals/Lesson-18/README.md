# Lesson 18: C-Style Strings

**Duration:** 2 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Understand C-style strings and character arrays
- Declare and initialize C-style strings
- Use standard C string functions
- Manipulate C-style strings
- Understand the difference between character arrays and C-style strings
- Work with string input and output

## 1. Introduction to C-Style Strings

### 1.1 What are C-Style Strings?

C-style strings are arrays of characters terminated by a null character (`'\0'`).

```cpp
#include <iostream>
using namespace std;

int main() {
    // Character array vs C-style string
    char charArray[5] = {'H', 'e', 'l', 'l', 'o'};  // Not a string (no null terminator)
    char cString[6] = {'H', 'e', 'l', 'l', 'o', '\0'};  // C-style string

    // Simpler initialization
    char str[] = "Hello";  // Automatically adds '\0'

    cout << "C-style string: " << str << endl;
    // cout << charArray << endl;  // Would print garbage after 'Hello'

    // Accessing individual characters
    cout << "First character: " << str[0] << endl;
    cout << "Null terminator at index: " << strlen(str) << endl;

    return 0;
}
```

### 1.2 String Declaration and Initialization

```cpp
#include <iostream>
#include <cstring>
using namespace std;

int main() {
    // Different ways to declare C-style strings

    // Method 1: String literal
    char str1[] = "Hello";  // Size is 6 (5 chars + '\0')

    // Method 2: Explicit array size
    char str2[10] = "Hello";  // Rest filled with '\0'

    // Method 3: Character by character
    char str3[6];
    str3[0] = 'H';
    str3[1] = 'e';
    str3[2] = 'l';
    str3[3] = 'l';
    str3[4] = 'o';
    str3[5] = '\0';  // Important!

    // Method 4: Using strcpy
    char str4[20];
    strcpy(str4, "World");

    cout << "str1: " << str1 << endl;
    cout << "str2: " << str2 << endl;
    cout << "str3: " << str3 << endl;
    cout << "str4: " << str4 << endl;

    return 0;
}
```

### 1.3 String Input and Output

```cpp
#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char name[50];
    char sentence[100];

    // cin reads until whitespace
    cout << "Enter your name: ";
    cin >> name;
    cout << "Hello, " << name << endl;

    // cin.getline() reads entire line
    cin.ignore();  // Clear newline from previous input
    cout << "Enter a sentence: ";
    cin.getline(sentence, 100);
    cout << "You said: " << sentence << endl;

    return 0;
}
```

## 2. C String Library Functions

### 2.1 Common String Functions

```cpp
#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char str1[50] = "Hello";
    char str2[50] = "World";
    char str3[50];

    // strlen - get string length
    cout << "Length of str1: " << strlen(str1) << endl;

    // strcpy - copy string
    strcpy(str3, str1);
    cout << "str3 after copy: " << str3 << endl;

    // strcat - concatenate strings
    strcat(str1, " ");
    strcat(str1, str2);
    cout << "Concatenated: " << str1 << endl;

    // strcmp - compare strings
    if (strcmp(str2, "World") == 0) {
        cout << "str2 is 'World'" << endl;
    }

    // strchr - find character
    char* pos = strchr(str2, 'r');
    if (pos != nullptr) {
        cout << "Found 'r' at position: " << (pos - str2) << endl;
    }

    return 0;
}
```

### 2.2 Safe String Functions (strn* variants)

```cpp
#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char dest[10];
    char source[] = "This is a very long string";

    // strcpy vs strncpy (safer)
    // strcpy(dest, source);  // DANGEROUS - buffer overflow!

    strncpy(dest, source, 9);  // Copy at most 9 characters
    dest[9] = '\0';  // Ensure null termination
    cout << "Safe copy: " << dest << endl;

    // strncat - safe concatenation
    char str1[20] = "Hello";
    char str2[] = " World!!!";
    strncat(str1, str2, 6);  // Concatenate at most 6 chars
    cout << "Safe concat: " << str1 << endl;

    // strncmp - compare first n characters
    if (strncmp("Hello", "Help", 3) == 0) {
        cout << "First 3 characters match" << endl;
    }

    return 0;
}
```

## 3. String Manipulation

### 3.1 Character Operations

```cpp
#include <iostream>
#include <cctype>
#include <cstring>
using namespace std;

void toUpperCase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

void toLowerCase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

void capitalize(char* str) {
    bool capitalizeNext = true;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace(str[i])) {
            capitalizeNext = true;
        } else if (capitalizeNext) {
            str[i] = toupper(str[i]);
            capitalizeNext = false;
        } else {
            str[i] = tolower(str[i]);
        }
    }
}

int main() {
    char text1[] = "hello world";
    char text2[] = "HELLO WORLD";
    char text3[] = "hello world from c++";

    toUpperCase(text1);
    cout << "Uppercase: " << text1 << endl;

    toLowerCase(text2);
    cout << "Lowercase: " << text2 << endl;

    capitalize(text3);
    cout << "Capitalized: " << text3 << endl;

    return 0;
}
```

### 3.2 String Searching and Replacing

```cpp
#include <iostream>
#include <cstring>
using namespace std;

int countOccurrences(const char* str, char ch) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ch) {
            count++;
        }
    }
    return count;
}

void replaceChar(char* str, char oldChar, char newChar) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == oldChar) {
            str[i] = newChar;
        }
    }
}

bool contains(const char* str, const char* substr) {
    return strstr(str, substr) != nullptr;
}

int main() {
    char text[] = "Hello World! Hello C++!";

    cout << "Count of 'l': " << countOccurrences(text, 'l') << endl;

    replaceChar(text, 'l', 'L');
    cout << "After replacement: " << text << endl;

    cout << "Contains 'World': " << (contains(text, "World") ? "Yes" : "No") << endl;
    cout << "Contains 'Python': " << (contains(text, "Python") ? "Yes" : "No") << endl;

    return 0;
}
```

### 3.3 String Reversal

```cpp
#include <iostream>
#include <cstring>
using namespace std;

void reverseString(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

bool isPalindrome(const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (tolower(str[i]) != tolower(str[len - 1 - i])) {
            return false;
        }
    }
    return true;
}

int main() {
    char str1[] = "Hello";
    char str2[] = "racecar";
    char str3[] = "A man a plan a canal Panama";

    reverseString(str1);
    cout << "Reversed: " << str1 << endl;

    cout << "'" << str2 << "' is " << (isPalindrome(str2) ? "" : "not ")
         << "a palindrome" << endl;

    return 0;
}
```

## 4. Practical Examples

### Example 1: Word Counter

```cpp
#include <iostream>
#include <cctype>
using namespace std;

int countWords(const char* str) {
    int count = 0;
    bool inWord = false;

    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace(str[i])) {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            count++;
        }
    }

    return count;
}

int countVowels(const char* str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char ch = tolower(str[i]);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            count++;
        }
    }
    return count;
}

int main() {
    char sentence[200];

    cout << "Enter a sentence: ";
    cin.getline(sentence, 200);

    cout << "\nAnalysis:" << endl;
    cout << "Characters: " << strlen(sentence) << endl;
    cout << "Words: " << countWords(sentence) << endl;
    cout << "Vowels: " << countVowels(sentence) << endl;

    return 0;
}
```

### Example 2: String Tokenization

```cpp
#include <iostream>
#include <cstring>
using namespace std;

void splitString(char* str, const char* delimiter) {
    char* token = strtok(str, delimiter);

    int count = 1;
    while (token != nullptr) {
        cout << "Token " << count++ << ": " << token << endl;
        token = strtok(nullptr, delimiter);
    }
}

int main() {
    char sentence[] = "Hello,World,C++,Programming";

    cout << "Original: " << sentence << endl;
    cout << "\nTokens:" << endl;
    splitString(sentence, ",");

    // Note: strtok modifies the original string
    char csv[] = "John,Doe,25,Engineer";
    cout << "\nParsing CSV:" << endl;
    splitString(csv, ",");

    return 0;
}
```

### Example 3: Simple Text Encryption

```cpp
#include <iostream>
#include <cstring>
using namespace std;

void caesarCipher(char* str, int shift) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i])) {
            char base = isupper(str[i]) ? 'A' : 'a';
            str[i] = (str[i] - base + shift) % 26 + base;
        }
    }
}

void rot13(char* str) {
    caesarCipher(str, 13);
}

int main() {
    char message1[] = "Hello World";
    char message2[] = "Secret Message";

    cout << "Original: " << message1 << endl;
    caesarCipher(message1, 3);
    cout << "Caesar cipher (+3): " << message1 << endl;
    caesarCipher(message1, -3);  // Decrypt
    cout << "Decrypted: " << message1 << endl;

    cout << "\nOriginal: " << message2 << endl;
    rot13(message2);
    cout << "ROT13: " << message2 << endl;
    rot13(message2);  // ROT13 is its own inverse
    cout << "Decrypted: " << message2 << endl;

    return 0;
}
```

## 5. String Arrays

### 5.1 Array of Strings

```cpp
#include <iostream>
#include <cstring>
using namespace std;

const int MAX_NAMES = 5;
const int MAX_LENGTH = 50;

int main() {
    char names[MAX_NAMES][MAX_LENGTH] = {
        "Alice",
        "Bob",
        "Charlie",
        "Diana",
        "Eve"
    };

    cout << "Names list:" << endl;
    for (int i = 0; i < MAX_NAMES; i++) {
        cout << (i + 1) << ". " << names[i] << endl;
    }

    // Search for a name
    char searchName[MAX_LENGTH];
    cout << "\nSearch for: ";
    cin >> searchName;

    bool found = false;
    for (int i = 0; i < MAX_NAMES; i++) {
        if (strcmp(names[i], searchName) == 0) {
            cout << "Found at position " << (i + 1) << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Not found" << endl;
    }

    return 0;
}
```

### 5.2 Sorting String Array

```cpp
#include <iostream>
#include <cstring>
using namespace std;

void sortStrings(char arr[][50], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (strcmp(arr[j], arr[j + 1]) > 0) {
                char temp[50];
                strcpy(temp, arr[j]);
                strcpy(arr[j], arr[j + 1]);
                strcpy(arr[j + 1], temp);
            }
        }
    }
}

int main() {
    char words[][50] = {"Zebra", "Apple", "Mango", "Banana", "Cherry"};
    int size = 5;

    cout << "Original order:" << endl;
    for (int i = 0; i < size; i++) {
        cout << words[i] << endl;
    }

    sortStrings(words, size);

    cout << "\nSorted order:" << endl;
    for (int i = 0; i < size; i++) {
        cout << words[i] << endl;
    }

    return 0;
}
```

## Exercises

### Exercise 1: String Validator
Write a function that validates if a string contains only alphabetic characters, only digits, or is alphanumeric.

### Exercise 2: Trim Function
Implement functions to trim leading and trailing whitespace from a string.

### Exercise 3: String Comparison
Create a case-insensitive string comparison function.

### Exercise 4: Anagram Checker
Write a function that checks if two strings are anagrams of each other.

### Exercise 5: Simple URL Parser
Parse a URL string into protocol, domain, and path components.

## Exercise Solutions

### Solution 1: String Validator

```cpp
#include <iostream>
#include <cctype>
using namespace std;

bool isAlpha(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) {
            return false;
        }
    }
    return true;
}

bool isNumeric(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool isAlphanumeric(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }
    return true;
}

int main() {
    const char* test1 = "Hello";
    const char* test2 = "12345";
    const char* test3 = "Hello123";
    const char* test4 = "Hello World";

    cout << "'" << test1 << "' is alpha: " << (isAlpha(test1) ? "Yes" : "No") << endl;
    cout << "'" << test2 << "' is numeric: " << (isNumeric(test2) ? "Yes" : "No") << endl;
    cout << "'" << test3 << "' is alphanumeric: " << (isAlphanumeric(test3) ? "Yes" : "No") << endl;
    cout << "'" << test4 << "' is alphanumeric: " << (isAlphanumeric(test4) ? "Yes" : "No") << endl;

    return 0;
}
```

### Solution 2: Trim Function

```cpp
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

void trimLeft(char* str) {
    int i = 0;
    while (str[i] != '\0' && isspace(str[i])) {
        i++;
    }

    if (i > 0) {
        int j = 0;
        while (str[i] != '\0') {
            str[j++] = str[i++];
        }
        str[j] = '\0';
    }
}

void trimRight(char* str) {
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        len--;
    }
    str[len] = '\0';
}

void trim(char* str) {
    trimLeft(str);
    trimRight(str);
}

int main() {
    char text1[] = "   Hello World   ";
    char text2[] = "   Leading spaces";
    char text3[] = "Trailing spaces   ";

    trim(text1);
    trimLeft(text2);
    trimRight(text3);

    cout << "Trimmed: '" << text1 << "'" << endl;
    cout << "Left trimmed: '" << text2 << "'" << endl;
    cout << "Right trimmed: '" << text3 << "'" << endl;

    return 0;
}
```

### Solution 3: String Comparison

```cpp
#include <iostream>
#include <cctype>
using namespace std;

int strcmpIgnoreCase(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        char c1 = tolower(str1[i]);
        char c2 = tolower(str2[i]);

        if (c1 != c2) {
            return c1 - c2;
        }
        i++;
    }
    return tolower(str1[i]) - tolower(str2[i]);
}

int main() {
    const char* str1 = "Hello";
    const char* str2 = "hello";
    const char* str3 = "HELLO";
    const char* str4 = "World";

    cout << "Comparing (case-insensitive):" << endl;
    cout << "'" << str1 << "' vs '" << str2 << "': "
         << (strcmpIgnoreCase(str1, str2) == 0 ? "Equal" : "Not equal") << endl;

    cout << "'" << str1 << "' vs '" << str3 << "': "
         << (strcmpIgnoreCase(str1, str3) == 0 ? "Equal" : "Not equal") << endl;

    cout << "'" << str1 << "' vs '" << str4 << "': "
         << (strcmpIgnoreCase(str1, str4) == 0 ? "Equal" : "Not equal") << endl;

    return 0;
}
```

### Solution 4: Anagram Checker

```cpp
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

void sortString(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (tolower(str[j]) > tolower(str[j + 1])) {
                char temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;
            }
        }
    }
}

bool areAnagrams(const char* str1, const char* str2) {
    // Ignore case and spaces
    char temp1[100], temp2[100];
    int idx1 = 0, idx2 = 0;

    // Copy non-space characters
    for (int i = 0; str1[i] != '\0'; i++) {
        if (!isspace(str1[i])) {
            temp1[idx1++] = tolower(str1[i]);
        }
    }
    temp1[idx1] = '\0';

    for (int i = 0; str2[i] != '\0'; i++) {
        if (!isspace(str2[i])) {
            temp2[idx2++] = tolower(str2[i]);
        }
    }
    temp2[idx2] = '\0';

    // Check lengths
    if (strlen(temp1) != strlen(temp2)) {
        return false;
    }

    // Sort and compare
    sortString(temp1);
    sortString(temp2);

    return strcmp(temp1, temp2) == 0;
}

int main() {
    const char* str1 = "listen";
    const char* str2 = "silent";
    const char* str3 = "hello";
    const char* str4 = "world";

    cout << "'" << str1 << "' and '" << str2 << "' are "
         << (areAnagrams(str1, str2) ? "" : "not ") << "anagrams" << endl;

    cout << "'" << str3 << "' and '" << str4 << "' are "
         << (areAnagrams(str3, str4) ? "" : "not ") << "anagrams" << endl;

    return 0;
}
```

### Solution 5: Simple URL Parser

```cpp
#include <iostream>
#include <cstring>
using namespace std;

void parseURL(const char* url, char* protocol, char* domain, char* path) {
    // Find protocol
    const char* protocolEnd = strstr(url, "://");
    if (protocolEnd != nullptr) {
        int len = protocolEnd - url;
        strncpy(protocol, url, len);
        protocol[len] = '\0';
        url = protocolEnd + 3;  // Skip "://"
    } else {
        strcpy(protocol, "");
    }

    // Find path
    const char* pathStart = strchr(url, '/');
    if (pathStart != nullptr) {
        int len = pathStart - url;
        strncpy(domain, url, len);
        domain[len] = '\0';
        strcpy(path, pathStart);
    } else {
        strcpy(domain, url);
        strcpy(path, "/");
    }
}

int main() {
    const char* url1 = "https://www.example.com/path/to/page";
    const char* url2 = "http://github.com/user/repo";
    const char* url3 = "ftp://files.server.org";

    char protocol[20], domain[100], path[100];

    parseURL(url1, protocol, domain, path);
    cout << "URL: " << url1 << endl;
    cout << "  Protocol: " << protocol << endl;
    cout << "  Domain: " << domain << endl;
    cout << "  Path: " << path << endl << endl;

    parseURL(url2, protocol, domain, path);
    cout << "URL: " << url2 << endl;
    cout << "  Protocol: " << protocol << endl;
    cout << "  Domain: " << domain << endl;
    cout << "  Path: " << path << endl << endl;

    parseURL(url3, protocol, domain, path);
    cout << "URL: " << url3 << endl;
    cout << "  Protocol: " << protocol << endl;
    cout << "  Domain: " << domain << endl;
    cout << "  Path: " << path << endl;

    return 0;
}
```

## Summary

In this lesson, you learned:

- **C-style strings** are character arrays terminated with '\0'
- Always ensure strings are **null-terminated**
- Use **<cstring>** library functions for string operations
- **strn* functions** are safer than their non-n counterparts
- Character arrays and C-style strings are **different**
- **Array of strings** uses 2D character arrays
- Always check **buffer sizes** to prevent overflow

## Checklist

- [ ] I understand C-style strings and null termination
- [ ] I can declare and initialize C-style strings
- [ ] I can use standard C string library functions
- [ ] I understand the difference between strcpy and strncpy
- [ ] I can manipulate strings (search, replace, reverse)
- [ ] I completed all exercises and understand the solutions
- [ ] I understand buffer overflow risks
- [ ] I know when to use C-style strings vs std::string

## Additional Resources

- **Next Lesson:** std::string Class
- **Related Topics:** String Streams, Regular Expressions, Unicode
- **Practice:** Implement more string algorithms and text processing tools

---

**Congratulations!** You've completed Lesson 18. You now understand C-style strings, which are fundamental to C++ and still widely used in systems programming.
