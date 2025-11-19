/*
 * Program: String Manipulation
 * Description: Advanced C-string operations - reverse, palindrome, case conversion
 * Compilation: g++ 06_string_manipulation.cpp -o 06_string_manipulation
 * Execution: ./06_string_manipulation
 */

#include <iostream>
#include <cstring>
#include <cctype>  // For toupper, tolower, isalpha, etc.
using namespace std;

void reverseString(char str[]) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = temp;
    }
}

bool isPalindrome(const char str[]) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        if (tolower(str[i]) != tolower(str[length - 1 - i])) {
            return false;
        }
    }
    return true;
}

void toUpperCase(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

void toLowerCase(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int countVowels(const char str[]) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char ch = tolower(str[i]);
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            count++;
        }
    }
    return count;
}

int countWords(const char str[]) {
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

int main() {
    cout << "=== String Manipulation ===" << endl << endl;

    // Reverse string
    cout << "1. Reverse String:" << endl;
    char str1[] = "Hello World";
    cout << "Original: " << str1 << endl;
    reverseString(str1);
    cout << "Reversed: " << str1 << endl << endl;

    // Palindrome check
    cout << "2. Palindrome Check:" << endl;
    char str2[] = "racecar";
    char str3[] = "Madam";
    char str4[] = "hello";

    cout << "\"" << str2 << "\" is " << (isPalindrome(str2) ? "" : "not ") << "a palindrome" << endl;
    cout << "\"" << str3 << "\" is " << (isPalindrome(str3) ? "" : "not ") << "a palindrome" << endl;
    cout << "\"" << str4 << "\" is " << (isPalindrome(str4) ? "" : "not ") << "a palindrome" << endl << endl;

    // Case conversion
    cout << "3. Case Conversion:" << endl;
    char str5[] = "Hello World";
    cout << "Original: " << str5 << endl;

    toUpperCase(str5);
    cout << "Uppercase: " << str5 << endl;

    toLowerCase(str5);
    cout << "Lowercase: " << str5 << endl << endl;

    // Count vowels
    cout << "4. Count Vowels:" << endl;
    char str6[] = "Programming in C++";
    cout << "String: " << str6 << endl;
    cout << "Number of vowels: " << countVowels(str6) << endl << endl;

    // Count words
    cout << "5. Count Words:" << endl;
    char str7[] = "The quick brown fox jumps";
    cout << "String: " << str7 << endl;
    cout << "Number of words: " << countWords(str7) << endl << endl;

    // Remove spaces
    cout << "6. Remove Spaces:" << endl;
    char str8[] = "Hello World From C++";
    cout << "Original: " << str8 << endl;

    int j = 0;
    for (int i = 0; str8[i] != '\0'; i++) {
        if (!isspace(str8[i])) {
            str8[j++] = str8[i];
        }
    }
    str8[j] = '\0';

    cout << "Without spaces: " << str8 << endl << endl;

    // Count character occurrences
    cout << "7. Count Character Occurrences:" << endl;
    char str9[] = "Mississippi";
    char searchChar = 's';
    int count = 0;

    cout << "String: " << str9 << endl;
    for (int i = 0; str9[i] != '\0'; i++) {
        if (tolower(str9[i]) == searchChar) {
            count++;
        }
    }
    cout << "Character '" << searchChar << "' appears " << count << " times" << endl;

    return 0;
}
