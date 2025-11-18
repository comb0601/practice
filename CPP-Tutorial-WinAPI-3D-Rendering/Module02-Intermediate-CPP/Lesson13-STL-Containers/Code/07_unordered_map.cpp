// Unordered map for hash table
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;
int main() {
    cout << "=== Unordered Map ===" << endl;
    unordered_map<string, int> scores;
    scores["Alice"] = 95; scores["Bob"] = 87; scores["Charlie"] = 92;
    cout << "Scores:" << endl;
    for(const auto& [name, score] : scores)
        cout << "  " << name << ": " << score << endl;
    cout << "Bob's score: " << scores["Bob"] << endl;
    scores["Alice"] += 5;
    cout << "After bonus: " << scores["Alice"] << endl;
    return 0;
}
