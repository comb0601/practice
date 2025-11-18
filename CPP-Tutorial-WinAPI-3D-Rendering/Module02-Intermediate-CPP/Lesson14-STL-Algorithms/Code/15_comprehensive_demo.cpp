#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;
int main() {
    cout << "=== Comprehensive STL Algorithms Demo ===" << endl;
    vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    cout << "Original: "; for(int x : v) cout << x << " "; cout << endl;
    sort(v.begin(), v.end());
    cout << "Sorted: "; for(int x : v) cout << x << " "; cout << endl;
    cout << "Sum: " << accumulate(v.begin(), v.end(), 0) << endl;
    cout << "Min: " << *min_element(v.begin(), v.end()) << endl;
    cout << "Max: " << *max_element(v.begin(), v.end()) << endl;
    cout << "Count > 5: " << count_if(v.begin(), v.end(), [](int x){ return x > 5; }) << endl;
    return 0;
}
