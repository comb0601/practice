/*
 * Lesson 11: Operator Overloading - Complete Class Example
 *
 * This example shows a fully-featured Matrix class with comprehensive
 * operator overloading demonstrating best practices.
 *
 * Compilation: g++ -std=c++17 14_complete_class_example.cpp -o matrix
 * Execution: ./matrix
 */

#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <cmath>

class Matrix {
private:
    double** data;
    size_t rows;
    size_t cols;

    void allocate() {
        data = new double*[rows];
        for (size_t i = 0; i < rows; ++i) {
            data[i] = new double[cols]();  // Initialize to zero
        }
    }

    void deallocate() {
        if (data) {
            for (size_t i = 0; i < rows; ++i) {
                delete[] data[i];
            }
            delete[] data;
        }
    }

public:
    // Constructor
    Matrix(size_t r = 2, size_t c = 2) : rows(r), cols(c) {
        allocate();
    }

    // Destructor
    ~Matrix() {
        deallocate();
    }

    // Copy constructor
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        allocate();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }

    // Move constructor (C++11)
    Matrix(Matrix&& other) noexcept
        : data(other.data), rows(other.rows), cols(other.cols) {
        other.data = nullptr;
        other.rows = 0;
        other.cols = 0;
    }

    // Copy assignment
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            deallocate();
            rows = other.rows;
            cols = other.cols;
            allocate();
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    data[i][j] = other.data[i][j];
                }
            }
        }
        return *this;
    }

    // Move assignment (C++11)
    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            deallocate();
            data = other.data;
            rows = other.rows;
            cols = other.cols;
            other.data = nullptr;
            other.rows = 0;
            other.cols = 0;
        }
        return *this;
    }

    // Subscript operator - returns row for chaining
    double* operator[](size_t row) {
        if (row >= rows) {
            throw std::out_of_range("Row index out of bounds");
        }
        return data[row];
    }

    const double* operator[](size_t row) const {
        if (row >= rows) {
            throw std::out_of_range("Row index out of bounds");
        }
        return data[row];
    }

    // Function call operator - alternative access
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

    // Arithmetic operators
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Invalid matrix dimensions for multiplication");
        }
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                for (size_t k = 0; k < cols; ++k) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    Matrix operator*(double scalar) const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    // Compound assignment operators
    Matrix& operator+=(const Matrix& other) {
        *this = *this + other;
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        *this = *this - other;
        return *this;
    }

    Matrix& operator*=(double scalar) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] *= scalar;
            }
        }
        return *this;
    }

    // Comparison operators
    bool operator==(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (std::abs(data[i][j] - other.data[i][j]) > 1e-10) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    // Unary operators
    Matrix operator-() const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = -data[i][j];
            }
        }
        return result;
    }

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
    friend std::istream& operator>>(std::istream& is, Matrix& m);

    // Helper methods
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    Matrix transpose() const {
        Matrix result(cols, rows);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[j][i] = data[i][j];
            }
        }
        return result;
    }
};

// Stream output operator
std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    for (size_t i = 0; i < m.rows; ++i) {
        os << "[ ";
        for (size_t j = 0; j < m.cols; ++j) {
            os << std::setw(8) << std::fixed << std::setprecision(2) << m.data[i][j];
            if (j < m.cols - 1) os << " ";
        }
        os << " ]";
        if (i < m.rows - 1) os << "\n";
    }
    return os;
}

// Stream input operator
std::istream& operator>>(std::istream& is, Matrix& m) {
    std::cout << "Enter " << m.rows << "x" << m.cols << " matrix elements:\n";
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            std::cout << "[" << i << "][" << j << "]: ";
            is >> m.data[i][j];
        }
    }
    return is;
}

// Non-member operator for scalar * matrix
Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar;
}

int main() {
    std::cout << "=== Complete Matrix Class Demo ===" << std::endl << std::endl;

    // Create matrices
    Matrix m1(2, 2);
    m1[0][0] = 1; m1[0][1] = 2;
    m1[1][0] = 3; m1[1][1] = 4;

    Matrix m2(2, 2);
    m2[0][0] = 5; m2[0][1] = 6;
    m2[1][0] = 7; m2[1][1] = 8;

    std::cout << "Matrix m1:\n" << m1 << std::endl << std::endl;
    std::cout << "Matrix m2:\n" << m2 << std::endl << std::endl;

    // Arithmetic operations
    Matrix sum = m1 + m2;
    std::cout << "m1 + m2:\n" << sum << std::endl << std::endl;

    Matrix diff = m1 - m2;
    std::cout << "m1 - m2:\n" << diff << std::endl << std::endl;

    Matrix prod = m1 * m2;
    std::cout << "m1 * m2:\n" << prod << std::endl << std::endl;

    Matrix scaled = m1 * 2.5;
    std::cout << "m1 * 2.5:\n" << scaled << std::endl << std::endl;

    Matrix scaled2 = 3.0 * m1;
    std::cout << "3.0 * m1:\n" << scaled2 << std::endl << std::endl;

    // Compound assignment
    Matrix m3 = m1;
    m3 += m2;
    std::cout << "m3 = m1; m3 += m2:\n" << m3 << std::endl << std::endl;

    // Unary minus
    Matrix negated = -m1;
    std::cout << "-m1:\n" << negated << std::endl << std::endl;

    // Comparison
    Matrix m4 = m1;
    std::cout << "m1 == m4: " << (m1 == m4 ? "true" : "false") << std::endl;
    std::cout << "m1 == m2: " << (m1 == m2 ? "true" : "false") << std::endl << std::endl;

    // Transpose
    Matrix transposed = m1.transpose();
    std::cout << "m1 transposed:\n" << transposed << std::endl << std::endl;

    // Function call operator
    std::cout << "Accessing m1(0,1): " << m1(0, 1) << std::endl;
    m1(0, 1) = 99;
    std::cout << "After m1(0,1) = 99:\n" << m1 << std::endl;

    return 0;
}
