/*
 * Program: Basic Inheritance
 * Description: Introduction to inheritance - base and derived classes
 * Compilation: g++ -std=c++17 01_basic_inheritance.cpp -o 01_basic_inheritance
 * Execution: ./01_basic_inheritance
 */

#include <iostream>
#include <string>
using namespace std;

class Animal {
public:
    string name;
    int age;

    void eat() {
        cout << name << " is eating" << endl;
    }

    void sleep() {
        cout << name << " is sleeping" << endl;
    }

    void display() {
        cout << "Animal: " << name << ", Age: " << age << endl;
    }
};

class Dog : public Animal {
public:
    string breed;

    void bark() {
        cout << name << " says: Woof! Woof!" << endl;
    }

    void displayDog() {
        cout << "Dog: " << name << ", Age: " << age << ", Breed: " << breed << endl;
    }
};

class Cat : public Animal {
public:
    string color;

    void meow() {
        cout << name << " says: Meow!" << endl;
    }
};

int main() {
    cout << "=== Basic Inheritance ===" << endl << endl;

    Dog dog;
    dog.name = "Buddy";
    dog.age = 3;
    dog.breed = "Golden Retriever";

    dog.display();      // Inherited from Animal
    dog.displayDog();   // Defined in Dog
    dog.eat();          // Inherited
    dog.bark();         // Dog-specific
    cout << endl;

    Cat cat;
    cat.name = "Whiskers";
    cat.age = 2;
    cat.color = "Orange";

    cat.display();      // Inherited
    cat.sleep();        // Inherited
    cat.meow();         // Cat-specific

    return 0;
}
