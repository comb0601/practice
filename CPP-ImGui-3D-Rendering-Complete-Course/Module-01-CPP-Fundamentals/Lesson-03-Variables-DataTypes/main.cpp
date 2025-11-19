#include <iostream>
#include <climits>
#include <cfloat>
#include <string>

/**
 * Lesson 03: Variables and Data Types
 * Comprehensive demonstration of all fundamental C++ data types
 */

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Lesson 03: Variables and Data Types" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // INTEGER TYPES
    std::cout << "=== INTEGER TYPES ===" << std::endl;
    short shortNum = 32000;
    int intNum = 2147483647;
    long longNum = 2147483647L;
    long long llNum = 9223372036854775807LL;

    std::cout << "short:     " << shortNum << " (size: " << sizeof(short) << " bytes)" << std::endl;
    std::cout << "int:       " << intNum << " (size: " << sizeof(int) << " bytes)" << std::endl;
    std::cout << "long:      " << longNum << " (size: " << sizeof(long) << " bytes)" << std::endl;
    std::cout << "long long: " << llNum << " (size: " << sizeof(long long) << " bytes)" << std::endl;
    std::cout << std::endl;

    // FLOATING POINT TYPES
    std::cout << "=== FLOATING POINT TYPES ===" << std::endl;
    float floatNum = 3.14159f;
    double doubleNum = 3.141592653589793;
    long double ldNum = 3.14159265358979323846L;

    std::cout << "float:       " << floatNum << " (size: " << sizeof(float) << " bytes)" << std::endl;
    std::cout << "double:      " << doubleNum << " (size: " << sizeof(double) << " bytes)" << std::endl;
    std::cout << "long double: " << ldNum << " (size: " << sizeof(long double) << " bytes)" << std::endl;
    std::cout << std::endl;

    // CHARACTER AND BOOLEAN TYPES
    std::cout << "=== CHARACTER AND BOOLEAN ===" << std::endl;
    char letter = 'A';
    bool isTrue = true;
    bool isFalse = false;

    std::cout << "char:  '" << letter << "' (ASCII: " << static_cast<int>(letter) << ")" << std::endl;
    std::cout << "bool:  " << std::boolalpha << isTrue << " or " << isFalse << std::endl;
    std::cout << std::endl;

    // STRING TYPE
    std::cout << "=== STRING TYPE ===" << std::endl;
    std::string message = "Hello, C++!";
    std::cout << "string: \"" << message << "\"" << std::endl;
    std::cout << std::endl;

    // AUTO KEYWORD
    std::cout << "=== AUTO KEYWORD ===" << std::endl;
    auto autoInt = 42;
    auto autoDouble = 3.14;
    auto autoString = std::string("Automatic type deduction");

    std::cout << "auto int:    " << autoInt << std::endl;
    std::cout << "auto double: " << autoDouble << std::endl;
    std::cout << "auto string: " << autoString << std::endl;

    return 0;
}
