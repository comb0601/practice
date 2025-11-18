# Lesson 19: std::string Class

**Duration:** 3 hours

## Learning Objectives

By the end of this lesson, you will be able to:
- Understand and use the std::string class
- Perform common string operations with std::string
- Compare std::string with C-style strings
- Use string methods effectively
- Work with string iterators
- Understand string performance considerations

## 1. Introduction to std::string

### 1.1 Basic std::string Usage

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    // Declaration and initialization
    string str1;                    // Empty string
    string str2 = "Hello";          // Initialize with literal
    string str3("World");           // Constructor initialization
    string str4(5, 'A');           // "AAAAA"
    string str5 = str2;             // Copy

    cout << "str1: '" << str1 << "'" << endl;
    cout << "str2: " << str2 << endl;
    cout << "str3: " << str3 << endl;
    cout << "str4: " << str4 << endl;
    cout << "str5: " << str5 << endl;

    // String assignment
    str1 = "New value";
    cout << "str1 after assignment: " << str1 << endl;

    return 0;
}
```

### 1.2 String Input and Output

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string name, sentence;

    // cin reads until whitespace
    cout << "Enter your name: ";
    cin >> name;
    cout << "Hello, " << name << endl;

    // getline reads entire line
    cin.ignore();  // Clear newline from buffer
    cout << "Enter a sentence: ";
    getline(cin, sentence);
    cout << "You said: " << sentence << endl;

    // getline with custom delimiter
    cout << "Enter items separated by commas: ";
    string items;
    getline(cin, items, ',');
    cout << "First item: " << items << endl;

    return 0;
}
```

## 2. String Operations

### 2.1 String Concatenation

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string first = "Hello";
    string second = "World";

    // Using + operator
    string result1 = first + " " + second;
    cout << "result1: " << result1 << endl;

    // Using += operator
    string result2 = first;
    result2 += " ";
    result2 += second;
    cout << "result2: " << result2 << endl;

    // Using append()
    string result3 = first;
    result3.append(" ").append(second);
    cout << "result3: " << result3 << endl;

    // Concatenate with C-string
    string result4 = first + " C++";
    cout << "result4: " << result4 << endl;

    return 0;
}
```

### 2.2 String Length and Capacity

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str = "Hello World";

    // Length/size (same thing)
    cout << "Length: " << str.length() << endl;
    cout << "Size: " << str.size() << endl;

    // Capacity and reserve
    cout << "Capacity: " << str.capacity() << endl;
    str.reserve(100);
    cout << "After reserve(100): " << str.capacity() << endl;

    // Empty check
    string empty;
    cout << "Is empty string empty? " << (empty.empty() ? "Yes" : "No") << endl;

    // Clear
    str.clear();
    cout << "After clear, length: " << str.length() << endl;

    return 0;
}
```

### 2.3 Accessing Characters

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str = "Hello";

    // Using [] operator
    cout << "First character: " << str[0] << endl;
    cout << "Last character: " << str[str.length() - 1] << endl;

    // Using at() method (with bounds checking)
    cout << "Character at index 2: " << str.at(2) << endl;

    // Modify characters
    str[0] = 'h';
    cout << "After modification: " << str << endl;

    // front() and back()
    cout << "Front: " << str.front() << endl;
    cout << "Back: " << str.back() << endl;

    // Iterate through string
    cout << "All characters: ";
    for (size_t i = 0; i < str.length(); i++) {
        cout << str[i] << " ";
    }
    cout << endl;

    return 0;
}
```

## 3. String Methods

### 3.1 Substring and Search

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str = "Hello World from C++";

    // Substring
    string sub1 = str.substr(0, 5);      // "Hello"
    string sub2 = str.substr(6, 5);      // "World"
    string sub3 = str.substr(17);        // "C++" (from index 17 to end)
    cout << "sub1: " << sub1 << endl;
    cout << "sub2: " << sub2 << endl;
    cout << "sub3: " << sub3 << endl;

    // Find
    size_t pos1 = str.find("World");
    if (pos1 != string::npos) {
        cout << "Found 'World' at position: " << pos1 << endl;
    }

    size_t pos2 = str.find("Python");
    if (pos2 == string::npos) {
        cout << "'Python' not found" << endl;
    }

    // Find character
    size_t pos3 = str.find('o');
    cout << "First 'o' at position: " << pos3 << endl;

    // Find last occurrence
    size_t pos4 = str.rfind('o');
    cout << "Last 'o' at position: " << pos4 << endl;

    return 0;
}
```

