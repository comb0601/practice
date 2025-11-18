#include <iostream>
using namespace std;
int main() {
    const int x = 10;
    // x = 20;  // Error: cannot modify const
    cout << "x = " << x << endl;
    const double PI = 3.14159;
    cout << "PI = " << PI << endl;
    return 0;
}
