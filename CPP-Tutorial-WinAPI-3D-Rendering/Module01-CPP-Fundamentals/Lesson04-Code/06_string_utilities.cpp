/*
 * Program: String Utility Functions
 * Description: Custom string manipulation functions
 * Compilation: g++ 06_string_utilities.cpp -o 06_string_utilities
 * Execution: ./06_string_utilities
 */

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Convert string to uppercase
string toUpperCase(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}

// Convert string to lowercase
string toLowerCase(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

// Reverse a string
string reverseString(string str) {
    string reversed = "";
    for (int i = str.length() - 1; i >= 0; i--) {
        reversed += str[i];
    }
    return reversed;
}

// Check if palindrome
bool isPalindrome(string str) {
    int left = 0, right = str.length() - 1;
    while (left < right) {
        if (tolower(str[left]) != tolower(str[right])) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

// Count vowels
int countVowels(string str) {
    int count = 0;
    for (char c : str) {
        char lower = tolower(c);
        if (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u') {
            count++;
        }
    }
    return count;
}

// Count consonants
int countConsonants(string str) {
    int count = 0;
    for (char c : str) {
        if (isalpha(c)) {
            char lower = tolower(c);
            if (!(lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u')) {
                count++;
            }
        }
    }
    return count;
}

// Count words
int countWords(string str) {
    int count = 0;
    bool inWord = false;

    for (char c : str) {
        if (isspace(c)) {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            count++;
        }
    }
    return count;
}

// Remove spaces
string removeSpaces(string str) {
    string result = "";
    for (char c : str) {
        if (!isspace(c)) {
            result += c;
        }
    }
    return result;
}

// Count character occurrences
int countChar(string str, char ch) {
    int count = 0;
    for (char c : str) {
        if (c == ch) {
            count++;
        }
    }
    return count;
}

// Replace character
string replaceChar(string str, char oldChar, char newChar) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == oldChar) {
            str[i] = newChar;
        }
    }
    return str;
}

int main() {
    cout << "=== String Utility Functions ===" << endl << endl;

    string text = "Hello World";

    // Case conversion
    cout << "1. Case Conversion:" << endl;
    cout << "Original: " << text << endl;
    cout << "Uppercase: " << toUpperCase(text) << endl;
    cout << "Lowercase: " << toLowerCase(text) << endl << endl;

    // Reverse
    cout << "2. Reverse String:" << endl;
    cout << "Original: " << text << endl;
    cout << "Reversed: " << reverseString(text) << endl << endl;

    // Palindrome
    cout << "3. Palindrome Check:" << endl;
    string pal1 = "racecar";
    string pal2 = "hello";
    cout << pal1 << " is palindrome? " << (isPalindrome(pal1) ? "Yes" : "No") << endl;
    cout << pal2 << " is palindrome? " << (isPalindrome(pal2) ? "Yes" : "No") << endl << endl;

    // Count vowels and consonants
    cout << "4. Count Vowels and Consonants:" << endl;
    cout << "Text: " << text << endl;
    cout << "Vowels: " << countVowels(text) << endl;
    cout << "Consonants: " << countConsonants(text) << endl << endl;

    // Count words
    cout << "5. Count Words:" << endl;
    string sentence = "The quick brown fox jumps";
    cout << "Sentence: " << sentence << endl;
    cout << "Word count: " << countWords(sentence) << endl << endl;

    // Remove spaces
    cout << "6. Remove Spaces:" << endl;
    cout << "Original: \"" << text << "\"" << endl;
    cout << "No spaces: \"" << removeSpaces(text) << "\"" << endl << endl;

    // Count character
    cout << "7. Count Character Occurrences:" << endl;
    cout << "Count 'l' in \"" << text << "\": " << countChar(text, 'l') << endl << endl;

    // Replace character
    cout << "8. Replace Character:" << endl;
    cout << "Original: " << text << endl;
    cout << "Replace 'o' with '0': " << replaceChar(text, 'o', '0') << endl;

    return 0;
}
