#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto pivot = partition(v.begin(), v.end(), [](int x){ return x % 2 == 0; });
    cout << "Even|Odd partitioned: "; for(int x : v) cout << x << " "; cout << endl;
    return 0;
}
