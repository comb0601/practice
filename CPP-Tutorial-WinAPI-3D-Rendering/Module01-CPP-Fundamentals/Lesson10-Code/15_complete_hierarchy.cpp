/*
 * Program: Complete Inheritance Hierarchy
 * Description: Comprehensive example with inheritance and polymorphism
 * Compilation: g++ -std=c++17 15_complete_hierarchy.cpp -o 15_complete_hierarchy
 * Execution: ./15_complete_hierarchy
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Vehicle {
protected:
    string brand;
    int year;

public:
    Vehicle(const string& b, int y) : brand(b), year(y) {}

    virtual void start() const {
        cout << brand << " is starting..." << endl;
    }

    virtual void stop() const {
        cout << brand << " is stopping..." << endl;
    }

    virtual void displayInfo() const {
        cout << year << " " << brand << endl;
    }

    virtual ~Vehicle() {
        cout << "Vehicle destructor: " << brand << endl;
    }
};

class Car : public Vehicle {
private:
    int numDoors;

public:
    Car(const string& b, int y, int doors)
        : Vehicle(b, y), numDoors(doors) {}

    void start() const override {
        cout << brand << " car starting engine..." << endl;
    }

    void displayInfo() const override {
        Vehicle::displayInfo();
        cout << "Type: Car, Doors: " << numDoors << endl;
    }

    ~Car() {
        cout << "Car destructor" << endl;
    }
};

class Motorcycle : public Vehicle {
private:
    bool hasSidecar;

public:
    Motorcycle(const string& b, int y, bool sidecar)
        : Vehicle(b, y), hasSidecar(sidecar) {}

    void start() const override {
        cout << brand << " motorcycle revving up..." << endl;
    }

    void displayInfo() const override {
        Vehicle::displayInfo();
        cout << "Type: Motorcycle, Sidecar: " << (hasSidecar ? "Yes" : "No") << endl;
    }

    ~Motorcycle() {
        cout << "Motorcycle destructor" << endl;
    }
};

class ElectricCar : public Car {
private:
    int batteryCapacity;

public:
    ElectricCar(const string& b, int y, int doors, int battery)
        : Car(b, y, doors), batteryCapacity(battery) {}

    void start() const override {
        cout << brand << " electric car powering on silently..." << endl;
    }

    void displayInfo() const override {
        Car::displayInfo();
        cout << "Battery: " << batteryCapacity << " kWh" << endl;
    }

    ~ElectricCar() {
        cout << "ElectricCar destructor" << endl;
    }
};

int main() {
    cout << "=== Complete Inheritance Hierarchy ===" << endl << endl;

    vector<Vehicle*> vehicles;
    vehicles.push_back(new Car("Toyota", 2024, 4));
    vehicles.push_back(new Motorcycle("Harley-Davidson", 2023, false));
    vehicles.push_back(new ElectricCar("Tesla", 2024, 4, 100));

    for (auto vehicle : vehicles) {
        vehicle->displayInfo();
        vehicle->start();
        vehicle->stop();
        cout << endl;
    }

    cout << "Cleaning up (destructors):" << endl;
    for (auto vehicle : vehicles) {
        delete vehicle;
    }

    return 0;
}
