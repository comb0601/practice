#include <iostream>
using namespace std;
class Counter {
    mutable int accessCount;
    int value;
public:
    Counter(int v) : value(v), accessCount(0) {}
    int getValue() const {
        accessCount++;  // OK: mutable member
        return value;
    }
    int getAccessCount() const {
        return accessCount;
    }
};
int main() {
    const Counter c(10);
    cout << c.getValue() << endl;
    cout << c.getValue() << endl;
    cout << "Accessed " << c.getAccessCount() << " times" << endl;
    return 0;
}
