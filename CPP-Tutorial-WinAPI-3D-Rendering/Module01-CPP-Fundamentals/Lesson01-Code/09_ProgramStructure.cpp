/*
 * Lesson 01 - Example 09: Understanding Program Structure
 *
 * This program explains each part of a C++ program's structure.
 * It helps you understand what each component does.
 *
 * Compile: g++ 09_ProgramStructure.cpp -o ProgramStructure
 */

// PREPROCESSOR DIRECTIVES
// These start with # and are processed before compilation
#include <iostream>  // Include the input/output stream library

// MAIN FUNCTION
// Every C++ program must have exactly one main() function
// Program execution starts here
int main()  // int means this function returns an integer
{           // Opening brace - starts function body

    // STATEMENTS
    // These are the instructions that the program executes
    // Each statement must end with a semicolon

    std::cout << "=== C++ Program Structure ===\n\n";

    std::cout << "1. PREPROCESSOR DIRECTIVES:\n";
    std::cout << "   - Start with #\n";
    std::cout << "   - Example: #include <iostream>\n";
    std::cout << "   - Processed before compilation\n\n";

    std::cout << "2. MAIN FUNCTION:\n";
    std::cout << "   - int main() { ... }\n";
    std::cout << "   - Program starts executing here\n";
    std::cout << "   - Must return an integer\n\n";

    std::cout << "3. STATEMENTS:\n";
    std::cout << "   - Instructions for the computer\n";
    std::cout << "   - Must end with semicolon (;)\n";
    std::cout << "   - Executed in order from top to bottom\n\n";

    std::cout << "4. COUT FOR OUTPUT:\n";
    std::cout << "   - std::cout << \"text\";\n";
    std::cout << "   - Sends output to console\n";
    std::cout << "   - << is the insertion operator\n\n";

    std::cout << "5. RETURN STATEMENT:\n";
    std::cout << "   - return 0;\n";
    std::cout << "   - Exits the program\n";
    std::cout << "   - 0 means successful execution\n\n";

    std::cout << "6. BRACES { }:\n";
    std::cout << "   - Define blocks of code\n";
    std::cout << "   - Must be balanced (open and close)\n\n";

    // RETURN STATEMENT
    // Returns control to the operating system
    // 0 typically means the program ran successfully
    return 0;

}           // Closing brace - ends function body
