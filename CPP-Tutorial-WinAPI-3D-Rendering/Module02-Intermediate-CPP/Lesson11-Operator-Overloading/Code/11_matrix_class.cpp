/*
 * Lesson 11: Operator Overloading
 * File: 11_matrix_class.cpp
 *
 * Comprehensive Matrix class for linear algebra operations.
 * Demonstrates operator overloading for 2D matrices.
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

class Matrix {
private:
    std::vector<std::vector<double>> data;
    size_t rows, cols;

public:
    // Constructor
    Matrix(size_t r, size_t c, double initVal = 0.0) : rows(r), cols(c) {
        data.resize(rows, std::vector<double>(cols, initVal));
    }

    // Constructor with initializer list
    Matrix(std::initializer_list<std::initializer_list<double>> list) {
        rows = list.size();
        cols = rows > 0 ? list.begin()->size() : 0;

        data.resize(rows);
        size_t i = 0;
        for (const auto& row : list) {
            if (row.size() != cols) {
                throw std::invalid_argument("All rows must have same number of columns");
            }
            data[i].assign(row.begin(), row.end());
            i++;
        }
    }

    // Getters
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    // Subscript operator for row access (allows matrix[i][j])
    std::vector<double>& operator[](size_t row) {
        if (row >= rows) {
            throw std::out_of_range("Row index out of bounds");
        }
        return data[row];
    }

    const std::vector<double>& operator[](size_t row) const {
        if (row >= rows) {
            throw std::out_of_range("Row index out of bounds");
        }
        return data[row];
    }

    // Function call operator for element access
    double& operator()(size_t row, size_t col) {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[row][col];
    }

    const double& operator()(size_t row, size_t col) const {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[row][col];
    }

    // Matrix addition
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match for addition");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result[i][j] = data[i][j] + other[i][j];
            }
        }
        return result;
    }

    // Matrix subtraction
    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match for subtraction");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result[i][j] = data[i][j] - other[i][j];
            }
        }
        return result;
    }

    // Matrix multiplication
    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
        }
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < other.cols; j++) {
                for (size_t k = 0; k < cols; k++) {
                    result[i][j] += data[i][k] * other[k][j];
                }
            }
        }
        return result;
    }

    // Scalar multiplication
    Matrix operator*(double scalar) const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    // Scalar division
    Matrix operator/(double scalar) const {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero");
        }
        return *this * (1.0 / scalar);
    }

    // Unary minus
    Matrix operator-() const {
        return *this * (-1.0);
    }

    // Compound assignment operators
    Matrix& operator+=(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match");
        }
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                data[i][j] += other[i][j];
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match");
        }
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                data[i][j] -= other[i][j];
            }
        }
        return *this;
    }

    Matrix& operator*=(double scalar) {
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                data[i][j] *= scalar;
            }
        }
        return *this;
    }

    Matrix& operator/=(double scalar) {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero");
        }
        return *this *= (1.0 / scalar);
    }

    // Comparison operators
    bool operator==(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        const double epsilon = 1e-10;
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                if (fabs(data[i][j] - other[i][j]) > epsilon) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    // Matrix operations
    Matrix transpose() const {
        Matrix result(cols, rows);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result[j][i] = data[i][j];
            }
        }
        return result;
    }

    // Get determinant (for square matrices)
    double determinant() const {
        if (rows != cols) {
            throw std::runtime_error("Determinant only defined for square matrices");
        }
        if (rows == 1) {
            return data[0][0];
        }
        if (rows == 2) {
            return data[0][0] * data[1][1] - data[0][1] * data[1][0];
        }
        // For larger matrices, use cofactor expansion
        double det = 0;
        for (size_t j = 0; j < cols; j++) {
            det += (j % 2 == 0 ? 1 : -1) * data[0][j] * minor(0, j).determinant();
        }
        return det;
    }

    // Get minor matrix (remove row i and column j)
    Matrix minor(size_t skipRow, size_t skipCol) const {
        Matrix result(rows - 1, cols - 1);
        size_t r = 0;
        for (size_t i = 0; i < rows; i++) {
            if (i == skipRow) continue;
            size_t c = 0;
            for (size_t j = 0; j < cols; j++) {
                if (j == skipCol) continue;
                result[r][c] = data[i][j];
                c++;
            }
            r++;
        }
        return result;
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (size_t i = 0; i < m.rows; i++) {
            os << "[";
            for (size_t j = 0; j < m.cols; j++) {
                os << std::setw(8) << std::fixed << std::setprecision(2) << m.data[i][j];
                if (j < m.cols - 1) os << " ";
            }
            os << " ]";
            if (i < m.rows - 1) os << "\n";
        }
        return os;
    }

    // Static factory methods
    static Matrix identity(size_t n) {
        Matrix result(n, n);
        for (size_t i = 0; i < n; i++) {
            result[i][i] = 1.0;
        }
        return result;
    }

    static Matrix zeros(size_t r, size_t c) {
        return Matrix(r, c, 0.0);
    }

    static Matrix ones(size_t r, size_t c) {
        return Matrix(r, c, 1.0);
    }
};

// Non-member scalar multiplication
Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar;
}

int main() {
    std::cout << "=== Matrix Class Demo ===\n\n";

    // Create matrices
    std::cout << "--- Creating Matrices ---\n";
    Matrix m1 = {{1, 2, 3},
                 {4, 5, 6},
                 {7, 8, 9}};
    std::cout << "m1 =\n" << m1 << "\n\n";

    Matrix m2 = {{9, 8, 7},
                 {6, 5, 4},
                 {3, 2, 1}};
    std::cout << "m2 =\n" << m2 << "\n\n";

    // Matrix addition
    std::cout << "--- Matrix Addition ---\n";
    Matrix sum = m1 + m2;
    std::cout << "m1 + m2 =\n" << sum << "\n\n";

    // Matrix subtraction
    std::cout << "--- Matrix Subtraction ---\n";
    Matrix diff = m1 - m2;
    std::cout << "m1 - m2 =\n" << diff << "\n\n";

    // Scalar multiplication
    std::cout << "--- Scalar Multiplication ---\n";
    Matrix scaled = m1 * 2;
    std::cout << "m1 * 2 =\n" << scaled << "\n\n";
    std::cout << "3 * m1 =\n" << (3 * m1) << "\n\n";

    // Matrix multiplication
    std::cout << "--- Matrix Multiplication ---\n";
    Matrix a = {{1, 2},
                {3, 4}};
    Matrix b = {{5, 6},
                {7, 8}};
    std::cout << "a =\n" << a << "\n\n";
    std::cout << "b =\n" << b << "\n\n";
    Matrix product = a * b;
    std::cout << "a * b =\n" << product << "\n\n";

    // Transpose
    std::cout << "--- Transpose ---\n";
    Matrix c = {{1, 2, 3},
                {4, 5, 6}};
    std::cout << "c =\n" << c << "\n\n";
    Matrix cT = c.transpose();
    std::cout << "c^T =\n" << cT << "\n\n";

    // Identity matrix
    std::cout << "--- Identity Matrix ---\n";
    Matrix I = Matrix::identity(3);
    std::cout << "I(3) =\n" << I << "\n\n";
    std::cout << "m1 * I =\n" << (m1 * I) << "\n\n";

    // Subscript operators
    std::cout << "--- Element Access ---\n";
    Matrix m3(3, 3);
    std::cout << "Created 3x3 matrix, filling with values:\n";
    int val = 1;
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            m3[i][j] = val++;
        }
    }
    std::cout << "m3 =\n" << m3 << "\n\n";

    // Function call operator
    std::cout << "Using function call operator:\n";
    std::cout << "m3(0, 0) = " << m3(0, 0) << "\n";
    std::cout << "m3(1, 1) = " << m3(1, 1) << "\n";
    std::cout << "m3(2, 2) = " << m3(2, 2) << "\n";
    m3(1, 1) = 100;
    std::cout << "After m3(1, 1) = 100:\n" << m3 << "\n\n";

    // Determinant
    std::cout << "--- Determinant ---\n";
    Matrix d2 = {{4, 6},
                 {3, 8}};
    std::cout << "d2 =\n" << d2 << "\n";
    std::cout << "det(d2) = " << d2.determinant() << "\n\n";

    Matrix d3 = {{6, 1, 1},
                 {4, -2, 5},
                 {2, 8, 7}};
    std::cout << "d3 =\n" << d3 << "\n";
    std::cout << "det(d3) = " << d3.determinant() << "\n\n";

    // Compound assignment
    std::cout << "--- Compound Assignment ---\n";
    Matrix m4 = {{1, 2},
                 {3, 4}};
    std::cout << "m4 =\n" << m4 << "\n\n";
    m4 += Matrix{{1, 1}, {1, 1}};
    std::cout << "After m4 += [[1,1],[1,1]]:\n" << m4 << "\n\n";
    m4 *= 2;
    std::cout << "After m4 *= 2:\n" << m4 << "\n\n";

    // Comparison
    std::cout << "--- Comparison ---\n";
    Matrix e1 = {{1, 2}, {3, 4}};
    Matrix e2 = {{1, 2}, {3, 4}};
    Matrix e3 = {{5, 6}, {7, 8}};
    std::cout << "e1 == e2: " << (e1 == e2 ? "true" : "false") << "\n";
    std::cout << "e1 == e3: " << (e1 == e3 ? "true" : "false") << "\n";
    std::cout << "e1 != e3: " << (e1 != e3 ? "true" : "false") << "\n\n";

    // Rotation matrix example
    std::cout << "--- Rotation Matrix (2D) ---\n";
    double angle = M_PI / 4;  // 45 degrees
    Matrix rotation = {{cos(angle), -sin(angle)},
                       {sin(angle), cos(angle)}};
    std::cout << "Rotation by 45°:\n" << rotation << "\n\n";

    Matrix point = {{1}, {0}};
    std::cout << "Point:\n" << point << "\n\n";
    Matrix rotated = rotation * point;
    std::cout << "Rotated point:\n" << rotated << "\n\n";

    // System of equations example
    std::cout << "--- Solving System: 2x + y = 5, x + y = 3 ---\n";
    Matrix A = {{2, 1}, {1, 1}};
    Matrix B = {{5}, {3}};
    std::cout << "Coefficient matrix A:\n" << A << "\n\n";
    std::cout << "Constants B:\n" << B << "\n\n";
    std::cout << "det(A) = " << A.determinant() << "\n";
    std::cout << "(Non-zero determinant means unique solution exists)\n";

    return 0;
}

/*
 * OUTPUT (partial):
 * =================================
 * === Matrix Class Demo ===
 *
 * --- Creating Matrices ---
 * m1 =
 * [    1.00     2.00     3.00 ]
 * [    4.00     5.00     6.00 ]
 * [    7.00     8.00     9.00 ]
 *
 * m2 =
 * [    9.00     8.00     7.00 ]
 * [    6.00     5.00     4.00 ]
 * [    3.00     2.00     1.00 ]
 *
 * --- Matrix Addition ---
 * m1 + m2 =
 * [   10.00    10.00    10.00 ]
 * [   10.00    10.00    10.00 ]
 * [   10.00    10.00    10.00 ]
 *
 * --- Matrix Subtraction ---
 * m1 - m2 =
 * [   -8.00    -6.00    -4.00 ]
 * [   -2.00     0.00     2.00 ]
 * [    4.00     6.00     8.00 ]
 *
 * ... (more output follows)
 */
