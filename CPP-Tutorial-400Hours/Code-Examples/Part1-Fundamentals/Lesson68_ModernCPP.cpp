/* Lesson 68: Modern C++ Features (C++11/14/17)
 * Compile: cl /std:c++17 Lesson68_ModernCPP.cpp */
#include <iostream>
#include <vector>
#include <memory>
using namespace std;

int main() {
    // AUTO KEYWORD
    auto x = 42;
    auto pi = 3.14;
    auto name = string("C++");

    // RANGE-BASED FOR
    vector<int> v = {1, 2, 3, 4, 5};
    for (const auto& num : v) {
        cout << num << " ";
    }
    cout << endl;

    // NULLPTR
    int* ptr = nullptr;

    // SMART POINTERS
    auto up = make_unique<int>(100);
    auto sp = make_shared<string>("Hello");

    // INITIALIZER LISTS
    vector<int> vec{1, 2, 3, 4, 5};

    // LAMBDA
    auto square = [](int x) { return x * x; };
    cout << "Square of 5: " << square(5) << endl;

    return 0;
}
