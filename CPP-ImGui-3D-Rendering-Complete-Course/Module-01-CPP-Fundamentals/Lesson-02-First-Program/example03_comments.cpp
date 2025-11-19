#include <iostream>

/**
 * Example 03: Different Types of Comments
 *
 * This program demonstrates the various ways to write comments in C++.
 * Comments are ignored by the compiler but are crucial for code documentation.
 */

// This is a single-line comment
// The compiler completely ignores this text

/*
 * This is a multi-line comment.
 * It can span multiple lines.
 * Useful for longer explanations.
 */

int main() {
    // Output program header
    std::cout << "=== Understanding Comments ===" << std::endl;

    /* You can also put multi-line comments on one line */
    std::cout << "Comments help document your code." << std::endl;

    // Comments can appear at the end of a line
    int x = 42;  // This is the answer to life, the universe, and everything

    /*
     * GOOD COMMENT PRACTICES:
     * - Explain WHY, not WHAT
     * - Document complex algorithms
     * - Warn about edge cases
     * - Provide usage examples
     */

    // BAD: The following line sets y to 10
    int y = 10;

    // GOOD: Using base-10 for user-friendly display
    int base = 10;

    // You can temporarily disable code with comments
    std::cout << "This line will execute" << std::endl;
    // std::cout << "This line is commented out" << std::endl;

    std::cout << std::endl;
    std::cout << "Value of x: " << x << std::endl;
    std::cout << "Value of y: " << y << std::endl;
    std::cout << "Base: " << base << std::endl;

    return 0;  // Return success
}

// Everything after main() won't execute unless called

/*
 * KEYBOARD SHORTCUTS FOR COMMENTING:
 *
 * Visual Studio Code / Visual Studio:
 * - Ctrl+/ (or Cmd+/ on Mac): Toggle line comment
 * - Shift+Alt+A: Toggle block comment
 *
 * Most IDEs have similar shortcuts!
 */
