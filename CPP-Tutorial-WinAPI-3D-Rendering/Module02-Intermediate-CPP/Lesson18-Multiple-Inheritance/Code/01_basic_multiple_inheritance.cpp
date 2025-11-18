#include <iostream>
using namespace std;
class Base1 {
public:
    void show1() { cout << "Base1" << endl; }
};
class Base2 {
public:
    void show2() { cout << "Base2" << endl; }
};
class Derived : public Base1, public Base2 {
public:
    void show() { cout << "Derived" << endl; }
};
int main() {
    Derived d;
    d.show1();
    d.show2();
    d.show();
    return 0;
}
