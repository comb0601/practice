/*
 * Program: RAII Examples
 * Description: RAII pattern with constructors and destructors
 * Compilation: g++ -std=c++17 11_raii_examples.cpp -o 11_raii_examples
 * Execution: ./11_raii_examples
 */

#include <iostream>
#include <fstream>
using namespace std;

class FileHandler {
private:
    ofstream file;
    string filename;

public:
    FileHandler(const string& fname) : filename(fname) {
        file.open(filename);
        cout << "File opened: " << filename << endl;
    }

    ~FileHandler() {
        if (file.is_open()) {
            file.close();
            cout << "File closed: " << filename << endl;
        }
    }

    void write(const string& text) {
        if (file.is_open()) {
            file << text << endl;
        }
    }
};

class ArrayWrapper {
private:
    int* data;
    int size;

public:
    ArrayWrapper(int s) : size(s) {
        data = new int[size];
        cout << "Array allocated: " << size << " elements" << endl;
    }

    ~ArrayWrapper() {
        delete[] data;
        cout << "Array deallocated" << endl;
    }

    void set(int index, int value) {
        if (index >= 0 && index < size) {
            data[index] = value;
        }
    }
};

int main() {
    cout << "=== RAII Examples ===" << endl << endl;

    {
        FileHandler fh("output.txt");
        fh.write("Hello, RAII!");
        fh.write("Automatic cleanup");
        // File automatically closed when fh goes out of scope
    }

    cout << "\nFile scope ended\n" << endl;

    {
        ArrayWrapper arr(10);
        arr.set(0, 42);
        // Array automatically deleted
    }

    cout << "\nArray scope ended" << endl;

    return 0;
}
