#include <iostream>
using namespace std;
class Data {
    int value;
public:
    Data(int v) : value(v) {}
    const int& getValue() const {
        return value;
    }
    int& getValue() {
        return value;
    }
};
int main() {
    Data d(10);
    cout << d.getValue() << endl;
    d.getValue() = 20;  // Modifiable
    cout << d.getValue() << endl;
    const Data cd(30);
    cout << cd.getValue() << endl;
    // cd.getValue() = 40;  // Error: const version returns const&
    return 0;
}
