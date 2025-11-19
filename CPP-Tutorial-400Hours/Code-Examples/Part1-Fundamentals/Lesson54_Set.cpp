/* Lesson 54: std::set
 * Compile: cl /std:c++17 Lesson54_Set.cpp */
#include <iostream>
#include <set>
using namespace std;

int main() {
    set<int> s = {5, 2, 8, 2, 1, 9};

    cout << "Set (unique, sorted): ";
    for (int x : s) cout << x << " ";
    cout << endl;

    s.insert(7);
    s.erase(2);

    cout << "After operations: ";
    for (int x : s) cout << x << " ";
    cout << endl;

    if (s.find(8) != s.end()) {
        cout << "Found 8" << endl;
    }

    return 0;
}
