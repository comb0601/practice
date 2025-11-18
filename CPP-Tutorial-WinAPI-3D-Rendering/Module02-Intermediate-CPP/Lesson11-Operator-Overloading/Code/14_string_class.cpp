/*
 * Lesson 11: Operator Overloading
 * File: 14_string_class.cpp
 *
 * Custom String class with comprehensive operator overloading.
 * Demonstrates memory management with operator overloading.
 */

#include <iostream>
#include <cstring>
#include <stdexcept>

class String {
private:
    char* data;
    size_t length;

public:
    // Default constructor
    String() : data(nullptr), length(0) {
        data = new char[1];
        data[0] = '\0';
    }

    // Constructor from C-string
    String(const char* str) {
        if (str == nullptr) {
            data = new char[1];
            data[0] = '\0';
            length = 0;
        } else {
            length = strlen(str);
            data = new char[length + 1];
            strcpy(data, str);
        }
    }

    // Constructor with repeated character
    String(size_t count, char ch) : length(count) {
        data = new char[length + 1];
        for (size_t i = 0; i < length; i++) {
            data[i] = ch;
        }
        data[length] = '\0';
    }

    // Copy constructor
    String(const String& other) : length(other.length) {
        data = new char[length + 1];
        strcpy(data, other.data);
    }

    // Destructor
    ~String() {
        delete[] data;
    }

    // Copy assignment operator
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    // Assignment from C-string
    String& operator=(const char* str) {
        if (str == nullptr) {
            delete[] data;
            data = new char[1];
            data[0] = '\0';
            length = 0;
        } else {
            delete[] data;
            length = strlen(str);
            data = new char[length + 1];
            strcpy(data, str);
        }
        return *this;
    }

    // Concatenation
    String operator+(const String& other) const {
        String result;
        result.length = length + other.length;
        delete[] result.data;
        result.data = new char[result.length + 1];
        strcpy(result.data, data);
        strcat(result.data, other.data);
        return result;
    }

    // Concatenation with C-string
    String operator+(const char* str) const {
        return *this + String(str);
    }

    // Compound concatenation
    String& operator+=(const String& other) {
        *this = *this + other;
        return *this;
    }

    String& operator+=(const char* str) {
        *this = *this + String(str);
        return *this;
    }

    String& operator+=(char ch) {
        char temp[2] = {ch, '\0'};
        *this += temp;
        return *this;
    }

    // Repetition (string * count)
    String operator*(size_t count) const {
        String result;
        result.length = length * count;
        delete[] result.data;
        result.data = new char[result.length + 1];
        result.data[0] = '\0';
        for (size_t i = 0; i < count; i++) {
            strcat(result.data, data);
        }
        return result;
    }

    // Subscript operator
    char& operator[](size_t index) {
        if (index >= length) {
            throw std::out_of_range("String index out of range");
        }
        return data[index];
    }

    const char& operator[](size_t index) const {
        if (index >= length) {
            throw std::out_of_range("String index out of range");
        }
        return data[index];
    }

    // Comparison operators
    bool operator==(const String& other) const {
        return strcmp(data, other.data) == 0;
    }

    bool operator==(const char* str) const {
        return strcmp(data, str) == 0;
    }

    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    bool operator!=(const char* str) const {
        return !(*this == str);
    }

    bool operator<(const String& other) const {
        return strcmp(data, other.data) < 0;
    }

    bool operator>(const String& other) const {
        return other < *this;
    }

    bool operator<=(const String& other) const {
        return !(other < *this);
    }

    bool operator>=(const String& other) const {
        return !(*this < other);
    }

    // Type conversion
    explicit operator bool() const {
        return length > 0;
    }

    operator const char*() const {
        return data;
    }

    // Utility functions
    size_t size() const { return length; }
    const char* c_str() const { return data; }
    bool empty() const { return length == 0; }

    void clear() {
        delete[] data;
        data = new char[1];
        data[0] = '\0';
        length = 0;
    }

    String substr(size_t start, size_t len = std::string::npos) const {
        if (start >= length) {
            return String();
        }
        size_t actualLen = (len == std::string::npos || start + len > length)
                           ? length - start : len;

        char* temp = new char[actualLen + 1];
        strncpy(temp, data + start, actualLen);
        temp[actualLen] = '\0';
        String result(temp);
        delete[] temp;
        return result;
    }

    int find(const String& str) const {
        const char* pos = strstr(data, str.data);
        return (pos != nullptr) ? (pos - data) : -1;
    }

