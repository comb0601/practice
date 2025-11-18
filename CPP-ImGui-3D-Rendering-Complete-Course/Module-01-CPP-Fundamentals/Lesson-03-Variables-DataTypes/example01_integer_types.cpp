#include <iostream>
#include <climits>

/**
 * Example 01: Integer Data Types
 * Demonstrates all integer types and their properties
 */

int main() {
    std::cout << "=== Integer Types in C++ ===" << std::endl;
    std::cout << std::endl;

    // short int (16 bits minimum)
    short s = 30000;
    std::cout << "short int:" << std::endl;
    std::cout << "  Value: " << s << std::endl;
    std::cout << "  Size: " << sizeof(s) << " bytes" << std::endl;
    std::cout << "  Range: " << SHRT_MIN << " to " << SHRT_MAX << std::endl;
    std::cout << std::endl;

    // int (32 bits on most systems)
    int i = 2000000000;
    std::cout << "int:" << std::endl;
    std::cout << "  Value: " << i << std::endl;
    std::cout << "  Size: " << sizeof(i) << " bytes" << std::endl;
    std::cout << "  Range: " << INT_MIN << " to " << INT_MAX << std::endl;
    std::cout << std::endl;

    // long int (32 or 64 bits)
    long l = 3000000000L;
    std::cout << "long int:" << std::endl;
    std::cout << "  Value: " << l << std::endl;
    std::cout << "  Size: " << sizeof(l) << " bytes" << std::endl;
    std::cout << "  Range: " << LONG_MIN << " to " << LONG_MAX << std::endl;
    std::cout << std::endl;

    // long long int (64 bits minimum)
    long long ll = 9000000000000000000LL;
    std::cout << "long long int:" << std::endl;
    std::cout << "  Value: " << ll << std::endl;
    std::cout << "  Size: " << sizeof(ll) << " bytes" << std::endl;
    std::cout << "  Range: " << LLONG_MIN << " to " << LLONG_MAX << std::endl;

    return 0;
}
