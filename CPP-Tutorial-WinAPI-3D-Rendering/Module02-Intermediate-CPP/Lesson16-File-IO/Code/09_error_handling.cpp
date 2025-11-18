#include <iostream>
#include <fstream>
using namespace std;
int main() {
    ifstream file("nonexistent.txt");
    if(!file) {
        cerr << "Error: Could not open file" << endl;
        return 1;
    }
    if(file.fail()) {
        cerr << "Read/write error" << endl;
    }
    if(file.eof()) {
        cout << "End of file reached" << endl;
    }
    return 0;
}