    int find(char ch) const {
        const char* pos = strchr(data, ch);
        return (pos != nullptr) ? (pos - data) : -1;
    }

    String toUpper() const {
        String result(*this);
        for (size_t i = 0; i < length; i++) {
            result.data[i] = toupper(data[i]);
        }
        return result;
    }

    String toLower() const {
        String result(*this);
        for (size_t i = 0; i < length; i++) {
            result.data[i] = tolower(data[i]);
        }
        return result;
    }

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        os << str.data;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, String& str) {
        char buffer[1000];
        is >> buffer;
        str = buffer;
        return is;
    }

    // Non-member function declarations
    friend String operator+(const char* lhs, const String& rhs);
    friend String operator*(size_t count, const String& str);
};

// Non-member operators
String operator+(const char* lhs, const String& rhs) {
    return String(lhs) + rhs;
}

String operator*(size_t count, const String& str) {
    return str * count;
}

int main() {
    std::cout << "=== String Class Demo ===\n\n";

    // Creating strings
    std::cout << "--- Creating Strings ---\n";
    String s1("Hello");
    String s2("World");
    String s3(5, '*');  // "*****"

    std::cout << "s1 = \"" << s1 << "\"\n";
    std::cout << "s2 = \"" << s2 << "\"\n";
    std::cout << "s3 = \"" << s3 << "\"\n\n";

    // Concatenation
    std::cout << "--- Concatenation ---\n";
    String s4 = s1 + " " + s2;
    std::cout << "s1 + \" \" + s2 = \"" << s4 << "\"\n";

    String s5 = "Hello, " + s2 + "!";
    std::cout << "\"Hello, \" + s2 + \"!\" = \"" << s5 << "\"\n\n";

    // Compound concatenation
    std::cout << "--- Compound Concatenation ---\n";
    String s6("C");
    std::cout << "s6 = \"" << s6 << "\"\n";
    s6 += "++";
    std::cout << "After s6 += \"++\": \"" << s6 << "\"\n";
    s6 += ' ';
    s6 += "Programming";
    std::cout << "After s6 += \" Programming\": \"" << s6 << "\"\n\n";

    // Repetition
    std::cout << "--- Repetition ---\n";
    String dash("-");
    String separator = dash * 40;
    std::cout << separator << "\n";

    String pattern("abc");
    String repeated = pattern * 3;
    std::cout << "\"abc\" * 3 = \"" << repeated << "\"\n";
    std::cout << "5 * \"xyz\" = \"" << (5 * String("xyz")) << "\"\n\n";

    // Subscript operator
    std::cout << "--- Subscript Operator ---\n";
    String s7("Hello");
    std::cout << "s7 = \"" << s7 << "\"\n";
    std::cout << "s7[0] = '" << s7[0] << "'\n";
    std::cout << "s7[4] = '" << s7[4] << "'\n";
    s7[0] = 'h';
    s7[4] = '!';
    std::cout << "After s7[0] = 'h' and s7[4] = '!': \"" << s7 << "\"\n\n";

    // Comparison
    std::cout << "--- Comparison ---\n";
    String str1("apple");
    String str2("banana");
    String str3("apple");

    std::cout << "str1 = \"" << str1 << "\"\n";
    std::cout << "str2 = \"" << str2 << "\"\n";
    std::cout << "str3 = \"" << str3 << "\"\n\n";

    std::cout << "str1 == str3: " << (str1 == str3 ? "true" : "false") << "\n";
    std::cout << "str1 != str2: " << (str1 != str2 ? "true" : "false") << "\n";
    std::cout << "str1 < str2: " << (str1 < str2 ? "true" : "false") << "\n";
    std::cout << "str2 > str1: " << (str2 > str1 ? "true" : "false") << "\n\n";

    // Sorting strings
    std::cout << "--- Sorting Strings ---\n";
    std::vector<String> words;
    words.push_back(String("zebra"));
    words.push_back(String("apple"));
    words.push_back(String("mango"));
    words.push_back(String("banana"));

    std::cout << "Unsorted: ";
    for (const auto& w : words) {
        std::cout << w << " ";
    }
    std::cout << "\n";

    std::sort(words.begin(), words.end());

    std::cout << "Sorted:   ";
    for (const auto& w : words) {
        std::cout << w << " ";
    }
    std::cout << "\n\n";

    // Substring
    std::cout << "--- Substring ---\n";
    String original("Hello, World!");
    std::cout << "original = \"" << original << "\"\n";
    std::cout << "substr(0, 5) = \"" << original.substr(0, 5) << "\"\n";
    std::cout << "substr(7) = \"" << original.substr(7) << "\"\n\n";

    // Find
    std::cout << "--- Find ---\n";
    String text("The quick brown fox jumps over the lazy dog");
    std::cout << "text = \"" << text << "\"\n";
    int pos1 = text.find(String("fox"));
    std::cout << "find(\"fox\") = " << pos1 << "\n";
    int pos2 = text.find('q');
    std::cout << "find('q') = " << pos2 << "\n";
    int pos3 = text.find(String("cat"));
    std::cout << "find(\"cat\") = " << pos3 << " (not found)\n\n";

    // Case conversion
    std::cout << "--- Case Conversion ---\n";
    String mixed("Hello, World!");
    std::cout << "original = \"" << mixed << "\"\n";
    std::cout << "toUpper() = \"" << mixed.toUpper() << "\"\n";
    std::cout << "toLower() = \"" << mixed.toLower() << "\"\n\n";

    // Boolean conversion
    std::cout << "--- Boolean Conversion ---\n";
    String empty;
    String notEmpty("Hello");
    std::cout << "Empty string as bool: "
              << (static_cast<bool>(empty) ? "true" : "false") << "\n";
    std::cout << "Non-empty string as bool: "
              << (static_cast<bool>(notEmpty) ? "true" : "false") << "\n\n";

    // Building strings
    std::cout << "--- Building Strings ---\n";
    String sentence;
    sentence = String("I") + " love " + "C++" + " programming!";
    std::cout << "Built sentence: \"" << sentence << "\"\n\n";

    // Text formatting
    std::cout << "--- Text Formatting ---\n";
    String header = "Title";
    String border = String(header.size(), '=');
    std::cout << border << "\n";
    std::cout << header << "\n";
    std::cout << border << "\n\n";

    // Menu creation
    std::cout << "--- Menu Creation ---\n";
    String menuLine = "* " + String("Option") + " *";
    String menuBorder = String(menuLine.size(), '-');
    std::cout << menuBorder << "\n";
    std::cout << menuLine << "\n";
    std::cout << menuBorder << "\n\n";

    // Progress bar simulation
    std::cout << "--- Progress Bar ---\n";
    for (int progress = 0; progress <= 100; progress += 20) {
        int filled = progress / 5;
        String bar = "[" + String(filled, '#') + String(20 - filled, '-') + "]";
        std::cout << bar << " " << progress << "%\n";
    }
    std::cout << "\n";

    // Palindrome check (bonus)
    std::cout << "--- Palindrome Check ---\n";
    auto isPalindrome = [](const String& str) {
        size_t len = str.size();
        for (size_t i = 0; i < len / 2; i++) {
            if (str[i] != str[len - 1 - i]) {
                return false;
            }
        }
        return true;
    };

    String word1("racecar");
    String word2("hello");
    std::cout << "\"" << word1 << "\" is palindrome: "
              << (isPalindrome(word1) ? "true" : "false") << "\n";
    std::cout << "\"" << word2 << "\" is palindrome: "
              << (isPalindrome(word2) ? "true" : "false") << "\n";

    return 0;
}

