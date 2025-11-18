#include <iostream>

/**
 * Solution 01: Your First Program
 *
 * This is a complete solution to Exercise 01.
 * Your solution might be different - that's okay!
 */

int main() {
    std::cout << "=== Exercise 01: About Me ===" << std::endl;
    std::cout << std::endl;

    // Print name
    std::cout << "My name is: Alex Johnson" << std::endl;

    // Print favorite programming language
    std::cout << "My favorite language is: Python" << std::endl;

    // Print learning goal
    std::cout << "I am learning C++ to: Build high-performance 3D graphics applications" << std::endl;

    std::cout << std::endl;
    std::cout << "Exercise 01 complete!" << std::endl;

    return 0;
}

/*
 * EXPECTED OUTPUT:
 *
 * === Exercise 01: About Me ===
 *
 * My name is: Alex Johnson
 * My favorite language is: Python
 * I am learning C++ to: Build high-performance 3D graphics applications
 *
 * Exercise 01 complete!
 *
 * KEY LEARNING POINTS:
 *
 * 1. std::cout is used for console output
 * 2. << operator chains output items
 * 3. std::endl adds a newline and flushes the buffer
 * 4. String literals are enclosed in double quotes
 * 5. Every statement ends with a semicolon
 * 6. main() returns 0 to indicate success
 */
