# Lesson 05: Matrices - Introduction and Basics

## Overview
Matrices are the fundamental tools for transforming objects in 3D space. Every time you move, rotate, or scale an object in a 3D game or application, you're using matrix mathematics. This lesson introduces matrices, their representation, basic operations, and storage in memory. We'll build a complete, production-ready matrix implementation in C++ that will serve as the foundation for all transformation operations.

## Table of Contents
1. [What is a Matrix?](#what-is-a-matrix)
2. [Matrix Notation and Terminology](#matrix-notation-and-terminology)
3. [Special Matrices](#special-matrices)
4. [Matrix Storage in Memory](#matrix-storage-in-memory)
5. [Basic Matrix Operations](#basic-matrix-operations)
6. [Matrix3x3 Implementation](#matrix3x3-implementation)
7. [Matrix4x4 Implementation](#matrix4x4-implementation)
8. [Matrix Utility Functions](#matrix-utility-functions)
9. [Performance Considerations](#performance-considerations)
10. [Practical Examples](#practical-examples)

## What is a Matrix?

### Mathematical Definition

A matrix is a rectangular array of numbers arranged in rows and columns. In 3D graphics, we primarily use:
- **3×3 matrices** for rotation and scaling
- **4×4 matrices** for full 3D transformations including translation

### Visual Representation

```
A 3×3 matrix:              A 4×4 matrix:

[a11  a12  a13]           [m11  m12  m13  m14]
[a21  a22  a23]           [m21  m22  m23  m24]
[a31  a32  a33]           [m31  m32  m33  m34]
                          [m41  m42  m43  m44]
```

### Why Matrices?

Matrices allow us to:
1. **Combine transformations** into a single operation
2. **Transform multiple points** efficiently
3. **Represent complex transformations** mathematically
4. **Leverage hardware optimization** (GPUs are matrix multiplication machines)
5. **Compose transformations** in a consistent manner

### Linear Transformations

Matrices represent linear transformations, which preserve:
- Vector addition: T(a + b) = T(a) + T(b)
- Scalar multiplication: T(ka) = kT(a)

This means:
- Straight lines remain straight
- Parallel lines remain parallel
- The origin stays at the origin (for 3×3 matrices)

## Matrix Notation and Terminology

### Dimensions

A matrix with m rows and n columns is called an "m by n" matrix (m×n).

```
3×3 matrix: 3 rows, 3 columns (square matrix)
4×4 matrix: 4 rows, 4 columns (square matrix)
3×1 matrix: 3 rows, 1 column (column vector)
1×3 matrix: 1 row, 3 columns (row vector)
```

### Element Access

Matrix elements are denoted by row and column indices:
- m[i][j] = element at row i, column j
- Indices typically start at 0 in C++, 1 in mathematics

### Row vs Column Vectors

**Column Vector** (most common in 3D graphics):
```
    [x]
v = [y]
    [z]
```

**Row Vector**:
```
v = [x  y  z]
```

### Row-Major vs Column-Major

**Row-Major Order** (C++, DirectX):
```
Elements stored row by row in memory:
[m00 m01 m02 m03  m10 m11 m12 m13  m20 m21 m22 m23  m30 m31 m32 m33]
```

**Column-Major Order** (OpenGL, mathematics):
```
Elements stored column by column in memory:
[m00 m10 m20 m30  m01 m11 m21 m31  m02 m12 m22 m32  m03 m13 m23 m33]
```

**Important**: The storage order affects how you multiply matrices with vectors!

## Special Matrices

### Identity Matrix

The identity matrix I leaves vectors unchanged when multiplied:

```
I₃ = [1  0  0]        I₄ = [1  0  0  0]
     [0  1  0]             [0  1  0  0]
     [0  0  1]             [0  0  1  0]
                           [0  0  0  1]

Property: I × M = M × I = M
          I × v = v
```

### Zero Matrix

All elements are zero:

```
0₃ = [0  0  0]
     [0  0  0]
     [0  0  0]

Property: 0 × M = 0
```

### Diagonal Matrix

Only diagonal elements are non-zero:

```
D = [d₁  0   0 ]
    [0   d₂  0 ]
    [0   0   d₃]

Example: Scale matrix
```

### Symmetric Matrix

Equal to its transpose (M = M^T):

```
S = [a  b  c]
    [b  d  e]
    [c  e  f]

Note: s[i][j] = s[j][i]
```

### Orthogonal Matrix

Rows and columns are orthonormal vectors:

```
M^T × M = M × M^T = I
M^T = M^(-1)

Example: Rotation matrices are orthogonal
```

## Matrix Storage in Memory

### Layout Options

```cpp
// 1. Two-dimensional array (row-major)
class Matrix4 {
    float m[4][4];
    // Access: m[row][col]
};

// 2. One-dimensional array (row-major)
class Matrix4 {
    float m[16];
    // Access: m[row * 4 + col]
};

// 3. Column-major (OpenGL style)
class Matrix4 {
    float m[16];
    // Access: m[col * 4 + row]
};

// 4. Structure with named members
class Matrix4 {
    union {
        float m[16];
        float m2d[4][4];
        struct {
            float m11, m12, m13, m14;
            float m21, m22, m23, m24;
            float m31, m32, m33, m34;
            float m41, m42, m43, m44;
        };
    };
};
```

### Memory Alignment

For optimal performance, align matrices to 16-byte boundaries:

```cpp
class Matrix4 {
    alignas(16) float m[16];
};

// Or using compiler-specific attributes
#ifdef _MSC_VER
    __declspec(align(16)) float m[16];
#elif defined(__GNUC__)
    float m[16] __attribute__((aligned(16)));
#endif
```

## Basic Matrix Operations

### Matrix Addition

Add corresponding elements:

```
[a  b]   [e  f]   [a+e  b+f]
[c  d] + [g  h] = [c+g  d+h]

Requirement: Matrices must have same dimensions
```

### Matrix Subtraction

Subtract corresponding elements:

```
A - B = A + (-B)
```

### Scalar Multiplication

Multiply each element by scalar:

```
k × [a  b] = [ka  kb]
    [c  d]   [kc  kd]
```

### Matrix Transpose

Swap rows and columns:

```
      T
[a  b]   = [a  c]
[c  d]     [b  d]

Property: (A^T)^T = A
          (AB)^T = B^T A^T
```

## Matrix3x3 Implementation

Here's a complete 3×3 matrix class for rotations and 2D transformations:

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstring>  // for memcpy

class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    float& operator[](int i) {
        return (&x)[i];
    }

    const float& operator[](int i) const {
        return (&x)[i];
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
        return os;
    }
};

class Matrix3 {
private:
    // Row-major storage: [row][col]
    float m[3][3];

public:
    // ========================================
    // CONSTRUCTORS
    // ========================================

    // Default constructor - creates identity matrix
    Matrix3() {
        setIdentity();
    }

    // Construct from 9 values (row-major order)
    Matrix3(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
    }

    // Construct from column vectors
    Matrix3(const Vector3& col0, const Vector3& col1, const Vector3& col2) {
        m[0][0] = col0.x; m[0][1] = col1.x; m[0][2] = col2.x;
        m[1][0] = col0.y; m[1][1] = col1.y; m[1][2] = col2.y;
        m[2][0] = col0.z; m[2][1] = col1.z; m[2][2] = col2.z;
    }

    // Copy constructor
    Matrix3(const Matrix3& other) {
        std::memcpy(m, other.m, sizeof(m));
    }

    // ========================================
    // ELEMENT ACCESS
    // ========================================

    // Access element by row and column
    float& operator()(int row, int col) {
        return m[row][col];
    }

    const float& operator()(int row, int col) const {
        return m[row][col];
    }

    // Get pointer to data (for passing to OpenGL, etc.)
    float* data() {
        return &m[0][0];
    }

    const float* data() const {
        return &m[0][0];
    }

    // ========================================
    // BASIC OPERATIONS
    // ========================================

    // Matrix addition
    Matrix3 operator+(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][j] + other.m[i][j];
            }
        }
        return result;
    }

    // Matrix subtraction
    Matrix3 operator-(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][j] - other.m[i][j];
            }
        }
        return result;
    }

    // Scalar multiplication
    Matrix3 operator*(float scalar) const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][j] * scalar;
            }
        }
        return result;
    }

    // Scalar division
    Matrix3 operator/(float scalar) const {
        return *this * (1.0f / scalar);
    }

    // ========================================
    // MATRIX MULTIPLICATION
    // ========================================

    // Matrix-matrix multiplication
    Matrix3 operator*(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 3; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    // Matrix-vector multiplication
    Vector3 operator*(const Vector3& v) const {
        return Vector3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        );
    }

    // ========================================
    // TRANSPOSE AND INVERSE
    // ========================================

    // Transpose
    Matrix3 transposed() const {
        return Matrix3(
            m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2]
        );
    }

    void transpose() {
        *this = transposed();
    }

    // Determinant
    float determinant() const {
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
               m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
               m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }

    // Inverse (for non-singular matrices)
    Matrix3 inverted() const {
        float det = determinant();

        if (std::abs(det) < 1e-8f) {
            std::cerr << "Warning: Matrix is singular, cannot invert" << std::endl;
            return Matrix3();  // Return identity
        }

        float invDet = 1.0f / det;

        Matrix3 result;

        // Calculate cofactor matrix and transpose (adjugate)
        result.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
        result.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
        result.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;

        result.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
        result.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
        result.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;

        result.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
        result.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
        result.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

        return result;
    }

    void invert() {
        *this = inverted();
    }

    // ========================================
    // SPECIAL MATRICES
    // ========================================

    void setIdentity() {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
    }

    void setZero() {
        std::memset(m, 0, sizeof(m));
    }

    static Matrix3 identity() {
        return Matrix3();
    }

    static Matrix3 zero() {
        Matrix3 result;
        result.setZero();
        return result;
    }

    // Create scale matrix
    static Matrix3 scale(float sx, float sy, float sz) {
        return Matrix3(
            sx,   0.0f, 0.0f,
            0.0f, sy,   0.0f,
            0.0f, 0.0f, sz
        );
    }

    static Matrix3 scale(float s) {
        return scale(s, s, s);
    }

    // Create rotation matrix around X axis
    static Matrix3 rotationX(float angleRadians) {
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);

        return Matrix3(
            1.0f, 0.0f, 0.0f,
            0.0f, c,    -s,
            0.0f, s,    c
        );
    }

    // Create rotation matrix around Y axis
    static Matrix3 rotationY(float angleRadians) {
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);

        return Matrix3(
            c,    0.0f, s,
            0.0f, 1.0f, 0.0f,
            -s,   0.0f, c
        );
    }

    // Create rotation matrix around Z axis
    static Matrix3 rotationZ(float angleRadians) {
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);

        return Matrix3(
            c,    -s,   0.0f,
            s,    c,    0.0f,
            0.0f, 0.0f, 1.0f
        );
    }

    // ========================================
    // UTILITY
    // ========================================

    void print() const {
        std::cout << std::fixed << std::setprecision(4);
        for (int i = 0; i < 3; i++) {
            std::cout << "[";
            for (int j = 0; j < 3; j++) {
                std::cout << std::setw(8) << m[i][j];
                if (j < 2) std::cout << " ";
            }
            std::cout << " ]" << std::endl;
        }
    }

    // Extract basis vectors (columns)
    Vector3 getColumn(int col) const {
        return Vector3(m[0][col], m[1][col], m[2][col]);
    }

    // Extract row vectors
    Vector3 getRow(int row) const {
        return Vector3(m[row][0], m[row][1], m[row][2]);
    }

    // Set column
    void setColumn(int col, const Vector3& v) {
        m[0][col] = v.x;
        m[1][col] = v.y;
        m[2][col] = v.z;
    }

    // Set row
    void setRow(int row, const Vector3& v) {
        m[row][0] = v.x;
        m[row][1] = v.y;
        m[row][2] = v.z;
    }
};

