#include <iostream>
#define DEBUG
#ifdef DEBUG
    #define LOG(x) cout << x << endl
#else
    #define LOG(x)
#endif
using namespace std;
int main() {
    LOG("Debug mode is ON");
    cout << "Program running..." << endl;
    return 0;
}
