#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {5, 2, 8, 1, 9};
    sort(v.begin(), v.end());
    cout << "Sorted: "; for(int x : v) cout << x << " "; cout << endl;
    sort(v.begin(), v.end(), greater<int>());
    cout << "Descending: "; for(int x : v) cout << x << " "; cout << endl;
    return 0;
}
