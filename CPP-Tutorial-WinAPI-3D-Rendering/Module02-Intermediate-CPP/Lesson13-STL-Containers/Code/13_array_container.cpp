// Fixed-size array container
#include <iostream>
#include <array>
using namespace std;
int main() {
    cout << "=== Array Container ===" << endl;
    array<int, 5> arr = {1, 2, 3, 4, 5};
    cout << "Array: "; for(int v : arr) cout << v << " "; cout << endl;
    cout << "Size: " << arr.size() << endl;
    cout << "Front: " << arr.front() << ", Back: " << arr.back() << endl;
    arr[2] = 100;
    cout << "After arr[2] = 100: "; for(int v : arr) cout << v << " "; cout << endl;
    arr.fill(0);
    cout << "After fill(0): "; for(int v : arr) cout << v << " "; cout << endl;
    return 0;
}
