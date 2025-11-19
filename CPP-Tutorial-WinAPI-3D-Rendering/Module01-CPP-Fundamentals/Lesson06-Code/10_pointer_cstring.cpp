/*
 * Program: Pointers and C-Strings
 * Description: Using pointers to manipulate C-style strings
 * Compilation: g++ -std=c++17 10_pointer_cstring.cpp -o 10_pointer_cstring
 * Execution: ./10_pointer_cstring
 */

#include <iostream>
#include <cstring>
using namespace std;

int stringLength(const char* str) {
    int length = 0;
    while (*str != '\0') {
        length++;
        str++;
    }
    return length;
}

void stringCopy(char* dest, const char* src) {
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';  // Null terminator
}

void stringConcatenate(char* dest, const char* src) {
    // Move to end of dest
    while (*dest != '\0') {
        dest++;
    }

    // Copy src to end of dest
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

bool stringCompare(const char* str1, const char* str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return false;
        }
        str1++;
        str2++;
    }
    return *str1 == *str2;  // Both should be '\0'
}

void stringReverse(char* str) {
    char* start = str;
    char* end = str;

    // Find end of string
    while (*end != '\0') {
        end++;
    }
    end--;  // Move back from '\0'

    // Swap characters
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

int main() {
    cout << "=== Pointers and C-Strings ===" << endl << endl;

    // String as pointer
    cout << "1. String as Pointer:" << endl;
    char str[] = "Hello";
    char* ptr = str;

    cout << "str = " << str << endl;
    cout << "ptr = " << ptr << endl;
    cout << "First character: *ptr = " << *ptr << endl;
    cout << "Second character: *(ptr+1) = " << *(ptr + 1) << endl << endl;

    // Traversing string with pointer
    cout << "2. Traversing String with Pointer:" << endl;
    cout << "Characters: ";
    ptr = str;
    while (*ptr != '\0') {
        cout << *ptr << " ";
        ptr++;
    }
    cout << endl << endl;

    // String length using pointer
    cout << "3. String Length:" << endl;
    const char* message = "Programming";
    cout << "String: " << message << endl;
    cout << "Length: " << stringLength(message) << endl;
    cout << "strlen: " << strlen(message) << endl << endl;

    // String copy
    cout << "4. String Copy:" << endl;
    char source[] = "Copy me";
    char destination[50];

    stringCopy(destination, source);

    cout << "Source: " << source << endl;
    cout << "Destination: " << destination << endl << endl;

    // String concatenation
    cout << "5. String Concatenation:" << endl;
    char greeting[50] = "Hello";
    const char* name = " World";

    cout << "Before: " << greeting << endl;
    stringConcatenate(greeting, name);
    cout << "After: " << greeting << endl << endl;

    // String comparison
    cout << "6. String Comparison:" << endl;
    const char* str1 = "Apple";
    const char* str2 = "Apple";
    const char* str3 = "Banana";

    cout << "\"" << str1 << "\" == \"" << str2 << "\": "
         << (stringCompare(str1, str2) ? "true" : "false") << endl;

    cout << "\"" << str1 << "\" == \"" << str3 << "\": "
         << (stringCompare(str1, str3) ? "true" : "false") << endl << endl;

    // String reverse
    cout << "7. String Reverse:" << endl;
    char word[] = "Programming";
    cout << "Original: " << word << endl;

    stringReverse(word);
    cout << "Reversed: " << word << endl << endl;

    // Pointer to string literal
    cout << "8. Pointer to String Literal:" << endl;
    const char* literal = "This is a string literal";
    cout << "Literal: " << literal << endl;
    cout << "Address: " << (void*)literal << endl;
    // literal[0] = 'X';  // ERROR - string literals are read-only!
    cout << endl;

    // Array of string pointers
    cout << "9. Array of String Pointers:" << endl;
    const char* fruits[] = {
        "Apple",
        "Banana",
        "Cherry",
        "Date",
        "Elderberry"
    };

    cout << "Fruits:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << i + 1 << ". " << fruits[i] << endl;
    }
    cout << endl;

    // Pointer arithmetic with strings
    cout << "10. Pointer Arithmetic:" << endl;
    const char* text = "C++ Programming";
    cout << "Full string: " << text << endl;
    cout << "From index 4: " << (text + 4) << endl;
    cout << "From index 7: " << (text + 7) << endl << endl;

    // Finding character in string
    cout << "11. Finding Character:" << endl;
    char sentence[] = "Find the letter e";
    char* found = sentence;

    while (*found != '\0' && *found != 'e') {
        found++;
    }

    if (*found == 'e') {
        cout << "Found 'e' at position: " << (found - sentence) << endl;
    } else {
        cout << "Character not found" << endl;
    }

    return 0;
}
