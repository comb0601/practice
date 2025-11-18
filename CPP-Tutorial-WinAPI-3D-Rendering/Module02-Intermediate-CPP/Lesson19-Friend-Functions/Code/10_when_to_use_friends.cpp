#include <iostream>
using namespace std;
// Use friends when:
// 1. Binary operators need symmetry
// 2. Stream operators (must be non-members)
// 3. Utility functions need private access
// 4. Improving encapsulation while allowing specific access
class Matrix {
    int data[2][2];
public:
    Matrix() {
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                data[i][j] = 0;
    }
    // Friend for matrix multiplication
    friend Matrix operator*(const Matrix&, const Matrix&);
    // Friend for output
    friend ostream& operator<<(ostream&, const Matrix&);
};
Matrix operator*(const Matrix& m1, const Matrix& m2) {
    Matrix result;
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            for(int k = 0; k < 2; k++)
                result.data[i][j] += m1.data[i][k] * m2.data[k][j];
    return result;
}
ostream& operator<<(ostream& os, const Matrix& m) {
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++)
            os << m.data[i][j] << " ";
        os << endl;
    }
    return os;
}
int main() {
    Matrix m1, m2;
    Matrix m3 = m1 * m2;
    cout << "Result:\n" << m3;
    return 0;
}
