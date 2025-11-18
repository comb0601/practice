#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {1, 1, 2, 2, 3, 3, 4, 5, 5};
    auto new_end = unique(v.begin(), v.end());
    v.erase(new_end, v.end());
    cout << "Unique: "; for(int x : v) cout << x << " "; cout << endl;
    return 0;
}
