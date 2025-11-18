#include <iostream>
#include <fstream>
#include <sys/stat.h>
using namespace std;
bool fileExists(const string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}
int main() {
    cout << "test.txt exists: " << (fileExists("test.txt") ? "Yes" : "No") << endl;
    ofstream file("test.txt");
    file << "Created" << endl;
    file.close();
    cout << "test.txt exists now: " << (fileExists("test.txt") ? "Yes" : "No") << endl;
    return 0;
}
