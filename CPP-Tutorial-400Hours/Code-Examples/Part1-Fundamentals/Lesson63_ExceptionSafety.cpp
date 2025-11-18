/* Lesson 63: Exception Safety
 * Compile: cl /std:c++17 Lesson63_ExceptionSafety.cpp */
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Resource {
public:
    Resource() { cout << "Resource acquired" << endl; }
    ~Resource() { cout << "Resource released" << endl; }
};

void unsafeFunction() {
    Resource* res = new Resource();
    throw runtime_error("Error!");
    delete res;  // Never executed - memory leak!
}

void safeFunction() {
    unique_ptr<Resource> res = make_unique<Resource>();
    throw runtime_error("Error!");
    // Automatically cleaned up even with exception
}

int main() {
    cout << "=== Safe Exception Handling ===" << endl;
    try {
        safeFunction();
    }
    catch (const exception& e) {
        cout << "Caught: " << e.what() << endl;
    }
    cout << "Resource automatically cleaned up" << endl;
    return 0;
}
