#include <iostream>
#include <cmath>
class Matrix4x4 {
public:
    float m[16];
    static Matrix4x4 Perspective(float fov, float aspect, float near, float far) {
        Matrix4x4 result;
        float f = 1.0f / tan(fov/2);
        result.m[0] = f/aspect; result.m[5] = f;
        result.m[10] = (far+near)/(near-far);
        result.m[11] = (2*far*near)/(near-far);
        result.m[14] = -1;
        return result;
    }
};
int main() {
    std::cout << "Projection Example 11" << std::endl;
    Matrix4x4 proj = Matrix4x4::Perspective(1.57f, 16.0f/9.0f, 0.1f, 1000.0f);
    return 0;
}
