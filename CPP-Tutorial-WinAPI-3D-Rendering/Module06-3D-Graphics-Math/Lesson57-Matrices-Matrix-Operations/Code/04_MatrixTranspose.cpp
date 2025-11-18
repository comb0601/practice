#include <iostream>
#include <cstring>
class Matrix4x4 {
public:
    float m[16];
    Matrix4x4() { memset(m, 0, sizeof(m)); }
    float& operator()(int r, int c) { return m[r*4+c]; }
    float operator()(int r, int c) const { return m[r*4+c]; }
    Matrix4x4 Transpose() const {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result.m[i*4+j] = (*this)(j,i);
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
    Matrix4x4 m;
    m(0,0) = 1; m(0,1) = 2; m(0,2) = 3; m(0,3) = 4;
    m(1,0) = 5; m(1,1) = 6; m(1,2) = 7; m(1,3) = 8;
    std::cout << "Original:" << std::endl; m.Print();
    std::cout << "\nTranspose:" << std::endl; m.Transpose().Print();
    return 0;
}
