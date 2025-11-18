#include <iostream>
using namespace std;
void func() {
    try {
        throw runtime_error("Error in func");
    } catch(...) {
        cout << "Caught in func, rethrowing..." << endl;
        throw;
    }
}
int main() {
    try {
        func();
    } catch(const exception& e) {
        cout << "Caught in main: " << e.what() << endl;
    }
    return 0;
}
