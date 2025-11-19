/* Lesson 64: File I/O
 * Compile: cl Lesson64_FileIO.cpp */
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    // WRITE TO FILE
    ofstream outFile("test.txt");
    if (outFile.is_open()) {
        outFile << "Hello, File!" << endl;
        outFile << "Line 2" << endl;
        outFile << "Number: " << 42 << endl;
        outFile.close();
        cout << "File written successfully" << endl;
    }

    // READ FROM FILE
    ifstream inFile("test.txt");
    if (inFile.is_open()) {
        string line;
        cout << "File contents:" << endl;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }

    return 0;
}
