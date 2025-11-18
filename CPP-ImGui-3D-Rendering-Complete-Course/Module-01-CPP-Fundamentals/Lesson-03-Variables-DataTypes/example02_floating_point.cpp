#include <iostream>
#include <iomanip>
#include <cfloat>

/**
 * Example 02: Floating Point Types
 * Demonstrates float, double, and long double
 */

int main() {
    std::cout << std::fixed << std::setprecision(15);
    std::cout << "=== Floating Point Types ===" << std::endl;
    std::cout << std::endl;

    // float (32 bits, ~7 significant digits)
    float f = 3.14159265358979f;
    std::cout << "float:" << std::endl;
    std::cout << "  Value: " << f << std::endl;
    std::cout << "  Size: " << sizeof(f) << " bytes" << std::endl;
    std::cout << "  Min: " << FLT_MIN << std::endl;
    std::cout << "  Max: " << FLT_MAX << std::endl;
    std::cout << std::endl;

    // double (64 bits, ~15 significant digits)
    double d = 3.14159265358979;
    std::cout << "double:" << std::endl;
    std::cout << "  Value: " << d << std::endl;
    std::cout << "  Size: " << sizeof(d) << " bytes" << std::endl;
    std::cout << "  Min: " << DBL_MIN << std::endl;
    std::cout << "  Max: " << DBL_MAX << std::endl;
    std::cout << std::endl;

    // long double (80 bits or more, ~18 significant digits)
    long double ld = 3.14159265358979323846L;
    std::cout << "long double:" << std::endl;
    std::cout << "  Value: " << ld << std::endl;
    std::cout << "  Size: " << sizeof(ld) << " bytes" << std::endl;
    std::cout << "  Min: " << LDBL_MIN << std::endl;
    std::cout << "  Max: " << LDBL_MAX << std::endl;

    return 0;
}
