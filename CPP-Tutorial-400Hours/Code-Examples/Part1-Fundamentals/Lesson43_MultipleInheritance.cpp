/*
 * Lesson 43: Multiple Inheritance
 * Compile: cl Lesson43_MultipleInheritance.cpp
 */

#include <iostream>
using namespace std;

class Engine {
protected:
    int horsepower;
public:
    Engine(int hp) : horsepower(hp) {}
    void showEngine() {
        cout << "Engine: " << horsepower << " HP" << endl;
    }
};

class Body {
protected:
    string color;
public:
    Body(string c) : color(c) {}
    void showBody() {
        cout << "Body Color: " << color << endl;
    }
};

class Car : public Engine, public Body {
private:
    string model;
public:
    Car(string m, int hp, string c) : Engine(hp), Body(c), model(m) {}
    void display() {
        cout << "Model: " << model << endl;
        showEngine();
        showBody();
    }
};

int main() {
    cout << "=== Multiple Inheritance Demo ===" << endl << endl;
    
    Car myCar("Tesla Model S", 670, "Red");
    myCar.display();
    
    return 0;
}