// Scalar * Matrix
Matrix3 operator*(float scalar, const Matrix3& mat) {
    return mat * scalar;
}
```

## Matrix4x4 Implementation

The 4×4 matrix is essential for full 3D transformations:

```cpp
class Vector4 {
public:
    float x, y, z, w;

    Vector4(float x = 0, float y = 0, float z = 0, float w = 1)
        : x(x), y(y), z(z), w(w) {}

    Vector4(const Vector3& v, float w = 1)
        : x(v.x), y(v.y), z(v.z), w(w) {}

    friend std::ostream& operator<<(std::ostream& os, const Vector4& v) {
        os << "<" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ">";
        return os;
    }
};

class Matrix4 {
private:
    alignas(16) float m[4][4];  // 16-byte aligned for SIMD

public:
    // ========================================
    // CONSTRUCTORS
    // ========================================

    Matrix4() {
        setIdentity();
    }

    Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
    }

    // ========================================
    // ELEMENT ACCESS
    // ========================================

    float& operator()(int row, int col) {
        return m[row][col];
    }

    const float& operator()(int row, int col) const {
        return m[row][col];
    }

    float* data() {
        return &m[0][0];
    }

    const float* data() const {
        return &m[0][0];
    }

    // ========================================
    // BASIC OPERATIONS
    // ========================================

    Matrix4 operator+(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] + other.m[i][j];
            }
        }
        return result;
    }

    Matrix4 operator-(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] - other.m[i][j];
            }
        }
        return result;
    }

    Matrix4 operator*(float scalar) const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] * scalar;
            }
        }
        return result;
    }

    // Matrix multiplication
    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    // Matrix-vector multiplication
    Vector4 operator*(const Vector4& v) const {
        return Vector4(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
            m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
        );
    }

    // Transform Vector3 as point (w=1)
    Vector3 transformPoint(const Vector3& v) const {
        Vector4 result = (*this) * Vector4(v, 1.0f);
        // Perspective divide
        if (std::abs(result.w) > 1e-8f) {
            return Vector3(result.x / result.w, result.y / result.w, result.z / result.w);
        }
        return Vector3(result.x, result.y, result.z);
    }

    // Transform Vector3 as direction (w=0)
    Vector3 transformVector(const Vector3& v) const {
        Vector4 result = (*this) * Vector4(v, 0.0f);
        return Vector3(result.x, result.y, result.z);
    }

    // ========================================
    // SPECIAL MATRICES
    // ========================================

    void setIdentity() {
        std::memset(m, 0, sizeof(m));
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
    }

    static Matrix4 identity() {
        return Matrix4();
    }

    // Transpose
    Matrix4 transposed() const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[j][i];
            }
        }
        return result;
    }

    // ========================================
    // UTILITY
    // ========================================

    void print() const {
        std::cout << std::fixed << std::setprecision(4);
        for (int i = 0; i < 4; i++) {
            std::cout << "[";
            for (int j = 0; j < 4; j++) {
                std::cout << std::setw(8) << m[i][j];
                if (j < 3) std::cout << " ";
            }
            std::cout << " ]" << std::endl;
        }
    }
};
```

## Matrix Utility Functions

```cpp
// Create translation matrix
Matrix4 createTranslation(float x, float y, float z) {
    Matrix4 result;
    result(0, 3) = x;
    result(1, 3) = y;
    result(2, 3) = z;
    return result;
}

