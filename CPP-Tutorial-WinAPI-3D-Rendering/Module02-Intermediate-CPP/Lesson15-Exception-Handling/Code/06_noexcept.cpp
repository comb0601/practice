#include <iostream>
using namespace std;
void safe_func() noexcept {
    cout << "This function doesn't throw" << endl;
}
void unsafe_func() {
    throw runtime_error("Error!");
}
int main() {
    safe_func();
    cout << "safe_func is noexcept: " << noexcept(safe_func()) << endl;
    cout << "unsafe_func is noexcept: " << noexcept(unsafe_func()) << endl;
    return 0;
}
