#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {5, 2, 8, 1, 9, 3};
    auto minElem = min_element(v.begin(), v.end());
    auto maxElem = max_element(v.begin(), v.end());
    cout << "Min: " << *minElem << endl;
    cout << "Max: " << *maxElem << endl;
    auto [min, max] = minmax_element(v.begin(), v.end());
    cout << "MinMax: " << *min << ", " << *max << endl;
    return 0;
}
