#include <iostream>
using namespace std;
void print(const int* ptr) {
    // Removing const (dangerous!)
    int* modifiable = const_cast<int*>(ptr);
    *modifiable = 100;
}
int main() {
    int x = 10;
    const int* ptr = &x;
    cout << "Before: " << x << endl;
    print(ptr);
    cout << "After: " << x << endl;
    // WARNING: const_cast should be used very rarely
    return 0;
}
