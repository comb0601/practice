#include <iostream>
#define VERSION 2
using namespace std;
int main() {
    #if VERSION == 1
        cout << "Version 1.0" << endl;
    #elif VERSION == 2
        cout << "Version 2.0" << endl;
    #else
        cout << "Unknown version" << endl;
    #endif
    return 0;
}
