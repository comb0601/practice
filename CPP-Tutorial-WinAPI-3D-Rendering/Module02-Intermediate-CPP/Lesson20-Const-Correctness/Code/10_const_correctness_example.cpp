#include <iostream>
#include <string>
using namespace std;
class Person {
    string name;
    int age;
public:
    Person(string n, int a) : name(n), age(a) {}
    const string& getName() const { return name; }
    int getAge() const { return age; }
    void setName(const string& n) { name = n; }
    void setAge(int a) { age = a; }
    void display() const {
        cout << name << " (" << age << ")" << endl;
    }
};
void printPerson(const Person& p) {
    p.display();  // OK: const function
    // p.setAge(30);  // Error: non-const function
}
int main() {
    Person p("Alice", 25);
    printPerson(p);
    p.setAge(26);
    printPerson(p);
    return 0;
}
