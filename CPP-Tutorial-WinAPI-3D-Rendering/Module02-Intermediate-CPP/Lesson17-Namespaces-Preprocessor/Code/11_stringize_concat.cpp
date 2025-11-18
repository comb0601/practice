#include <iostream>
#define STRINGIZE(x) #x
#define CONCAT(a,b) a##b
using namespace std;
int main() {
    cout << STRINGIZE(Hello World) << endl;
    int xy = 100;
    cout << "CONCAT(x,y) = " << CONCAT(x,y) << endl;
    return 0;
}
