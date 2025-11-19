/*
 * Program: Initialization List
 * Description: Member initialization lists
 * Compilation: g++ -std=c++17 06_initialization_list.cpp -o 06_initialization_list
 * Execution: ./06_initialization_list
 */

#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    const int id;          // const member
    string& name;          // reference member
    int age;

public:
    // Must use initialization list for const and reference members
    Person(int i, string& n, int a) : id(i), name(n), age(a) {
        cout << "Person created: " << name << endl;
    }

    void display() const {
        cout << "ID: " << id << ", Name: " << name << ", Age: " << age << endl;
    }
};

class Rectangle {
private:
    int width;
    int height;

public:
    // Initialization list (more efficient)
    Rectangle(int w, int h) : width(w), height(h) {
        // Members already initialized before body
        cout << "Rectangle: " << width << "x" << height << endl;
    }

    // vs. assignment in body (less efficient)
    // Rectangle(int w, int h) {
    //     width = w;    // Assignment, not initialization
    //     height = h;
    // }
};

int main() {
    cout << "=== Initialization List ===" << endl << endl;

    Rectangle rect(10, 5);

    string name = "Alice";
    Person person(1001, name, 25);
    person.display();

    return 0;
}
