/* Lesson 80: C++ Best Practices
 * Compile: cl /std:c++17 Lesson80_BestPractices.cpp */
#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

// 1. Use const correctness
void printValue(const int& value) {
    cout << "Value: " << value << endl;
}

// 2. Use references to avoid copying
void processVector(const vector<int>& vec) {
    for (const auto& item : vec) {
        cout << item << " ";
    }
    cout << endl;
}

// 3. RAII - Resource Acquisition Is Initialization
class Resource {
public:
    Resource() { cout << "Resource acquired" << endl; }
    ~Resource() { cout << "Resource released" << endl; }
};

int main() {
    cout << "=== C++ Best Practices ===" << endl << endl;

    // 1. PREFER auto
    auto x = 42;
    auto name = string("C++");

    // 2. USE SMART POINTERS
    auto ptr = make_unique<int>(100);

    // 3. INITIALIZE VARIABLES
    int value = 0;
    vector<int> vec{1, 2, 3, 4, 5};

    // 4. USE CONST
    const double PI = 3.14159;

    // 5. RANGE-BASED FOR LOOPS
    for (const auto& num : vec) {
        // ...
    }

    // 6. RAII
    {
        Resource res;
        // Automatically cleaned up
    }

    cout << "\nBest Practices Summary:" << endl;
    cout << "1. Use const correctness" << endl;
    cout << "2. Prefer smart pointers over raw pointers" << endl;
    cout << "3. Use RAII for resource management" << endl;
    cout << "4. Initialize all variables" << endl;
    cout << "5. Use auto for type deduction" << endl;
    cout << "6. Prefer references over pointers" << endl;
    cout << "7. Use range-based for loops" << endl;
    cout << "8. Follow the Rule of Zero/Three/Five" << endl;
    cout << "9. Use STL containers and algorithms" << endl;
    cout << "10. Write self-documenting code" << endl;

    return 0;
}
