#include <iostream>
using namespace std;
class Base {
public:
    int value = 10;
};
class Derived1 : virtual public Base {};
class Derived2 : virtual public Base {};
class Final : public Derived1, public Derived2 {};
int main() {
    Final f;
    cout << "value = " << f.value << endl;  // No ambiguity!
    return 0;
}
