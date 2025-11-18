#include <iostream>
using namespace std;
class Base {
    int value;
    friend void showBase(Base&);
public:
    Base(int v) : value(v) {}
};
class Derived : public Base {
    int extra;
public:
    Derived(int v, int e) : Base(v), extra(e) {}
    friend void showDerived(Derived&);
};
void showBase(Base& b) {
    cout << "Base value: " << b.value << endl;
}
void showDerived(Derived& d) {
    // Can access Derived's private members
    cout << "Derived extra: " << d.extra << endl;
    // Cannot access Base's private members!
    // cout << d.value << endl;  // Error!
}
int main() {
    Base b(10);
    Derived d(10, 20);
    showBase(b);
    showDerived(d);
    return 0;
}
