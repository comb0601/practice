#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3, 2, 4, 2, 5};
    cout << "Original: "; for(int x : v) cout << x << " "; cout << endl;
    auto new_end = remove(v.begin(), v.end(), 2);
    v.erase(new_end, v.end());
    cout << "After remove(2): "; for(int x : v) cout << x << " "; cout << endl;
    return 0;
}
