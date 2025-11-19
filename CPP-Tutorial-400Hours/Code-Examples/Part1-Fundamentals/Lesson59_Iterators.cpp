/* Lesson 59: Iterators
 * Compile: cl /std:c++17 Lesson59_Iterators.cpp */
#include <iostream>
#include <vector>
#include <list>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5};

    cout << "Forward iteration: ";
    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse iteration: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    list<int> lst = {10, 20, 30};
    auto it = lst.begin();
    ++it;
    lst.insert(it, 15);

    cout << "List after insert: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    return 0;
}
