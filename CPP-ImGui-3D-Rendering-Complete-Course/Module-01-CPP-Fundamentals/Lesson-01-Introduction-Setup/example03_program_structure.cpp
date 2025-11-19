#include <iostream>  // Preprocessor directive - includes input/output library

/**
 * Example 03: C++ Program Structure
 *
 * This program demonstrates the basic structure of a C++ program:
 * 1. Preprocessor directives (#include)
 * 2. Comments (single-line // and multi-line)
 * 3. The main() function
 * 4. Statements and expressions
 * 5. Return value
 */

// This is a single-line comment
// The compiler ignores these lines

/*
   This is a multi-line comment.
   You can write multiple lines here.
   Useful for longer explanations.
*/

// The main function - program execution starts here
int main() {
    // Variable declaration
    int number = 42;

    // Output statement
    std::cout << "The number is: " << number << std::endl;

    // Another statement
    std::cout << "Program structure components:" << std::endl;
    std::cout << "1. Preprocessor directives" << std::endl;
    std::cout << "2. Comments" << std::endl;
    std::cout << "3. main() function" << std::endl;
    std::cout << "4. Statements (end with ;)" << std::endl;
    std::cout << "5. Return statement" << std::endl;

    // Return 0 indicates successful program execution
    return 0;
}

// Note: Any code after main() won't execute unless called