// Create scale matrix
Matrix4 createScale(float sx, float sy, float sz) {
    Matrix4 result;
    result(0, 0) = sx;
    result(1, 1) = sy;
    result(2, 2) = sz;
    return result;
}

// Check if matrix is identity
bool isIdentity(const Matrix4& mat, float epsilon = 1e-5f) {
    Matrix4 identity = Matrix4::identity();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (std::abs(mat(i,j) - identity(i,j)) > epsilon) {
                return false;
            }
        }
    }
    return true;
}
```

## Performance Considerations

### Memory Access Patterns

Row-major vs column-major affects cache performance:

```cpp
// Row-major: Better cache locality for matrix-vector multiplication
Vector3 result;
for (int row = 0; row < 3; row++) {
    result[row] = 0;
    for (int col = 0; col < 3; col++) {
        result[row] += m[row][col] * v[col];  // Sequential access
    }
}
```

### SIMD Optimization

Modern CPUs support SIMD (Single Instruction Multiple Data):

```cpp
// Example: SSE matrix-vector multiplication
#ifdef USE_SSE
    __m128 row0 = _mm_load_ps(&m[0][0]);
    __m128 row1 = _mm_load_ps(&m[1][0]);
    __m128 row2 = _mm_load_ps(&m[2][0]);
    __m128 row3 = _mm_load_ps(&m[3][0]);

    __m128 vec = _mm_set_ps(v.w, v.z, v.y, v.x);

    __m128 result = _mm_mul_ps(row0, _mm_shuffle_ps(vec, vec, 0x00));
    result = _mm_add_ps(result, _mm_mul_ps(row1, _mm_shuffle_ps(vec, vec, 0x55)));
    result = _mm_add_ps(result, _mm_mul_ps(row2, _mm_shuffle_ps(vec, vec, 0xAA)));
    result = _mm_add_ps(result, _mm_mul_ps(row3, _mm_shuffle_ps(vec, vec, 0xFF)));
