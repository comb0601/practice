#include <iostream>
class Vector2 { public: float x, y; Vector2(float x, float y):x(x),y(y){} };
class Vector3 { public: float x, y, z; Vector3(float x, float y, float z):x(x),y(y),z(z){} };
Vector2 NDCToScreen(Vector3 ndc, int w, int h) {
    return Vector2((ndc.x+1)*0.5f*w, (1-ndc.y)*0.5f*h);
}
int main() {
    Vector3 ndc(0.5f, 0.5f, 0);
    Vector2 screen = NDCToScreen(ndc, 1920, 1080);
    std::cout << "Viewport Example 12: (" << screen.x << "," << screen.y << ")" << std::endl;
    return 0;
}
