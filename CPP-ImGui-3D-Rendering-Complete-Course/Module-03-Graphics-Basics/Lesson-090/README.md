# Lesson 090: Graphics Basics Project: Software Renderer

**Duration:** 3 hours

## Learning Objectives
- Understand Graphics Basics Project: Software Renderer fundamentals
- Implement practical SoftwareRenderer examples
- Master key algorithms and techniques
- Apply concepts to graphics programming
- Build reusable code components

## Introduction

This lesson focuses on Graphics Basics Project: Software Renderer, which is essential for understanding how graphics systems work and preparing you for OpenGL and 3D rendering programming.

## Core Concepts

### Mathematical Foundation

```cpp
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

// Graphics Basics Project: Software Renderer implementation
class SoftwareRendererExample {
private:
    std::vector<float> data;

public:
    SoftwareRendererExample() {
        std::cout << "Initializing Graphics Basics Project: Software Renderer" << std::endl;
    }

    void calculate() {
        std::cout << "Calculating Graphics Basics Project: Software Renderer operations" << std::endl;
        // Implementation
    }

    void display() const {
        std::cout << "Displaying results for Graphics Basics Project: Software Renderer" << std::endl;
        for (const auto& val : data) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    SoftwareRendererExample example;
    example.calculate();
    example.display();

    std::cout << "Graphics Basics Project: Software Renderer demonstration complete" << std::endl;

    return 0;
}
```

## Practical Example 1: Basic Implementation

```cpp
#include <iostream>
#include <cmath>

class PracticalImplementation {
private:
    float x, y, z;

public:
    PracticalImplementation(float a, float b, float c)
        : x(a), y(b), z(c) {}

    float magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    void normalize() {
        float mag = magnitude();
        if (mag > 0.0f) {
            x /= mag;
            y /= mag;
            z /= mag;
        }
    }

    void display() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

int main() {
    PracticalImplementation impl(3.0f, 4.0f, 5.0f);

    std::cout << "Original: ";
    impl.display();
    std::cout << "Magnitude: " << impl.magnitude() << std::endl;

    impl.normalize();
    std::cout << "Normalized: ";
    impl.display();
    std::cout << "New magnitude: " << impl.magnitude() << std::endl;

    return 0;
}
```

## Practical Example 2: Advanced Usage

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

class AdvancedImplementation {
private:
    std::vector<float> values;

public:
    void addValue(float v) {
        values.push_back(v);
    }

    void process() {
        std::cout << "Processing " << values.size() << " values" << std::endl;

        // Transform data
        std::transform(values.begin(), values.end(), values.begin(),
                      [](float v) { return std::sin(v); });
    }

    float getMax() const {
        if (values.empty()) return 0.0f;
        return *std::max_element(values.begin(), values.end());
    }

    void display() const {
        std::cout << "Values: ";
        for (float v : values) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
        std::cout << "Max: " << getMax() << std::endl;
    }
};

int main() {
    AdvancedImplementation adv;

    for (int i = 0; i < 10; ++i) {
        adv.addValue(i * 0.5f);
    }

    adv.process();
    adv.display();

    return 0;
}
```

## Practical Example 3: Graphics Application

```cpp
#include <iostream>
#include <cmath>
#include <vector>

// Graphics-focused implementation
struct Point2D {
    float x, y;

    Point2D(float a = 0, float b = 0) : x(a), y(b) {}

    float distance(const Point2D& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx*dx + dy*dy);
    }

    void display() const {
        std::cout << "(" << x << ", " << y << ")";
    }
};

class Shape {
protected:
    std::vector<Point2D> vertices;

public:
    virtual ~Shape() = default;

    void addVertex(const Point2D& p) {
        vertices.push_back(p);
    }

    virtual float calculateArea() const = 0;

    void display() const {
        std::cout << "Vertices: ";
        for (const auto& v : vertices) {
            v.display();
            std::cout << " ";
        }
        std::cout << std::endl;
    }
};

class Triangle : public Shape {
public:
    float calculateArea() const override {
        if (vertices.size() != 3) return 0.0f;

        // Using Heron's formula
        float a = vertices[0].distance(vertices[1]);
        float b = vertices[1].distance(vertices[2]);
        float c = vertices[2].distance(vertices[0]);
        float s = (a + b + c) / 2.0f;

        return std::sqrt(s * (s-a) * (s-b) * (s-c));
    }
};

