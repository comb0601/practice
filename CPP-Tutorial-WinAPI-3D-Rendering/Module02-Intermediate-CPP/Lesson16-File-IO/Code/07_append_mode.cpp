#include <iostream>
#include <fstream>
using namespace std;
int main() {
    ofstream file("log.txt", ios::app);
    file << "Log entry 1" << endl;
    file.close();
    file.open("log.txt", ios::app);
    file << "Log entry 2" << endl;
    file.close();
    ifstream in("log.txt");
    string line;
    while(getline(in, line)) cout << line << endl;
    return 0;
}
