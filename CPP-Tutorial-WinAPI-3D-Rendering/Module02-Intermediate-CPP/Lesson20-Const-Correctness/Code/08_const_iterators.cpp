#include <iostream>
#include <vector>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    // Non-const iterator
    for(vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        *it *= 2;  // Can modify
    }
    // Const iterator
    for(vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
        // *it = 0;  // Error: cannot modify
    }
    cout << endl;
    return 0;
}
