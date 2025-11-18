/**
 * Basic Function Templates
 * Demonstrates simple function template usage with type deduction
 */

#include <iostream>
#include <string>
using namespace std;

// Generic max function
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// Generic min function
template <typename T>
T minimum(T a, T b) {
    return (a < b) ? a : b;
}

// Generic swap function
template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Generic print function
template <typename T>
void print(const T& value) {
    cout << value << endl;
}

// Generic absolute value
template <typename T>
T absolute(T value) {
    return (value < 0) ? -value : value;
}

int main() {
    cout << "=== Basic Function Templates ===" << endl << endl;

    // Integer operations
    cout << "Integer operations:" << endl;
    int x = 10, y = 20;
    cout << "max(" << x << ", " << y << ") = " << maximum(x, y) << endl;
    cout << "min(" << x << ", " << y << ") = " << minimum(x, y) << endl;

    cout << "Before swap: x = " << x << ", y = " << y << endl;
    swap(x, y);
    cout << "After swap: x = " << x << ", y = " << y << endl;
    cout << endl;

    // Double operations
    cout << "Double operations:" << endl;
    double a = 3.14, b = 2.71;
    cout << "max(" << a << ", " << b << ") = " << maximum(a, b) << endl;
    cout << "min(" << a << ", " << b << ") = " << minimum(a, b) << endl;
    cout << "absolute(-5.5) = " << absolute(-5.5) << endl;
    cout << endl;

    // String operations
    cout << "String operations:" << endl;
    string s1 = "apple", s2 = "banana";
    cout << "max(\"" << s1 << "\", \"" << s2 << "\") = " << maximum(s1, s2) << endl;
    cout << "min(\"" << s1 << "\", \"" << s2 << "\") = " << minimum(s1, s2) << endl;
    cout << endl;

    // Character operations
    cout << "Character operations:" << endl;
    char c1 = 'A', c2 = 'Z';
    cout << "max('" << c1 << "', '" << c2 << "') = " << maximum(c1, c2) << endl;
    cout << "Before swap: c1 = " << c1 << ", c2 = " << c2 << endl;
    swap(c1, c2);
    cout << "After swap: c1 = " << c1 << ", c2 = " << c2 << endl;
    cout << endl;

    // Generic print
    cout << "Generic print function:" << endl;
    print(42);
    print(3.14159);
    print("Hello, Templates!");
    print('X');

    return 0;
}
