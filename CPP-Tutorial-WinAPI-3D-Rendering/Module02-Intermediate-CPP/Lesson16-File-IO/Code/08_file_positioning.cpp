#include <iostream>
#include <fstream>
using namespace std;
int main() {
    ofstream out("pos.txt");
    out << "0123456789";
    out.close();
    fstream file("pos.txt", ios::in | ios::out);
    file.seekg(5);
    char ch;
    file.get(ch);
    cout << "Char at position 5: " << ch << endl;
    file.seekp(0);
    file << "X";
    file.close();
    ifstream in("pos.txt");
    string content;
    in >> content;
    cout << "Content: " << content << endl;
    return 0;
}
