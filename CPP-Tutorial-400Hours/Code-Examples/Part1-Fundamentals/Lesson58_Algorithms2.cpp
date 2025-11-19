/* Lesson 58: STL Algorithms Part 2
 * Compile: cl /std:c++17 Lesson58_Algorithms2.cpp */
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5};

    int sum = accumulate(v.begin(), v.end(), 0);
    cout << "Sum: " << sum << endl;

    int product = accumulate(v.begin(), v.end(), 1, multiplies<int>());
    cout << "Product: " << product << endl;

    auto minIt = min_element(v.begin(), v.end());
    auto maxIt = max_element(v.begin(), v.end());
    cout << "Min: " << *minIt << ", Max: " << *maxIt << endl;

    transform(v.begin(), v.end(), v.begin(), [](int x) { return x * 2; });
    cout << "After transform: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    return 0;
}