/*
 * OUTPUT:
 * =================================
 * === String Class Demo ===
 *
 * --- Creating Strings ---
 * s1 = "Hello"
 * s2 = "World"
 * s3 = "*****"
 *
 * --- Concatenation ---
 * s1 + " " + s2 = "Hello World"
 * "Hello, " + s2 + "!" = "Hello, World!"
 *
 * --- Compound Concatenation ---
 * s6 = "C"
 * After s6 += "++": "C++"
 * After s6 += " Programming": "C++ Programming"
 *
 * --- Repetition ---
 * ----------------------------------------
 * "abc" * 3 = "abcabcabc"
 * 5 * "xyz" = "xyzxyzxyzxyzxyz"
 *
 * --- Subscript Operator ---
 * s7 = "Hello"
 * s7[0] = 'H'
 * s7[4] = 'o'
 * After s7[0] = 'h' and s7[4] = '!': "hell!"
 *
 * --- Comparison ---
 * str1 = "apple"
 * str2 = "banana"
 * str3 = "apple"
 *
 * str1 == str3: true
 * str1 != str2: true
 * str1 < str2: true
 * str2 > str1: true
 *
 * --- Sorting Strings ---
 * Unsorted: zebra apple mango banana
 * Sorted:   apple banana mango zebra
 *
 * ... (more output follows)
 */
