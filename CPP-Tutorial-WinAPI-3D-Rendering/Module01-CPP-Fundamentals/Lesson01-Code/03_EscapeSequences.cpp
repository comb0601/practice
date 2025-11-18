/*
 * Lesson 01 - Example 03: Escape Sequences
 *
 * Demonstrates various escape sequences in C++.
 * Escape sequences start with backslash (\) and have special meanings.
 *
 * Compile: g++ 03_EscapeSequences.cpp -o EscapeSequences
 */

#include <iostream>

int main()
{
    std::cout << "=== Escape Sequences Demo ===\n\n";

    // Newline
    std::cout << "1. Newline (\\n):\n";
    std::cout << "Line 1\nLine 2\nLine 3\n\n";

    // Tab
    std::cout << "2. Tab (\\t):\n";
    std::cout << "Column1\tColumn2\tColumn3\n";
    std::cout << "Data1\tData2\tData3\n\n";

    // Backslash
    std::cout << "3. Backslash (\\\\):\n";
    std::cout << "Path: C:\\Program Files\\MyApp\n\n";

    // Quotes
    std::cout << "4. Double Quote (\\\"):\n";
    std::cout << "He said, \"Hello World!\"\n\n";

    std::cout << "5. Single Quote (\\'):\n";
    std::cout << "Character: 'A'\n\n";

    // Carriage return (moves cursor to start of line)
    std::cout << "6. Carriage Return (\\r):\n";
    std::cout << "Loading...\\r";
    std::cout << "Complete!\n\n";

    // Backspace
    std::cout << "7. Backspace (\\b):\n";
    std::cout << "Typo\b\bo fix\n\n";

    // Alert/Bell (may beep on some systems)
    std::cout << "8. Alert (\\a):\n";
    std::cout << "This might beep!\a\n";

    return 0;
}
