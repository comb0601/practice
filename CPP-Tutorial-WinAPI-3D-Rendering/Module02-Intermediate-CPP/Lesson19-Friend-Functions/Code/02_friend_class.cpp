#include <iostream>
using namespace std;
class Box {
    int width;
    friend class BoxPrinter;
public:
    Box(int w) : width(w) {}
};
class BoxPrinter {
public:
    void print(Box& b) {
        cout << "Width: " << b.width << endl;
    }
};
int main() {
    Box box(20);
    BoxPrinter printer;
    printer.print(box);
    return 0;
}
