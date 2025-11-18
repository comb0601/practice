// Unordered set for fast lookup
#include <iostream>
#include <unordered_set>
using namespace std;
int main() {
    cout << "=== Unordered Set ===" << endl;
    unordered_set<int> us = {5, 2, 8, 1, 9, 3};
    cout << "Unordered set: "; for(int v : us) cout << v << " "; cout << endl;
    us.insert(7); us.insert(4);
    cout << "After insertions: "; for(int v : us) cout << v << " "; cout << endl;
    cout << "Lookup 5: " << (us.find(5) != us.end() ? "Found" : "Not found") << endl;
    cout << "Lookup 10: " << (us.find(10) != us.end() ? "Found" : "Not found") << endl;
    return 0;
}
