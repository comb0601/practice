/* Lesson 57: STL Algorithms Part 1
 * Compile: cl /std:c++17 Lesson57_Algorithms1.cpp */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    vector<int> v = {5, 2, 8, 1, 9, 3};

    sort(v.begin(), v.end());
    cout << "Sorted: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    reverse(v.begin(), v.end());
    cout << "Reversed: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    auto it = find(v.begin(), v.end(), 8);
    if (it != v.end()) {
        cout << "Found 8 at position " << (it - v.begin()) << endl;
    }

    int count = count_if(v.begin(), v.end(), [](int x) { return x > 5; });
    cout << "Elements > 5: " << count << endl;

    return 0;
}
