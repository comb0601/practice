#include <iostream>
#include <string>

/**
 * Example 05: Understanding Namespaces
 *
 * Demonstrates:
 * - What namespaces are and why they exist
 * - Using the std:: prefix
 * - Using declarations
 * - Best practices
 */

// Example namespace demonstrating the concept
namespace MyCode {
    void greet() {
        std::cout << "Hello from MyCode namespace!" << std::endl;
    }

    int version = 1;
}

namespace OtherCode {
    void greet() {
        std::cout << "Hello from OtherCode namespace!" << std::endl;
    }

    int version = 2;
}

int main() {
    std::cout << "=== Understanding Namespaces ===" << std::endl;
    std::cout << std::endl;

    // Method 1: Full qualification (RECOMMENDED)
    std::cout << "Method 1: Using std:: prefix" << std::endl;
    std::cout << "This is the clearest and safest approach." << std::endl;
    std::cout << std::endl;

    // Method 2: Using declarations for specific names
    std::cout << "Method 2: Using declarations" << std::endl;
    {
        using std::cout;
        using std::endl;
        cout << "Now we can use cout without std::" << endl;
        cout << "But only in this scope!" << endl;
    }
    std::cout << std::endl;

    // Demonstrating namespace usage
    std::cout << "Calling functions from different namespaces:" << std::endl;
    MyCode::greet();
    OtherCode::greet();
    std::cout << std::endl;

    std::cout << "Accessing variables from different namespaces:" << std::endl;
    std::cout << "MyCode version: " << MyCode::version << std::endl;
    std::cout << "OtherCode version: " << OtherCode::version << std::endl;
    std::cout << std::endl;

    // Why namespaces matter
    std::cout << "Why use namespaces?" << std::endl;
    std::cout << "1. Prevent name conflicts" << std::endl;
    std::cout << "2. Organize code logically" << std::endl;
    std::cout << "3. Make code origin clear" << std::endl;
    std::cout << "4. Allow same names in different contexts" << std::endl;

    return 0;
}

/*
 * NAMESPACE BEST PRACTICES:
 *
 * GOOD:
 * - std::cout << "text";              // Clear and explicit
 * - using std::cout;                  // OK in .cpp files
 * - using std::string;                // Selective imports
 *
 * AVOID:
 * - using namespace std;              // Especially in headers!
 *   (Can cause name conflicts and makes code less clear)
 *
 * NEVER in header files:
 * - using namespace std;              // Pollutes global namespace
 *
 * In this course, we'll primarily use std:: prefix for clarity.
 */
