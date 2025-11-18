/*
 * Program: Data Type Sizes
 * Description: Displays the size and range of various data types
 * Compilation: g++ 10_data_type_sizes.cpp -o 10_data_type_sizes
 * Execution: ./10_data_type_sizes
 */

#include <iostream>
#include <limits>
using namespace std;

int main() {
    cout << "=== Data Type Sizes and Ranges ===" << endl << endl;

    // Integer types
    cout << "Integer Types:" << endl;
    cout << "char: " << sizeof(char) << " byte(s), "
         << "Range: " << (int)numeric_limits<char>::min()
         << " to " << (int)numeric_limits<char>::max() << endl;

    cout << "short: " << sizeof(short) << " byte(s), "
         << "Range: " << numeric_limits<short>::min()
         << " to " << numeric_limits<short>::max() << endl;

    cout << "int: " << sizeof(int) << " byte(s), "
         << "Range: " << numeric_limits<int>::min()
         << " to " << numeric_limits<int>::max() << endl;

    cout << "long: " << sizeof(long) << " byte(s), "
         << "Range: " << numeric_limits<long>::min()
         << " to " << numeric_limits<long>::max() << endl;

    cout << "long long: " << sizeof(long long) << " byte(s), "
         << "Range: " << numeric_limits<long long>::min()
         << " to " << numeric_limits<long long>::max() << endl << endl;

    // Unsigned integer types
    cout << "Unsigned Integer Types:" << endl;
    cout << "unsigned char: " << sizeof(unsigned char) << " byte(s), "
         << "Range: " << (int)numeric_limits<unsigned char>::min()
         << " to " << (int)numeric_limits<unsigned char>::max() << endl;

    cout << "unsigned short: " << sizeof(unsigned short) << " byte(s), "
         << "Range: " << numeric_limits<unsigned short>::min()
         << " to " << numeric_limits<unsigned short>::max() << endl;

    cout << "unsigned int: " << sizeof(unsigned int) << " byte(s), "
         << "Range: " << numeric_limits<unsigned int>::min()
         << " to " << numeric_limits<unsigned int>::max() << endl;

    cout << "unsigned long: " << sizeof(unsigned long) << " byte(s), "
         << "Range: " << numeric_limits<unsigned long>::min()
         << " to " << numeric_limits<unsigned long>::max() << endl << endl;

    // Floating-point types
    cout << "Floating-Point Types:" << endl;
    cout << "float: " << sizeof(float) << " byte(s), "
         << "Range: " << numeric_limits<float>::min()
         << " to " << numeric_limits<float>::max() << endl;

    cout << "double: " << sizeof(double) << " byte(s), "
         << "Range: " << numeric_limits<double>::min()
         << " to " << numeric_limits<double>::max() << endl;

    cout << "long double: " << sizeof(long double) << " byte(s), "
         << "Range: " << numeric_limits<long double>::min()
         << " to " << numeric_limits<long double>::max() << endl << endl;

    // Boolean type
    cout << "Boolean Type:" << endl;
    cout << "bool: " << sizeof(bool) << " byte(s)" << endl << endl;

    // Precision
    cout << "Floating-Point Precision:" << endl;
    cout << "float: " << numeric_limits<float>::digits10 << " decimal digits" << endl;
    cout << "double: " << numeric_limits<double>::digits10 << " decimal digits" << endl;
    cout << "long double: " << numeric_limits<long double>::digits10 << " decimal digits" << endl;

    return 0;
}
