/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 02: Vector Constructors
 *
 * This program demonstrates different types of constructors for Vector3:
 * - Default constructor
 * - Parameterized constructor
 * - Copy constructor
 * - Single value constructor (sets all components to same value)
 *
 * Compile: g++ -o 02_VectorConstructors 02_VectorConstructors.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>

class Vector3 {
public:
    float x, y, z;

    // Default constructor - initializes to (0, 0, 0)
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {
        std::cout << "  [Default Constructor] Created: ";
        PrintInline();
    }

    // Parameterized constructor - initialize with specific x, y, z
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {
        std::cout << "  [Parameterized Constructor] Created: ";
        PrintInline();
    }

    // Copy constructor - create a copy of another vector
    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {
        std::cout << "  [Copy Constructor] Copied: ";
        PrintInline();
    }

    // Single value constructor - sets all components to the same value
    // Useful for quick initialization like Vector3(1.0f) = (1, 1, 1)
    explicit Vector3(float value) : x(value), y(value), z(value) {
        std::cout << "  [Single Value Constructor] Created: ";
        PrintInline();
    }

    // Assignment operator
    Vector3& operator=(const Vector3& other) {
        std::cout << "  [Assignment Operator] Assigning: ";
        other.PrintInline();
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    void PrintInline() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

// Helper function to demonstrate copy constructor
Vector3 CreateVector(float x, float y, float z) {
    std::cout << "Inside CreateVector function:" << std::endl;
    Vector3 temp(x, y, z);
    std::cout << "Returning from CreateVector..." << std::endl;
    return temp;  // May invoke copy constructor (or RVO)
}

int main() {
    std::cout << "=== Vector Constructor Demonstrations ===" << std::endl;
    std::cout << std::endl;

    // 1. Default Constructor
    std::cout << "1. DEFAULT CONSTRUCTOR:" << std::endl;
    Vector3 v1;
    std::cout << "   Result: ";
    v1.Print();
    std::cout << std::endl;

    // 2. Parameterized Constructor
    std::cout << "2. PARAMETERIZED CONSTRUCTOR:" << std::endl;
    Vector3 v2(3.0f, 4.0f, 5.0f);
    std::cout << "   Result: ";
    v2.Print();
    std::cout << std::endl;

    // 3. Copy Constructor
    std::cout << "3. COPY CONSTRUCTOR:" << std::endl;
    std::cout << "   Creating v3 as copy of v2..." << std::endl;
    Vector3 v3(v2);
    std::cout << "   Result: ";
    v3.Print();
    std::cout << "   Original v2: ";
    v2.Print();
    std::cout << std::endl;

    // 4. Single Value Constructor
    std::cout << "4. SINGLE VALUE CONSTRUCTOR:" << std::endl;
    Vector3 v4(7.0f);  // Sets all components to 7
    std::cout << "   Result: ";
    v4.Print();
    std::cout << std::endl;

    // 5. Assignment Operator
    std::cout << "5. ASSIGNMENT OPERATOR:" << std::endl;
    Vector3 v5;
    std::cout << "   Before assignment: ";
    v5.Print();
    std::cout << "   Assigning v2 to v5..." << std::endl;
    v5 = v2;
    std::cout << "   After assignment: ";
    v5.Print();
    std::cout << std::endl;

    // 6. Array of Vectors
    std::cout << "6. ARRAY OF VECTORS (calls default constructor):" << std::endl;
    Vector3 vectors[3];
    std::cout << "   Array created with 3 vectors:" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "   vectors[" << i << "]: ";
        vectors[i].Print();
    }
    std::cout << std::endl;

    // 7. Function Return (may use copy constructor)
    std::cout << "7. FUNCTION RETURN VALUE:" << std::endl;
    Vector3 v6 = CreateVector(1.0f, 2.0f, 3.0f);
    std::cout << "   Returned vector: ";
    v6.Print();
    std::cout << std::endl;

    // 8. Initializing with expressions
    std::cout << "8. INITIALIZATION WITH EXPRESSIONS:" << std::endl;
    float a = 2.0f;
    float b = 3.0f;
    Vector3 v7(a * 2, b + 1, a * b);
    std::cout << "   Result: ";
    v7.Print();
    std::cout << std::endl;

    // 9. Const vectors
    std::cout << "9. CONST VECTORS:" << std::endl;
    const Vector3 v8(1.0f, 0.0f, 0.0f);
    std::cout << "   Const vector: ";
    v8.Print();
    std::cout << "   Length: " << v8.Length() << std::endl;
    // Note: Can only call const methods on const objects
    std::cout << std::endl;

    // 10. Temporary objects
    std::cout << "10. TEMPORARY OBJECTS:" << std::endl;
    float len = Vector3(3.0f, 4.0f, 0.0f).Length();
    std::cout << "   Length of temporary vector (3, 4, 0): " << len << std::endl;

    return 0;
}

/*
 * EXPECTED OUTPUT (may vary slightly due to compiler optimizations):
 * ==================================================================
 *
 * === Vector Constructor Demonstrations ===
 *
 * 1. DEFAULT CONSTRUCTOR:
 *   [Default Constructor] Created: (0, 0, 0)
 *    Result: (0, 0, 0)
 *
 * 2. PARAMETERIZED CONSTRUCTOR:
 *   [Parameterized Constructor] Created: (3, 4, 5)
 *    Result: (3, 4, 5)
 *
 * 3. COPY CONSTRUCTOR:
 *    Creating v3 as copy of v2...
 *   [Copy Constructor] Copied: (3, 4, 5)
 *    Result: (3, 4, 5)
 *    Original v2: (3, 4, 5)
 *
 * 4. SINGLE VALUE CONSTRUCTOR:
 *   [Single Value Constructor] Created: (7, 7, 7)
 *    Result: (7, 7, 7)
 *
 * 5. ASSIGNMENT OPERATOR:
 *   [Default Constructor] Created: (0, 0, 0)
 *    Before assignment: (0, 0, 0)
 *    Assigning v2 to v5...
 *   [Assignment Operator] Assigning: (3, 4, 5)
 *    After assignment: (3, 4, 5)
 *
 * KEY CONCEPTS:
 * =============
 * 1. Default constructor: No parameters, sets to (0,0,0)
 * 2. Parameterized constructor: Accepts x, y, z values
 * 3. Copy constructor: Creates duplicate of existing vector
 * 4. Single value constructor: Sets all components to same value
 * 5. Assignment operator: Copies values from one vector to another
 * 6. Const correctness: Methods that don't modify should be const
 * 7. RVO (Return Value Optimization): Compiler may optimize away copies
 * 8. Explicit keyword: Prevents implicit type conversions
 */
