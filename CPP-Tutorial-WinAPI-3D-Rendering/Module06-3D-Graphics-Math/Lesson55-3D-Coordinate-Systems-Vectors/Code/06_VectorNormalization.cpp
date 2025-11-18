/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 06: Vector Normalization
 *
 * This program demonstrates:
 * - Normalizing vectors (creating unit vectors)
 * - Difference between Normalize() and Normalized()
 * - Applications of normalized vectors
 * - Direction vectors vs position vectors
 *
 * Compile: g++ -o 06_VectorNormalization 06_VectorNormalization.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float LengthSquared() const {
        return x * x + y * y + z * z;
    }

    // Normalize this vector (modifies the vector in-place)
    // After normalization, length will be 1 (unit vector)
    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
            z /= len;
        }
    }

    // Return a normalized copy (doesn't modify this vector)
    Vector3 Normalized() const {
        Vector3 result(*this);
        result.Normalize();
        return result;
    }

    // Check if this is a unit vector (length ≈ 1)
    bool IsNormalized(float epsilon = 0.0001f) const {
        float lenSq = LengthSquared();
        return std::abs(lenSq - 1.0f) < epsilon;
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
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
    std::cout << "=== Vector Normalization Demonstrations ===" << std::endl;
    std::cout << std::endl;

    // 1. BASIC NORMALIZATION
    std::cout << "1. BASIC NORMALIZATION:" << std::endl;

    Vector3 v1(3, 4, 0);
    std::cout << "  Original: ";
    v1.Print();
    std::cout << " -> Length: " << v1.Length() << std::endl;

    Vector3 v1n = v1.Normalized();
    std::cout << "  Normalized: ";
    v1n.Print();
    std::cout << " -> Length: " << v1n.Length() << std::endl;

    std::cout << "  Manual calculation:" << std::endl;
    std::cout << "    Length = sqrt(3² + 4²) = 5" << std::endl;
    std::cout << "    Normalized = (3/5, 4/5, 0/5) = (0.6, 0.8, 0)" << std::endl;
    std::cout << std::endl;

    // 2. NORMALIZE vs NORMALIZED
    std::cout << "2. NORMALIZE() vs NORMALIZED():" << std::endl;

    Vector3 v2(10, 0, 0);
    std::cout << "  v2 before Normalized(): ";
    v2.Print();
    std::cout << " -> Length: " << v2.Length() << std::endl;

    Vector3 v2n = v2.Normalized();
    std::cout << "  v2 after Normalized():  ";
    v2.Print();
    std::cout << " -> Length: " << v2.Length() << std::endl;
    std::cout << "  Returned vector:        ";
    v2n.Print();
    std::cout << " -> Length: " << v2n.Length() << std::endl;
    std::cout << "  (Original unchanged, returns new vector)" << std::endl;
    std::cout << std::endl;

    Vector3 v3(10, 0, 0);
    std::cout << "  v3 before Normalize(): ";
    v3.Print();
    std::cout << " -> Length: " << v3.Length() << std::endl;

    v3.Normalize();
    std::cout << "  v3 after Normalize():  ";
    v3.Print();
    std::cout << " -> Length: " << v3.Length() << std::endl;
    std::cout << "  (Original modified in-place)" << std::endl;
    std::cout << std::endl;

    // 3. NORMALIZING AXIS VECTORS
    std::cout << "3. NORMALIZING AXIS VECTORS:" << std::endl;

    Vector3 axes[] = {
        Vector3(1, 0, 0),
        Vector3(0, 1, 0),
        Vector3(0, 0, 1),
        Vector3(5, 0, 0),
        Vector3(0, 10, 0),
        Vector3(0, 0, 100)
    };

    for (int i = 0; i < 6; i++) {
        Vector3 normalized = axes[i].Normalized();
        std::cout << "  ";
        axes[i].Print();
        std::cout << " -> ";
        normalized.Print();
        std::cout << " (len=" << normalized.Length() << ")" << std::endl;
    }
    std::cout << "  All axis-aligned vectors normalize to unit axes!" << std::endl;
    std::cout << std::endl;

    // 4. DIRECTION VECTORS
    std::cout << "4. DIRECTION VECTORS:" << std::endl;

    Vector3 start(2, 3, 4);
    Vector3 end(10, 7, 8);

    std::cout << "  Start point: ";
    start.Print();
    std::cout << std::endl;
    std::cout << "  End point:   ";
    end.Print();
    std::cout << std::endl;

    Vector3 direction = end - start;
    std::cout << "  Direction (unnormalized): ";
    direction.Print();
    std::cout << " -> Length: " << direction.Length() << std::endl;

    Vector3 directionN = direction.Normalized();
    std::cout << "  Direction (normalized):   ";
    directionN.Print();
    std::cout << " -> Length: " << directionN.Length() << std::endl;
    std::cout << "  Normalized direction represents PURE direction (no magnitude)" << std::endl;
    std::cout << std::endl;

    // 5. CHECKING IF NORMALIZED
    std::cout << "5. CHECKING IF ALREADY NORMALIZED:" << std::endl;

    Vector3 unit(1, 0, 0);
    Vector3 notUnit(5, 0, 0);

    std::cout << "  ";
    unit.Print();
    std::cout << " -> IsNormalized: " << (unit.IsNormalized() ? "YES" : "NO") << std::endl;

    std::cout << "  ";
    notUnit.Print();
    std::cout << " -> IsNormalized: " << (notUnit.IsNormalized() ? "YES" : "NO") << std::endl;
    std::cout << std::endl;

    // 6. PRACTICAL: MOVEMENT IN DIRECTION
    std::cout << "6. PRACTICAL: MOVEMENT IN SPECIFIC DIRECTION:" << std::endl;

    Vector3 playerPos(0, 0, 0);
    Vector3 targetPos(10, 10, 0);
    float speed = 2.0f;  // units per second
    float deltaTime = 1.0f;  // 1 second

    std::cout << "  Player position: ";
    playerPos.Print();
    std::cout << std::endl;
    std::cout << "  Target position: ";
    targetPos.Print();
    std::cout << std::endl;
    std::cout << "  Speed: " << speed << " units/second" << std::endl;
    std::cout << std::endl;

    Vector3 toTarget = targetPos - playerPos;
    std::cout << "  Vector to target: ";
    toTarget.Print();
    std::cout << " (length: " << toTarget.Length() << ")" << std::endl;

    Vector3 moveDirection = toTarget.Normalized();
    std::cout << "  Move direction: ";
    moveDirection.Print();
    std::cout << " (length: " << moveDirection.Length() << ")" << std::endl;

    Vector3 velocity = moveDirection * speed;
    std::cout << "  Velocity: ";
    velocity.Print();
    std::cout << " (length: " << velocity.Length() << ")" << std::endl;

    Vector3 newPosition = playerPos + velocity * deltaTime;
    std::cout << "  New position after 1 second: ";
    newPosition.Print();
    std::cout << std::endl;
    std::cout << std::endl;

    // 7. NORMALIZING DIAGONAL VECTORS
    std::cout << "7. NORMALIZING DIAGONAL VECTORS:" << std::endl;

    Vector3 diagonals[] = {
        Vector3(1, 1, 0),
        Vector3(1, 1, 1),
        Vector3(1, 0, 1),
        Vector3(0, 1, 1)
    };

    for (int i = 0; i < 4; i++) {
        Vector3 normalized = diagonals[i].Normalized();
        std::cout << "  ";
        diagonals[i].Print();
        std::cout << " (len=" << std::setw(6) << diagonals[i].Length() << ") -> ";
        normalized.Print();
        std::cout << " (len=" << normalized.Length() << ")" << std::endl;
    }
    std::cout << std::endl;

    // 8. ZERO VECTOR WARNING
    std::cout << "8. ZERO VECTOR NORMALIZATION:" << std::endl;

    Vector3 zero(0, 0, 0);
    std::cout << "  Zero vector: ";
    zero.Print();
    std::cout << " -> Length: " << zero.Length() << std::endl;

    Vector3 zeroN = zero.Normalized();
    std::cout << "  Normalized zero: ";
    zeroN.Print();
    std::cout << " -> Length: " << zeroN.Length() << std::endl;
    std::cout << "  WARNING: Zero vector cannot be normalized (no direction!)" << std::endl;
    std::cout << "  Our implementation returns (0,0,0) to avoid division by zero" << std::endl;
    std::cout << std::endl;

    // 9. PRACTICAL: SURFACE NORMAL
    std::cout << "9. PRACTICAL: SURFACE NORMALS:" << std::endl;
    std::cout << "  A surface normal is a unit vector perpendicular to a surface" << std::endl;

    Vector3 floorNormal(0, 1, 0);
    std::cout << "  Floor normal (up):    ";
    floorNormal.Print();
    std::cout << " -> Normalized: " << (floorNormal.IsNormalized() ? "YES" : "NO") << std::endl;

    Vector3 wallNormal(1, 0, 0);
    std::cout << "  Wall normal (right):  ";
    wallNormal.Print();
    std::cout << " -> Normalized: " << (wallNormal.IsNormalized() ? "YES" : "NO") << std::endl;

    Vector3 slopeNormal(1, 1, 0);  // Not normalized
    std::cout << "  Slope (unnormalized): ";
    slopeNormal.Print();
    std::cout << " -> Normalized: " << (slopeNormal.IsNormalized() ? "YES" : "NO") << std::endl;

    slopeNormal.Normalize();
    std::cout << "  Slope (normalized):   ";
    slopeNormal.Print();
    std::cout << " -> Normalized: " << (slopeNormal.IsNormalized() ? "YES" : "NO") << std::endl;

    return 0;
}

