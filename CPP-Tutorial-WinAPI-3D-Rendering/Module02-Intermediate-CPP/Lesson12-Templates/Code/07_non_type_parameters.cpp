/**
 * Non-Type Template Parameters
 * Demonstrates templates with constant value parameters
 */

#include <iostream>
#include <stdexcept>
using namespace std;

// Fixed-size array with size as template parameter
template <typename T, size_t N>
class FixedArray {
private:
    T data[N];

public:
    // Constructor
    FixedArray() {
        for (size_t i = 0; i < N; i++) {
            data[i] = T();
        }
    }

    // Subscript operator
    T& operator[](size_t index) {
        if (index >= N) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= N) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    // Get size (compile-time constant)
    constexpr size_t size() const { return N; }

    // Fill with value
    void fill(const T& value) {
        for (size_t i = 0; i < N; i++) {
            data[i] = value;
        }
    }

    // Print
    void print() const {
        cout << "[";
        for (size_t i = 0; i < N; i++) {
            cout << data[i];
            if (i < N - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

// Matrix with compile-time dimensions
template <typename T, size_t Rows, size_t Cols>
class Matrix {
private:
    T data[Rows][Cols];

public:
    Matrix() {
        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < Cols; j++) {
                data[i][j] = T();
            }
        }
    }

    T& at(size_t row, size_t col) {
        if (row >= Rows || col >= Cols) {
            throw out_of_range("Matrix index out of bounds");
        }
        return data[row][col];
    }

    constexpr size_t rows() const { return Rows; }
    constexpr size_t cols() const { return Cols; }

    void print() const {
        for (size_t i = 0; i < Rows; i++) {
            cout << "[";
            for (size_t j = 0; j < Cols; j++) {
                cout << data[i][j];
                if (j < Cols - 1) cout << " ";
            }
            cout << "]" << endl;
        }
    }
};

// Power function - compile-time computation
template <int Base, int Exponent>
struct Power {
    static const int value = Base * Power<Base, Exponent - 1>::value;
};

template <int Base>
struct Power<Base, 0> {
    static const int value = 1;
};

// Factorial - compile-time computation
template <int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static const int value = 1;
};

int main() {
    cout << "=== Non-Type Template Parameters ===" << endl << endl;

    // Fixed-size arrays
    cout << "Fixed-Size Arrays:" << endl;
    FixedArray<int, 5> arr1;
    for (size_t i = 0; i < arr1.size(); i++) {
        arr1[i] = (i + 1) * 10;
    }
    cout << "arr1 (size " << arr1.size() << "): ";
    arr1.print();

    FixedArray<double, 3> arr2;
    arr2[0] = 3.14;
    arr2[1] = 2.71;
    arr2[2] = 1.41;
    cout << "arr2 (size " << arr2.size() << "): ";
    arr2.print();

    FixedArray<char, 10> arr3;
    arr3.fill('*');
    cout << "arr3 (size " << arr3.size() << "): ";
    arr3.print();
    cout << endl;

    // Different sizes create different types
    // FixedArray<int, 5> and FixedArray<int, 10> are DIFFERENT types
    FixedArray<int, 10> arr4;
    arr4.fill(7);
    cout << "arr4 (size " << arr4.size() << "): ";
    arr4.print();
    cout << endl;

    // Matrices
    cout << "Fixed-Size Matrices:" << endl;
    Matrix<int, 3, 3> mat1;
    for (size_t i = 0; i < mat1.rows(); i++) {
        for (size_t j = 0; j < mat1.cols(); j++) {
            mat1.at(i, j) = i * 3 + j + 1;
        }
    }
    cout << "3x3 Matrix:" << endl;
    mat1.print();
    cout << endl;

    Matrix<double, 2, 4> mat2;
    for (size_t i = 0; i < mat2.rows(); i++) {
        for (size_t j = 0; j < mat2.cols(); j++) {
            mat2.at(i, j) = (i + 1) * (j + 1) * 1.5;
        }
    }
    cout << "2x4 Matrix:" << endl;
    mat2.print();
    cout << endl;

    // Compile-time computations
    cout << "Compile-Time Computations:" << endl;
    cout << "2^10 = " << Power<2, 10>::value << endl;
    cout << "3^5 = " << Power<3, 5>::value << endl;
    cout << "5^3 = " << Power<5, 3>::value << endl;
    cout << endl;

    cout << "Factorials:" << endl;
    cout << "5! = " << Factorial<5>::value << endl;
    cout << "7! = " << Factorial<7>::value << endl;
    cout << "10! = " << Factorial<10>::value << endl;
    cout << endl;

    // These values are computed at compile time!
    constexpr int power = Power<2, 8>::value;
    constexpr int fact = Factorial<6>::value;
    cout << "constexpr values (computed at compile time):" << endl;
    cout << "2^8 = " << power << endl;
    cout << "6! = " << fact << endl;

    return 0;
}
