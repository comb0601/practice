#include <iostream>
#include <cstring>
class Matrix4x4 {
public:
    float m[16];
    Matrix4x4() { memset(m, 0, sizeof(m)); }
    float& operator()(int row, int col) { return m[row * 4 + col]; }
    float operator()(int row, int col) const { return m[row * 4 + col]; }
    void Print() const {
        for (int i = 0; i < 4; i++) {
            std::cout << "[ ";
            for (int j = 0; j < 4; j++)
                std::cout << (*this)(i,j) << " ";
            std::cout << "]" << std::endl;
        }
    }
};
int main() {
    Matrix4x4 mat;
    mat(0,0) = 1; mat(1,1) = 2; mat(2,2) = 3; mat(3,3) = 4;
    std::cout << "Matrix:" << std::endl;
    mat.Print();
    return 0;
}
