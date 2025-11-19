/*
 * Program: Constructor Inheritance
 * Description: Constructors and destructors in inheritance
 * Compilation: g++ -std=c++17 03_constructor_inheritance.cpp -o 03_constructor_inheritance
 * Execution: ./03_constructor_inheritance
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
        cout << "Person constructor: " << name << endl;
    }

    ~Person() {
        cout << "Person destructor: " << name << endl;
    }

    void display() {
        cout << "Person: " << name << ", Age: " << age << endl;
    }
};

class Student : public Person {
private:
    int studentId;
    double gpa;

public:
    Student(const string& n, int a, int id, double g)
        : Person(n, a), studentId(id), gpa(g) {
        cout << "Student constructor: ID " << studentId << endl;
    }

    ~Student() {
        cout << "Student destructor: ID " << studentId << endl;
    }

    void displayStudent() {
        cout << "Student: " << name << ", Age: " << age
             << ", ID: " << studentId << ", GPA: " << gpa << endl;
    }
};

int main() {
    cout << "=== Constructor Inheritance ===" << endl << endl;

    cout << "Creating student:" << endl;
    {
        Student s("Alice", 20, 12345, 3.8);
        s.displayStudent();
        cout << "\nLeaving scope..." << endl;
    }
    cout << "Destructors called in reverse order" << endl;

    return 0;
}
