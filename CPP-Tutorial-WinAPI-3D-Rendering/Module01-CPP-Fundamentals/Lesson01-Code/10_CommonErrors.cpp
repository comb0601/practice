/*
 * Lesson 01 - Example 10: Common Errors (COMMENTED OUT)
 *
 * This file shows common beginner mistakes in C++.
 * All errors are commented out so the file compiles.
 *
 * To learn from this file:
 * 1. Read each error example
 * 2. Uncomment one error at a time
 * 3. Try to compile and see the error message
 * 4. Fix the error
 * 5. Move to the next error
 *
 * Compile: g++ 10_CommonErrors.cpp -o CommonErrors
 */

#include <iostream>

int main()
{
    std::cout << "=== Common C++ Errors ===\n\n";

    // CORRECT EXAMPLES (These work):
    std::cout << "1. Correct statement with semicolon\n";

    // ERROR 1: Missing semicolon
    // std::cout << "This will cause an error"  // ERROR: Missing ;

    // ERROR 2: Wrong case (C++ is case-sensitive)
    // Int x = 5;  // ERROR: Should be 'int' not 'Int'
    // STD::cout << "Hello\n";  // ERROR: Should be 'std' not 'STD'

    // ERROR 3: Missing #include
    // If you comment out #include <iostream>, you'll get errors

    // ERROR 4: Wrong main function signature
    // void main()  // WRONG: Should return int
    // Main()       // WRONG: Must be 'main' not 'Main'

    // ERROR 5: Mismatched braces
    // if (true) {
    //     std::cout << "Missing closing brace\n";
    // ERROR: No closing brace

    // ERROR 6: Using single quotes for strings
    // std::cout << 'Hello';  // ERROR: Strings use double quotes "

    // ERROR 7: Missing stream insertion operator
    // std::cout "Hello\n";  // ERROR: Missing <<

    // ERROR 8: Wrong escape sequence
    // std::cout << "Path: C:\new\folder\n";  // ERROR: \n is newline!
    // Correct: std::cout << "Path: C:\\new\\folder\\n";

    // ERROR 9: Forgetting return statement
    // (Your compiler might warn about this)

    std::cout << "\n2. This program compiles because errors are commented out\n";
    std::cout << "3. Try uncommenting each error to see what happens!\n";
    std::cout << "4. Read the compiler error messages carefully\n";
    std::cout << "5. Fix the error and compile again\n";

    return 0;  // CORRECT: Return statement
}

// ERROR 10: Code after main function's closing brace
// This code would never execute (but it's a syntax error if it's not a function)
// std::cout << "This won't work\n";
