/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 03: Vector Arithmetic Operations
 *
 * This program demonstrates basic arithmetic operations on vectors:
 * - Addition (v1 + v2)
 * - Subtraction (v1 - v2)
 * - Scalar multiplication (v * scalar)
 * - Scalar division (v / scalar)
 * - Negation (-v)
 *
 * Compile: g++ -o 03_VectorArithmetic 03_VectorArithmetic.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

    // Vector addition: v1 + v2 = (x1+x2, y1+y2, z1+z2)
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // Vector subtraction: v1 - v2 = (x1-x2, y1-y2, z1-z2)
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // Scalar multiplication: v * s = (x*s, y*s, z*s)
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division: v / s = (x/s, y/s, z/s)
    Vector3 operator/(float scalar) const {
        if (scalar == 0.0f) {
            std::cerr << "ERROR: Division by zero!" << std::endl;
            return Vector3(0, 0, 0);
        }
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    // Unary negation: -v = (-x, -y, -z)
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    void Print() const {
        std::cout << "(" << std::setw(7) << std::setprecision(2) << std::fixed
                  << x << ", " << y << ", " << z << ")";
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

// Non-member function: scalar * vector
Vector3 operator*(float scalar, const Vector3& v) {
    return v * scalar;
}

void PrintOperation(const std::string& desc, const Vector3& result) {
    std::cout << "  " << std::setw(30) << std::left << desc;
    result.Print();
    std::cout << std::endl;
}

int main() {
    std::cout << "=== Vector Arithmetic Operations ===" << std::endl;
    std::cout << std::endl;

    // Define test vectors
    Vector3 v1(3.0f, 4.0f, 5.0f);
    Vector3 v2(1.0f, 2.0f, 3.0f);

    std::cout << "Test vectors:" << std::endl;
    std::cout << "  v1 = ";
    v1.Print();
    std::cout << std::endl;
    std::cout << "  v2 = ";
    v2.Print();
    std::cout << std::endl << std::endl;

    // 1. VECTOR ADDITION
    std::cout << "1. VECTOR ADDITION:" << std::endl;
    Vector3 sum = v1 + v2;
    PrintOperation("v1 + v2 =", sum);
    std::cout << "  Mathematical: (3+1, 4+2, 5+3) = (4, 6, 8)" << std::endl;
    std::cout << std::endl;

    // 2. VECTOR SUBTRACTION
    std::cout << "2. VECTOR SUBTRACTION:" << std::endl;
    Vector3 diff = v1 - v2;
    PrintOperation("v1 - v2 =", diff);
    std::cout << "  Mathematical: (3-1, 4-2, 5-3) = (2, 2, 2)" << std::endl;

    Vector3 diff2 = v2 - v1;
    PrintOperation("v2 - v1 =", diff2);
    std::cout << "  Note: Subtraction is NOT commutative" << std::endl;
    std::cout << std::endl;

    // 3. SCALAR MULTIPLICATION
    std::cout << "3. SCALAR MULTIPLICATION:" << std::endl;
    Vector3 scaled = v1 * 2.0f;
    PrintOperation("v1 * 2 =", scaled);
    std::cout << "  Mathematical: (3*2, 4*2, 5*2) = (6, 8, 10)" << std::endl;

    Vector3 scaled2 = 0.5f * v1;
    PrintOperation("0.5 * v1 =", scaled2);
    std::cout << "  Scaling by 0.5 halves the length" << std::endl;

    Vector3 scaled3 = v1 * -1.0f;
    PrintOperation("v1 * -1 =", scaled3);
    std::cout << "  Scaling by -1 reverses direction" << std::endl;
    std::cout << std::endl;

    // 4. SCALAR DIVISION
    std::cout << "4. SCALAR DIVISION:" << std::endl;
    Vector3 divided = v1 / 2.0f;
    PrintOperation("v1 / 2 =", divided);
    std::cout << "  Mathematical: (3/2, 4/2, 5/2) = (1.5, 2, 2.5)" << std::endl;

    Vector3 divided2 = v2 / 3.0f;
    PrintOperation("v2 / 3 =", divided2);
    std::cout << std::endl;

    // 5. UNARY NEGATION
    std::cout << "5. UNARY NEGATION:" << std::endl;
    Vector3 neg = -v1;
    PrintOperation("-v1 =", neg);
    std::cout << "  Negation reverses all components" << std::endl;
    std::cout << "  Original: ";
    v1.Print();
    std::cout << std::endl;
    std::cout << "  Negated:  ";
    neg.Print();
    std::cout << std::endl << std::endl;

    // 6. COMBINED OPERATIONS
    std::cout << "6. COMBINED OPERATIONS:" << std::endl;
    Vector3 result1 = (v1 + v2) * 2.0f;
    PrintOperation("(v1 + v2) * 2 =", result1);

    Vector3 result2 = v1 * 3.0f - v2;
    PrintOperation("v1 * 3 - v2 =", result2);

    Vector3 result3 = (v1 - v2) / 2.0f;
    PrintOperation("(v1 - v2) / 2 =", result3);
    std::cout << std::endl;

    // 7. GEOMETRIC INTERPRETATIONS
    std::cout << "7. GEOMETRIC INTERPRETATIONS:" << std::endl;

    Vector3 pointA(0.0f, 0.0f, 0.0f);
    Vector3 pointB(10.0f, 0.0f, 0.0f);

    std::cout << "  Point A: ";
    pointA.Print();
    std::cout << std::endl;
    std::cout << "  Point B: ";
    pointB.Print();
    std::cout << std::endl;

    Vector3 direction = pointB - pointA;
    std::cout << "  Direction from A to B: ";
    direction.Print();
    std::cout << std::endl;

    Vector3 midpoint = (pointA + pointB) / 2.0f;
    std::cout << "  Midpoint: ";
    midpoint.Print();
    std::cout << std::endl;
    std::cout << std::endl;

    // 8. DISPLACEMENT AND MOVEMENT
    std::cout << "8. DISPLACEMENT AND MOVEMENT:" << std::endl;
    Vector3 position(0.0f, 0.0f, 0.0f);
    Vector3 velocity(1.0f, 2.0f, 0.0f);
    float time = 5.0f;

    std::cout << "  Starting position: ";
    position.Print();
    std::cout << std::endl;
    std::cout << "  Velocity: ";
    velocity.Print();
    std::cout << std::endl;
    std::cout << "  Time: " << time << " seconds" << std::endl;

    Vector3 displacement = velocity * time;
    std::cout << "  Displacement: ";
    displacement.Print();
    std::cout << std::endl;

    Vector3 newPosition = position + displacement;
    std::cout << "  New position: ";
    newPosition.Print();
    std::cout << std::endl;
    std::cout << std::endl;

    // 9. VECTOR LENGTHS UNDER OPERATIONS
    std::cout << "9. VECTOR LENGTHS:" << std::endl;
    std::cout << "  |v1| = " << v1.Length() << std::endl;
    std::cout << "  |v2| = " << v2.Length() << std::endl;
    std::cout << "  |v1 + v2| = " << sum.Length() << std::endl;
    std::cout << "  |v1 - v2| = " << diff.Length() << std::endl;
    std::cout << "  |v1 * 2| = " << scaled.Length() << " (twice |v1|)" << std::endl;
    std::cout << "  |-v1| = " << neg.Length() << " (same as |v1|)" << std::endl;

    return 0;
}

/*
 * KEY CONCEPTS:
 * =============
 * 1. Vector Addition: Component-wise addition
 *    - Geometric: Place vectors head-to-tail
 *    - Commutative: v1 + v2 = v2 + v1
 *    - Associative: (v1 + v2) + v3 = v1 + (v2 + v3)
 *
 * 2. Vector Subtraction: Component-wise subtraction
 *    - Geometric: Direction from second to first point
 *    - NOT commutative: v1 - v2 ≠ v2 - v1
 *
 * 3. Scalar Multiplication: Scales the vector
 *    - s > 1: Lengthens vector
 *    - 0 < s < 1: Shortens vector
 *    - s < 0: Reverses direction
 *    - s = 0: Zero vector
 *
 * 4. Scalar Division: Equivalent to multiplication by 1/s
 *    - Watch for division by zero!
 *
 * 5. Negation: Reverses direction
 *    - Same magnitude, opposite direction
 *    - -v = v * (-1)
 *
 * 6. Applications:
 *    - Displacement: position + velocity * time
 *    - Midpoint: (p1 + p2) / 2
 *    - Direction: destination - source
 */
