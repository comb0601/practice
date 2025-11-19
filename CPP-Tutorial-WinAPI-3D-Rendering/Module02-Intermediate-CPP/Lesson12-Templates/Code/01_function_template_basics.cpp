/*
 * Lesson 12: Templates - Function Template Basics
 *
 * This example demonstrates basic function template syntax and usage.
 *
 * Compilation: g++ -std=c++17 01_function_template_basics.cpp -o func_template
 * Execution: ./func_template
 */

#include <iostream>
#include <string>

// Basic function template
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// Function template with multiple type parameters
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// Function template with non-type parameter
template<typename T, int N>
T multiply(T value) {
    return value * N;
}

// Function template specialization
template<typename T>
void print(T value) {
    std::cout << value;
}

// Specialization for const char*
template<>
void print<const char*>(const char* value) {
    std::cout << "String: " << value;
}

// Template with default type
template<typename T = int>
T getDefault() {
    return T();
}

int main() {
    std::cout << "=== Function Template Basics ===" << std::endl << std::endl;

    // Basic template usage
    std::cout << "1. Basic Templates:" << std::endl;
    std::cout << "max(10, 20) = " << maximum(10, 20) << std::endl;
    std::cout << "max(3.14, 2.71) = " << maximum(3.14, 2.71) << std::endl;
    std::cout << "max('a', 'z') = " << maximum('a', 'z') << std::endl;

    // Explicit type specification
    std::cout << "\n2. Explicit Type Specification:" << std::endl;
    std::cout << "max<double>(10, 20.5) = " << maximum<double>(10, 20.5) << std::endl;

    // Multiple type parameters
    std::cout << "\n3. Multiple Type Parameters:" << std::endl;
    std::cout << "add(10, 20.5) = " << add(10, 20.5) << std::endl;
    std::cout << "add(3.14, 2) = " << add(3.14, 2) << std::endl;

    // Non-type parameters
    std::cout << "\n4. Non-Type Parameters:" << std::endl;
    std::cout << "multiply<int, 10>(5) = " << multiply<int, 10>(5) << std::endl;
    std::cout << "multiply<double, 3>(2.5) = " << multiply<double, 3>(2.5) << std::endl;

    // Template specialization
    std::cout << "\n5. Template Specialization:" << std::endl;
    print(42); std::cout << std::endl;
    print(3.14); std::cout << std::endl;
    print("Hello"); std::cout << std::endl;

    // Default template parameter
    std::cout << "\n6. Default Template Parameter:" << std::endl;
    std::cout << "getDefault<int>() = " << getDefault<int>() << std::endl;
    std::cout << "getDefault<double>() = " << getDefault<double>() << std::endl;
    std::cout << "getDefault() = " << getDefault() << std::endl;

    return 0;
}
