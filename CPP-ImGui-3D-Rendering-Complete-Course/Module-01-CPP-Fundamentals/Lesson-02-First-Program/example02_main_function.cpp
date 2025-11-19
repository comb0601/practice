#include <iostream>

/**
 * Example 02: Understanding the main() Function
 *
 * Demonstrates:
 * - Different main() function signatures
 * - Return values and their meanings
 * - How the program entry point works
 */

// This is the standard main() function with no parameters
int main() {
    std::cout << "=== Understanding main() ===" << std::endl;
    std::cout << std::endl;

    std::cout << "This program started at the main() function." << std::endl;
    std::cout << "Everything inside the curly braces { } is the function body." << std::endl;
    std::cout << std::endl;

    std::cout << "Return value meanings:" << std::endl;
    std::cout << "  0  = Success (everything worked correctly)" << std::endl;
    std::cout << "  1  = Generic error" << std::endl;
    std::cout << "  2+ = Specific error codes" << std::endl;
    std::cout << std::endl;

    // Conditional return based on logic
    bool everythingOk = true;

    if (everythingOk) {
        std::cout << "Program completed successfully!" << std::endl;
        std::cout << "Returning 0 to indicate success." << std::endl;
        return 0;  // Success
    } else {
        std::cout << "An error occurred!" << std::endl;
        std::cout << "Returning 1 to indicate an error." << std::endl;
        return 1;  // Error
    }
}

/*
 * Note: You can also have main() with command-line arguments:
 *
 * int main(int argc, char* argv[]) {
 *     // argc is the number of arguments
 *     // argv is an array of argument strings
 *     return 0;
 * }
 *
 * We'll explore this in later lessons!
 */
