/* Lesson 79: Debugging Techniques
 * Compile: cl /Zi Lesson79_Debugging.cpp */
#include <iostream>
#include <cassert>
using namespace std;

void debugPrint(int value) {
    #ifdef DEBUG
        cout << "DEBUG: value = " << value << endl;
    #endif
}

int divide(int a, int b) {
    assert(b != 0 && "Division by zero!");
    return a / b;
}

int main() {
    cout << "=== Debugging Techniques ===" << endl << endl;

    // 1. COUT DEBUGGING
    int x = 10;
    cout << "x = " << x << endl;

    // 2. ASSERT
    int result = divide(10, 2);
    cout << "Result: " << result << endl;

    // 3. BREAKPOINTS (use debugger)
    cout << "Set breakpoint here in IDE" << endl;

    // 4. CONDITIONAL COMPILATION
    debugPrint(42);

    cout << "\nDebugging tips:" << endl;
    cout << "- Use Visual Studio debugger (F5)" << endl;
    cout << "- Set breakpoints (F9)" << endl;
    cout << "- Step through code (F10/F11)" << endl;
    cout << "- Watch variables" << endl;
    cout << "- Use assert() for conditions" << endl;

    return 0;
}
