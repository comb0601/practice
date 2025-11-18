// Word frequency counter using map
#include <iostream>
#include <map>
#include <string>
#include <sstream>
using namespace std;
int main() {
    cout << "=== Word Frequency Counter ===" << endl;
    string text = "the quick brown fox jumps over the lazy dog the fox is quick";
    map<string, int> frequency;
    istringstream iss(text);
    string word;
    while(iss >> word) frequency[word]++;
    cout << "Word frequencies:" << endl;
    for(const auto& [word, count] : frequency)
        cout << "  " << word << ": " << count << endl;
    return 0;
}