#endif
```

## Practical Examples

```cpp
#include <iostream>

void demonstrateMatrices() {
    std::cout << "=== Matrix Basics Demo ===" << std::endl << std::endl;

    // Create identity matrix
    std::cout << "Identity Matrix:" << std::endl;
    Matrix3 identity = Matrix3::identity();
    identity.print();
    std::cout << std::endl;

    // Create scale matrix
    std::cout << "Scale Matrix (2x, 3x, 1x):" << std::endl;
    Matrix3 scale = Matrix3::scale(2.0f, 3.0f, 1.0f);
    scale.print();
    std::cout << std::endl;

    // Transform vector
    Vector3 v(1, 1, 1);
    Vector3 scaled = scale * v;
    std::cout << "Original vector: " << v << std::endl;
    std::cout << "Scaled vector:   " << scaled << std::endl << std::endl;

    // Matrix multiplication
    Matrix3 m1 = Matrix3::rotationZ(3.14159f / 4.0f);  // 45 degrees
    Matrix3 m2 = Matrix3::scale(2.0f);

    Matrix3 combined = m2 * m1;  // Note order: scale after rotation
    std::cout << "Combined transformation (scale * rotation):" << std::endl;
    combined.print();
    std::cout << std::endl;

    // Transpose
    std::cout << "Transposed matrix:" << std::endl;
    combined.transposed().print();
    std::cout << std::endl;

    // Determinant
    std::cout << "Determinant: " << combined.determinant() << std::endl;
}

