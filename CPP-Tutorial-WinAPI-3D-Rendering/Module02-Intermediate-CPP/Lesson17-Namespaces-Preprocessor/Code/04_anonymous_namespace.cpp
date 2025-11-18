#include <iostream>
using namespace std;
namespace {
    int secret = 42;
    void helper() { cout << "Anonymous namespace function" << endl; }
}
int main() {
    cout << "Secret: " << secret << endl;
    helper();
    return 0;
}
