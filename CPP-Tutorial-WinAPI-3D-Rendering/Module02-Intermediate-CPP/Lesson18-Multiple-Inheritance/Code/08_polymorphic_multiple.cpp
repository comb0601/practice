#include <iostream>
using namespace std;
class Shape {
public:
    virtual void draw() = 0;
};
class Color {
public:
    virtual void fill() = 0;
};
class ColoredCircle : public Shape, public Color {
public:
    void draw() override { cout << "Drawing circle" << endl; }
    void fill() override { cout << "Filling with color" << endl; }
};
int main() {
    ColoredCircle cc;
    Shape* s = &cc;
    Color* c = &cc;
    s->draw();
    c->fill();
    return 0;
}