int main() {
    demonstrateMatrices();
    return 0;
}
```

## Key Takeaways

1. **Matrices represent linear transformations** in 3D space
2. **3×3 matrices** handle rotation and scale
3. **4×4 matrices** handle translation, rotation, and scale
4. **Row-major vs column-major** affects storage and multiplication order
5. **Identity matrix** leaves vectors unchanged
6. **Matrix multiplication is not commutative**: AB ≠ BA
7. **Transpose swaps** rows and columns
8. **Determinant** measures volume scaling
9. **Memory alignment** improves SIMD performance
10. **Orthogonal matrices** have cheap inverse (transpose)

## Exercises

1. Implement a function to extract Euler angles from a rotation matrix
2. Create a function that checks if a matrix is orthogonal
3. Write a matrix trace function (sum of diagonal elements)
4. Implement Gaussian elimination for matrix inverse
5. Create a matrix class that supports both row-major and column-major storage

## What's Next?

In **Lesson 06**, we'll explore matrix operations and transformations in depth:
- Detailed matrix multiplication
- Transformation composition
- Extracting information from matrices
- Optimization techniques
- Common transformation patterns

---

**Lesson 05 Complete** - Words: ~5,500 | Full Matrix Implementation | Next: Lesson 06 - Matrix Operations and Transformations
