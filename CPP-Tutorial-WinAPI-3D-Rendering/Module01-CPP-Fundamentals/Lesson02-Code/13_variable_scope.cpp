/*
 * Program: Variable Scope Demo
 * Description: Demonstrates local, global, and block scope
 * Compilation: g++ 13_variable_scope.cpp -o 13_variable_scope
 * Execution: ./13_variable_scope
 */

#include <iostream>
using namespace std;

// Global variables (accessible from anywhere)
int globalVar = 100;
const double PI = 3.14159;

void demoFunction() {
    // Local variable in function
    int localVar = 50;

    cout << "Inside demoFunction():" << endl;
    cout << "  Local variable: " << localVar << endl;
    cout << "  Global variable: " << globalVar << endl;

    // Modifying global variable
    globalVar = 200;
    cout << "  Modified global variable to: " << globalVar << endl;
}

int main() {
    cout << "=== Variable Scope Demo ===" << endl << endl;

    // Local variable in main
    int mainVar = 10;

    cout << "1. Main function scope:" << endl;
    cout << "  Main variable: " << mainVar << endl;
    cout << "  Global variable: " << globalVar << endl;
    cout << "  Global constant PI: " << PI << endl << endl;

    // Block scope
    cout << "2. Block scope:" << endl;
    {
        int blockVar = 20;
        cout << "  Inside block, blockVar: " << blockVar << endl;
        cout << "  Inside block, mainVar: " << mainVar << endl;

        // Variable shadowing
        int mainVar = 30;  // Shadows the outer mainVar
        cout << "  Shadowed mainVar: " << mainVar << endl;
    }
    // blockVar is not accessible here
    cout << "  Outside block, mainVar: " << mainVar << endl << endl;

    // Function scope
    cout << "3. Function scope:" << endl;
    demoFunction();
    cout << "Back in main, global variable is now: " << globalVar << endl << endl;

    // Loop scope
    cout << "4. Loop scope:" << endl;
    for (int i = 0; i < 3; i++) {
        int loopVar = i * 10;
        cout << "  Iteration " << i << ", loopVar: " << loopVar << endl;
    }
    // i and loopVar are not accessible here

    cout << "\n5. Static local variables:" << endl;
    for (int i = 0; i < 3; i++) {
        static int staticVar = 0;  // Initialized only once
        int normalVar = 0;

        staticVar++;
        normalVar++;

        cout << "  Iteration " << i
             << " - Static: " << staticVar
             << ", Normal: " << normalVar << endl;
    }

    return 0;
}