/*
 * KEY CONCEPTS:
 * =============
 *
 * 1. NORMALIZATION:
 *    - Process of making a vector unit length (length = 1)
 *    - Formula: v_normalized = v / ||v||
 *    - Preserves direction, removes magnitude
 *
 * 2. UNIT VECTORS:
 *    - Vectors with length exactly 1
 *    - Represent pure direction
 *    - Essential for:
 *      * Direction vectors
 *      * Surface normals
 *      * Rotation axes
 *
 * 3. TWO NORMALIZATION METHODS:
 *    - Normalize(): Modifies vector in-place
 *    - Normalized(): Returns normalized copy
 *
 * 4. WHEN TO NORMALIZE:
 *    - Getting direction from point A to B
 *    - Before using vector for lighting calculations
 *    - When you need consistent movement speed
 *    - For surface normals
 *
 * 5. SPECIAL CASES:
 *    - Zero vector: Cannot be normalized (undefined direction)
 *    - Already normalized: Safe to normalize again (no change)
 *    - Very small vectors: May have precision issues
 *
 * 6. PRACTICAL APPLICATIONS:
 *    - Movement: direction = (target - position).Normalized()
 *    - Velocity: velocity = direction * speed
 *    - Normals: For lighting and collision response
 *    - Camera: Forward, up, right vectors (all normalized)
 */
