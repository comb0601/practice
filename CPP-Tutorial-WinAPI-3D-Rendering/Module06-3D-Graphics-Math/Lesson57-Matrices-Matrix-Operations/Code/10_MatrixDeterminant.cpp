/* Lesson 57 Example 10 - Matrix Operations */
#include <iostream>
class Matrix4x4 {
public:
    float m[16];
    Matrix4x4() { for(int i=0; i<16; i++) m[i] = 0; }
    void Print() const {
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++)
                std::cout << m[i*4+j] << " ";
            std::cout << std::endl;
        }
    }
};
int main() {
    Matrix4x4 mat;
    std::cout << "Matrix Example 10" << std::endl;
    mat.Print();
    return 0;
}
