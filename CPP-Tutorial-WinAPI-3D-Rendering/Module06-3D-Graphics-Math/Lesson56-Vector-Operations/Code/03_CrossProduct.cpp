/*
 * Lesson 56: Vector Operations
 * Example 03: Cross Product Basics
 */

#include <iostream>
#include <cmath>

class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 Cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    float Dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

int main() {
    std::cout << "=== Cross Product Demonstrations ===" << std::endl << std::endl;

    // 1. Basic cross product
    std::cout << "1. BASIC CROSS PRODUCT:" << std::endl;
    Vector3 v1(1, 0, 0);
    Vector3 v2(0, 1, 0);
    Vector3 cross = v1.Cross(v2);

    std::cout << "  v1 = "; v1.Print(); std::cout << std::endl;
    std::cout << "  v2 = "; v2.Print(); std::cout << std::endl;
    std::cout << "  v1 × v2 = "; cross.Print(); std::cout << std::endl;
    std::cout << "  (Right-handed: X × Y = Z)" << std::endl << std::endl;

    // 2. Anti-commutativity
    std::cout << "2. ANTI-COMMUTATIVE PROPERTY:" << std::endl;
    Vector3 cross1 = v1.Cross(v2);
    Vector3 cross2 = v2.Cross(v1);

    std::cout << "  v1 × v2 = "; cross1.Print(); std::cout << std::endl;
    std::cout << "  v2 × v1 = "; cross2.Print(); std::cout << std::endl;
    std::cout << "  (Opposite directions)" << std::endl << std::endl;

    // 3. Perpendicularity
    std::cout << "3. RESULT IS PERPENDICULAR:" << std::endl;
    Vector3 a(1, 2, 3);
    Vector3 b(4, 5, 6);
    Vector3 c = a.Cross(b);

    std::cout << "  a = "; a.Print(); std::cout << std::endl;
    std::cout << "  b = "; b.Print(); std::cout << std::endl;
    std::cout << "  c = a × b = "; c.Print(); std::cout << std::endl;
    std::cout << "  c · a = " << c.Dot(a) << " (should be ~0)" << std::endl;
    std::cout << "  c · b = " << c.Dot(b) << " (should be ~0)" << std::endl << std::endl;

    // 4. Parallel vectors = zero
    std::cout << "4. PARALLEL VECTORS:" << std::endl;
    Vector3 d(2, 0, 0);
    Vector3 e(4, 0, 0);
    Vector3 f = d.Cross(e);

    std::cout << "  d = "; d.Print(); std::cout << std::endl;
    std::cout << "  e = "; e.Print(); std::cout << " (parallel to d)" << std::endl;
    std::cout << "  d × e = "; f.Print();
    std::cout << " (parallel vectors → zero cross)" << std::endl << std::endl;

    // 5. Magnitude interpretation
    std::cout << "5. MAGNITUDE = AREA OF PARALLELOGRAM:" << std::endl;
    Vector3 g(3, 0, 0);
    Vector3 h(0, 4, 0);
    Vector3 crossGH = g.Cross(h);

    std::cout << "  g = "; g.Print(); std::cout << std::endl;
    std::cout << "  h = "; h.Print(); std::cout << std::endl;
    std::cout << "  |g × h| = " << crossGH.Length() << std::endl;
    std::cout << "  Area of parallelogram = 3 * 4 = 12" << std::endl;

    return 0;
}
