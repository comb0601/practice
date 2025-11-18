/**
 * Class Template: Generic Pair
 * Demonstrates pair class with two different types
 */

#include <iostream>
#include <string>
using namespace std;

template <typename T1, typename T2>
class Pair {
private:
    T1 first;
    T2 second;

public:
    // Constructor
    Pair() : first(T1()), second(T2()) {}
    Pair(T1 f, T2 s) : first(f), second(s) {}

    // Getters
    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }

    // Setters
    void setFirst(T1 f) { first = f; }
    void setSecond(T2 s) { second = s; }

    // Print
    void print() const {
        cout << "(" << first << ", " << second << ")";
    }

    // Swap
    void swap() {
        // Only works if T1 and T2 are the same type
        // This will cause compilation error if types differ
    }

    // Comparison
    bool operator==(const Pair& other) const {
        return first == other.first && second == other.second;
    }

    bool operator!=(const Pair& other) const {
        return !(*this == other);
    }
};

// Helper function to create pairs
template <typename T1, typename T2>
Pair<T1, T2> makePair(T1 first, T2 second) {
    return Pair<T1, T2>(first, second);
}

int main() {
    cout << "=== Generic Pair Template ===" << endl << endl;

    // Integer-Double pair
    cout << "Integer-Double Pair:" << endl;
    Pair<int, double> p1(5, 3.14);
    p1.print();
    cout << endl;
    cout << "First: " << p1.getFirst() << endl;
    cout << "Second: " << p1.getSecond() << endl;
    cout << endl;

    // String-Integer pair (name and age)
    cout << "Name-Age Pairs:" << endl;
    Pair<string, int> person1("Alice", 25);
    Pair<string, int> person2("Bob", 30);
    Pair<string, int> person3("Charlie", 35);

    person1.print();
    cout << " - " << person1.getFirst() << " is " << person1.getSecond() << " years old" << endl;
    person2.print();
    cout << " - " << person2.getFirst() << " is " << person2.getSecond() << " years old" << endl;
    person3.print();
    cout << " - " << person3.getFirst() << " is " << person3.getSecond() << " years old" << endl;
    cout << endl;

    // Coordinates (string and pair of doubles)
    cout << "City Coordinates:" << endl;
    Pair<string, Pair<double, double>> city1("New York", makePair(40.7128, -74.0060));
    Pair<string, Pair<double, double>> city2("London", makePair(51.5074, -0.1278));
    Pair<string, Pair<double, double>> city3("Tokyo", makePair(35.6762, 139.6503));

    cout << city1.getFirst() << ": ";
    city1.getSecond().print();
    cout << endl;

    cout << city2.getFirst() << ": ";
    city2.getSecond().print();
    cout << endl;

    cout << city3.getFirst() << ": ";
    city3.getSecond().print();
    cout << endl;
    cout << endl;

    // Boolean-String pair (status and message)
    cout << "Status-Message Pairs:" << endl;
    Pair<bool, string> result1(true, "Operation successful");
    Pair<bool, string> result2(false, "Error occurred");

    if (result1.getFirst()) {
        cout << "Success: " << result1.getSecond() << endl;
    }

    if (!result2.getFirst()) {
        cout << "Failure: " << result2.getSecond() << endl;
    }
    cout << endl;

    // Using makePair helper
    cout << "Using makePair helper:" << endl;
    auto p2 = makePair(42, 3.14);
    auto p3 = makePair(string("Hello"), 100);
    auto p4 = makePair('A', true);

    p2.print();
    cout << endl;
    p3.print();
    cout << endl;
    p4.print();
    cout << endl;
    cout << endl;

    // Comparison
    cout << "Comparison:" << endl;
    Pair<int, int> coord1(10, 20);
    Pair<int, int> coord2(10, 20);
    Pair<int, int> coord3(15, 25);

    cout << "coord1: ";
    coord1.print();
    cout << ", coord2: ";
    coord2.print();
    cout << " -> Equal: " << (coord1 == coord2 ? "Yes" : "No") << endl;

    cout << "coord1: ";
    coord1.print();
    cout << ", coord3: ";
    coord3.print();
    cout << " -> Equal: " << (coord1 == coord3 ? "Yes" : "No") << endl;

    return 0;
}
