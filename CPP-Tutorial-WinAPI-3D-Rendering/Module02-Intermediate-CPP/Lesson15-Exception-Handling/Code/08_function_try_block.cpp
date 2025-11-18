#include <iostream>
using namespace std;
class MyClass {
public:
    MyClass() try {
        throw runtime_error("Constructor exception");
    } catch(const exception& e) {
        cout << "Caught in constructor: " << e.what() << endl;
        throw;
    }
};
int main() {
    try {
        MyClass obj;
    } catch(const exception& e) {
        cout << "Caught in main: " << e.what() << endl;
    }
    return 0;
}
