/*
 * Lesson 2: Data Types - Understanding C++ Data Types
 *
 * This program demonstrates all fundamental C++ data types:
 * - Integer types (int, short, long, long long)
 * - Floating-point types (float, double, long double)
 * - Character type (char)
 * - Boolean type (bool)
 * - String type (std::string)
 * - Size and range of each type
 *
 * Compile: cl Lesson02_DataTypes.cpp
 */

#include <iostream>
#include <string>
#include <limits>   // For numeric_limits
#include <iomanip>  // For formatting output
using namespace std;

int main() {
    cout << "=== C++ Data Types Demo ===" << endl << endl;

    // INTEGER TYPES
    cout << "=== INTEGER TYPES ===" << endl;

    // int - standard integer (usually 4 bytes)
    int standardInt = 2147483647;
    cout << "int: " << standardInt << endl;
    cout << "  Size: " << sizeof(int) << " bytes" << endl;
    cout << "  Range: " << numeric_limits<int>::min() << " to "
         << numeric_limits<int>::max() << endl << endl;

    // short int - smaller integer (usually 2 bytes)
    short shortInt = 32767;
    cout << "short: " << shortInt << endl;
    cout << "  Size: " << sizeof(short) << " bytes" << endl;
    cout << "  Range: " << numeric_limits<short>::min() << " to "
         << numeric_limits<short>::max() << endl << endl;

    // long int - long integer (at least 4 bytes)
    long longInt = 2147483647L;
    cout << "long: " << longInt << endl;
    cout << "  Size: " << sizeof(long) << " bytes" << endl;
    cout << "  Range: " << numeric_limits<long>::min() << " to "
         << numeric_limits<long>::max() << endl << endl;

    // long long int - very long integer (at least 8 bytes)
    long long longLongInt = 9223372036854775807LL;
    cout << "long long: " << longLongInt << endl;
    cout << "  Size: " << sizeof(long long) << " bytes" << endl;
    cout << "  Range: " << numeric_limits<long long>::min() << " to "
         << numeric_limits<long long>::max() << endl << endl;

    // unsigned variants (only positive numbers)
    unsigned int unsignedInt = 4294967295U;
    cout << "unsigned int: " << unsignedInt << endl;
    cout << "  Size: " << sizeof(unsigned int) << " bytes" << endl;
    cout << "  Range: 0 to " << numeric_limits<unsigned int>::max() << endl << endl;

    // FLOATING-POINT TYPES
    cout << "=== FLOATING-POINT TYPES ===" << endl;

    // float - single precision (4 bytes)
    float floatNum = 3.14159f;
    cout << "float: " << floatNum << endl;
    cout << "  Size: " << sizeof(float) << " bytes" << endl;
    cout << "  Precision: ~" << numeric_limits<float>::digits10 << " decimal digits" << endl << endl;

    // double - double precision (8 bytes)
    double doubleNum = 3.14159265358979;
    cout << "double: " << doubleNum << endl;
    cout << "  Size: " << sizeof(double) << " bytes" << endl;
    cout << "  Precision: ~" << numeric_limits<double>::digits10 << " decimal digits" << endl << endl;

    // long double - extended precision
    long double longDoubleNum = 3.14159265358979323846L;
    cout << "long double: " << longDoubleNum << endl;
    cout << "  Size: " << sizeof(long double) << " bytes" << endl;
    cout << "  Precision: ~" << numeric_limits<long double>::digits10 << " decimal digits" << endl << endl;

    // CHARACTER TYPE
    cout << "=== CHARACTER TYPE ===" << endl;
    char character = 'A';
    char digit = '5';
    char symbol = '@';

    cout << "char: " << character << endl;
    cout << "  Size: " << sizeof(char) << " byte" << endl;
    cout << "  ASCII value of '" << character << "': " << (int)character << endl;
    cout << "  Examples: '" << character << "', '" << digit << "', '" << symbol << "'" << endl << endl;

    // BOOLEAN TYPE
    cout << "=== BOOLEAN TYPE ===" << endl;
    bool isTrue = true;
    bool isFalse = false;

    cout << "bool: " << endl;
    cout << "  Size: " << sizeof(bool) << " byte" << endl;
    cout << "  true value: " << isTrue << " (displayed as 1)" << endl;
    cout << "  false value: " << isFalse << " (displayed as 0)" << endl;
    cout << "  As text: " << boolalpha << isTrue << ", " << isFalse << endl << endl;

    // STRING TYPE
    cout << "=== STRING TYPE ===" << endl;
    string text = "Hello, C++!";
    string emptyString = "";

    cout << "string: \"" << text << "\"" << endl;
    cout << "  Length: " << text.length() << " characters" << endl;
    cout << "  Size in memory: " << sizeof(text) << " bytes (object size)" << endl << endl;

    // TYPE MODIFIERS
    cout << "=== SIGNED vs UNSIGNED ===" << endl;
    signed int signedInt = -100;
    unsigned int unsignedPositive = 100;

    cout << "signed int: " << signedInt << " (can be negative)" << endl;
    cout << "unsigned int: " << unsignedPositive << " (only positive)" << endl << endl;

    // SPECIAL VALUES
    cout << "=== SPECIAL VALUES ===" << endl;
    cout << "Null character: '\\0' (ASCII " << (int)'\0' << ")" << endl;
    cout << "Null pointer: nullptr" << endl;

    return 0;
}

/*
 * Expected Output (values may vary by system):
 * === C++ Data Types Demo ===
 *
 * === INTEGER TYPES ===
 * int: 2147483647
 *   Size: 4 bytes
 *   Range: -2147483648 to 2147483647
 *
 * short: 32767
 *   Size: 2 bytes
 *   Range: -32768 to 32767
 *
 * long: 2147483647
 *   Size: 4 bytes
 *   Range: -2147483648 to 2147483647
 *
 * long long: 9223372036854775807
 *   Size: 8 bytes
 *   Range: -9223372036854775808 to 9223372036854775807
 *
 * ... (and so on for all data types)
 */
