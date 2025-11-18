#include <iostream>
#include <cstring>
class Matrix4x4 {
public:
    float m[16];
    Matrix4x4() { memset(m, 0, sizeof(m)); }
    static Matrix4x4 Identity() {
        Matrix4x4 result;
        result.m[0] = result.m[5] = result.m[10] = result.m[15] = 1.0f;
        return result;
    }
    float operator()(int r, int c) const { return m[r*4+c]; }
    void Print() const {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                std::cout << (*this)(i,j) << " ";
            std::cout << std::endl;
        }
    }
};
int main() {
    Matrix4x4 identity = Matrix4x4::Identity();
    std::cout << "Identity Matrix:" << std::endl;
    identity.Print();
    return 0;
}
