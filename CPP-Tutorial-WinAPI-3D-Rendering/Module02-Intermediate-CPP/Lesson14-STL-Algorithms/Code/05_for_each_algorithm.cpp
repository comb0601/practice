#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    for_each(v.begin(), v.end(), [](int x){ cout << x << " "; });
    cout << endl;
    for_each(v.begin(), v.end(), [](int& x){ x *= 2; });
    cout << "Doubled: "; for(int x : v) cout << x << " "; cout << endl;
    return 0;
}
