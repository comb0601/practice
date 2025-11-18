#include <iostream>
using namespace std;
void may_throw() {
    throw runtime_error("Error");
}
void wont_throw() noexcept {
    cout << "Safe function" << endl;
}
int main() {
    try {
        wont_throw();
        may_throw();
    } catch(const exception& e) {
        cout << "Caught: " << e.what() << endl;
    }
    return 0;
}
