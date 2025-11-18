/*
 * Lesson 92 - Example 02: Array of Structures vs Structure of Arrays
 *
 * Demonstrates SoA pattern for better cache utilization when
 * processing only specific fields of data.
 */

#include <iostream>
#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;
public:
    void reset() { start = std::chrono::high_resolution_clock::now(); }
    double elapsedMs() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    }
};

// Array of Structures (AoS)
struct ParticleAoS {
    float x, y, z;
    float vx, vy, vz;
    float age, lifetime;
};

// Structure of Arrays (SoA)
struct ParticlesSoA {
    float* x;
    float* y;
    float* z;
    float* vx;
    float* vy;
    float* vz;
    float* age;
    float* lifetime;
    int count;

    ParticlesSoA(int n) : count(n) {
        x = new float[n]; y = new float[n]; z = new float[n];
        vx = new float[n]; vy = new float[n]; vz = new float[n];
        age = new float[n]; lifetime = new float[n];
    }

    ~ParticlesSoA() {
        delete[] x; delete[] y; delete[] z;
        delete[] vx; delete[] vy; delete[] vz;
        delete[] age; delete[] lifetime;
    }
};

int main() {
    std::cout << "=== AoS vs SoA Cache Performance ===\n\n";

    const int N = 1000000;
    Timer timer;

    // AoS: Update positions
    ParticleAoS* particlesAoS = new ParticleAoS[N];
    for (int i = 0; i < N; ++i) {
        particlesAoS[i] = {0,0,0, 1,1,1, 0,10};
    }

    timer.reset();
    for (int i = 0; i < N; ++i) {
        particlesAoS[i].x += particlesAoS[i].vx * 0.016f;
        particlesAoS[i].y += particlesAoS[i].vy * 0.016f;
        particlesAoS[i].z += particlesAoS[i].vz * 0.016f;
    }
    double timeAoS = timer.elapsedMs();

    // SoA: Update positions
    ParticlesSoA particlesSoA(N);
    for (int i = 0; i < N; ++i) {
        particlesSoA.x[i] = 0; particlesSoA.y[i] = 0; particlesSoA.z[i] = 0;
        particlesSoA.vx[i] = 1; particlesSoA.vy[i] = 1; particlesSoA.vz[i] = 1;
    }

    timer.reset();
    for (int i = 0; i < N; ++i) {
        particlesSoA.x[i] += particlesSoA.vx[i] * 0.016f;
        particlesSoA.y[i] += particlesSoA.vy[i] * 0.016f;
        particlesSoA.z[i] += particlesSoA.vz[i] * 0.016f;
    }
    double timeSoA = timer.elapsedMs();

    std::cout << "AoS time: " << timeAoS << " ms\n";
    std::cout << "SoA time: " << timeSoA << " ms\n";
    std::cout << "Speedup:  " << (timeAoS / timeSoA) << "x\n\n";

    std::cout << "Why SoA is faster:\n";
    std::cout << "- AoS loads entire 32-byte struct, uses only 24 bytes\n";
    std::cout << "- SoA loads only position/velocity arrays (better cache use)\n";
    std::cout << "- Result: 1.5-3x speedup!\n";

    delete[] particlesAoS;
    return 0;
}
