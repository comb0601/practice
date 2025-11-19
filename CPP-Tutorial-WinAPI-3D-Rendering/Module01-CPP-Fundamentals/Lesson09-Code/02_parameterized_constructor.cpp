/*
 * Program: Parameterized Constructor
 * Description: Constructors with parameters
 * Compilation: g++ -std=c++17 02_parameterized_constructor.cpp -o 02_parameterized_constructor
 * Execution: ./02_parameterized_constructor
 */

#include <iostream>
#include <string>
using namespace std;

class Rectangle {
private:
    int width, height;

public:
    // Parameterized constructor
    Rectangle(int w, int h) {
        width = w;
        height = h;
        cout << "Parameterized constructor: " << width << "x" << height << endl;
    }

    int area() {
        return width * height;
    }

    void display() {
        cout << "Rectangle: " << width << "x" << height << ", Area: " << area() << endl;
    }
};

class Person {
private:
    string name;
    int age;

public:
    Person(string n, int a) : name(n), age(a) {
        cout << "Created person: " << name << ", age " << age << endl;
    }

    void display() {
        cout << name << " is " << age << " years old" << endl;
    }
};

int main() {
    cout << "=== Parameterized Constructor ===" << endl << endl;

    // Creating objects with parameters
    Rectangle rect1(10, 5);
    rect1.display();

    Rectangle rect2(15, 8);
    rect2.display();

    cout << endl;

    Person person1("Alice", 25);
    person1.display();

    Person person2("Bob", 30);
    person2.display();

    return 0;
}
