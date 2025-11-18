/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 07: Zero and Unit Vectors
 *
 * This program demonstrates:
 * - Zero vector properties and uses
 * - Unit vectors (basis vectors)
 * - Standard basis: x-hat, y-hat, z-hat
 * - Creating custom unit vectors
 *
 * Compile: g++ -o 07_ZeroAndUnitVectors 07_ZeroAndUnitVectors.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Static factory methods for common vectors
    static Vector3 Zero() { return Vector3(0, 0, 0); }
    static Vector3 One() { return Vector3(1, 1, 1); }
    static Vector3 UnitX() { return Vector3(1, 0, 0); }
    static Vector3 UnitY() { return Vector3(0, 1, 0); }
    static Vector3 UnitZ() { return Vector3(0, 0, 1); }
    static Vector3 Right() { return Vector3(1, 0, 0); }
    static Vector3 Up() { return Vector3(0, 1, 0); }
    static Vector3 Forward() { return Vector3(0, 0, 1); }
    static Vector3 Left() { return Vector3(-1, 0, 0); }
    static Vector3 Down() { return Vector3(0, -1, 0); }
    static Vector3 Back() { return Vector3(0, 0, -1); }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float LengthSquared() const {
        return x * x + y * y + z * z;
    }

    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
            z /= len;
        }
    }

    Vector3 Normalized() const {
        Vector3 result(*this);
        result.Normalize();
        return result;
    }

    bool IsZero(float epsilon = 0.0001f) const {
        return LengthSquared() < epsilon * epsilon;
    }

    bool IsUnit(float epsilon = 0.0001f) const {
        float lenSq = LengthSquared();
        return std::abs(lenSq - 1.0f) < epsilon;
    }

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(4)
                  << "(" << std::setw(7) << x << ", "
                  << std::setw(7) << y << ", "
                  << std::setw(7) << z << ")";
    }
};

