#include <iostream>
using namespace std;
namespace VeryLongNamespaceName {
    int value = 42;
}
namespace Short = VeryLongNamespaceName;
int main() {
    cout << "Using alias: " << Short::value << endl;
    cout << "Full name: " << VeryLongNamespaceName::value << endl;
    return 0;
}
