/* Lesson 50: STL Overview
 * Compile: cl /std:c++17 Lesson50_STLOverview.cpp */
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

int main() {
    vector<int> vec = {1, 2, 3, 4, 5};
    cout << "Vector: ";
    for (int x : vec) cout << x << " ";
    cout << endl;

    list<string> lst = {"apple", "banana", "cherry"};
    cout << "List: ";
    for (const string& s : lst) cout << s << " ";
    cout << endl;

    map<string, int> ages;
    ages["Alice"] = 25;
    ages["Bob"] = 30;
    cout << "Map: Alice=" << ages["Alice"] << endl;

    set<int> s = {3, 1, 4, 1, 5, 9};
    cout << "Set: ";
    for (int x : s) cout << x << " ";
    cout << endl;

    return 0;
}
