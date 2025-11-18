#include <iostream>
using namespace std;
class Box {
    int width;
    friend void printWidth(Box&);
public:
    Box(int w) : width(w) {}
};
void printWidth(Box& b) {
    cout << "Width: " << b.width << endl;
}
int main() {
    Box box(10);
    printWidth(box);
    return 0;
}
