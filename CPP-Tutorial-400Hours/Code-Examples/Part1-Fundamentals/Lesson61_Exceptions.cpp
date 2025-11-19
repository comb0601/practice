/* Lesson 61: Exception Handling
 * Compile: cl Lesson61_Exceptions.cpp */
#include <iostream>
#include <stdexcept>
using namespace std;

double divide(double a, double b) {
    if (b == 0) {
        throw runtime_error("Division by zero!");
    }
    return a / b;
}

int main() {
    try {
        cout << "10 / 2 = " << divide(10, 2) << endl;
        cout << "10 / 0 = " << divide(10, 0) << endl;
    }
    catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }
    catch (...) {
        cout << "Unknown error!" << endl;
    }

    cout << "Program continues..." << endl;
    return 0;
}
