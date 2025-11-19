/*
 * Lesson 35: Encapsulation - Data Hiding
 * Compile: cl Lesson35_Encapsulation.cpp
 */

#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    int age;
    string ssn;  // Sensitive data

public:
    // Constructor
    Person(string n, int a, string s) : name(n), age(a), ssn(s) {}

    // Getters
    string getName() const { return name; }
    int getAge() const { return age; }

    // Setters with validation
    void setName(string n) {
        if (!n.empty()) {
            name = n;
        }
    }

    void setAge(int a) {
        if (a >= 0 && a <= 150) {
            age = a;
        } else {
            cout << "Invalid age!" << endl;
        }
    }

    // SSN only accessible with authorization
    string getSSN(string password) const {
        if (password == "secret123") {
            return ssn;
        }
        return "***-**-****";
    }

    void display() const {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
    }
};

class Temperature {
private:
    double celsius;

public:
    Temperature(double c) : celsius(c) {}

    // Read-only access
    double getCelsius() const { return celsius; }

    // Computed properties
    double getFahrenheit() const {
        return (celsius * 9.0 / 5.0) + 32.0;
    }

    double getKelvin() const {
        return celsius + 273.15;
    }

    // Controlled modification
    void setCelsius(double c) {
        if (c >= -273.15) {  // Absolute zero
            celsius = c;
        } else {
            cout << "Temperature below absolute zero!" << endl;
        }
    }

    void display() const {
        cout << "Temperature:" << endl;
        cout << "  Celsius: " << celsius << "°C" << endl;
        cout << "  Fahrenheit: " << getFahrenheit() << "°F" << endl;
        cout << "  Kelvin: " << getKelvin() << "K" << endl;
    }
};

class Counter {
private:
    int count;
    const int maxCount;

public:
    Counter(int max) : count(0), maxCount(max) {}

    void increment() {
        if (count < maxCount) {
            count++;
        } else {
            cout << "Maximum count reached!" << endl;
        }
    }

    void reset() {
        count = 0;
    }

    int getCount() const { return count; }
    int getMaxCount() const { return maxCount; }
};

int main() {
    cout << "=== Encapsulation Demo ===" << endl << endl;

    // PERSON
    cout << "=== PERSON ===" << endl;
    Person person("John Doe", 30, "123-45-6789");
    person.display();

    cout << "\nSSN with wrong password: " << person.getSSN("wrong") << endl;
    cout << "SSN with correct password: " << person.getSSN("secret123") << endl;

    person.setAge(35);
    person.setAge(-5);  // Invalid
    cout << "\nAfter update:" << endl;
    person.display();
    cout << endl;

    // TEMPERATURE
    cout << "=== TEMPERATURE ===" << endl;
    Temperature temp(25.0);
    temp.display();

    cout << "\nSetting to 100°C:" << endl;
    temp.setCelsius(100.0);
    temp.display();

    cout << "\nTrying to set below absolute zero:" << endl;
    temp.setCelsius(-300.0);  // Invalid
    cout << endl;

    // COUNTER
    cout << "=== COUNTER ===" << endl;
    Counter counter(5);
    cout << "Max count: " << counter.getMaxCount() << endl;

    for (int i = 0; i < 7; i++) {
        counter.increment();
        cout << "Count: " << counter.getCount() << endl;
    }

    counter.reset();
    cout << "After reset: " << counter.getCount() << endl;

    return 0;
}
