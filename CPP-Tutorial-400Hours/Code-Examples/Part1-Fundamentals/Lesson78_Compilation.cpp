/* Lesson 78: Compilation Process
 * Compile: cl Lesson78_Compilation.cpp */
#include <iostream>
using namespace std;

void externalFunction();  // Declaration

int main() {
    cout << "=== Compilation Process ===" << endl;
    cout << "1. Preprocessing: Handles #include, #define" << endl;
    cout << "2. Compilation: Converts to object code" << endl;
    cout << "3. Linking: Combines object files" << endl;
    cout << "4. Execution: Runs the program" << endl;

    cout << "\nCompile commands:" << endl;
    cout << "MSVC: cl /std:c++17 file.cpp" << endl;
    cout << "GCC: g++ -std=c++17 file.cpp -o file" << endl;
    cout << "Clang: clang++ -std=c++17 file.cpp -o file" << endl;

    return 0;
}
