#include <iostream>
using namespace std;
int main() {
    try {
        throw runtime_error("Test exception");
    } catch(const exception& e) {
        cout << "Caught: " << e.what() << endl;
    }
    return 0;
}
