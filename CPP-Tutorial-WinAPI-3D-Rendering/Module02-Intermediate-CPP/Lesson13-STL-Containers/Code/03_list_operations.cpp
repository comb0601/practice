// List operations and manipulations
#include <iostream>
#include <list>
using namespace std;
int main() {
    cout << "=== List Operations ===" << endl;
    list<int> lst = {5, 2, 8, 1, 9, 3};
    cout << "Original: "; for(int v : lst) cout << v << " "; cout << endl;
    lst.sort();
    cout << "Sorted: "; for(int v : lst) cout << v << " "; cout << endl;
    lst.reverse();
    cout << "Reversed: "; for(int v : lst) cout << v << " "; cout << endl;
    lst.unique();
    cout << "Unique: "; for(int v : lst) cout << v << " "; cout << endl;
    lst.remove_if([](int x){ return x > 5; });
    cout << "After remove_if(>5): "; for(int v : lst) cout << v << " "; cout << endl;
    return 0;
}
