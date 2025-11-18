#include <iostream>
using namespace std;
class Resource {
public:
    Resource() { cout << "Resource acquired" << endl; }
    ~Resource() { cout << "Resource released" << endl; }
};
int main() {
    try {
        Resource r;
        throw runtime_error("Error!");
    } catch(const exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
    cout << "Resource was properly cleaned up" << endl;
    return 0;
}
