#include <iostream>
using namespace std;
namespace MyNamespace {
    int value = 42;
    void print() { cout << "Value: " << value << endl; }
}
int main() {
    cout << MyNamespace::value << endl;
    MyNamespace::print();
    return 0;
}
