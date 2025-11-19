/* Lesson 45: Abstract Classes - Pure Virtual Functions
 * Compile: cl Lesson45_AbstractClasses.cpp */
#include <iostream>
using namespace std;

class Animal {
public:
    virtual void makeSound() = 0;  // Pure virtual
    virtual void move() = 0;
    virtual ~Animal() {}
};

class Dog : public Animal {
public:
    void makeSound() override { cout << "Woof!" << endl; }
    void move() override { cout << "Dog runs" << endl; }
};

class Bird : public Animal {
public:
    void makeSound() override { cout << "Chirp!" << endl; }
    void move() override { cout << "Bird flies" << endl; }
};

int main() {
    Animal *animals[2];
    animals[0] = new Dog();
    animals[1] = new Bird();

    for (int i = 0; i < 2; i++) {
        animals[i]->makeSound();
        animals[i]->move();
        cout << endl;
        delete animals[i];
    }
    return 0;
}
