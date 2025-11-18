/*
 * Lesson 11: Operator Overloading
 * File: 10_vector3d_class.cpp
 *
 * Comprehensive 3D Vector class with full operator overloading.
 * Useful for 3D graphics, physics simulations, and game development.
 */

#include <iostream>
#include <cmath>
#include <stdexcept>

class Vector3D {
private:
    double x, y, z;

public:
    // Constructors
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    // Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    // Setters
    void setX(double newX) { x = newX; }
    void setY(double newY) { y = newY; }
    void setZ(double newZ) { z = newZ; }

    // Vector addition
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    // Vector subtraction
    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    // Unary minus (negation)
    Vector3D operator-() const {
        return Vector3D(-x, -y, -z);
    }

    // Scalar multiplication (vector * scalar)
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division
    Vector3D operator/(double scalar) const {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero");
        }
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }

    // Dot product (vector * vector)
    double operator*(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Compound assignment operators
    Vector3D& operator+=(const Vector3D& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3D& operator-=(const Vector3D& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3D& operator/=(double scalar) {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero");
        }
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Comparison operators
    bool operator==(const Vector3D& other) const {
        const double epsilon = 1e-10;
        return fabs(x - other.x) < epsilon &&
               fabs(y - other.y) < epsilon &&
               fabs(z - other.z) < epsilon;
    }

    bool operator!=(const Vector3D& other) const {
        return !(*this == other);
    }

    // Subscript operator for component access
    double& operator[](size_t index) {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Vector3D index out of range");
    }

    const double& operator[](size_t index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Vector3D index out of range");
    }

    // Vector operations
    double magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    double magnitudeSquared() const {
        return x * x + y * y + z * z;
    }

    Vector3D normalize() const {
        double mag = magnitude();
        if (mag == 0) {
            throw std::runtime_error("Cannot normalize zero vector");
        }
        return *this / mag;
    }

    double distanceTo(const Vector3D& other) const {
        return (*this - other).magnitude();
    }

    double angleTo(const Vector3D& other) const {
        double dot = (*this) * other;
        double mags = magnitude() * other.magnitude();
        if (mags == 0) {
            throw std::runtime_error("Cannot compute angle with zero vector");
        }
        return acos(dot / mags);
    }

    // Cross product
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Project this vector onto another
    Vector3D projectOnto(const Vector3D& other) const {
        double dot = (*this) * other;
        double magSq = other.magnitudeSquared();
        if (magSq == 0) {
            throw std::runtime_error("Cannot project onto zero vector");
        }
        return other * (dot / magSq);
    }

    // Reflect this vector across a normal
    Vector3D reflect(const Vector3D& normal) const {
        return *this - normal * (2 * ((*this) * normal));
    }

    // Linear interpolation
    Vector3D lerp(const Vector3D& other, double t) const {
        return *this * (1 - t) + other * t;
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    // Stream input
    friend std::istream& operator>>(std::istream& is, Vector3D& v) {
        is >> v.x >> v.y >> v.z;
        return is;
    }
};

// Non-member scalar multiplication (scalar * vector)
Vector3D operator*(double scalar, const Vector3D& v) {
    return v * scalar;
}

// Common vector constants
namespace Vectors {
    const Vector3D Zero(0, 0, 0);
    const Vector3D UnitX(1, 0, 0);
    const Vector3D UnitY(0, 1, 0);
    const Vector3D UnitZ(0, 0, 1);
    const Vector3D One(1, 1, 1);
}

int main() {
    std::cout << "=== Vector3D Class Demo ===\n\n";

    // Create vectors
    Vector3D v1(1, 2, 3);
    Vector3D v2(4, 5, 6);
    Vector3D v3(1, 0, 0);

    std::cout << "--- Basic Vectors ---\n";
    std::cout << "v1 = " << v1 << "\n";
    std::cout << "v2 = " << v2 << "\n";
    std::cout << "v3 = " << v3 << "\n\n";

    // Arithmetic operations
    std::cout << "--- Arithmetic Operations ---\n";
    std::cout << "v1 + v2 = " << (v1 + v2) << "\n";
    std::cout << "v1 - v2 = " << (v1 - v2) << "\n";
    std::cout << "-v1 = " << -v1 << "\n\n";

    // Scalar operations
    std::cout << "--- Scalar Operations ---\n";
    std::cout << "v1 * 2 = " << (v1 * 2) << "\n";
    std::cout << "3 * v1 = " << (3 * v1) << "\n";
    std::cout << "v1 / 2 = " << (v1 / 2) << "\n\n";

    // Dot product
    std::cout << "--- Dot Product ---\n";
    std::cout << "v1 · v2 = " << (v1 * v2) << "\n";
    std::cout << "v1 · v3 = " << (v1 * v3) << "\n\n";

    // Cross product
    std::cout << "--- Cross Product ---\n";
    Vector3D cross1 = v1.cross(v2);
    std::cout << "v1 × v2 = " << cross1 << "\n";
    std::cout << "Verify perpendicular: v1 · (v1 × v2) = " << (v1 * cross1) << "\n";
    std::cout << "Verify perpendicular: v2 · (v1 × v2) = " << (v2 * cross1) << "\n\n";

    // Standard basis cross products
    std::cout << "Standard basis cross products:\n";
    std::cout << "UnitX × UnitY = " << Vectors::UnitX.cross(Vectors::UnitY) << "\n";
    std::cout << "UnitY × UnitZ = " << Vectors::UnitY.cross(Vectors::UnitZ) << "\n";
    std::cout << "UnitZ × UnitX = " << Vectors::UnitZ.cross(Vectors::UnitX) << "\n\n";

    // Magnitude and normalization
    std::cout << "--- Magnitude and Normalization ---\n";
    std::cout << "|v1| = " << v1.magnitude() << "\n";
    std::cout << "|v2| = " << v2.magnitude() << "\n";

    Vector3D v1Norm = v1.normalize();
    std::cout << "v1 normalized = " << v1Norm << "\n";
    std::cout << "|v1 normalized| = " << v1Norm.magnitude() << "\n\n";

    // Distance between vectors
    std::cout << "--- Distance ---\n";
    std::cout << "Distance from v1 to v2 = " << v1.distanceTo(v2) << "\n\n";

    // Angle between vectors
    std::cout << "--- Angle Between Vectors ---\n";
    double angle = v1.angleTo(v2);
    std::cout << "Angle between v1 and v2 = " << angle << " radians\n";
    std::cout << "                          = " << (angle * 180.0 / M_PI) << " degrees\n\n";

    // Right angle test
    Vector3D vx(1, 0, 0);
    Vector3D vy(0, 1, 0);
    double rightAngle = vx.angleTo(vy);
    std::cout << "Angle between UnitX and UnitY = " << rightAngle << " radians\n";
    std::cout << "                                = " << (rightAngle * 180.0 / M_PI) << " degrees\n\n";

    // Compound assignment
    std::cout << "--- Compound Assignment ---\n";
    Vector3D v4(10, 20, 30);
    std::cout << "v4 = " << v4 << "\n";
    v4 += Vector3D(1, 2, 3);
    std::cout << "After v4 += (1,2,3): " << v4 << "\n";
    v4 *= 2;
    std::cout << "After v4 *= 2: " << v4 << "\n";
    v4 /= 2;
    std::cout << "After v4 /= 2: " << v4 << "\n\n";

    // Subscript operator
    std::cout << "--- Subscript Operator ---\n";
    Vector3D v5(7, 8, 9);
    std::cout << "v5 = " << v5 << "\n";
    std::cout << "v5[0] = " << v5[0] << "\n";
    std::cout << "v5[1] = " << v5[1] << "\n";
    std::cout << "v5[2] = " << v5[2] << "\n";
    v5[1] = 100;
    std::cout << "After v5[1] = 100: " << v5 << "\n\n";

    // Comparison
    std::cout << "--- Comparison ---\n";
    Vector3D v6(1, 2, 3);
    Vector3D v7(1, 2, 3);
    Vector3D v8(3, 2, 1);
    std::cout << "v6 = " << v6 << "\n";
    std::cout << "v7 = " << v7 << "\n";
    std::cout << "v8 = " << v8 << "\n";
    std::cout << "v6 == v7: " << (v6 == v7 ? "true" : "false") << "\n";
    std::cout << "v6 == v8: " << (v6 == v8 ? "true" : "false") << "\n";
    std::cout << "v6 != v8: " << (v6 != v8 ? "true" : "false") << "\n\n";

    // Projection
    std::cout << "--- Projection ---\n";
    Vector3D v9(3, 4, 0);
    Vector3D v10(1, 0, 0);
    Vector3D proj = v9.projectOnto(v10);
    std::cout << "v9 = " << v9 << "\n";
    std::cout << "v10 = " << v10 << "\n";
    std::cout << "v9 projected onto v10 = " << proj << "\n\n";

    // Reflection
    std::cout << "--- Reflection ---\n";
    Vector3D incoming(1, -1, 0);
    Vector3D normal(0, 1, 0);
    Vector3D reflected = incoming.reflect(normal);
    std::cout << "Incoming vector: " << incoming << "\n";
    std::cout << "Surface normal: " << normal << "\n";
    std::cout << "Reflected vector: " << reflected << "\n\n";

    // Linear interpolation
    std::cout << "--- Linear Interpolation (Lerp) ---\n";
    Vector3D start(0, 0, 0);
    Vector3D end(10, 10, 10);
    std::cout << "Start: " << start << "\n";
    std::cout << "End: " << end << "\n";
    for (double t = 0; t <= 1.0; t += 0.25) {
        Vector3D lerped = start.lerp(end, t);
        std::cout << "t = " << t << ": " << lerped << "\n";
    }
    std::cout << "\n";

    // Physics example: Force and acceleration
    std::cout << "--- Physics Example ---\n";
    Vector3D force(10, 0, 0);      // 10N in x-direction
    double mass = 2.0;              // 2kg
    Vector3D acceleration = force / mass;
    std::cout << "Force: " << force << " N\n";
    std::cout << "Mass: " << mass << " kg\n";
    std::cout << "Acceleration: " << acceleration << " m/s²\n";
    std::cout << "|Acceleration|: " << acceleration.magnitude() << " m/s²\n\n";

    // Graphics example: Surface normal
    std::cout << "--- Graphics Example: Surface Normal ---\n";
    Vector3D p1(0, 0, 0);
    Vector3D p2(1, 0, 0);
    Vector3D p3(0, 1, 0);
    Vector3D edge1 = p2 - p1;
    Vector3D edge2 = p3 - p1;
    Vector3D surfaceNormal = edge1.cross(edge2).normalize();
    std::cout << "Triangle vertices: " << p1 << ", " << p2 << ", " << p3 << "\n";
    std::cout << "Surface normal: " << surfaceNormal << "\n";

    return 0;
}

/*
 * OUTPUT (partial):
 * =================================
 * === Vector3D Class Demo ===
 *
 * --- Basic Vectors ---
 * v1 = (1, 2, 3)
 * v2 = (4, 5, 6)
 * v3 = (1, 0, 0)
 *
 * --- Arithmetic Operations ---
 * v1 + v2 = (5, 7, 9)
 * v1 - v2 = (-3, -3, -3)
 * -v1 = (-1, -2, -3)
 *
 * --- Scalar Operations ---
 * v1 * 2 = (2, 4, 6)
 * 3 * v1 = (3, 6, 9)
 * v1 / 2 = (0.5, 1, 1.5)
 *
 * --- Dot Product ---
 * v1 · v2 = 32
 * v1 · v3 = 1
 *
 * --- Cross Product ---
 * v1 × v2 = (-3, 6, -3)
 * Verify perpendicular: v1 · (v1 × v2) = 0
 * Verify perpendicular: v2 · (v1 × v2) = 0
 *
 * Standard basis cross products:
 * UnitX × UnitY = (0, 0, 1)
 * UnitY × UnitZ = (1, 0, 0)
 * UnitZ × UnitX = (0, 1, 0)
 *
 * --- Magnitude and Normalization ---
 * |v1| = 3.74166
 * |v2| = 8.77496
 * v1 normalized = (0.267261, 0.534522, 0.801784)
 * |v1 normalized| = 1
 *
 * ... (more output follows)
 */
