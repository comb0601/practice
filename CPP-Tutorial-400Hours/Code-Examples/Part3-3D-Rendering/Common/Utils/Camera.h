#pragma once
#include "../Math3D/Math3D.h"

namespace Utils {

class Camera {
public:
    // Camera properties
    Math3D::Vector3 position;
    Math3D::Vector3 front;
    Math3D::Vector3 up;
    Math3D::Vector3 right;
    Math3D::Vector3 worldUp;

    // Euler angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    // Constructor
    Camera(Math3D::Vector3 position = Math3D::Vector3(0.0f, 0.0f, 3.0f),
           Math3D::Vector3 up = Math3D::Vector3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f)
        : position(position)
        , worldUp(up)
        , yaw(yaw)
        , pitch(pitch)
        , front(Math3D::Vector3(0.0f, 0.0f, -1.0f))
        , movementSpeed(2.5f)
        , mouseSensitivity(0.1f)
        , zoom(45.0f)
    {
        UpdateCameraVectors();
    }

    // Get view matrix
    Math3D::Matrix4 GetViewMatrix() const {
        return Math3D::Matrix4::LookAt(position, position + front, up);
    }

    // Get projection matrix
    Math3D::Matrix4 GetProjectionMatrix(float aspectRatio, float nearPlane = 0.1f, float farPlane = 100.0f) const {
        return Math3D::Matrix4::Perspective(Math3D::Radians(zoom), aspectRatio, nearPlane, farPlane);
    }

    // Process keyboard input
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    void ProcessKeyboard(Movement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;

        switch (direction) {
            case FORWARD:
                position += front * velocity;
                break;
            case BACKWARD:
                position -= front * velocity;
                break;
            case LEFT:
                position -= right * velocity;
                break;
            case RIGHT:
                position += right * velocity;
                break;
            case UP:
                position += up * velocity;
                break;
            case DOWN:
                position -= up * velocity;
                break;
        }
    }

    // Process mouse movement
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Constrain pitch to prevent screen flip
        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        UpdateCameraVectors();
    }

    // Process mouse scroll
    void ProcessMouseScroll(float yoffset) {
        zoom -= yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    // Set position and look at target
    void LookAt(const Math3D::Vector3& target) {
        Math3D::Vector3 direction = (target - position).Normalized();

        // Calculate yaw and pitch from direction
        yaw = Math3D::Degrees(std::atan2(direction.z, direction.x));
        pitch = Math3D::Degrees(std::asin(direction.y));

        UpdateCameraVectors();
    }

    // Set camera position
    void SetPosition(const Math3D::Vector3& pos) {
        position = pos;
    }

    // Get frustum for culling
    Math3D::Frustum GetFrustum(float aspectRatio, float nearPlane = 0.1f, float farPlane = 100.0f) const {
        Math3D::Matrix4 viewProj = GetProjectionMatrix(aspectRatio, nearPlane, farPlane) * GetViewMatrix();
        Math3D::Frustum frustum;
        frustum.ExtractFromMatrix(viewProj);
        return frustum;
    }

private:
    // Update camera vectors from euler angles
    void UpdateCameraVectors() {
        // Calculate the new front vector
        Math3D::Vector3 newFront;
        newFront.x = std::cos(Math3D::Radians(yaw)) * std::cos(Math3D::Radians(pitch));
        newFront.y = std::sin(Math3D::Radians(pitch));
        newFront.z = std::sin(Math3D::Radians(yaw)) * std::cos(Math3D::Radians(pitch));
        front = newFront.Normalized();

        // Recalculate right and up vectors
        right = front.Cross(worldUp).Normalized();
        up = right.Cross(front).Normalized();
    }
};

} // namespace Utils