int main() {
    std::cout << "=== Zero and Unit Vector Demonstrations ===" << std::endl;
    std::cout << std::endl;

    // 1. THE ZERO VECTOR
    std::cout << "1. THE ZERO VECTOR:" << std::endl;

    Vector3 zero = Vector3::Zero();
    std::cout << "  Zero vector: ";
    zero.Print();
    std::cout << std::endl;
    std::cout << "  Length: " << zero.Length() << std::endl;
    std::cout << "  Is zero: " << (zero.IsZero() ? "YES" : "NO") << std::endl;
    std::cout << "  Is unit: " << (zero.IsUnit() ? "YES" : "NO") << std::endl;
    std::cout << std::endl;

    std::cout << "  PROPERTIES OF ZERO VECTOR:" << std::endl;
    std::cout << "  - Only vector with length 0" << std::endl;
    std::cout << "  - Has no direction" << std::endl;
    std::cout << "  - Identity element for vector addition: v + 0 = v" << std::endl;
    std::cout << "  - Cannot be normalized (undefined direction)" << std::endl;
    std::cout << std::endl;

    // 2. STANDARD BASIS VECTORS (UNIT AXES)
    std::cout << "2. STANDARD BASIS VECTORS:" << std::endl;

    Vector3 unitX = Vector3::UnitX();
    Vector3 unitY = Vector3::UnitY();
    Vector3 unitZ = Vector3::UnitZ();

    std::cout << "  X-axis (x-hat): ";
    unitX.Print();
    std::cout << " -> Length: " << unitX.Length() << ", Is unit: " << (unitX.IsUnit() ? "YES" : "NO") << std::endl;

    std::cout << "  Y-axis (y-hat): ";
    unitY.Print();
    std::cout << " -> Length: " << unitY.Length() << ", Is unit: " << (unitY.IsUnit() ? "YES" : "NO") << std::endl;

    std::cout << "  Z-axis (z-hat): ";
    unitZ.Print();
    std::cout << " -> Length: " << unitZ.Length() << ", Is unit: " << (unitZ.IsUnit() ? "YES" : "NO") << std::endl;
    std::cout << std::endl;

    std::cout << "  These are ORTHONORMAL basis vectors:" << std::endl;
    std::cout << "  - Orthogonal: perpendicular to each other" << std::endl;
    std::cout << "  - Normal: each has unit length" << std::endl;
    std::cout << std::endl;

    // 3. DIRECTION UNIT VECTORS
    std::cout << "3. DIRECTION UNIT VECTORS (Right-Handed System):" << std::endl;

    std::cout << "  Right:   ";
    Vector3::Right().Print();
    std::cout << " (positive X)" << std::endl;

    std::cout << "  Left:    ";
    Vector3::Left().Print();
    std::cout << " (negative X)" << std::endl;

    std::cout << "  Up:      ";
    Vector3::Up().Print();
    std::cout << " (positive Y)" << std::endl;

    std::cout << "  Down:    ";
    Vector3::Down().Print();
    std::cout << " (negative Y)" << std::endl;

    std::cout << "  Forward: ";
    Vector3::Forward().Print();
    std::cout << " (positive Z)" << std::endl;

    std::cout << "  Back:    ";
    Vector3::Back().Print();
    std::cout << " (negative Z)" << std::endl;
    std::cout << std::endl;

    // 4. EXPRESSING VECTORS IN BASIS
    std::cout << "4. EXPRESSING VECTORS USING BASIS:" << std::endl;

    Vector3 v1(3, 4, 5);
    std::cout << "  v1 = ";
    v1.Print();
    std::cout << std::endl;
    std::cout << "  v1 = 3*(1,0,0) + 4*(0,1,0) + 5*(0,0,1)" << std::endl;
    std::cout << "  v1 = 3*x-hat + 4*y-hat + 5*z-hat" << std::endl;

    Vector3 v1_reconstructed = Vector3::UnitX() * 3.0f +
                                Vector3::UnitY() * 4.0f +
                                Vector3::UnitZ() * 5.0f;
    std::cout << "  Reconstructed: ";
    v1_reconstructed.Print();
    std::cout << std::endl;
    std::cout << std::endl;

    // 5. CREATING CUSTOM UNIT VECTORS
    std::cout << "5. CREATING CUSTOM UNIT VECTORS:" << std::endl;

    Vector3 diagonal(1, 1, 1);
    std::cout << "  Diagonal (1,1,1): ";
    diagonal.Print();
    std::cout << " -> Length: " << diagonal.Length() << std::endl;

    Vector3 diagonalUnit = diagonal.Normalized();
    std::cout << "  Normalized:       ";
    diagonalUnit.Print();
    std::cout << " -> Length: " << diagonalUnit.Length() << std::endl;
    std::cout << "  This is a unit vector pointing along the (1,1,1) diagonal" << std::endl;
    std::cout << std::endl;

    Vector3 custom(3, 4, 0);
    std::cout << "  Custom (3,4,0): ";
    custom.Print();
    std::cout << " -> Length: " << custom.Length() << std::endl;

    Vector3 customUnit = custom.Normalized();
    std::cout << "  Normalized:     ";
    customUnit.Print();
    std::cout << " -> Length: " << customUnit.Length() << std::endl;
    std::cout << std::endl;

    // 6. UNIT VECTOR PROPERTIES
    std::cout << "6. UNIT VECTOR PROPERTIES:" << std::endl;

    Vector3 u1 = Vector3::UnitX();
    Vector3 u2 = Vector3::UnitY();

    std::cout << "  u1 = ";
    u1.Print();
    std::cout << " (length: " << u1.Length() << ")" << std::endl;

    Vector3 scaled = u1 * 5.0f;
    std::cout << "  u1 * 5 = ";
    scaled.Print();
    std::cout << " (length: " << scaled.Length() << ")" << std::endl;
    std::cout << "  Scaling a unit vector by k gives vector of length k" << std::endl;
    std::cout << std::endl;

    // 7. THE "ONE" VECTOR
    std::cout << "7. THE 'ONE' VECTOR:" << std::endl;

    Vector3 one = Vector3::One();
    std::cout << "  One vector: ";
    one.Print();
    std::cout << std::endl;
    std::cout << "  Length: " << one.Length() << std::endl;
    std::cout << "  Is unit: " << (one.IsUnit() ? "YES" : "NO") << std::endl;
    std::cout << "  NOTE: (1,1,1) is NOT a unit vector!" << std::endl;
    std::cout << "  Length = sqrt(1² + 1² + 1²) = sqrt(3) ≈ 1.732" << std::endl;
    std::cout << std::endl;

    Vector3 oneNormalized = one.Normalized();
    std::cout << "  One normalized: ";
    oneNormalized.Print();
    std::cout << std::endl;
    std::cout << "  Length: " << oneNormalized.Length() << std::endl;
    std::cout << "  Is unit: " << (oneNormalized.IsUnit() ? "YES" : "NO") << std::endl;
    std::cout << std::endl;

    // 8. PRACTICAL: CAMERA VECTORS
    std::cout << "8. PRACTICAL: CAMERA COORDINATE SYSTEM:" << std::endl;
    std::cout << "  A typical camera uses three orthonormal vectors:" << std::endl;

    Vector3 cameraRight = Vector3::Right();
    Vector3 cameraUp = Vector3::Up();
    Vector3 cameraForward = Vector3::Forward();

    std::cout << "  Right:   ";
    cameraRight.Print();
    std::cout << " (length: " << cameraRight.Length() << ")" << std::endl;

    std::cout << "  Up:      ";
    cameraUp.Print();
    std::cout << " (length: " << cameraUp.Length() << ")" << std::endl;

    std::cout << "  Forward: ";
    cameraForward.Print();
    std::cout << " (length: " << cameraForward.Length() << ")" << std::endl;

    std::cout << "  All three are unit vectors, forming an orthonormal basis" << std::endl;
    std::cout << std::endl;

    // 9. CHECKING VECTOR TYPES
    std::cout << "9. IDENTIFYING VECTOR TYPES:" << std::endl;

    Vector3 vectors[] = {
        Vector3(0, 0, 0),
        Vector3(1, 0, 0),
        Vector3(0, 1, 0),
        Vector3(5, 0, 0),
        Vector3(1, 1, 1),
        Vector3(0.707f, 0.707f, 0)
    };

    for (int i = 0; i < 6; i++) {
        std::cout << "  ";
        vectors[i].Print();
        std::cout << " -> ";

        if (vectors[i].IsZero()) {
            std::cout << "ZERO vector";
        } else if (vectors[i].IsUnit()) {
            std::cout << "UNIT vector";
        } else {
            std::cout << "Regular vector (len=" << vectors[i].Length() << ")";
        }
        std::cout << std::endl;
    }

    return 0;
}

