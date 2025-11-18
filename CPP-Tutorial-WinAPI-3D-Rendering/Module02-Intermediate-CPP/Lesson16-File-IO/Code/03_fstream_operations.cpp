#include <iostream>
#include <fstream>
using namespace std;
int main() {
    fstream file;
    file.open("data.txt", ios::out);
    file << "Writing data" << endl;
    file.close();
    file.open("data.txt", ios::in);
    string line;
    getline(file, line);
    cout << "Read: " << line << endl;
    file.close();
    return 0;
}
