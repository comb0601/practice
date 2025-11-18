#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;
double divide(double a, double b) {
    if(b == 0) throw invalid_argument("Division by zero");
    return a / b;
}
int main() {
    try {
        cout << "10 / 2 = " << divide(10, 2) << endl;
        cout << "10 / 0 = " << divide(10, 0) << endl;
    } catch(const invalid_argument& e) {
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}
