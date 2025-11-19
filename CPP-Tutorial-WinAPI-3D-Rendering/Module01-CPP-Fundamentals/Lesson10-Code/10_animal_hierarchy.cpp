/*
 * Program: Animal Hierarchy
 * Description: Animal inheritance hierarchy
 * Compilation: g++ -std=c++17 10_animal_hierarchy.cpp -o 10_animal_hierarchy
 * Execution: ./10_animal_hierarchy
 */

#include <iostream>
#include <string>
using namespace std;

class Animal {
protected:
    string name;
    int age;

public:
    Animal(const string& n, int a) : name(n), age(a) {}

    virtual void makeSound() const = 0;
    virtual void move() const = 0;

    virtual void display() const {
        cout << "Name: " << name << ", Age: " << age << endl;
    }

    virtual ~Animal() {}
};

class Mammal : public Animal {
protected:
    bool hasFur;

public:
    Mammal(const string& n, int a, bool fur)
        : Animal(n, a), hasFur(fur) {}

    void display() const override {
        Animal::display();
        cout << "Has fur: " << (hasFur ? "Yes" : "No") << endl;
    }
};

class Dog : public Mammal {
private:
    string breed;

public:
    Dog(const string& n, int a, const string& b)
        : Mammal(n, a, true), breed(b) {}

    void makeSound() const override {
        cout << name << " barks: Woof! Woof!" << endl;
    }

    void move() const override {
        cout << name << " runs on four legs" << endl;
    }

    void display() const override {
        Mammal::display();
        cout << "Breed: " << breed << endl;
    }
};

class Bird : public Animal {
protected:
    double wingSpan;

public:
    Bird(const string& n, int a, double ws)
        : Animal(n, a), wingSpan(ws) {}

    void makeSound() const override {
        cout << name << " chirps: Tweet! Tweet!" << endl;
    }

    void move() const override {
        cout << name << " flies in the sky" << endl;
    }

    void display() const override {
        Animal::display();
        cout << "Wing span: " << wingSpan << " cm" << endl;
    }
};

int main() {
    cout << "=== Animal Hierarchy ===" << endl << endl;

    Dog dog("Buddy", 3, "Golden Retriever");
    Bird bird("Tweety", 1, 15.5);

    Animal* animals[] = {&dog, &bird};

    for (auto animal : animals) {
        animal->display();
        animal->makeSound();
        animal->move();
        cout << endl;
    }

    return 0;
}
