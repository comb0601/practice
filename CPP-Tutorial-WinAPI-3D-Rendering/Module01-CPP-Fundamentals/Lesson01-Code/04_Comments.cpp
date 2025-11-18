/*
 * Lesson 01 - Example 04: Comments in C++
 *
 * This program demonstrates different types of comments in C++.
 * Comments are ignored by the compiler and are used to explain code.
 *
 * This is a multi-line comment.
 * It can span multiple lines.
 *
 * Compile: g++ 04_Comments.cpp -o Comments
 */

#include <iostream>

int main()
{
    // This is a single-line comment
    // Single-line comments start with //

    std::cout << "Comments Demo\n\n";

    // You can put comments after code on the same line
    std::cout << "Hello!\n";  // This prints Hello!

    /*
     * Multi-line comments start with /*
     * and end with */
    /* They can span multiple lines
     * and are useful for longer explanations
     */

    std::cout << "Types of comments:\n";
    std::cout << "1. Single-line: // comment\n";
    std::cout << "2. Multi-line: /* comment */\n";

    // Good commenting practices:
    // - Explain WHY, not WHAT
    // - Keep comments up-to-date
    // - Don't over-comment obvious code
    // - Use comments for complex logic

    // std::cout << "This line won't execute\n";  // Commented out

    /* You can temporarily disable code with comments
    std::cout << "This is disabled\n";
    std::cout << "This too\n";
    */

    std::cout << "\nProgram complete!\n";

    return 0;  // Return 0 means successful execution
}