/*
 * KEY CONCEPTS:
 * =============
 *
 * 1. ZERO VECTOR:
 *    - Notation: 0⃗ or (0, 0, 0)
 *    - Only vector with zero length
 *    - No defined direction
 *    - Identity for addition: v + 0 = v
 *    - Cannot be normalized
 *
 * 2. UNIT VECTORS:
 *    - Vectors with length exactly 1
 *    - Represent pure direction
 *    - Any vector can create a unit vector: û = v / ||v||
 *
 * 3. STANDARD BASIS:
 *    - x̂ = (1, 0, 0) - X-axis
 *    - ŷ = (0, 1, 0) - Y-axis
 *    - ẑ = (0, 0, 1) - Z-axis
 *    - Orthonormal: perpendicular + unit length
 *
 * 4. DIRECTION VECTORS:
 *    - Right/Left: ±X direction
 *    - Up/Down: ±Y direction
 *    - Forward/Back: ±Z direction
 *
 * 5. VECTOR DECOMPOSITION:
 *    - Any vector = combination of basis vectors
 *    - (x, y, z) = x*x̂ + y*ŷ + z*ẑ
 *
 * 6. COMMON MISTAKE:
 *    - (1, 1, 1) is NOT a unit vector!
 *    - Length = √3 ≈ 1.732
 *    - Must normalize: (1,1,1)/√3 ≈ (0.577, 0.577, 0.577)
 *
 * 7. APPLICATIONS:
 *    - Camera systems (right, up, forward)
 *    - Coordinate transformations
 *    - Direction indicators
 *    - Reference frames
 */