### 3.2 String Modification

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str = "Hello World";

    // Insert
    str.insert(5, ",");
    cout << "After insert: " << str << endl;

    // Erase
    str.erase(5, 1);  // Remove comma
    cout << "After erase: " << str << endl;

    // Replace
    str.replace(6, 5, "C++");
    cout << "After replace: " << str << endl;

    // Push and pop
    str.push_back('!');
    cout << "After push_back: " << str << endl;

    str.pop_back();
    cout << "After pop_back: " << str << endl;

    return 0;
}
```

### 3.3 String Comparison

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str1 = "Apple";
    string str2 = "Banana";
    string str3 = "Apple";

    // Using == operator
    if (str1 == str3) {
        cout << str1 << " equals " << str3 << endl;
    }

    // Using compare() method
    int cmp = str1.compare(str2);
    if (cmp < 0) {
        cout << str1 << " comes before " << str2 << endl;
    }

    // Relational operators
    if (str1 < str2) {
        cout << str1 << " < " << str2 << endl;
    }

    return 0;
}
```

## 4. Advanced String Operations

### 4.1 String Conversion

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    // String to number
    string numStr1 = "42";
    string numStr2 = "3.14159";

    int num1 = stoi(numStr1);
    double num2 = stod(numStr2);

    cout << "Integer: " << num1 << endl;
    cout << "Double: " << num2 << endl;

    // Number to string
    int value1 = 100;
    double value2 = 2.71828;

    string str1 = to_string(value1);
    string str2 = to_string(value2);

    cout << "From int: " << str1 << endl;
    cout << "From double: " << str2 << endl;

    return 0;
}
```

### 4.2 String Iterators

```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    string str = "Hello World";

    // Forward iteration
    cout << "Forward: ";
    for (string::iterator it = str.begin(); it != str.end(); ++it) {
        cout << *it;
    }
    cout << endl;

    // Reverse iteration
    cout << "Reverse: ";
    for (string::reverse_iterator it = str.rbegin(); it != str.rend(); ++it) {
        cout << *it;
    }
    cout << endl;

    // Range-based for loop (C++11)
    cout << "Range-based: ";
    for (char c : str) {
        cout << c;
    }
    cout << endl;

    // Using algorithms
    string str2 = str;
    transform(str2.begin(), str2.end(), str2.begin(), ::toupper);
    cout << "Uppercase: " << str2 << endl;

    return 0;
}
```

## 5. Practical Examples

### Example 1: Email Validator

```cpp
#include <iostream>
#include <string>
using namespace std;

bool isValidEmail(const string& email) {
    // Find @ symbol
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) {
        return false;
    }

    // Find dot after @
    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == email.length() - 1) {
        return false;
    }

    // Check for spaces
    if (email.find(' ') != string::npos) {
        return false;
    }

    return true;
}

int main() {
    string emails[] = {
        "user@example.com",
        "invalid.email",
        "@example.com",
        "user@",
        "user @example.com"
    };

    for (const string& email : emails) {
        cout << email << " is " << (isValidEmail(email) ? "valid" : "invalid") << endl;
    }

    return 0;
}
```

### Example 2: Word Frequency Counter

```cpp
#include <iostream>
#include <string>
#include <map>
#include <sstream>
using namespace std;

