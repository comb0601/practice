#include <iostream>
#define PI 3.14159
#define SQUARE(x) ((x) * (x))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
using namespace std;
int main() {
    cout << "PI = " << PI << endl;
    cout << "SQUARE(5) = " << SQUARE(5) << endl;
    cout << "MAX(10, 20) = " << MAX(10, 20) << endl;
    return 0;
}
