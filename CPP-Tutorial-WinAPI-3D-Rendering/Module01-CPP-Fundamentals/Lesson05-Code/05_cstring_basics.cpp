/*
 * Program: C-String Basics
 * Description: Introduction to C-style strings (null-terminated character arrays)
 * Compilation: g++ 05_cstring_basics.cpp -o 05_cstring_basics
 * Execution: ./05_cstring_basics
 */

#include <iostream>
#include <cstring>  // For C-string functions
using namespace std;

int main() {
    cout << "=== C-String Basics ===" << endl << endl;

    // C-string declaration and initialization
    cout << "1. C-String Declaration:" << endl;
    char str1[] = "Hello";  // Compiler adds '\0' automatically
    char str2[20] = "World";  // Extra space for modifications
    char str3[] = {'H', 'i', '\0'};  // Manual null terminator

    cout << "str1: " << str1 << endl;
    cout << "str2: " << str2 << endl;
    cout << "str3: " << str3 << endl << endl;

    // Length of string
    cout << "2. String Length:" << endl;
    cout << "Length of \"" << str1 << "\": " << strlen(str1) << " characters" << endl;
    cout << "Array size: " << sizeof(str1) << " bytes" << endl;
    cout << "Length of \"" << str2 << "\": " << strlen(str2) << " characters" << endl;
    cout << "Array size: " << sizeof(str2) << " bytes" << endl << endl;

    // String copy
    cout << "3. String Copy (strcpy):" << endl;
    char destination[50];
    strcpy(destination, "Original");
    cout << "After strcpy: " << destination << endl;

    strcpy(destination, "Modified");
    cout << "After second strcpy: " << destination << endl << endl;

    // String concatenation
    cout << "4. String Concatenation (strcat):" << endl;
    char greeting[50] = "Hello";
    cout << "Before: " << greeting << endl;
    strcat(greeting, " World");
    cout << "After strcat: " << greeting << endl;
    strcat(greeting, "!");
    cout << "After second strcat: " << greeting << endl << endl;

    // String comparison
    cout << "5. String Comparison (strcmp):" << endl;
    char str4[] = "Apple";
    char str5[] = "Banana";
    char str6[] = "Apple";

    int result1 = strcmp(str4, str5);
    int result2 = strcmp(str4, str6);
    int result3 = strcmp(str5, str4);

    cout << "strcmp(\"Apple\", \"Banana\"): " << result1;
    cout << " (negative = first is less)" << endl;

    cout << "strcmp(\"Apple\", \"Apple\"): " << result2;
    cout << " (zero = equal)" << endl;

    cout << "strcmp(\"Banana\", \"Apple\"): " << result3;
    cout << " (positive = first is greater)" << endl << endl;

    // Character access and modification
    cout << "6. Character Access:" << endl;
    char word[] = "Hello";
    cout << "Original: " << word << endl;
    cout << "First character: " << word[0] << endl;
    cout << "Last character: " << word[strlen(word) - 1] << endl;

    word[0] = 'J';  // Modify first character
    cout << "After changing first char: " << word << endl << endl;

    // Input C-string
    cout << "7. Input C-String:" << endl;
    char name[50];
    cout << "Enter your name: ";
    cin.getline(name, 50);  // Read line with spaces
    cout << "Hello, " << name << "!" << endl << endl;

    // Iterate through C-string
    cout << "8. Iterate Through String:" << endl;
    char message[] = "Programming";
    cout << "Characters in \"" << message << "\": ";
    for (int i = 0; message[i] != '\0'; i++) {
        cout << message[i] << " ";
    }
    cout << endl;

    return 0;
}
