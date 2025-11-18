#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main() {
    ofstream out("lines.txt");
    out << "Line 1\nLine 2\nLine 3\nLine 4\nLine 5\n";
    out.close();
    ifstream in("lines.txt");
    string line;
    int count = 0;
    while(getline(in, line)) count++;
    cout << "Total lines: " << count << endl;
    return 0;
}
