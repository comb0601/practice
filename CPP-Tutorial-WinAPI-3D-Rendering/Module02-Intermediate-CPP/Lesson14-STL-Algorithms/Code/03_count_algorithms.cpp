#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3, 2, 4, 2, 5};
    cout << "Count of 2: " << count(v.begin(), v.end(), 2) << endl;
    cout << "Count > 2: " << count_if(v.begin(), v.end(), [](int x){ return x > 2; }) << endl;
    return 0;
}
