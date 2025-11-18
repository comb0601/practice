#include <iostream>
using namespace std;
// Normally in header file:
// #ifndef MY_HEADER_H
// #define MY_HEADER_H
// ...declarations...
// #endif
int main() {
    cout << "Include guards prevent multiple inclusions" << endl;
    return 0;
}
