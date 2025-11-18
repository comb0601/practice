#include <iostream>
using namespace std;
class Fraction {
    int num, den;
public:
    Fraction(int n = 0, int d = 1) : num(n), den(d) {}
    friend ostream& operator<<(ostream&, const Fraction&);
    friend istream& operator>>(istream&, Fraction&);
};
ostream& operator<<(ostream& os, const Fraction& f) {
    os << f.num << "/" << f.den;
    return os;
}
istream& operator>>(istream& is, Fraction& f) {
    char slash;
    is >> f.num >> slash >> f.den;
    return is;
}
int main() {
    Fraction f1(3, 4);
    cout << "Fraction: " << f1 << endl;
    cout << "Enter fraction (n/d): ";
    Fraction f2;
    // cin >> f2;  // Uncomment for input
    // cout << "You entered: " << f2 << endl;
    return 0;
}
