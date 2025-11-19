/*
 * Lesson 59 - Quaternions and Rotation
 * Advanced rotation using quaternions (avoids gimbal lock)
 *
 * Compilation: g++ -std=c++11 -o output filename.cpp -lm
 */

#include <iostream>
#include <iomanip>
#include <cmath>

#define PI 3.14159265358979323846f

class Vector3 {
public:
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
    float Length() const { return sqrt(x*x + y*y + z*z); }
    Vector3 Normalized() const {
        float len = Length();
        return len > 0 ? Vector3(x/len, y/len, z/len) : Vector3();
    }
    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

class Quaternion {
public:
    float w, x, y, z;  // w + xi + yj + zk

    Quaternion(float w=1, float x=0, float y=0, float z=0)
        : w(w), x(x), y(y), z(z) {}

    // Create from axis-angle
    static Quaternion FromAxisAngle(const Vector3& axis, float angle) {
        float halfAngle = angle / 2.0f;
        float s = sin(halfAngle);
        Vector3 normAxis = axis.Normalized();
        return Quaternion(cos(halfAngle), normAxis.x*s, normAxis.y*s, normAxis.z*s);
    }

    // Quaternion multiplication
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w*q.w - x*q.x - y*q.y - z*q.z,
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w
        );
    }

    // Magnitude
    float Length() const {
        return sqrt(w*w + x*x + y*y + z*z);
    }

    // Normalize
    Quaternion Normalized() const {
        float len = Length();
        return len > 0 ? Quaternion(w/len, x/len, y/len, z/len) : Quaternion();
    }

    // Conjugate
    Quaternion Conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }

    // Inverse
    Quaternion Inverse() const {
        float lenSq = w*w + x*x + y*y + z*z;
        Quaternion conj = Conjugate();
        return Quaternion(conj.w/lenSq, conj.x/lenSq, conj.y/lenSq, conj.z/lenSq);
    }

    // Rotate vector
    Vector3 Rotate(const Vector3& v) const {
        Quaternion p(0, v.x, v.y, v.z);
        Quaternion result = (*this) * p * Conjugate();
        return Vector3(result.x, result.y, result.z);
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(4);
        std::cout << w << " + " << x << "i + " << y << "j + " << z << "k";
    }

    // Convert to Euler angles (in radians)
    void ToEulerAngles(float& pitch, float& yaw, float& roll) const {
        // Roll (x-axis rotation)
        float sinr_cosp = 2 * (w * x + y * z);
        float cosr_cosp = 1 - 2 * (x * x + y * y);
        roll = atan2(sinr_cosp, cosr_cosp);

        // Pitch (y-axis rotation)
        float sinp = 2 * (w * y - z * x);
        if (fabs(sinp) >= 1)
            pitch = copysign(PI / 2, sinp);
        else
            pitch = asin(sinp);

        // Yaw (z-axis rotation)
        float siny_cosp = 2 * (w * z + x * y);
        float cosy_cosp = 1 - 2 * (y * y + z * z);
        yaw = atan2(siny_cosp, cosy_cosp);
    }
};

int main() {
    std::cout << "=== Lesson 59: Quaternion Operations ===\n\n";

    // Create quaternion from axis-angle
    Vector3 axis(0, 1, 0);  // Y-axis
    float angle = PI / 4;   // 45 degrees
    Quaternion q = Quaternion::FromAxisAngle(axis, angle);

    std::cout << "Quaternion (45° around Y-axis):\n";
    q.Print();
    std::cout << "\n\n";

    // Rotate a vector
    Vector3 point(1, 0, 0);
    std::cout << "Original point: "; point.Print(); std::cout << "\n";

    Vector3 rotated = q.Rotate(point);
    std::cout << "Rotated point:  "; rotated.Print(); std::cout << "\n\n";

    // Quaternion multiplication (combining rotations)
    Quaternion q2 = Quaternion::FromAxisAngle(Vector3(0,1,0), PI/6);  // 30°
    Quaternion combined = q * q2;  // 45° + 30° = 75° total
    std::cout << "Combined rotation (75° around Y):\n";
    combined.Print();
    std::cout << "\n\n";

    Vector3 final = combined.Rotate(point);
    std::cout << "Final rotated point: "; final.Print(); std::cout << "\n\n";

    // Convert back to Euler angles
    float pitch, yaw, roll;
    combined.ToEulerAngles(pitch, yaw, roll);
    std::cout << "Euler angles:\n";
    std::cout << "  Pitch: " << pitch * 180/PI << "°\n";
    std::cout << "  Yaw:   " << yaw * 180/PI << "°\n";
    std::cout << "  Roll:  " << roll * 180/PI << "°\n";

    std::cout << "\n=== Program Complete ===\n";
    return 0;
}
