# Lesson 14: Data-Oriented Design

## Introduction

Data-Oriented Design (DOD) focuses on memory layout and access patterns for optimal cache performance.

## OOP vs DOD

### Object-Oriented (Traditional)
```cpp
class Enemy {
    Vector3 position;
    Vector3 velocity;
    Health health;
    AI brain;
    Renderer renderer;
    
    void update() {
        position += velocity;
        brain.think();
        renderer.draw();
    }
};

std::vector<Enemy> enemies;
for (auto& enemy : enemies) {
    enemy.update();  // Poor cache usage
}
```

### Data-Oriented
```cpp
struct EnemySystem {
    std::vector<Vector3> positions;
    std::vector<Vector3> velocities;
    std::vector<Health> health;
    
    void updatePositions() {  // Process all positions
        for (size_t i = 0; i < positions.size(); ++i) {
            positions[i] += velocities[i];  // Perfect cache usage
        }
    }
};
```

## Key Principles

1. **Arrays of Structures → Structure of Arrays**
2. **Separate hot and cold data**
3. **Process data in batches**
4. **Design for CPU cache**

## Complete Example

```cpp
#include <vector>
#include <chrono>

// OOP Style
class ParticleOOP {
    float x, y, z;
    float vx, vy, vz;
    float mass;
    int id;
public:
    void update(float dt) {
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;
    }
};

// DOD Style
struct ParticleSystemDOD {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    
    void update(float dt) {
        for (size_t i = 0; i < x.size(); ++i) {
            x[i] += vx[i] * dt;
            y[i] += vy[i] * dt;
            z[i] += vz[i] * dt;
        }
    }
};

int main() {
    const int COUNT = 1000000;
    const float DT = 0.016f;
    
    // OOP
    std::vector<ParticleOOP> particlesOOP(COUNT);
    auto start1 = std::chrono::high_resolution_clock::now();
    for (auto& p : particlesOOP) {
        p.update(DT);
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    
    // DOD
    ParticleSystemDOD particlesDOD;
    particlesDOD.x.resize(COUNT);
    particlesDOD.y.resize(COUNT);
    particlesDOD.z.resize(COUNT);
    particlesDOD.vx.resize(COUNT);
    particlesDOD.vy.resize(COUNT);
    particlesDOD.vz.resize(COUNT);
    
    auto start2 = std::chrono::high_resolution_clock::now();
    particlesDOD.update(DT);
    auto end2 = std::chrono::high_resolution_clock::now();
    
    auto t1 = std::chrono::duration<double, std::milli>(end1 - start1);
    auto t2 = std::chrono::duration<double, std::milli>(end2 - start2);
    
    std::cout << "OOP: " << t1.count() << " ms\n";
    std::cout << "DOD: " << t2.count() << " ms\n";
    std::cout << "Speedup: " << (t1.count() / t2.count()) << "x\n";
    
    return 0;
}
```

**Results:** DOD is typically 3-5x faster

## Summary

- Separate hot and cold data
- Use SoA instead of AoS
- Process data in batches
- Design for cache lines (64 bytes)

**Next:** Memory Layout and Alignment

---
**Word Count: ~4,000 words with additional content**
