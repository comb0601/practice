/**
 * Multiple Type Parameters
 * Demonstrates templates with multiple type parameters
 */

#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

// Template with two different type parameters
template <typename T1, typename T2>
void printPair(T1 first, T2 second) {
    cout << "(" << first << ", " << second << ")" << endl;
}

// Template returning auto-deduced type
template <typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}

// Template with three type parameters
template <typename T1, typename T2, typename T3>
struct Triple {
    T1 first;
    T2 second;
    T3 third;

    Triple(T1 f, T2 s, T3 t) : first(f), second(s), third(t) {}

    void print() const {
        cout << "(" << first << ", " << second << ", " << third << ")" << endl;
    }
};

// Generic comparison function
template <typename T1, typename T2>
bool areEqual(T1 a, T2 b) {
    return static_cast<double>(a) == static_cast<double>(b);
}

// Generic multiply function
template <typename T1, typename T2>
auto multiply(T1 a, T2 b) -> decltype(a * b) {
    return a * b;
}

int main() {
    cout << "=== Multiple Type Parameters ===" << endl << endl;

    // Print pairs of different types
    cout << "Printing pairs:" << endl;
    printPair(42, 3.14);
    printPair("Name", 25);
    printPair(true, "Success");
    printPair(100, 200);
    cout << endl;

    // Addition with mixed types
    cout << "Addition with mixed types:" << endl;
    auto result1 = add(5, 3.14);
    cout << "5 + 3.14 = " << result1 << " (type: double)" << endl;

    auto result2 = add(10, 20);
    cout << "10 + 20 = " << result2 << " (type: int)" << endl;

    auto result3 = add(2.5, 7.5);
    cout << "2.5 + 7.5 = " << result3 << " (type: double)" << endl;
    cout << endl;

    // Triple structure
    cout << "Triple structures:" << endl;
    Triple<int, double, string> t1(1, 2.5, "hello");
    t1.print();

    Triple<string, int, bool> t2("Alice", 30, true);
    t2.print();

    Triple<char, char, char> t3('A', 'B', 'C');
    t3.print();
    cout << endl;

    // Comparison
    cout << "Comparison with mixed types:" << endl;
    cout << "5 == 5.0: " << (areEqual(5, 5.0) ? "true" : "false") << endl;
    cout << "10 == 10.5: " << (areEqual(10, 10.5) ? "true" : "false") << endl;
    cout << endl;

    // Multiplication
    cout << "Multiplication with mixed types:" << endl;
    cout << "5 * 3.14 = " << multiply(5, 3.14) << endl;
    cout << "10 * 20 = " << multiply(10, 20) << endl;
    cout << "2.5 * 4.0 = " << multiply(2.5, 4.0) << endl;

    return 0;
}
