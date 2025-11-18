#include <iostream>
#include <string>
using namespace std;
class Named {
    string name;
public:
    Named(string n) : name(n) {}
    string getName() { return name; }
};
class Aged {
    int age;
public:
    Aged(int a) : age(a) {}
    int getAge() { return age; }
};
class Person : public Named, public Aged {
public:
    Person(string n, int a) : Named(n), Aged(a) {}
    void display() {
        cout << getName() << " is " << getAge() << " years old" << endl;
    }
};
int main() {
    Person p("Alice", 30);
    p.display();
    return 0;
}
