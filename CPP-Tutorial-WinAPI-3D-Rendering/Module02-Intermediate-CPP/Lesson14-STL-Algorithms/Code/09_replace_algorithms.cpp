#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3, 2, 4, 2, 5};
    replace(v.begin(), v.end(), 2, 99);
    cout << "After replace(2, 99): "; for(int x : v) cout << x << " "; cout << endl;
    replace_if(v.begin(), v.end(), [](int x){ return x < 3; }, 0);
    cout << "After replace_if(<3, 0): "; for(int x : v) cout << x << " "; cout << endl;
    return 0;
}
