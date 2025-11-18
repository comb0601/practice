#include <iostream>
using namespace std;
class Base {
public:
    virtual void show() { cout << "Base" << endl; }
};
class Derived1 : public Base {
public:
    void show() override { cout << "Derived1" << endl; }
};
class Derived2 : public Base {
public:
    void show() override { cout << "Derived2" << endl; }
};
class Final : public Derived1, public Derived2 {};
int main() {
    Final f;
    f.Derived1::show();
    f.Derived2::show();
    return 0;
}
