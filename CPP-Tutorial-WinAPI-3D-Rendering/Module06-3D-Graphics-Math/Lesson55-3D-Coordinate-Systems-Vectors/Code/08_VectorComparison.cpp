/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 08: Vector Comparison and Floating-Point Issues
 *
 * This program demonstrates:
 * - Exact comparison with == and !=
 * - Epsilon comparison for floating-point vectors
 * - Why floating-point comparison is tricky
 * - Safe comparison methods
 *
 * Compile: g++ -o 08_VectorComparison 08_VectorComparison.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Exact comparison (dangerous with floating-point!)
    bool operator==(const Vector3& v) const {
        return (x == v.x) && (y == v.y) && (z == v.z);
    }

    bool operator!=(const Vector3& v) const {
        return !(*this == v);
    }

    // Epsilon-based comparison (recommended)
    bool Equals(const Vector3& v, float epsilon = 0.0001f) const {
        return std::abs(x - v.x) < epsilon &&
               std::abs(y - v.y) < epsilon &&
               std::abs(z - v.z) < epsilon;
    }

    // Distance-based comparison
    bool ApproximatelyEquals(const Vector3& v, float epsilon = 0.0001f) const {
        float dx = x - v.x;
        float dy = y - v.y;
        float dz = z - v.z;
        float distSq = dx * dx + dy * dy + dz * dz;
        return distSq < epsilon * epsilon;
    }

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

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalized() const {
        float len = Length();
        if (len > 0.0f) {
            return Vector3(x / len, y / len, z / len);
        }
        return Vector3(0, 0, 0);
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(10)
                  << "(" << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "=== Vector Comparison Demonstrations ===" << std::endl;
    std::cout << std::endl;

    // 1. EXACT COMPARISON - SIMPLE CASES
    std::cout << "1. EXACT COMPARISON (==):" << std::endl;

    Vector3 v1(1, 2, 3);
    Vector3 v2(1, 2, 3);
    Vector3 v3(1, 2, 4);

    std::cout << "  v1 = ";
    v1.Print();
    std::cout << std::endl;
    std::cout << "  v2 = ";
    v2.Print();
    std::cout << std::endl;
    std::cout << "  v3 = ";
    v3.Print();
    std::cout << std::endl;

    std::cout << "  v1 == v2: " << (v1 == v2 ? "true" : "false") << std::endl;
    std::cout << "  v1 == v3: " << (v1 == v3 ? "true" : "false") << std::endl;
    std::cout << "  v1 != v3: " << (v1 != v3 ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // 2. FLOATING-POINT PRECISION ISSUES
    std::cout << "2. FLOATING-POINT PRECISION ISSUES:" << std::endl;

    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b = a / 3.0f * 3.0f;  // Should equal a, but...

    std::cout << "  a       = ";
    a.Print();
    std::cout << std::endl;
    std::cout << "  b=a/3*3 = ";
    b.Print();
    std::cout << std::endl;

    std::cout << "  a == b (exact): " << (a == b ? "true" : "false") << std::endl;
    std::cout << "  WHY FALSE? Floating-point rounding errors!" << std::endl;
    std::cout << std::endl;

    // 3. EPSILON COMPARISON - SOLUTION
    std::cout << "3. EPSILON COMPARISON (Recommended):" << std::endl;

    std::cout << "  Using Equals() with epsilon = 0.0001:" << std::endl;
    std::cout << "  a.Equals(b): " << (a.Equals(b) ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // 4. MORE PRECISION EXAMPLES
    std::cout << "4. MORE FLOATING-POINT EXAMPLES:" << std::endl;

    Vector3 c(0.1f, 0.2f, 0.3f);
    Vector3 d = c * 10.0f / 10.0f;

    std::cout << "  c         = ";
    c.Print();
    std::cout << std::endl;
    std::cout << "  d=c*10/10 = ";
    d.Print();
    std::cout << std::endl;
    std::cout << "  c == d (exact):   " << (c == d ? "true" : "false") << std::endl;
    std::cout << "  c.Equals(d):      " << (c.Equals(d) ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // 5. NORMALIZATION COMPARISON
    std::cout << "5. COMPARING NORMALIZED VECTORS:" << std::endl;

    Vector3 v4(1, 0, 0);
    Vector3 v5(100, 0, 0);
    Vector3 v4n = v4.Normalized();
    Vector3 v5n = v5.Normalized();

    std::cout << "  v4 = ";
    v4.Print();
    std::cout << " -> Normalized: ";
    v4n.Print();
    std::cout << std::endl;

    std::cout << "  v5 = ";
    v5.Print();
    std::cout << " -> Normalized: ";
    v5n.Print();
    std::cout << std::endl;

    std::cout << "  v4n == v5n (exact):   " << (v4n == v5n ? "true" : "false") << std::endl;
    std::cout << "  v4n.Equals(v5n):      " << (v4n.Equals(v5n) ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // 6. DIFFERENT EPSILON VALUES
    std::cout << "6. EPSILON VALUE EFFECTS:" << std::endl;

    Vector3 e(1.0f, 2.0f, 3.0f);
    Vector3 f(1.001f, 2.001f, 3.001f);

    std::cout << "  e = ";
    e.Print();
    std::cout << std::endl;
    std::cout << "  f = ";
    f.Print();
    std::cout << std::endl;

    std::cout << "  Difference: ";
    (f - e).Print();
    std::cout << std::endl;

    std::cout << "  e.Equals(f, 0.0001):  " << (e.Equals(f, 0.0001f) ? "true" : "false") << std::endl;
    std::cout << "  e.Equals(f, 0.001):   " << (e.Equals(f, 0.001f) ? "true" : "false") << std::endl;
    std::cout << "  e.Equals(f, 0.01):    " << (e.Equals(f, 0.01f) ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // 7. COMPONENT-WISE vs DISTANCE-BASED
    std::cout << "7. COMPONENT-WISE vs DISTANCE-BASED:" << std::endl;

    Vector3 g(0, 0, 0);
    Vector3 h(0.0001f, 0.0001f, 0.0001f);

    std::cout << "  g = ";
    g.Print();
    std::cout << std::endl;
    std::cout << "  h = ";
    h.Print();
    std::cout << std::endl;

    float epsilon = 0.0001f;
    std::cout << "  Using epsilon = " << epsilon << std::endl;

    std::cout << "  Component-wise (Equals):          " << (g.Equals(h, epsilon) ? "true" : "false") << std::endl;
    std::cout << "  Distance-based (ApproximatelyEquals): " << (g.ApproximatelyEquals(h, epsilon) ? "true" : "false") << std::endl;

    float dist = (h - g).Length();
    std::cout << "  Actual distance: " << dist << std::endl;
    std::cout << "  Component max diff: " << h.x << std::endl;
    std::cout << std::endl;

    // 8. PRACTICAL: TESTING ZERO VECTOR
    std::cout << "8. TESTING FOR ZERO VECTOR:" << std::endl;

    Vector3 almostZero(0.00001f, 0.00001f, 0.00001f);
    Vector3 zero(0, 0, 0);

    std::cout << "  almostZero = ";
    almostZero.Print();
    std::cout << std::endl;

    std::cout << "  almostZero == (0,0,0): " << (almostZero == zero ? "true" : "false") << std::endl;
    std::cout << "  almostZero.Equals((0,0,0), 0.0001): " << (almostZero.Equals(zero, 0.0001f) ? "true" : "false") << std::endl;
    std::cout << "  almostZero.Length() < 0.0001: " << ((almostZero.Length() < 0.0001f) ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // 9. PRACTICAL RECOMMENDATIONS
    std::cout << "9. PRACTICAL RECOMMENDATIONS:" << std::endl;
    std::cout << "  ✓ PREFER: Epsilon-based comparison (Equals)" << std::endl;
    std::cout << "  ✓ USE: Appropriate epsilon for your application" << std::endl;
    std::cout << "  ✓ CONSIDER: Distance-based for spatial proximity" << std::endl;
    std::cout << "  ✗ AVOID: Exact comparison (==) for computed values" << std::endl;
    std::cout << "  ✗ AVOID: Too small epsilon (may fail due to precision)" << std::endl;
    std::cout << "  ✗ AVOID: Too large epsilon (may accept wrong values)" << std::endl;
    std::cout << std::endl;

    std::cout << "  Common epsilon values:" << std::endl;
    std::cout << "  - 1e-6 (0.000001): High precision" << std::endl;
    std::cout << "  - 1e-4 (0.0001):   General purpose (recommended)" << std::endl;
    std::cout << "  - 1e-3 (0.001):    Loose comparison" << std::endl;
    std::cout << "  - 1e-2 (0.01):     Very loose (gameplay)" << std::endl;

    return 0;
}

/*
 * KEY CONCEPTS:
 * =============
 *
 * 1. FLOATING-POINT PROBLEMS:
 *    - Computers cannot represent all decimal numbers exactly
 *    - Arithmetic operations accumulate errors
 *    - 0.1 + 0.2 ≠ 0.3 in floating-point!
 *
 * 2. EXACT COMPARISON:
 *    - operator== checks bit-by-bit equality
 *    - Works for integers, unreliable for floats
 *    - Only use for vectors with known exact values
 *
 * 3. EPSILON COMPARISON:
 *    - Accept values "close enough" to equal
 *    - |a - b| < ε for each component
 *    - Recommended for all floating-point comparisons
 *
 * 4. CHOOSING EPSILON:
 *    - Depends on application requirements
 *    - Too small: fails due to precision limits
 *    - Too large: accepts incorrect values
 *    - Typical: 0.0001 for graphics
 *
 * 5. COMPONENT-WISE vs DISTANCE:
 *    - Component-wise: Check each x, y, z separately
 *    - Distance-based: Check overall distance
 *    - Distance-based may be too loose for some cases
 *
 * 6. SPECIAL CASES:
 *    - Zero vector: Use Length() < epsilon
 *    - Unit vector: Use |Length() - 1| < epsilon
 *    - Parallel vectors: Compare normalized versions
 *
 * 7. BEST PRACTICES:
 *    - Always use epsilon for computed vectors
 *    - Choose epsilon based on application scale
 *    - Document epsilon values in code
 *    - Test edge cases
 */
