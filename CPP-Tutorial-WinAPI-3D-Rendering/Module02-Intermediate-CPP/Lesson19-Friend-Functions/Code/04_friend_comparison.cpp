#include <iostream>
using namespace std;
class Point {
    int x, y;
public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    friend bool operator==(const Point&, const Point&);
    friend bool operator!=(const Point&, const Point&);
};
bool operator==(const Point& p1, const Point& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}
bool operator!=(const Point& p1, const Point& p2) {
    return !(p1 == p2);
}
int main() {
    Point p1(1, 2), p2(1, 2), p3(3, 4);
    cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << endl;
    cout << "p1 != p3: " << (p1 != p3 ? "true" : "false") << endl;
    return 0;
}
