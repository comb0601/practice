/* Lesson 52: std::list
 * Compile: cl /std:c++17 Lesson52_List.cpp */
#include <iostream>
#include <list>
using namespace std;

int main() {
    list<int> lst = {1, 2, 3, 4, 5};

    lst.push_front(0);
    lst.push_back(6);

    cout << "List: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    lst.remove(3);
    lst.reverse();

    cout << "After reverse: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    return 0;
}
