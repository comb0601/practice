#include <iostream>
using namespace std;
namespace Outer {
    namespace Inner {
        int value = 100;
        void print() { cout << "Inner value: " << value << endl; }
    }
}
int main() {
    Outer::Inner::print();
    cout << Outer::Inner::value << endl;
    return 0;
}
