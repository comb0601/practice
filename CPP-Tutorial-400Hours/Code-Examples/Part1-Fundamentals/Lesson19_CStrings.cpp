/*
 * Lesson 19: C-Style Strings - Character Arrays
 * Compile: cl Lesson19_CStrings.cpp
 */

#include <iostream>
#include <cstring>
using namespace std;

int main() {
    cout << "=== C-Style Strings Demo ===" << endl << endl;

    // STRING DECLARATION
    char str1[20] = "Hello";
    char str2[] = "World";
    char str3[20];

    // STRING LENGTH
    cout << "Length of '" << str1 << "': " << strlen(str1) << endl << endl;

    // STRING COPY
    strcpy(str3, str1);
    cout << "Copied string: " << str3 << endl << endl;

    // STRING CONCATENATION
    strcat(str1, " ");
    strcat(str1, str2);
    cout << "Concatenated: " << str1 << endl << endl;

    // STRING COMPARISON
    char s1[] = "Apple";
    char s2[] = "Banana";
    int cmp = strcmp(s1, s2);
    cout << "Comparing '" << s1 << "' and '" << s2 << "': ";
    if (cmp < 0) cout << s1 << " comes before " << s2 << endl;
    else if (cmp > 0) cout << s1 << " comes after " << s2 << endl;
    else cout << "Equal" << endl;
    cout << endl;

    // CHARACTER MANIPULATION
    char text[] = "Hello World";
    cout << "Original: " << text << endl;

    // To uppercase
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] -= 32;
        }
    }
    cout << "Uppercase: " << text << endl << endl;

    // CHARACTER SEARCH
    char sentence[] = "C++ Programming";
    char *pos = strchr(sentence, 'P');
    if (pos != nullptr) {
        cout << "'P' found at position: " << (pos - sentence) << endl;
    }
    cout << endl;

    // SUBSTRING SEARCH
    char haystack[] = "This is a test";
    char needle[] = "is";
    char *found = strstr(haystack, needle);
    if (found != nullptr) {
        cout << "'" << needle << "' found in '" << haystack << "'" << endl;
    }
    cout << endl;

    // STRING TOKENIZATION
    char data[] = "apple,banana,cherry";
    char *token = strtok(data, ",");
    cout << "Tokens: ";
    while (token != nullptr) {
        cout << token << " ";
        token = strtok(nullptr, ",");
    }
    cout << endl;

    return 0;
}
