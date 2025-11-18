/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 04: Complete Vector Operator Overloading
 *
 * This program demonstrates comprehensive operator overloading for Vector3:
 * - Binary operators: +, -, *, /
 * - Compound assignment: +=, -=, *=, /=
 * - Comparison: ==, !=
 * - Unary: -, +
 * - Stream operators: <<, >>
 *
 * Compile: g++ -o 04_VectorOperators 04_VectorOperators.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Binary arithmetic operators (return new vector)
    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }

    Vector3 operator/(float s) const {
        return Vector3(x / s, y / s, z / s);
    }

    // Compound assignment operators (modify this vector)
    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3& operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vector3& operator/=(float s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    // Unary operators
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    Vector3 operator+() const {
        return *this;  // Unary plus returns copy
    }

    // Comparison operators
    bool operator==(const Vector3& v) const {
        return (x == v.x) && (y == v.y) && (z == v.z);
    }

    bool operator!=(const Vector3& v) const {
        return !(*this == v);
    }

    // Array subscript operator
    float& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        return z;
    }

    const float& operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        return z;
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

// Non-member operators
Vector3 operator*(float s, const Vector3& v) {
    return v * s;
}

// Stream output operator
std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// Stream input operator
std::istream& operator>>(std::istream& is, Vector3& v) {
    is >> v.x >> v.y >> v.z;
    return is;
}

