/*
 * Program: Car Class
 * Description: Car class with fuel management
 * Compilation: g++ -std=c++17 14_car_class.cpp -o 14_car_class
 * Execution: ./14_car_class
 */

#include <iostream>
#include <string>
using namespace std;

class Car {
private:
    string brand;
    string model;
    int year;
    double fuelLevel;      // in gallons
    double fuelCapacity;   // tank capacity
    double mileage;        // miles traveled
    double mpg;            // miles per gallon

public:
    Car(const string& b, const string& m, int y, double cap, double efficiency)
        : brand(b), model(m), year(y), fuelCapacity(cap), mpg(efficiency),
          fuelLevel(0), mileage(0) {}

    void refuel(double gallons) {
        double newLevel = fuelLevel + gallons;
        if (newLevel > fuelCapacity) {
            fuelLevel = fuelCapacity;
            cout << "Tank full! Added " << (fuelCapacity - fuelLevel + gallons) << " gallons" << endl;
        } else {
            fuelLevel = newLevel;
            cout << "Added " << gallons << " gallons" << endl;
        }
    }

    void drive(double miles) {
        double fuelNeeded = miles / mpg;
        if (fuelNeeded <= fuelLevel) {
            fuelLevel -= fuelNeeded;
            mileage += miles;
            cout << "Drove " << miles << " miles" << endl;
        } else {
            double possibleMiles = fuelLevel * mpg;
            mileage += possibleMiles;
            fuelLevel = 0;
            cout << "Ran out of fuel after " << possibleMiles << " miles!" << endl;
        }
    }

    void displayInfo() const {
        cout << "\n" << year << " " << brand << " " << model << endl;
        cout << "Fuel: " << fuelLevel << "/" << fuelCapacity << " gallons" << endl;
        cout << "Mileage: " << mileage << " miles" << endl;
        cout << "Efficiency: " << mpg << " MPG" << endl;
    }
};

int main() {
    cout << "=== Car Class ===" << endl << endl;

    Car car("Toyota", "Camry", 2024, 15.8, 32.0);

    car.displayInfo();

    cout << "\nRefueling..." << endl;
    car.refuel(10);

    car.displayInfo();

    cout << "\nDriving..." << endl;
    car.drive(150);

    car.displayInfo();

    cout << "\nDriving again..." << endl;
    car.drive(200);

    car.displayInfo();

    return 0;
}
