/*
 * Program: Constructor Initialization Order
 * Description: Order of member initialization
 * Compilation: g++ -std=c++17 10_constructor_order.cpp -o 10_constructor_order
 * Execution: ./10_constructor_order
 */

#include <iostream>
using namespace std;

class Member1 {
public:
    Member1() {
        cout << "  Member1 constructor" << endl;
    }
    ~Member1() {
        cout << "  Member1 destructor" << endl;
    }
};

class Member2 {
public:
    Member2() {
        cout << "  Member2 constructor" << endl;
    }
    ~Member2() {
        cout << "  Member2 destructor" << endl;
    }
};

class Member3 {
public:
    Member3() {
        cout << "  Member3 constructor" << endl;
    }
    ~Member3() {
        cout << "  Member3 destructor" << endl;
    }
};

class Container {
private:
    Member1 m1;
    Member2 m2;
    Member3 m3;

public:
    Container() {
        cout << "  Container constructor" << endl;
    }

    ~Container() {
        cout << "  Container destructor" << endl;
    }
};

int main() {
    cout << "=== Constructor Initialization Order ===" << endl << endl;

    cout << "Creating Container:" << endl;
    Container c;

    cout << "\nExiting (destructors in reverse order):" << endl;
    return 0;
}
