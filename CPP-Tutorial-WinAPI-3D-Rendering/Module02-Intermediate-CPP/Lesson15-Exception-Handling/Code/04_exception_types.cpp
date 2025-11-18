#include <iostream>
#include <stdexcept>
using namespace std;
int main() {
    try {
        throw out_of_range("Index out of range");
    } catch(const out_of_range& e) {
        cout << "out_of_range: " << e.what() << endl;
    }
    try {
        throw invalid_argument("Invalid argument");
    } catch(const invalid_argument& e) {
        cout << "invalid_argument: " << e.what() << endl;
    }
    return 0;
}