int main() {
    std::cout << "=== Complete Vector Operator Demonstrations ===" << std::endl;
    std::cout << std::endl;

    // Test vectors
    Vector3 v1(3, 4, 5);
    Vector3 v2(1, 2, 3);
    Vector3 v3;

    std::cout << "Initial vectors:" << std::endl;
    std::cout << "  v1 = " << v1 << std::endl;
    std::cout << "  v2 = " << v2 << std::endl;
    std::cout << "  v3 = " << v3 << std::endl;
    std::cout << std::endl;

    // 1. BINARY ARITHMETIC OPERATORS
    std::cout << "1. BINARY ARITHMETIC OPERATORS:" << std::endl;
    std::cout << "  v1 + v2 = " << (v1 + v2) << std::endl;
    std::cout << "  v1 - v2 = " << (v1 - v2) << std::endl;
    std::cout << "  v1 * 2  = " << (v1 * 2.0f) << std::endl;
    std::cout << "  2 * v1  = " << (2.0f * v1) << std::endl;
    std::cout << "  v1 / 2  = " << (v1 / 2.0f) << std::endl;
    std::cout << std::endl;

    // 2. COMPOUND ASSIGNMENT OPERATORS
    std::cout << "2. COMPOUND ASSIGNMENT OPERATORS:" << std::endl;

    v3 = v1;
    std::cout << "  v3 = " << v3;
    v3 += v2;
    std::cout << "  ->  v3 += v2  ->  " << v3 << std::endl;

    v3 = v1;
    std::cout << "  v3 = " << v3;
    v3 -= v2;
    std::cout << "  ->  v3 -= v2  ->  " << v3 << std::endl;

    v3 = v1;
    std::cout << "  v3 = " << v3;
    v3 *= 3.0f;
    std::cout << "  ->  v3 *= 3   ->  " << v3 << std::endl;

    v3 = v1;
    std::cout << "  v3 = " << v3;
    v3 /= 2.0f;
    std::cout << "  ->  v3 /= 2   ->  " << v3 << std::endl;
    std::cout << std::endl;

    // 3. UNARY OPERATORS
    std::cout << "3. UNARY OPERATORS:" << std::endl;
    std::cout << "  v1    = " << v1 << std::endl;
    std::cout << "  -v1   = " << (-v1) << std::endl;
    std::cout << "  +v1   = " << (+v1) << std::endl;
    std::cout << "  -(-v1) = " << (-(-v1)) << std::endl;
    std::cout << std::endl;

    // 4. COMPARISON OPERATORS
    std::cout << "4. COMPARISON OPERATORS:" << std::endl;
    Vector3 v4(3, 4, 5);
    Vector3 v5(3, 4, 6);

    std::cout << "  v1 = " << v1 << std::endl;
    std::cout << "  v4 = " << v4 << std::endl;
    std::cout << "  v5 = " << v5 << std::endl;
    std::cout << "  v1 == v4: " << (v1 == v4 ? "true" : "false") << std::endl;
    std::cout << "  v1 == v5: " << (v1 == v5 ? "true" : "false") << std::endl;
    std::cout << "  v1 != v4: " << (v1 != v4 ? "true" : "false") << std::endl;
    std::cout << "  v1 != v5: " << (v1 != v5 ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // 5. ARRAY SUBSCRIPT OPERATOR
    std::cout << "5. ARRAY SUBSCRIPT OPERATOR:" << std::endl;
    std::cout << "  v1 = " << v1 << std::endl;
    std::cout << "  v1[0] = " << v1[0] << " (x component)" << std::endl;
    std::cout << "  v1[1] = " << v1[1] << " (y component)" << std::endl;
    std::cout << "  v1[2] = " << v1[2] << " (z component)" << std::endl;

    std::cout << "  Modifying v1[1] to 10..." << std::endl;
    v1[1] = 10.0f;
    std::cout << "  v1 = " << v1 << std::endl;
    v1[1] = 4.0f;  // Restore
    std::cout << std::endl;

    // 6. CHAINING OPERATIONS
    std::cout << "6. CHAINING OPERATIONS:" << std::endl;
    v3 = v1;
    std::cout << "  v3 = " << v3 << std::endl;
    v3 += v2;
    v3 *= 2.0f;
    v3 -= Vector3(1, 1, 1);
    std::cout << "  After: v3 += v2, v3 *= 2, v3 -= (1,1,1)" << std::endl;
    std::cout << "  v3 = " << v3 << std::endl;
    std::cout << std::endl;

    // 7. EXPRESSION EVALUATION
    std::cout << "7. COMPLEX EXPRESSIONS:" << std::endl;
    Vector3 result = (v1 + v2) * 2.0f - v1 / 2.0f;
    std::cout << "  (v1 + v2) * 2 - v1 / 2 = " << result << std::endl;

    Vector3 result2 = v1 * 0.5f + v2 * 0.5f;  // Average
    std::cout << "  v1 * 0.5 + v2 * 0.5 = " << result2 << " (average)" << std::endl;
    std::cout << std::endl;

    // 8. PRACTICAL EXAMPLE: Linear Interpolation
    std::cout << "8. LINEAR INTERPOLATION (LERP):" << std::endl;
    Vector3 start(0, 0, 0);
    Vector3 end(10, 10, 10);

    std::cout << "  Start: " << start << std::endl;
    std::cout << "  End:   " << end << std::endl;

    for (float t = 0.0f; t <= 1.0f; t += 0.25f) {
        Vector3 lerp = start + (end - start) * t;
        std::cout << "  t = " << std::fixed << std::setprecision(2) << t
                  << ": " << lerp << std::endl;
    }
    std::cout << std::endl;

    // 9. PRACTICAL EXAMPLE: Physics Update
    std::cout << "9. PHYSICS UPDATE SIMULATION:" << std::endl;
    Vector3 position(0, 10, 0);
    Vector3 velocity(5, 0, 0);
    Vector3 acceleration(0, -9.8f, 0);  // Gravity
    float dt = 0.1f;  // Time step

    std::cout << "  Initial state:" << std::endl;
    std::cout << "    Position:     " << position << std::endl;
    std::cout << "    Velocity:     " << velocity << std::endl;
    std::cout << "    Acceleration: " << acceleration << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < 5; i++) {
        velocity += acceleration * dt;
        position += velocity * dt;
        std::cout << "  Step " << (i+1) << ": pos = " << position
                  << ", vel = " << velocity << std::endl;
    }

    return 0;
}

/*
 * KEY CONCEPTS:
 * =============
 *
 * 1. OPERATOR CATEGORIES:
 *    - Binary operators: Create new vector (const)
 *    - Compound assignment: Modify existing vector (return reference)
 *    - Unary operators: Single operand
 *    - Comparison: Return bool
 *    - Subscript: Array-like access
 *
 * 2. RETURN TYPES:
 *    - operator+, -, *, /: Return Vector3 by value
 *    - operator+=, -=, *=, /=: Return Vector3& (reference)
 *    - operator==, !=: Return bool
 *    - operator[]: Return float& (for modification)
 *
 * 3. CONST CORRECTNESS:
 *    - Methods that don't modify: marked const
 *    - Allows use with const references
 *    - Better optimization opportunities
 *
 * 4. NON-MEMBER FUNCTIONS:
 *    - operator*(scalar, vector): For commutativity
 *    - operator<<, >>: Stream I/O
 *
 * 5. PRACTICAL PATTERNS:
 *    - LERP: start + t * (end - start)
 *    - Physics: pos += vel * dt, vel += accel * dt
 *    - Average: (v1 + v2) / 2 or v1 * 0.5 + v2 * 0.5
 */
