/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 01: Basic Vector3 Class
 *
 * This program demonstrates a simple Vector3 class with basic functionality.
 * We'll create a minimal but functional 3D vector class with constructors
 * and basic member variables.
 *
 * Compile: g++ -o 01_BasicVector3Class 01_BasicVector3Class.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>

// Simple Vector3 class representing a point or direction in 3D space
class Vector3 {
public:
    // Public member variables for easy access
    float x, y, z;

    // Default constructor - initializes to origin (0, 0, 0)
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {
        std::cout << "Default constructor called: ";
        Print();
    }

    // Parameterized constructor - initialize with specific values
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {
        std::cout << "Parameterized constructor called: ";
        Print();
    }

    // Calculate the magnitude (length) of the vector
    // Formula: ||v|| = sqrt(x² + y² + z²)
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Print the vector in a readable format
    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

int main() {
    std::cout << "=== Basic Vector3 Class Demo ===" << std::endl;
    std::cout << std::endl;

    // Create vectors using different constructors
    std::cout << "Creating default vector:" << std::endl;
    Vector3 v1;
    std::cout << std::endl;

    std::cout << "Creating vector at (1, 2, 3):" << std::endl;
    Vector3 v2(1.0f, 2.0f, 3.0f);
    std::cout << std::endl;

    std::cout << "Creating vector at (5, 0, 0):" << std::endl;
    Vector3 v3(5.0f, 0.0f, 0.0f);
    std::cout << std::endl;

    // Calculate and display lengths
    std::cout << "=== Vector Lengths ===" << std::endl;
    std::cout << "v1 length: " << v1.Length() << std::endl;
    std::cout << "v2 length: " << v2.Length() << std::endl;
    std::cout << "v3 length: " << v3.Length() << std::endl;
    std::cout << std::endl;

    // Create some common vectors
    std::cout << "=== Common Vectors ===" << std::endl;
    Vector3 origin(0.0f, 0.0f, 0.0f);
    std::cout << "Origin: ";
    origin.Print();

    Vector3 unitX(1.0f, 0.0f, 0.0f);
    std::cout << "Unit X-axis: ";
    unitX.Print();

    Vector3 unitY(0.0f, 1.0f, 0.0f);
    std::cout << "Unit Y-axis: ";
    unitY.Print();

    Vector3 unitZ(0.0f, 0.0f, 1.0f);
    std::cout << "Unit Z-axis: ";
    unitZ.Print();
    std::cout << std::endl;

    // Access individual components
    std::cout << "=== Component Access ===" << std::endl;
    Vector3 v4(3.5f, 7.2f, -2.1f);
    std::cout << "Vector v4: ";
    v4.Print();
    std::cout << "  x component: " << v4.x << std::endl;
    std::cout << "  y component: " << v4.y << std::endl;
    std::cout << "  z component: " << v4.z << std::endl;
    std::cout << std::endl;

    // Modify components
    std::cout << "=== Modifying Components ===" << std::endl;
    v4.x = 10.0f;
    v4.y = 20.0f;
    v4.z = 30.0f;
    std::cout << "After modification: ";
    v4.Print();

    return 0;
}

/*
 * EXPECTED OUTPUT:
 * =================
 *
 * === Basic Vector3 Class Demo ===
 *
 * Creating default vector:
 * Default constructor called: (0, 0, 0)
 *
 * Creating vector at (1, 2, 3):
 * Parameterized constructor called: (1, 2, 3)
 *
 * Creating vector at (5, 0, 0):
 * Parameterized constructor called: (5, 0, 0)
 *
 * === Vector Lengths ===
 * v1 length: 0
 * v2 length: 3.74166
 * v3 length: 5
 *
 * === Common Vectors ===
 * Default constructor called: (0, 0, 0)
 * Origin: (0, 0, 0)
 * Parameterized constructor called: (1, 0, 0)
 * Unit X-axis: (1, 0, 0)
 * Parameterized constructor called: (0, 1, 0)
 * Unit Y-axis: (0, 1, 0)
 * Parameterized constructor called: (0, 0, 1)
 * Unit Z-axis: (0, 0, 1)
 *
 * === Component Access ===
 * Parameterized constructor called: (3.5, 7.2, -2.1)
 * Vector v4: (3.5, 7.2, -2.1)
 *   x component: 3.5
 *   y component: 7.2
 *   z component: -2.1
 *
 * === Modifying Components ===
 * After modification: (10, 20, 30)
 *
 * KEY CONCEPTS:
 * =============
 * 1. Vector3 stores three float values (x, y, z)
 * 2. Default constructor initializes to origin (0, 0, 0)
 * 3. Parameterized constructor sets specific values
 * 4. Length calculated using 3D Pythagorean theorem
 * 5. Components are publicly accessible for convenience
 */
