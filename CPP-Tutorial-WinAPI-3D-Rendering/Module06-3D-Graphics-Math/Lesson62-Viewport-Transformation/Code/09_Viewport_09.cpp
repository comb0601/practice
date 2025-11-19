/*
 * Lesson 62 - Viewport Transformation
 * Converting NDC coordinates to screen space
 *
 * Compilation: g++ -std=c++11 -o output filename.cpp
 */

#include <iostream>
#include <iomanip>

class Vector3 {
public:
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}

    void Print() const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

class Viewport {
public:
    float x, y, width, height;
    float minDepth, maxDepth;

    Viewport(float x=0, float y=0, float w=800, float h=600,
             float minD=0, float maxD=1)
        : x(x), y(y), width(w), height(h), minDepth(minD), maxDepth(maxD) {}

    // Transform from NDC (-1 to 1) to screen coordinates
    Vector3 Transform(const Vector3& ndc) const {
        float sx = x + (ndc.x + 1.0f) * 0.5f * width;
        float sy = y + (1.0f - ndc.y) * 0.5f * height;  // Flip Y
        float sz = minDepth + ndc.z * (maxDepth - minDepth);
        return Vector3(sx, sy, sz);
    }

    void Print() const {
        std::cout << "Viewport: (" << x << ", " << y << ") "
                  << width << "x" << height << "\n";
        std::cout << "Depth: [" << minDepth << ", " << maxDepth << "]\n";
    }
};

int main() {
    std::cout << "=== Lesson 62: Viewport Transformation ===\n\n";

    Viewport vp(0, 0, 1920, 1080, 0.0f, 1.0f);
    std::cout << "Screen Viewport:\n";
    vp.Print();

    std::cout << "\nNDC to Screen Transformations:\n";

    // Center of screen (NDC origin)
    Vector3 center(0, 0, 0.5f);
    Vector3 screenCenter = vp.Transform(center);
    std::cout << "NDC ";
    center.Print();
    std::cout << " -> Screen ";
    screenCenter.Print();
    std::cout << "\n";

    // Top-left corner
    Vector3 topLeft(-1, 1, 0);
    Vector3 screenTopLeft = vp.Transform(topLeft);
    std::cout << "NDC ";
    topLeft.Print();
    std::cout << " -> Screen ";
    screenTopLeft.Print();
    std::cout << "\n";

    // Bottom-right corner
    Vector3 bottomRight(1, -1, 1);
    Vector3 screenBottomRight = vp.Transform(bottomRight);
    std::cout << "NDC ";
    bottomRight.Print();
    std::cout << " -> Screen ";
    screenBottomRight.Print();
    std::cout << "\n";

    std::cout << "\n=== Program Complete ===\n";
    return 0;
}
