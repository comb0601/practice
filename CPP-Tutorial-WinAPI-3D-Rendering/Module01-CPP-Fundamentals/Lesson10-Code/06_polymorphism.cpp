/*
 * Program: Polymorphism
 * Description: Demonstrating runtime polymorphism
 * Compilation: g++ -std=c++17 06_polymorphism.cpp -o 06_polymorphism
 * Execution: ./06_polymorphism
 */

#include <iostream>
#include <vector>
using namespace std;

class Animal {
public:
    virtual void makeSound() {
        cout << "Animal sound" << endl;
    }

    virtual void move() {
        cout << "Animal moves" << endl;
    }

    virtual ~Animal() {}
};

class Dog : public Animal {
public:
    void makeSound() override {
        cout << "Woof! Woof!" << endl;
    }

    void move() override {
        cout << "Dog runs" << endl;
    }
};

class Cat : public Animal {
public:
    void makeSound() override {
        cout << "Meow!" << endl;
    }

    void move() override {
        cout << "Cat prowls" << endl;
    }
};

class Bird : public Animal {
public:
    void makeSound() override {
        cout << "Chirp! Chirp!" << endl;
    }

    void move() override {
        cout << "Bird flies" << endl;
    }
};

void performActions(Animal* animal) {
    animal->makeSound();
    animal->move();
}

int main() {
    cout << "=== Polymorphism ===" << endl << endl;

    Dog dog;
    Cat cat;
    Bird bird;

    vector<Animal*> animals = {&dog, &cat, &bird};

    for (auto animal : animals) {
        performActions(animal);
        cout << endl;
    }

    return 0;
}
