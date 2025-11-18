#include <iostream>
#include <cstring>
class Matrix4x4 {
public:
    float m[16];
    Matrix4x4() { memset(m, 0, sizeof(m)); }
    float& operator()(int r, int c) { return m[r*4+c]; }
    float operator()(int r, int c) const { return m[r*4+c]; }
    Matrix4x4 operator*(const Matrix4x4& b) const {
        Matrix4x4 result;
        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 4; col++) {
                float sum = 0;
                for (int k = 0; k < 4; k++)
                    sum += (*this)(row,k) * b(k,col);
                result.m[row*4+col] = sum;
            }
        return result;
    }
    void Print() const {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                std::cout << (*this)(i,j) << " ";
            std::cout << std::endl;
        }
    }
};
int main() {
    Matrix4x4 a, b;
    a(0,0) = 1; a(0,1) = 2; b(0,0) = 3; b(1,0) = 4;
    Matrix4x4 c = a * b;
    std::cout << "A * B =" << std::endl;
    c.Print();
    return 0;
}
