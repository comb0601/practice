/*
 * Lesson 60 - Camera Systems
 * First-person and third-person camera implementation
 *
 * Compilation: g++ -std=c++11 -o output filename.cpp -lm
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>

#define PI 3.14159265358979323846f

class Vector3 {
public:
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x+v.x, y+v.y, z+v.z);
    }
    Vector3 operator-(const Vector3& v) const {
        return Vector3(x-v.x, y-v.y, z-v.z);
    }
    Vector3 operator*(float s) const {
        return Vector3(x*s, y*s, z*s);
    }

    float Dot(const Vector3& v) const {
        return x*v.x + y*v.y + z*v.z;
    }

    Vector3 Cross(const Vector3& v) const {
        return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }

    float Length() const {
        return sqrt(x*x + y*y + z*z);
    }

    Vector3 Normalized() const {
        float len = Length();
        return len > 0 ? Vector3(x/len, y/len, z/len) : Vector3();
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

class Matrix4x4 {
public:
    float m[16];

    Matrix4x4() {
        memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    void Print() const {
        std::cout << std::fixed << std::setprecision(3);
        for (int row = 0; row < 4; row++) {
            std::cout << "[ ";
            for (int col = 0; col < 4; col++)
                std::cout << std::setw(8) << m[row*4+col] << " ";
            std::cout << "]\n";
        }
    }

    static Matrix4x4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
        Vector3 zaxis = (eye - target).Normalized();
        Vector3 xaxis = up.Cross(zaxis).Normalized();
        Vector3 yaxis = zaxis.Cross(xaxis);

        Matrix4x4 result;
        result.m[0] = xaxis.x;  result.m[1] = xaxis.y;  result.m[2] = xaxis.z;  result.m[3] = -xaxis.Dot(eye);
        result.m[4] = yaxis.x;  result.m[5] = yaxis.y;  result.m[6] = yaxis.z;  result.m[7] = -yaxis.Dot(eye);
        result.m[8] = zaxis.x;  result.m[9] = zaxis.y;  result.m[10] = zaxis.z; result.m[11] = -zaxis.Dot(eye);
        result.m[12] = 0;       result.m[13] = 0;       result.m[14] = 0;       result.m[15] = 1;
        return result;
    }
};

class Camera {
public:
    Vector3 position;
    Vector3 target;
    Vector3 up;
    float yaw, pitch;

    Camera() : position(0, 0, 10), target(0, 0, 0), up(0, 1, 0), yaw(0), pitch(0) {}

    Camera(const Vector3& pos, const Vector3& tgt, const Vector3& upVec)
        : position(pos), target(tgt), up(upVec), yaw(0), pitch(0) {}

    void MoveForward(float distance) {
        Vector3 forward = (target - position).Normalized();
        position = position + forward * distance;
        target = target + forward * distance;
    }

    void MoveRight(float distance) {
        Vector3 forward = (target - position).Normalized();
        Vector3 right = forward.Cross(up).Normalized();
        position = position + right * distance;
        target = target + right * distance;
    }

    void MoveUp(float distance) {
        position.y += distance;
        target.y += distance;
    }

    void RotateYaw(float angle) {
        yaw += angle;
        UpdateTargetFromAngles();
    }

    void RotatePitch(float angle) {
        pitch += angle;
        if (pitch > PI/2 - 0.1f) pitch = PI/2 - 0.1f;
        if (pitch < -PI/2 + 0.1f) pitch = -PI/2 + 0.1f;
        UpdateTargetFromAngles();
    }

    Matrix4x4 GetViewMatrix() const {
        return Matrix4x4::LookAt(position, target, up);
    }

    void Print() const {
        std::cout << "Camera Position: "; position.Print(); std::cout << "\n";
        std::cout << "Camera Target:   "; target.Print(); std::cout << "\n";
        std::cout << "Yaw: " << (yaw * 180/PI) << "°, Pitch: " << (pitch * 180/PI) << "°\n";
    }

private:
    void UpdateTargetFromAngles() {
        float distance = (target - position).Length();
        Vector3 direction(
            cos(pitch) * sin(yaw),
            sin(pitch),
            cos(pitch) * cos(yaw)
        );
        target = position + direction.Normalized() * distance;
    }
};

int main() {
    std::cout << "=== Lesson 60: Camera Systems ===\n\n";

    Camera cam(Vector3(0, 5, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

    std::cout << "Initial camera:\n";
    cam.Print();

    std::cout << "\nView Matrix:\n";
    cam.GetViewMatrix().Print();

    std::cout << "\n--- Moving forward 5 units ---\n";
    cam.MoveForward(5);
    cam.Print();

    std::cout << "\n--- Rotating 45° yaw ---\n";
    cam.RotateYaw(PI/4);
    cam.Print();

    std::cout << "\n--- Moving right 3 units ---\n";
    cam.MoveRight(3);
    cam.Print();

    std::cout << "\nFinal View Matrix:\n";
    cam.GetViewMatrix().Print();

    std::cout << "\n=== Program Complete ===\n";
    return 0;
}
