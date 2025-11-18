#include <iostream>
using namespace std;
class Base {
public:
    int value = 10;
};
class Derived1 : public Base {};
class Derived2 : public Base {};
class Final : public Derived1, public Derived2 {};
int main() {
    Final f;
    // f.value;  // Ambiguous!
    cout << "Derived1::value = " << f.Derived1::value << endl;
    cout << "Derived2::value = " << f.Derived2::value << endl;
    return 0;
}
