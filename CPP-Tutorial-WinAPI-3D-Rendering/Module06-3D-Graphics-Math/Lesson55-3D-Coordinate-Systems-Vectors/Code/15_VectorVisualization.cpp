/*
 * Lesson 55: 3D Coordinate Systems & Vectors
 * Example 15: Vector Visualization and Debugging
 *
 * Compile: g++ -o 15_VectorVisualization 15_VectorVisualization.cpp -std=c++11 -Wall
 */

#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalized() const {
        float len = Length();
        return (len > 0) ? Vector3(x/len, y/len, z/len) : Vector3(0,0,0);
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    // Different print formats
    void Print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }

    void PrintDetailed() const {
        std::cout << "Vector3{x=" << x << ", y=" << y << ", z=" << z
                  << ", len=" << Length() << "}";
    }

    void PrintFormatted(int width = 6, int precision = 2) const {
        std::cout << std::fixed << std::setprecision(precision)
                  << "(" << std::setw(width) << x << ", "
                  << std::setw(width) << y << ", "
                  << std::setw(width) << z << ")";
    }

    std::string ToString() const {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }

    // ASCII visualization
    void VisualizeXY() const {
        std::cout << "  XY Plane Visualization:" << std::endl;
        int gridSize = 20;
        int centerX = gridSize / 2;
        int centerY = gridSize / 2;

        int pointX = centerX + (int)(x * 2);
        int pointY = centerY - (int)(y * 2);  // Flip Y for screen coords

        for (int row = 0; row < gridSize; row++) {
            std::cout << "  ";
            for (int col = 0; col < gridSize; col++) {
                if (row == centerY && col == centerX) {
                    std::cout << "+";  // Origin
                } else if (row == centerY) {
                    std::cout << "-";  // X-axis
                } else if (col == centerX) {
                    std::cout << "|";  // Y-axis
                } else if (row == pointY && col == pointX) {
                    std::cout << "*";  // Vector point
                } else {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
    }
};

// Stream operator for easy printing
std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

int main() {
    std::cout << "=== Vector Visualization and Debugging ===" << std::endl;
    std::cout << std::endl;

    // 1. Different print formats
    std::cout << "1. DIFFERENT PRINT FORMATS:" << std::endl;
    Vector3 v(3.14159f, 2.71828f, 1.41421f);

    std::cout << "  Basic:     ";
    v.Print();
    std::cout << std::endl;

    std::cout << "  Detailed:  ";
    v.PrintDetailed();
    std::cout << std::endl;

    std::cout << "  2 digits:  ";
    v.PrintFormatted(8, 2);
    std::cout << std::endl;

    std::cout << "  4 digits:  ";
    v.PrintFormatted(8, 4);
    std::cout << std::endl;

    std::cout << "  Stream:    " << v << std::endl;

    std::cout << "  ToString:  " << v.ToString() << std::endl;
    std::cout << std::endl;

    // 2. Tabular display
    std::cout << "2. TABULAR DISPLAY:" << std::endl;
    Vector3 vectors[] = {
        Vector3(1, 0, 0),
        Vector3(0, 1, 0),
        Vector3(0, 0, 1),
        Vector3(1, 1, 1),
        Vector3(3, 4, 5)
    };

    std::cout << "  " << std::setw(3) << "#"
              << " | " << std::setw(20) << "Vector"
              << " | " << std::setw(8) << "Length"
              << " | " << std::setw(20) << "Normalized" << std::endl;
    std::cout << "  " << std::string(60, '-') << std::endl;

    for (int i = 0; i < 5; i++) {
        std::cout << "  " << std::setw(3) << i << " | ";
        vectors[i].PrintFormatted(5, 2);
        std::cout << " | " << std::setw(8) << std::fixed << std::setprecision(4)
                  << vectors[i].Length() << " | ";
        vectors[i].Normalized().PrintFormatted(5, 2);
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 3. XY plane visualization
    std::cout << "3. XY PLANE VISUALIZATION:" << std::endl;
    Vector3 v1(2, 3, 0);
    std::cout << "  Vector: " << v1 << std::endl;
    v1.VisualizeXY();
    std::cout << std::endl;

    // 4. Vector comparison display
    std::cout << "4. VECTOR COMPARISON:" << std::endl;
    Vector3 a(5, 0, 0);
    Vector3 b(3, 4, 0);
    Vector3 diff = b - a;

    std::cout << "  A: " << a << " (len=" << a.Length() << ")" << std::endl;
    std::cout << "  B: " << b << " (len=" << b.Length() << ")" << std::endl;
    std::cout << "  B-A: " << diff << " (len=" << diff.Length() << ")" << std::endl;
    std::cout << std::endl;

    // 5. Debug output template
    std::cout << "5. DEBUG OUTPUT TEMPLATE:" << std::endl;
    Vector3 debugVec(10, 20, 30);
    std::cout << "  [DEBUG] Vector: " << debugVec
              << " | Length: " << debugVec.Length()
              << " | Normalized: " << debugVec.Normalized() << std::endl;
    std::cout << std::endl;

    // 6. Component breakdown
    std::cout << "6. COMPONENT BREAKDOWN:" << std::endl;
    Vector3 complex(3.5f, 7.2f, -2.1f);
    std::cout << "  Vector: " << complex << std::endl;
    std::cout << "  ├─ X: " << complex.x << std::endl;
    std::cout << "  ├─ Y: " << complex.y << std::endl;
    std::cout << "  └─ Z: " << complex.z << std::endl;
    std::cout << "  Magnitude: " << complex.Length() << std::endl;

    return 0;
}
