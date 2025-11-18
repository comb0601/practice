#include <iostream>
using namespace std;
void level2() {
    throw runtime_error("Level 2 error");
}
void level1() {
    try {
        level2();
    } catch(const exception& e) {
        cout << "Level 1 caught: " << e.what() << endl;
        throw;
    }
}
int main() {
    try {
        level1();
    } catch(const exception& e) {
        cout << "Main caught: " << e.what() << endl;
    }
    return 0;
}
