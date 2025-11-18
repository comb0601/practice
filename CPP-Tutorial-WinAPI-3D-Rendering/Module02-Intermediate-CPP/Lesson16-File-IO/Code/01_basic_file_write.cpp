#include <iostream>
#include <fstream>
using namespace std;
int main() {
    ofstream file("output.txt");
    if(file.is_open()) {
        file << "Hello, File I/O!" << endl;
        file << "Line 2" << endl;
        file.close();
        cout << "File written successfully" << endl;
    }
    return 0;
}
