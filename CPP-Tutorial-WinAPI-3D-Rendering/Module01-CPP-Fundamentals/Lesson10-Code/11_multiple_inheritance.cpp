/*
 * Program: Multiple Inheritance
 * Description: Inheriting from multiple base classes
 * Compilation: g++ -std=c++17 11_multiple_inheritance.cpp -o 11_multiple_inheritance
 * Execution: ./11_multiple_inheritance
 */

#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string name;
    int age;

public:
    Person(const string& n, int a) : name(n), age(a) {
        cout << "Person constructor" << endl;
    }

    void displayPerson() const {
        cout << "Person: " << name << ", Age: " << age << endl;
    }
};

class Employee {
protected:
    int employeeId;
    double salary;

public:
    Employee(int id, double sal) : employeeId(id), salary(sal) {
        cout << "Employee constructor" << endl;
    }

    void displayEmployee() const {
        cout << "Employee ID: " << employeeId << ", Salary: $" << salary << endl;
    }
};

class Manager : public Person, public Employee {
private:
    string department;

public:
    Manager(const string& n, int a, int id, double sal, const string& dept)
        : Person(n, a), Employee(id, sal), department(dept) {
        cout << "Manager constructor" << endl;
    }

    void display() const {
        displayPerson();
        displayEmployee();
        cout << "Department: " << department << endl;
    }
};

int main() {
    cout << "=== Multiple Inheritance ===" << endl << endl;

    Manager mgr("John Smith", 35, 1001, 75000, "Engineering");
    mgr.display();

    return 0;
}
