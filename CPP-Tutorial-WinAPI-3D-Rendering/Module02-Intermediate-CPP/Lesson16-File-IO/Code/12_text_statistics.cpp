#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main() {
    ofstream out("stats.txt");
    out << "Hello World\nThis is a test\nCounting words and lines\n";
    out.close();
    ifstream in("stats.txt");
    string word;
    int words = 0, lines = 0, chars = 0;
    string line;
    while(getline(in, line)) {
        lines++;
        chars += line.length();
    }
    in.clear();
    in.seekg(0);
    while(in >> word) words++;
    cout << "Lines: " << lines << endl;
    cout << "Words: " << words << endl;
    cout << "Chars: " << chars << endl;
    return 0;
}
