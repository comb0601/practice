# Lesson 57: Matrices & Matrix Operations

## Course Information
- **Module**: Module 6 - 3D Graphics Mathematics
- **Lesson**: 57 of 64  
- **Estimated Time**: 4-5 hours
- **Difficulty**: Advanced

## Table of Contents
1. [Introduction to Matrices](#intro)
2. [Matrix Representation](#representation)
3. [Matrix Multiplication](#multiplication)
4. [Identity Matrix](#identity)
5. [Transpose](#transpose)
6. [Inverse Matrix](#inverse)
7. [Matrix-Vector Multiplication](#matrix-vector)
8. [Practical Applications](#applications)

## 1. Introduction to Matrices {#intro}

Matrices are the foundation of 3D transformations. A **4x4 matrix** can represent:
- Translation (movement)
- Rotation
- Scaling
- Projection
- Combined transformations

**Matrix Notation:**
```
      Column 0  Column 1  Column 2  Column 3
Row 0 [ m00      m01       m02       m03 ]
Row 1 [ m10      m11       m12       m13 ]
Row 2 [ m20      m21       m22       m23 ]
Row 3 [ m30      m31       m32       m33 ]
```

## 2. Matrix Representation {#representation}

### Storage (Row-Major vs Column-Major)

**Row-Major** (C/C++ arrays):
```cpp
float m[16] = {
    m00, m01, m02, m03,  // Row 0
    m10, m11, m12, m13,  // Row 1
    m20, m21, m22, m23,  // Row 2
    m30, m31, m32, m33   // Row 3
};
```

**Column-Major** (OpenGL):
```cpp
float m[16] = {
    m00, m10, m20, m30,  // Column 0
    m01, m11, m21, m31,  // Column 1
    m02, m12, m22, m32,  // Column 2
    m03, m13, m23, m33   // Column 3
};
```

### Matrix4x4 Class

```cpp
class Matrix4x4 {
public:
    float m[16];  // Row-major storage

    Matrix4x4();  // Identity
    float& operator()(int row, int col);
    Matrix4x4 operator*(const Matrix4x4& other) const;
    Vector3 operator*(const Vector3& v) const;
};
```

## 3. Matrix Multiplication {#multiplication}

### Formula

For C = A × B:
```
C[i][j] = Σ(k=0 to 3) A[i][k] * B[k][j]
```

### Properties

- **NOT Commutative**: A×B ≠ B×A
- **Associative**: (A×B)×C = A×(B×C)
- **Identity**: A×I = I×A = A

### Implementation

```cpp
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& b) const {
    Matrix4x4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += (*this)(row, k) * b(k, col);
            }
            result(row, col) = sum;
        }
    }
    return result;
}
```

## 4. Identity Matrix {#identity}

The identity matrix leaves vectors unchanged:

```
I = [ 1  0  0  0 ]
    [ 0  1  0  0 ]
    [ 0  0  1  0 ]
    [ 0  0  0  1 ]
```

## 5. Transpose {#transpose}

Swap rows and columns:

```
A = [ a  b  c ]       A^T = [ a  d  g ]
    [ d  e  f ]             [ b  e  h ]
    [ g  h  i ]             [ c  f  i ]
```

```cpp
Matrix4x4 Transpose() const {
    Matrix4x4 result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result(i, j) = (*this)(j, i);
    return result;
}
```

## 6. Inverse Matrix {#inverse}

The inverse satisfies: A × A⁻¹ = I

For 4x4 matrices, use:
- Gauss-Jordan elimination
- Adjugate method
- Decomposition methods (LU, Cholesky)

## 7. Matrix-Vector Multiplication {#matrix-vector}

```cpp
Vector3 Matrix4x4::operator*(const Vector3& v) const {
    float x = m[0]*v.x + m[1]*v.y + m[2]*v.z + m[3];
    float y = m[4]*v.x + m[5]*v.y + m[6]*v.z + m[7];
    float z = m[8]*v.x + m[9]*v.y + m[10]*v.z + m[11];
    float w = m[12]*v.x + m[13]*v.y + m[14]*v.z + m[15];
    return Vector3(x/w, y/w, z/w);  // Perspective divide
}
```

## 8. Applications {#applications}

- **Transformations**: Combine translate, rotate, scale
- **Camera**: View and projection matrices
- **Skeletal Animation**: Bone transformations
- **Instancing**: Per-object transforms

## Code Examples

1. **01_Matrix4x4Class.cpp** - Basic matrix class
2. **02_IdentityMatrix.cpp** - Identity matrix
3. **03_MatrixMultiplication.cpp** - Matrix * matrix
4. **04_MatrixTranspose.cpp** - Transpose operation
5. **05_MatrixInverse.cpp** - Matrix inversion
6. **06_MatrixVectorMult.cpp** - Matrix * vector
7. **07_MatrixComposition.cpp** - Combining matrices
8. **08_MatrixDecomposition.cpp** - Extracting components
9. **09_RowMajorColMajor.cpp** - Storage formats
10. **10_MatrixDeterminant.cpp** - Determinant calculation
11. **11_MatrixTrace.cpp** - Trace operation
12. **12_MatrixComparison.cpp** - Equality testing
13. **13_MatrixPrinting.cpp** - Debug output
14. **14_SubMatrices.cpp** - 3x3 submatrices
15. **15_MatrixBenchmark.cpp** - Performance testing

**End of Lesson 57**
