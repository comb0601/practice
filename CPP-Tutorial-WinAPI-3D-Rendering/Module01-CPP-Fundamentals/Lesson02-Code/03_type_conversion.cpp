/*
 * Program: Type Conversion Demo
 * Description: Demonstrates implicit and explicit type conversion in C++
 * Compilation: g++ 03_type_conversion.cpp -o 03_type_conversion
 * Execution: ./03_type_conversion
 */

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    cout << "=== Type Conversion Demo ===" << endl << endl;

    // Implicit conversion (automatic)
    cout << "1. Implicit Conversion:" << endl;
    int intValue = 42;
    double doubleValue = intValue;  // int to double (automatic)
    cout << "Integer: " << intValue << " -> Double: " << doubleValue << endl << endl;

    // Explicit conversion (casting)
    cout << "2. Explicit Conversion (C-style cast):" << endl;
    double pi = 3.14159;
    int truncatedPi = (int)pi;  // double to int (data loss)
    cout << "Double: " << pi << " -> Integer: " << truncatedPi << endl << endl;

    // Static cast (C++ style)
    cout << "3. Static Cast (C++ style):" << endl;
    double price = 99.99;
    int wholePrice = static_cast<int>(price);
    cout << "Price: $" << price << " -> Whole: $" << wholePrice << endl << endl;

    // Integer division vs float division
    cout << "4. Integer Division vs Float Division:" << endl;
    int a = 7, b = 2;
    cout << "Integer division: " << a << " / " << b << " = " << (a / b) << endl;
    cout << "Float division: " << a << " / " << b << " = "
         << (static_cast<double>(a) / b) << endl << endl;

    // Char to int conversion
    cout << "5. Character to Integer:" << endl;
    char letter = 'A';
    int asciiValue = static_cast<int>(letter);
    cout << "Character: " << letter << " -> ASCII: " << asciiValue << endl;
    cout << "Character: " << (char)(asciiValue + 32) << " (lowercase)" << endl << endl;

    // Mixed arithmetic
    cout << "6. Mixed Arithmetic:" << endl;
    int intNum = 10;
    double doubleNum = 3.5;
    double result = intNum + doubleNum;  // int promoted to double
    cout << intNum << " + " << doubleNum << " = " << result << endl;
    cout << "Result type: double (automatic promotion)" << endl;

    return 0;
}
