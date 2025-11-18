/*
 * Lesson 32: Constructors - Object Initialization
 * Compile: cl Lesson32_Constructors.cpp
 */

#include <iostream>
#include <string>
using namespace std;

class Point {
public:
    int x, y;

    // Default constructor
    Point() {
        x = 0;
        y = 0;
        cout << "Default constructor called" << endl;
    }

    // Parameterized constructor
    Point(int xVal, int yVal) {
        x = xVal;
        y = yVal;
        cout << "Parameterized constructor called" << endl;
    }

    // Constructor with default parameters
    Point(int val) : x(val), y(val) {
        cout << "Single parameter constructor called" << endl;
    }

    void display() {
        cout << "Point(" << x << ", " << y << ")" << endl;
    }
};

class Student {
private:
    string name;
    int age;
    double gpa;

public:
    // Constructor with member initializer list
    Student(string n, int a, double g) : name(n), age(a), gpa(g) {
        cout << "Student created: " << name << endl;
    }

    void display() {
        cout << name << " (Age: " << age << ", GPA: " << gpa << ")" << endl;
    }
};

class Counter {
private:
    int count;
    static int objectCount;

public:
    Counter() : count(0) {
        objectCount++;
        cout << "Counter #" << objectCount << " created" << endl;
    }

    ~Counter() {
        cout << "Counter destroyed" << endl;
    }

    void increment() { count++; }
    int getCount() { return count; }
    static int getObjectCount() { return objectCount; }
};

int Counter::objectCount = 0;

int main() {
    cout << "=== Constructors Demo ===" << endl << endl;

    // DEFAULT CONSTRUCTOR
    cout << "=== DEFAULT CONSTRUCTOR ===" << endl;
    Point p1;
    p1.display();
    cout << endl;

    // PARAMETERIZED CONSTRUCTOR
    cout << "=== PARAMETERIZED CONSTRUCTOR ===" << endl;
    Point p2(10, 20);
    p2.display();
    cout << endl;

    // SINGLE PARAMETER CONSTRUCTOR
    cout << "=== SINGLE PARAMETER ===" << endl;
    Point p3(5);
    p3.display();
    cout << endl;

    // MEMBER INITIALIZER LIST
    cout << "=== MEMBER INITIALIZER LIST ===" << endl;
    Student s1("Alice", 20, 3.8);
    s1.display();
    cout << endl;

    // DESTRUCTOR
    cout << "=== DESTRUCTOR ===" << endl;
    {
        Counter c1, c2, c3;
        cout << "Objects created: " << Counter::getObjectCount() << endl;
    }
    cout << "Objects destroyed when scope ended" << endl;

    return 0;
}
