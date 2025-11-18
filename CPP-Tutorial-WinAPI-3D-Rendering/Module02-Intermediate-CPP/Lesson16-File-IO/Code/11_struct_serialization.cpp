#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct Person {
    char name[50];
    int age;
    double salary;
};
int main() {
    Person p1 = {"Alice", 30, 50000.0};
    ofstream out("person.dat", ios::binary);
    out.write(reinterpret_cast<char*>(&p1), sizeof(Person));
    out.close();
    Person p2;
    ifstream in("person.dat", ios::binary);
    in.read(reinterpret_cast<char*>(&p2), sizeof(Person));
    in.close();
    cout << "Name: " << p2.name << endl;
    cout << "Age: " << p2.age << endl;
    cout << "Salary: " << p2.salary << endl;
    return 0;
}
