#include <iostream>
using namespace std;
class MyClass {
    int value;
public:
    MyClass(int v) : value(v) {}
    int getValue() const {
        // Cannot modify member variables
        return value;
    }
    void setValue(int v) {
        value = v;  // OK: non-const function
    }
};
int main() {
    const MyClass obj(10);
    cout << obj.getValue() << endl;  // OK
    // obj.setValue(20);             // Error: const object
    return 0;
}
