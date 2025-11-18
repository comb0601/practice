#include <iostream>
using namespace std;
class ClassB;
class ClassA {
    int valueA;
public:
    ClassA(int v) : valueA(v) {}
    friend void compare(ClassA&, ClassB&);
};
class ClassB {
    int valueB;
public:
    ClassB(int v) : valueB(v) {}
    friend void compare(ClassA&, ClassB&);
};
void compare(ClassA& a, ClassB& b) {
    cout << "A: " << a.valueA << ", B: " << b.valueB << endl;
    if(a.valueA > b.valueB)
        cout << "A is greater" << endl;
    else
        cout << "B is greater or equal" << endl;
}
int main() {
    ClassA a(10);
    ClassB b(20);
    compare(a, b);
    return 0;
}
