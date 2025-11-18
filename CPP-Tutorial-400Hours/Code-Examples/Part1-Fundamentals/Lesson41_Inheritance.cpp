/*
 * Lesson 41: Inheritance - Code Reuse through Class Hierarchies
 * Compile: cl Lesson41_Inheritance.cpp
 */

#include <iostream>
#include <string>
using namespace std;

// Base class
class Animal {
protected:
    string name;
    int age;

public:
    Animal(string n, int a) : name(n), age(a) {
        cout << "Animal constructor called" << endl;
    }

    void eat() {
        cout << name << " is eating" << endl;
    }

    void sleep() {
        cout << name << " is sleeping" << endl;
    }

    void display() {
        cout << "Name: " << name << ", Age: " << age << endl;
    }
};

// Derived class
class Dog : public Animal {
private:
    string breed;

public:
    Dog(string n, int a, string b) : Animal(n, a), breed(b) {
        cout << "Dog constructor called" << endl;
    }

    void bark() {
        cout << name << " is barking: Woof! Woof!" << endl;
    }

    void displayDog() {
        display();
        cout << "Breed: " << breed << endl;
    }
};

class Cat : public Animal {
public:
    Cat(string n, int a) : Animal(n, a) {
        cout << "Cat constructor called" << endl;
    }

    void meow() {
        cout << name << " is meowing: Meow! Meow!" << endl;
    }
};

// Multilevel inheritance
class Vehicle {
protected:
    string brand;
    int year;

public:
    Vehicle(string b, int y) : brand(b), year(y) {}

    void displayVehicle() {
        cout << "Brand: " << brand << ", Year: " << year << endl;
    }
};

class Car : public Vehicle {
protected:
    int doors;

public:
    Car(string b, int y, int d) : Vehicle(b, y), doors(d) {}

    void displayCar() {
        displayVehicle();
        cout << "Doors: " << doors << endl;
    }
};

class SportsCar : public Car {
private:
    int topSpeed;

public:
    SportsCar(string b, int y, int d, int s) : Car(b, y, d), topSpeed(s) {}

    void displaySportsCar() {
        displayCar();
        cout << "Top Speed: " << topSpeed << " mph" << endl;
    }
};

int main() {
    cout << "=== Inheritance Demo ===" << endl << endl;

    // SIMPLE INHERITANCE
    cout << "=== DOG (Derived from Animal) ===" << endl;
    Dog dog("Buddy", 3, "Golden Retriever");
    dog.displayDog();
    dog.eat();
    dog.bark();
    cout << endl;

    cout << "=== CAT (Derived from Animal) ===" << endl;
    Cat cat("Whiskers", 2);
    cat.display();
    cat.sleep();
    cat.meow();
    cout << endl;

    // MULTILEVEL INHERITANCE
    cout << "=== MULTILEVEL INHERITANCE ===" << endl;
    SportsCar ferrari("Ferrari", 2023, 2, 211);
    ferrari.displaySportsCar();

    return 0;
}
