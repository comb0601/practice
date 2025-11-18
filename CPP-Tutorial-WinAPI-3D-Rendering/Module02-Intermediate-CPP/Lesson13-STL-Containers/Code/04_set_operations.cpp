// Set operations and algorithms
#include <iostream>
#include <set>
using namespace std;
int main() {
    cout << "=== Set Operations ===" << endl;
    set<int> s = {5, 2, 8, 1, 9, 3};
    cout << "Set (auto-sorted): "; for(int v : s) cout << v << " "; cout << endl;
    s.insert(7); s.insert(4);
    cout << "After insertions: "; for(int v : s) cout << v << " "; cout << endl;
    cout << "Contains 5: " << (s.count(5) > 0 ? "Yes" : "No") << endl;
    cout << "Contains 10: " << (s.count(10) > 0 ? "Yes" : "No") << endl;
    s.erase(5);
    cout << "After erase(5): "; for(int v : s) cout << v << " "; cout << endl;
    return 0;
}