int main() {
    Triangle tri;
    tri.addVertex(Point2D(0, 0));
    tri.addVertex(Point2D(3, 0));
    tri.addVertex(Point2D(0, 4));

    tri.display();
    std::cout << "Area: " << tri.calculateArea() << std::endl;

    return 0;
}
```

## Practical Example 4: Real-Time Processing

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

class RealtimeProcessor {
private:
    std::vector<float> buffer;
    size_t bufferSize;
    size_t currentIndex;

public:
    RealtimeProcessor(size_t size)
        : bufferSize(size), currentIndex(0) {
        buffer.resize(bufferSize, 0.0f);
    }

    void addSample(float sample) {
        buffer[currentIndex] = sample;
        currentIndex = (currentIndex + 1) % bufferSize;
    }

    float getAverage() const {
        float sum = 0.0f;
        for (float v : buffer) {
            sum += v;
        }
        return sum / bufferSize;
    }

    void processFrame() {
        // Simulate frame processing
        for (size_t i = 0; i < bufferSize; ++i) {
            buffer[i] *= 0.95f;  // Decay
        }
    }

    void display() const {
        std::cout << "Average: " << getAverage() << std::endl;
    }
};

int main() {
    RealtimeProcessor processor(60);

    // Simulate 60 frames
    for (int frame = 0; frame < 60; ++frame) {
        float sample = std::sin(frame * 0.1f) * 100.0f;
        processor.addSample(sample);

        if (frame % 10 == 0) {
            std::cout << "Frame " << frame << ": ";
            processor.display();
        }

        processor.processFrame();
    }

    return 0;
}
```

## Exercises

### Exercise 1: Vector Math
**Problem:** Implement a 2D vector class with basic operations.

**Solution:**
```cpp
#include <iostream>
#include <cmath>

class Vector2D {
private:
    float x, y;

public:
    Vector2D(float a = 0, float b = 0) : x(a), y(b) {}

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    float dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }

    float magnitude() const {
        return std::sqrt(x*x + y*y);
    }

    void display() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};

int main() {
    Vector2D v1(3, 4);
    Vector2D v2(1, 2);

    std::cout << "v1: "; v1.display();
    std::cout << "v2: "; v2.display();

    Vector2D v3 = v1 + v2;
    std::cout << "v1 + v2: "; v3.display();

    std::cout << "v1 magnitude: " << v1.magnitude() << std::endl;
    std::cout << "v1 dot v2: " << v1.dot(v2) << std::endl;

    return 0;
}
```

### Exercise 2: Color Representation
**Problem:** Create a color class with RGB components.

**Solution:**
```cpp
#include <iostream>
#include <algorithm>

class Color {
private:
    float r, g, b;

    float clamp(float value) const {
        return std::max(0.0f, std::min(1.0f, value));
    }

public:
    Color(float red = 0, float green = 0, float blue = 0)
        : r(clamp(red)), g(clamp(green)), b(clamp(blue)) {}

    Color blend(const Color& other, float alpha) const {
        return Color(
            r * (1 - alpha) + other.r * alpha,
            g * (1 - alpha) + other.g * alpha,
            b * (1 - alpha) + other.b * alpha
        );
    }

    Color operator*(float intensity) const {
        return Color(r * intensity, g * intensity, b * intensity);
    }

    void display() const {
        std::cout << "RGB(" << r << ", " << g << ", " << b << ")" << std::endl;
    }
};

int main() {
    Color red(1.0f, 0.0f, 0.0f);
    Color blue(0.0f, 0.0f, 1.0f);

    Color purple = red.blend(blue, 0.5f);

    std::cout << "Red: "; red.display();
    std::cout << "Blue: "; blue.display();
    std::cout << "Purple: "; purple.display();

    Color dimRed = red * 0.5f;
    std::cout << "Dim red: "; dimRed.display();

    return 0;
}
```

### Exercise 3: Matrix Operations
**Problem:** Implement a 2x2 matrix with multiplication.

