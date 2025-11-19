/* Lesson 66: File Streams
 * Compile: cl Lesson66_FileStreams.cpp */
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    // APPEND MODE
    ofstream appendFile("log.txt", ios::app);
    appendFile << "New log entry" << endl;
    appendFile.close();

    // READ/WRITE MODE
    fstream file("data.txt", ios::in | ios::out | ios::trunc);
    file << "Test data" << endl;
    file.seekg(0);
    string line;
    getline(file, line);
    cout << "Read: " << line << endl;
    file.close();

    return 0;
}
