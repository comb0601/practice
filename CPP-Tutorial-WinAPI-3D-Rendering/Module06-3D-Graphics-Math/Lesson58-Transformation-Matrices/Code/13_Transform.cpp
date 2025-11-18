#include <iostream>
#include <cmath>
class Matrix4x4 {
public:
    float m[16];
    Matrix4x4() { for(int i=0;i<16;i++) m[i]=(i%5==0)?1.0f:0.0f; }
    static Matrix4x4 Translation(float x, float y, float z) {
        Matrix4x4 result;
        result.m[3]=x; result.m[7]=y; result.m[11]=z;
        return result;
    }
    static Matrix4x4 RotationZ(float angle) {
        Matrix4x4 result;
        float c=cos(angle), s=sin(angle);
        result.m[0]=c; result.m[1]=-s; result.m[4]=s; result.m[5]=c;
        return result;
    }
    void Print() {
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++) std::cout << m[i*4+j] << " ";
            std::cout << std::endl;
        }
    }
};
int main() {
    std::cout << "Transformation Example 13" << std::endl;
    Matrix4x4 t = Matrix4x4::Translation(5,10,0);
    t.Print();
    return 0;
}
