#include <iostream>

/**
 * Example 04: Understanding Namespaces
 *
 * Demonstrates:
 * - What namespaces are
 * - Using std:: prefix
 * - The 'using' directive
 * - Why we typically use std::
 */

// Option 1: Always use std:: prefix (RECOMMENDED)
void example_with_prefix() {
    std::cout << "Using std:: prefix (recommended)" << std::endl;
    std::cout << "This makes code clear and avoids naming conflicts" << std::endl;
}

// Option 2: Using directive (use cautiously)
void example_with_using() {
    using std::cout;
    using std::endl;

    cout << "Using 'using' declarations" << endl;
    cout << "Only cout and endl are imported here" << endl;
}

int main() {
    std::cout << "=== Namespace Examples ===" << std::endl;
    std::cout << std::endl;

    example_with_prefix();
    std::cout << std::endl;

    example_with_using();
    std::cout << std::endl;

    // Why namespaces exist
    std::cout << "Why use namespaces?" << std::endl;
    std::cout << "- Prevent naming conflicts" << std::endl;
    std::cout << "- Organize code into logical groups" << std::endl;
    std::cout << "- std:: contains standard library components" << std::endl;

    return 0;
}

/*
 * IMPORTANT NOTE:
 *
 * You might see "using namespace std;" in some code.
 * This imports ALL names from std namespace.
 *
 * using namespace std;  // NOT RECOMMENDED
 *
 * Why avoid it?
 * - Can cause naming conflicts
 * - Makes code less clear
 * - Can break when new standard library features are added
 *
 * In this course, we'll always use std:: prefix for clarity.
 */
