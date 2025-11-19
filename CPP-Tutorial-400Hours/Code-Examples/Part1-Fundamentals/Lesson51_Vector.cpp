/* Lesson 51: std::vector
 * Compile: cl /std:c++17 Lesson51_Vector.cpp */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    cout << "Vector: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    cout << "Size: " << v.size() << endl;
    cout << "First: " << v.front() << ", Last: " << v.back() << endl;

    v.insert(v.begin() + 1, 15);
    v.pop_back();

    sort(v.begin(), v.end(), greater<int>());
    cout << "Sorted desc: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    return 0;
}
