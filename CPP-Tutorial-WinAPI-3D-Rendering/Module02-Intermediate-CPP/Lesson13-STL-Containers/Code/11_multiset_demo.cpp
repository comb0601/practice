// Multiset allowing duplicates
#include <iostream>
#include <set>
using namespace std;
int main() {
    cout << "=== Multiset Demo ===" << endl;
    multiset<int> ms = {5, 2, 8, 2, 9, 5, 3};
    cout << "Multiset: "; for(int v : ms) cout << v << " "; cout << endl;
    cout << "Count of 2: " << ms.count(2) << endl;
    cout << "Count of 5: " << ms.count(5) << endl;
    ms.erase(ms.find(2));  // Remove one occurrence
    cout << "After erasing one 2: "; for(int v : ms) cout << v << " "; cout << endl;
    ms.erase(5);  // Remove all occurrences
    cout << "After erasing all 5s: "; for(int v : ms) cout << v << " "; cout << endl;
    return 0;
}