**Solution:**
```cpp
#include <iostream>

class Matrix2x2 {
private:
    float m[2][2];

public:
    Matrix2x2(float m00 = 1, float m01 = 0,
             float m10 = 0, float m11 = 1) {
        m[0][0] = m00; m[0][1] = m01;
        m[1][0] = m10; m[1][1] = m11;
    }

    Matrix2x2 operator*(const Matrix2x2& other) const {
        return Matrix2x2(
            m[0][0]*other.m[0][0] + m[0][1]*other.m[1][0],
            m[0][0]*other.m[0][1] + m[0][1]*other.m[1][1],
            m[1][0]*other.m[0][0] + m[1][1]*other.m[1][0],
            m[1][0]*other.m[0][1] + m[1][1]*other.m[1][1]
        );
    }

    void display() const {
        std::cout << "[ " << m[0][0] << " " << m[0][1] << " ]" << std::endl;
        std::cout << "[ " << m[1][0] << " " << m[1][1] << " ]" << std::endl;
    }
};

int main() {
    Matrix2x2 m1(2, 0, 0, 2);  // Scale by 2
    Matrix2x2 m2(0, -1, 1, 0); // Rotate 90 degrees

    std::cout << "Matrix 1:" << std::endl;
    m1.display();

    std::cout << "\nMatrix 2:" << std::endl;
    m2.display();

    Matrix2x2 m3 = m1 * m2;
    std::cout << "\nM1 * M2:" << std::endl;
    m3.display();

    return 0;
}
```

### Exercise 4: Interpolation
**Problem:** Implement linear interpolation for animation.

**Solution:**
```cpp
#include <iostream>
#include <cmath>

float lerp(float start, float end, float t) {
    return start + (end - start) * t;
}

class Animator {
private:
    float startValue;
    float endValue;
    float duration;
    float currentTime;

public:
    Animator(float start, float end, float dur)
        : startValue(start), endValue(end),
          duration(dur), currentTime(0) {}

    void update(float deltaTime) {
        currentTime += deltaTime;
        if (currentTime > duration) {
            currentTime = duration;
        }
    }

    float getCurrentValue() const {
        float t = currentTime / duration;
        return lerp(startValue, endValue, t);
    }

    bool isComplete() const {
        return currentTime >= duration;
    }
};

int main() {
    Animator anim(0.0f, 100.0f, 2.0f);

    float deltaTime = 0.1f;
    while (!anim.isComplete()) {
        std::cout << "Value: " << anim.getCurrentValue() << std::endl;
        anim.update(deltaTime);
    }

    std::cout << "Final value: " << anim.getCurrentValue() << std::endl;

    return 0;
}
```

### Exercise 5: Bounding Box
**Problem:** Implement an axis-aligned bounding box for collision detection.

**Solution:**
```cpp
#include <iostream>
#include <algorithm>

struct Point2D {
    float x, y;
    Point2D(float a = 0, float b = 0) : x(a), y(b) {}
};

class AABB {
private:
    Point2D min, max;

public:
    AABB(const Point2D& minimum, const Point2D& maximum)
        : min(minimum), max(maximum) {}

    bool contains(const Point2D& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y;
    }

    bool intersects(const AABB& other) const {
        return !(max.x < other.min.x || min.x > other.max.x ||
                max.y < other.min.y || min.y > other.max.y);
    }

    void display() const {
        std::cout << "AABB[(" << min.x << "," << min.y << ") to ("
                  << max.x << "," << max.y << ")]" << std::endl;
    }
};

int main() {
    AABB box1(Point2D(0, 0), Point2D(10, 10));
    AABB box2(Point2D(5, 5), Point2D(15, 15));
    AABB box3(Point2D(20, 20), Point2D(30, 30));

    box1.display();
    box2.display();
    box3.display();

    std::cout << "Box1 intersects Box2: "
              << (box1.intersects(box2) ? "Yes" : "No") << std::endl;
    std::cout << "Box1 intersects Box3: "
              << (box1.intersects(box3) ? "Yes" : "No") << std::endl;

    Point2D p(5, 5);
    std::cout << "Box1 contains (5,5): "
              << (box1.contains(p) ? "Yes" : "No") << std::endl;

    return 0;
}
```

## Summary

In this lesson, you learned:
- Fundamental concepts of Graphics Basics Project: Software Renderer
- Mathematical foundations for graphics
- Practical implementation techniques
- Real-world applications
- Integration with graphics systems

## Checklist

- [ ] Understand Graphics Basics Project: Software Renderer concepts
- [ ] Can implement basic algorithms
- [ ] Know practical applications
- [ ] Completed all 5 exercises
- [ ] Ready for 3D graphics programming

## Next Lesson

Continue to build on these graphics fundamentals as we progress toward OpenGL and 3D rendering.
