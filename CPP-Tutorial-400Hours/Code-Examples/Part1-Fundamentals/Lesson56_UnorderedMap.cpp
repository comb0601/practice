/* Lesson 56: std::unordered_map
 * Compile: cl /std:c++17 Lesson56_UnorderedMap.cpp */
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

int main() {
    unordered_map<string, int> scores;

    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;

    cout << "Scores (unordered):" << endl;
    for (const auto& pair : scores) {
        cout << pair.first << ": " << pair.second << endl;
    }

    cout << "Bob's score: " << scores["Bob"] << endl;

    return 0;
}
