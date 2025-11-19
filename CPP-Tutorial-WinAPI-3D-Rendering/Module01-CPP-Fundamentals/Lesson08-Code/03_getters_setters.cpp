/*
 * Program: Getters and Setters
 * Description: Encapsulation with getter and setter methods
 * Compilation: g++ -std=c++17 03_getters_setters.cpp -o 03_getters_setters
 * Execution: ./03_getters_setters
 */

#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    int age;
    double height;  // in meters

public:
    // Getters
    string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    double getHeight() const {
        return height;
    }

    // Setters with validation
    void setName(const string& newName) {
        if (!newName.empty()) {
            name = newName;
        } else {
            cout << "Name cannot be empty!" << endl;
        }
    }

    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 150) {
            age = newAge;
        } else {
            cout << "Invalid age!" << endl;
        }
    }

    void setHeight(double newHeight) {
        if (newHeight > 0 && newHeight < 3.0) {
            height = newHeight;
        } else {
            cout << "Invalid height!" << endl;
        }
    }

    void display() const {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << " years" << endl;
        cout << "Height: " << height << " m" << endl;
    }
};

class Temperature {
private:
    double celsius;

public:
    // Getter for Celsius
    double getCelsius() const {
        return celsius;
    }

    // Getter for Fahrenheit (calculated)
    double getFahrenheit() const {
        return (celsius * 9.0 / 5.0) + 32.0;
    }

    // Setter for Celsius
    void setCelsius(double temp) {
        celsius = temp;
    }

    // Setter for Fahrenheit
    void setFahrenheit(double temp) {
        celsius = (temp - 32.0) * 5.0 / 9.0;
    }
};

int main() {
    cout << "=== Getters and Setters ===" << endl << endl;

    // Person
    cout << "1. Person Example:" << endl;
    Person person;

    person.setName("Alice Smith");
    person.setAge(25);
    person.setHeight(1.65);

    person.display();
    cout << endl;

    // Validation
    cout << "2. Validation:" << endl;
    person.setAge(200);  // Invalid
    person.setAge(30);   // Valid
    cout << "Age after validation: " << person.getAge() << endl;
    cout << endl;

    // Temperature
    cout << "3. Temperature Conversion:" << endl;
    Temperature temp;

    temp.setCelsius(25);
    cout << temp.getCelsius() << "°C = "
         << temp.getFahrenheit() << "°F" << endl;

    temp.setFahrenheit(98.6);
    cout << temp.getFahrenheit() << "°F = "
         << temp.getCelsius() << "°C" << endl;

    return 0;
}
