#include <iostream>
using namespace std;
class Number {
    int value;
public:
    Number(int v) : value(v) {}
    // Member function: obj + int
    Number operator+(int n) {
        return Number(value + n);
    }
    // Friend function: int + obj (for symmetry)
    friend Number operator+(int n, const Number& obj) {
        return Number(n + obj.value);
    }
};
int main() {
    Number n(10);
    Number n1 = n + 5;    // Uses member function
    Number n2 = 5 + n;    // Uses friend function
    return 0;
}
