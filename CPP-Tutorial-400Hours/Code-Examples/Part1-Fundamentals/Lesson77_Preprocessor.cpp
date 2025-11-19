/* Lesson 77: Preprocessor Directives
 * Compile: cl Lesson77_Preprocessor.cpp */
#include <iostream>
using namespace std;

#define PI 3.14159
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef _WIN32
    #define PLATFORM "Windows"
#else
    #define PLATFORM "Other"
#endif

#ifndef DEBUG
    #define DEBUG 0
#endif

int main() {
    cout << "PI: " << PI << endl;
    cout << "SQUARE(5): " << SQUARE(5) << endl;
    cout << "MAX(10, 20): " << MAX(10, 20) << endl;
    cout << "Platform: " << PLATFORM << endl;

    #if DEBUG
        cout << "Debug mode" << endl;
    #else
        cout << "Release mode" << endl;
    #endif

    cout << "File: " << __FILE__ << endl;
    cout << "Line: " << __LINE__ << endl;

    return 0;
}