int main() {
    string text = "the quick brown fox jumps over the lazy dog the fox";
    map<string, int> wordCount;

    istringstream iss(text);
    string word;

    while (iss >> word) {
        wordCount[word]++;
    }

    cout << "Word frequencies:" << endl;
    for (const auto& pair : wordCount) {
        cout << pair.first << ": " << pair.second << endl;
    }

    return 0;
}
```

### Example 3: String Utilities

```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string toUpper(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

bool startsWith(const string& str, const string& prefix) {
    return str.find(prefix) == 0;
}

bool endsWith(const string& str, const string& suffix) {
    if (suffix.length() > str.length()) return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

int main() {
    string test = "  Hello World  ";

    cout << "Original: '" << test << "'" << endl;
    cout << "Trimmed: '" << trim(test) << "'" << endl;
    cout << "Lowercase: '" << toLower(test) << "'" << endl;
    cout << "Uppercase: '" << toUpper(test) << "'" << endl;

    string filename = "document.txt";
    cout << filename << " starts with 'doc': " << startsWith(filename, "doc") << endl;
    cout << filename << " ends with '.txt': " << endsWith(filename, ".txt") << endl;

    return 0;
}
```

## Exercises

### Exercise 1: Palindrome Checker
Write a function that checks if a string is a palindrome (reads the same forwards and backwards).

### Exercise 2: String Reversal
Implement multiple methods to reverse a string.

### Exercise 3: Word Count
Create a function that counts words, sentences, and paragraphs in a text.

### Exercise 4: Caesar Cipher
Implement encryption and decryption using the Caesar cipher.

### Exercise 5: String Tokenizer
Write a function to split a string by a delimiter into a vector of strings.

## Exercise Solutions

### Solution 1: Palindrome Checker

```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

bool isPalindrome(string str) {
    // Remove spaces and convert to lowercase
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    // Check palindrome
    string reversed = str;
    reverse(reversed.begin(), reversed.end());
    return str == reversed;
}

int main() {
    string tests[] = {
        "racecar",
        "hello",
        "A man a plan a canal Panama",
        "Was it a car or a cat I saw"
    };

    for (const string& test : tests) {
        cout << "'" << test << "' is " << (isPalindrome(test) ? "" : "not ")
             << "a palindrome" << endl;
    }

    return 0;
}
```

### Solution 2: String Reversal

```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// Method 1: Using reverse()
string reverseMethod1(string str) {
    reverse(str.begin(), str.end());
    return str;
}

// Method 2: Manual swap
string reverseMethod2(string str) {
    int n = str.length();
    for (int i = 0; i < n / 2; i++) {
        swap(str[i], str[n - 1 - i]);
    }
    return str;
}

// Method 3: Build reversed string
string reverseMethod3(const string& str) {
    string reversed;
    for (int i = str.length() - 1; i >= 0; i--) {
        reversed += str[i];
    }
    return reversed;
}

int main() {
    string original = "Hello World";

    cout << "Original: " << original << endl;
    cout << "Method 1: " << reverseMethod1(original) << endl;
    cout << "Method 2: " << reverseMethod2(original) << endl;
    cout << "Method 3: " << reverseMethod3(original) << endl;

    return 0;
}
```

### Solution 3: Word Count

```cpp
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct TextStats {
    int words;
    int sentences;
    int paragraphs;
};

TextStats analyzeText(const string& text) {
    TextStats stats = {0, 0, 0};

    istringstream iss(text);
    string word;

    while (iss >> word) {
        stats.words++;

        // Count sentences
        if (word.back() == '.' || word.back() == '!' || word.back() == '?') {
            stats.sentences++;
        }
    }

    // Count paragraphs (separated by double newline)
    size_t pos = 0;
    stats.paragraphs = 1;
    while ((pos = text.find("\n\n", pos)) != string::npos) {
        stats.paragraphs++;
        pos += 2;
    }

    return stats;
}

int main() {
    string text = "This is a test. It has multiple sentences! "
                  "Does it work? Yes it does.";

    TextStats stats = analyzeText(text);

    cout << "Text analysis:" << endl;
    cout << "Words: " << stats.words << endl;
    cout << "Sentences: " << stats.sentences << endl;
    cout << "Paragraphs: " << stats.paragraphs << endl;

    return 0;
}
```

### Solution 4: Caesar Cipher

```cpp
#include <iostream>
#include <string>
using namespace std;

string caesarEncrypt(const string& text, int shift) {
    string result = text;
    shift = shift % 26;

    for (char& c : result) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base + shift + 26) % 26 + base;
        }
    }

    return result;
}

string caesarDecrypt(const string& text, int shift) {
    return caesarEncrypt(text, -shift);
}

int main() {
    string message = "Hello World";
    int shift = 3;

    string encrypted = caesarEncrypt(message, shift);
    string decrypted = caesarDecrypt(encrypted, shift);

    cout << "Original: " << message << endl;
    cout << "Encrypted: " << encrypted << endl;
    cout << "Decrypted: " << decrypted << endl;

    return 0;
}
```

### Solution 5: String Tokenizer

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

int main() {
    string csv = "apple,banana,cherry,date";
    string path = "/usr/local/bin/program";

    vector<string> fruits = split(csv, ',');
    vector<string> dirs = split(path, '/');

    cout << "CSV tokens:" << endl;
    for (const string& fruit : fruits) {
        cout << "  " << fruit << endl;
    }

    cout << "\nPath tokens:" << endl;
    for (const string& dir : dirs) {
        cout << "  " << dir << endl;
    }

    return 0;
}
```

## Summary

In this lesson, you learned:

- **std::string** is a powerful C++ class for string handling
- Strings are **dynamically sized** and **memory-safe**
- Common operations: concatenation, searching, substring, replace
- String conversion between strings and numbers
- **Iterators** for string traversal
- **std::string** is generally preferred over C-style strings
- Performance: std::string has overhead but provides safety

## Checklist

- [ ] I understand the std::string class and its advantages
- [ ] I can perform basic string operations
- [ ] I can search and manipulate strings
- [ ] I can convert between strings and numbers
- [ ] I can use string iterators
- [ ] I completed all exercises and understand the solutions
- [ ] I know when to use std::string vs C-style strings
- [ ] I can write string processing utilities

## Additional Resources

- **Next Lesson:** String Manipulation
- **Related Topics:** String Streams, Regular Expressions, Unicode
- **Practice:** Implement text processing tools and parsers

---

**Congratulations!** You've completed Lesson 19. You now understand std::string, which is the modern and safe way to handle strings in C++.
