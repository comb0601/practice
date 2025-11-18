#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> src = {1, 2, 3, 4, 5};
    vector<int> dst(src.size());
    copy(src.begin(), src.end(), dst.begin());
    cout << "Copied: "; for(int x : dst) cout << x << " "; cout << endl;
    vector<int> filtered;
    copy_if(src.begin(), src.end(), back_inserter(filtered), [](int x){ return x > 2; });
    cout << "Filtered (>2): "; for(int x : filtered) cout << x << " "; cout << endl;
    return 0;
}
