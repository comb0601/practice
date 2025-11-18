#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    auto it = find(v.begin(), v.end(), 3);
    if(it != v.end()) cout << "Found 3 at index " << distance(v.begin(), it) << endl;
    auto it2 = find_if(v.begin(), v.end(), [](int x){ return x > 3; });
    if(it2 != v.end()) cout << "First > 3: " << *it2 << endl;
    return 0;
}
