#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    bool found = binary_search(v.begin(), v.end(), 5);
    cout << "binary_search(5): " << (found ? "Found" : "Not found") << endl;
    auto lower = lower_bound(v.begin(), v.end(), 5);
    auto upper = upper_bound(v.begin(), v.end(), 5);
    cout << "lower_bound(5): " << *lower << endl;
    cout << "upper_bound(5): " << *upper << endl;
    return 0;
}
