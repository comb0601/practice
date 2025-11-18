/*
 * Program: Inline Functions Demo
 * Description: Demonstrates inline functions for performance optimization
 * Compilation: g++ 08_inline_functions.cpp -o 08_inline_functions -O2
 * Execution: ./08_inline_functions
 */

#include <iostream>
using namespace std;

// Inline function - compiler attempts to replace function call with function body
inline int square(int x) {
    return x * x;
}

inline int cube(int x) {
    return x * x * x;
}

inline int max(int a, int b) {
    return (a > b) ? a : b;
}

inline int min(int a, int b) {
    return (a < b) ? a : b;
}

inline double circleArea(double radius) {
    return 3.14159265359 * radius * radius;
}

// Regular function for comparison
int squareRegular(int x) {
    return x * x;
}

int main() {
    cout << "=== Inline Functions Demo ===" << endl << endl;

    cout << "1. Inline Square Function:" << endl;
    cout << "Square of 5: " << square(5) << endl;
    cout << "Square of 10: " << square(10) << endl << endl;

    cout << "2. Inline Cube Function:" << endl;
    cout << "Cube of 3: " << cube(3) << endl;
    cout << "Cube of 5: " << cube(5) << endl << endl;

    cout << "3. Inline Max/Min Functions:" << endl;
    cout << "Max(15, 20): " << max(15, 20) << endl;
    cout << "Min(15, 20): " << min(15, 20) << endl << endl;

    cout << "4. Inline Circle Area:" << endl;
    cout << "Area (radius=5): " << circleArea(5) << endl;
    cout << "Area (radius=10): " << circleArea(10) << endl << endl;

    // Performance demonstration
    cout << "5. Performance Note:" << endl;
    cout << "Inline functions are suggested to be expanded at call site by compiler" << endl;
    cout << "This can improve performance by eliminating function call overhead" << endl;
    cout << "Best for small, frequently called functions" << endl << endl;

    cout << "Inline advantages:" << endl;
    cout << "- Eliminates function call overhead" << endl;
    cout << "- Faster execution for small functions" << endl;
    cout << "- Maintains code readability" << endl << endl;

    cout << "Inline limitations:" << endl;
    cout << "- Compiler may ignore inline suggestion for large functions" << endl;
    cout << "- Can increase code size if overused" << endl;
    cout << "- Not suitable for recursive functions" << endl;

    return 0;
}
