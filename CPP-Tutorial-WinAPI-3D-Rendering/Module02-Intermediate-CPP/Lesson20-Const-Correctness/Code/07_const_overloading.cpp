#include <iostream>
using namespace std;
class Array {
    int data[5];
public:
    Array() {
        for(int i = 0; i < 5; i++) data[i] = i;
    }
    int& operator[](int index) {
        cout << "Non-const version" << endl;
        return data[index];
    }
    const int& operator[](int index) const {
        cout << "Const version" << endl;
        return data[index];
    }
};
int main() {
    Array arr;
    arr[0] = 100;  // Non-const version
    cout << arr[0] << endl;
    const Array carr;
    // carr[0] = 100;  // Error
    cout << carr[0] << endl;  // Const version
    return 0;
}
