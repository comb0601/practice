// Multimap with duplicate keys
#include <iostream>
#include <map>
#include <string>
using namespace std;
int main() {
    cout << "=== Multimap Demo ===" << endl;
    multimap<string, int> scores;
    scores.insert({"Alice", 95}); scores.insert({"Alice", 87});
    scores.insert({"Bob", 92}); scores.insert({"Bob", 88});
    cout << "Scores:" << endl;
    for(const auto& [name, score] : scores)
        cout << "  " << name << ": " << score << endl;
    auto range = scores.equal_range("Alice");
    cout << "Alice's scores: ";
    for(auto it = range.first; it != range.second; ++it)
        cout << it->second << " ";
    cout << endl;
    return 0;
}
