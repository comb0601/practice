#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    reverse(v.begin(), v.end());
    cout << "Reversed: "; for(int x : v) cout << x << " "; cout << endl;
    rotate(v.begin(), v.begin() + 2, v.end());
    cout << "Rotated: "; for(int x : v) cout << x << " "; cout << endl;
    return 0;
}
