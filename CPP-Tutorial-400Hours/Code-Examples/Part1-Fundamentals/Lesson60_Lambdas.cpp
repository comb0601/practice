/* Lesson 60: Lambda Expressions (C++11)
 * Compile: cl /std:c++17 Lesson60_Lambdas.cpp */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    auto add = [](int a, int b) { return a + b; };
    cout << "5 + 3 = " << add(5, 3) << endl;

    int x = 10;
    auto addX = [x](int a) { return a + x; };
    cout << "15 + x = " << addX(15) << endl;

    auto increment = [&x]() { x++; };
    increment();
    cout << "After increment: x = " << x << endl;

    vector<int> v = {1, 2, 3, 4, 5};
    for_each(v.begin(), v.end(), [](int& n) { n *= 2; });

    cout << "After doubling: ";
    for (int n : v) cout << n << " ";
    cout << endl;

    return 0;
}
