/*
 * Program: Explicit Constructors
 * Description: Preventing implicit conversions
 * Compilation: g++ -std=c++17 13_explicit_constructor.cpp -o 13_explicit_constructor
 * Execution: ./13_explicit_constructor
 */

#include <iostream>
using namespace std;

class Distance {
private:
    int meters;

public:
    // Without explicit - allows implicit conversion
    Distance(int m) : meters(m) {
        cout << "Distance: " << meters << " meters" << endl;
    }

    int getMeters() const { return meters; }
};

class SafeDistance {
private:
    int meters;

public:
    // With explicit - prevents implicit conversion
    explicit SafeDistance(int m) : meters(m) {
        cout << "SafeDistance: " << meters << " meters" << endl;
    }

    int getMeters() const { return meters; }
};

void printDistance(Distance d) {
    cout << "Distance: " << d.getMeters() << " m" << endl;
}

void printSafeDistance(SafeDistance d) {
    cout << "SafeDistance: " << d.getMeters() << " m" << endl;
}

int main() {
    cout << "=== Explicit Constructors ===" << endl << endl;

    // Implicit conversion allowed
    Distance d1 = 100;  // OK
    printDistance(500);  // OK - implicit conversion

    // Explicit conversion required
    SafeDistance sd1(200);  // OK
    // SafeDistance sd2 = 300;  // ERROR - implicit conversion not allowed
    // printSafeDistance(600);  // ERROR

    // Must be explicit
    printSafeDistance(SafeDistance(600));  // OK

    return 0;
}
