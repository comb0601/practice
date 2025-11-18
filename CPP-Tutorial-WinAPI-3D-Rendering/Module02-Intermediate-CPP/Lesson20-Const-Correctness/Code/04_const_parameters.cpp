#include <iostream>
using namespace std;
void printValue(const int& x) {
    // x = 20;  // Error
    cout << "Value: " << x << endl;
}
void modifyValue(int& x) {
    x = 20;  // OK
}
int main() {
    int val = 10;
    printValue(val);
    modifyValue(val);
    cout << "Modified: " << val << endl;
    return 0;
}
