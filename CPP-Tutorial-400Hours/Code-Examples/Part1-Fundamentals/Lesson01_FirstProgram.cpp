/*
 * Lesson 1: First Program - Understanding Program Structure
 *
 * This program demonstrates the basic structure of a C++ program including:
 * - Preprocessor directives (#include)
 * - Comments (single-line and multi-line)
 * - The main function
 * - Statements and expressions
 * - Return values
 *
 * Compile: cl Lesson01_FirstProgram.cpp
 */

// Preprocessor directive - includes the iostream library for input/output
#include <iostream>

/*
 * Multi-line comment
 * These can span multiple lines and are useful for
 * longer explanations of code functionality
 */

// Using namespace std to avoid writing std:: every time
using namespace std;

// The main function - entry point of every C++ program
int main() {
    // Single-line comment - explains the next line of code

    cout << "=== C++ Program Structure Demo ===" << endl;
    cout << endl;

    // 1. Header section (preprocessor directives at top)
    cout << "1. Preprocessor Directives: #include <iostream>" << endl;

    // 2. Namespace usage
    cout << "2. Namespace: using namespace std;" << endl;

    // 3. Main function
    cout << "3. Main Function: int main() { ... }" << endl;

    // 4. Statements end with semicolons
    cout << "4. Each statement ends with a semicolon ;" << endl;

    // 5. Blocks are defined with curly braces { }
    cout << "5. Code blocks use curly braces { }" << endl;

    // 6. Return statement
    cout << "6. Return 0 indicates successful execution" << endl;

    cout << endl;
    cout << "Program executed successfully!" << endl;

    // Return 0 to indicate successful execution
    return 0;
}

/*
 * Expected Output:
 * === C++ Program Structure Demo ===
 *
 * 1. Preprocessor Directives: #include <iostream>
 * 2. Namespace: using namespace std;
 * 3. Main Function: int main() { ... }
 * 4. Each statement ends with a semicolon ;
 * 5. Code blocks use curly braces { }
 * 6. Return 0 indicates successful execution
 *
 * Program executed successfully!
 */
