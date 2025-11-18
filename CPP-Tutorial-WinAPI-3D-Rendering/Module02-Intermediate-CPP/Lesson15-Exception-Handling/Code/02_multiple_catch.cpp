#include <iostream>
using namespace std;
int main() {
    try {
        throw 42;
    } catch(int e) {
        cout << "Int exception: " << e << endl;
    } catch(const char* e) {
        cout << "String exception: " << e << endl;
    } catch(...) {
        cout << "Unknown exception" << endl;
    }
    return 0;
}
