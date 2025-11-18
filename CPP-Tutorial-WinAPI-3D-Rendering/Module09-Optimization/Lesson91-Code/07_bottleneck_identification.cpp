/*
 * Lesson 91 - Example 07: Bottleneck Identification
 *
 * Demonstrates identifying performance bottlenecks in a simulated application.
 * Shows before/after optimization of the bottleneck.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;

public:
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsedMilliseconds() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0;
    }
};

// Simulated game components
void processInput() {
    Timer t;
    t.reset();

    // Simulate input processing (very fast)
    volatile int dummy = 0;
    for (int i = 0; i < 1000; ++i) {
        dummy += i;
    }

    std::cout << "  Input:  " << t.elapsedMilliseconds() << " ms\n";
}

void updatePhysics() {
    Timer t;
    t.reset();

    // Simulate physics (moderate)
    volatile double result = 0;
    for (int i = 0; i < 100000; ++i) {
        result += std::sin(i * 0.001);
    }

    std::cout << "  Physics: " << t.elapsedMilliseconds() << " ms\n";
}

// SLOW VERSION: Bottleneck!
void renderSceneSlow() {
    Timer t;
    t.reset();

    // Simulate rendering with MANY draw calls (BAD!)
    // Each draw call has overhead
    const int NUM_OBJECTS = 10000;

    for (int obj = 0; obj < NUM_OBJECTS; ++obj) {
        // Simulate draw call overhead
        volatile double dummy = 0;
        for (int i = 0; i < 100; ++i) {
            dummy += std::sqrt(static_cast<double>(i));
        }
    }

    std::cout << "  Render: " << t.elapsedMilliseconds() << " ms  ← BOTTLENECK!\n";
}

// FAST VERSION: Optimized!
void renderSceneFast() {
    Timer t;
    t.reset();

    // Simulate batched rendering (GOOD!)
    // Fewer draw calls by batching objects
    const int NUM_OBJECTS = 10000;
    const int BATCH_SIZE = 100;
    const int NUM_BATCHES = NUM_OBJECTS / BATCH_SIZE;

    for (int batch = 0; batch < NUM_BATCHES; ++batch) {
        // One draw call per batch instead of per object
        volatile double dummy = 0;
        for (int i = 0; i < 100; ++i) {
            dummy += std::sqrt(static_cast<double>(i));
        }
    }

    std::cout << "  Render: " << t.elapsedMilliseconds() << " ms  ✓ Optimized\n";
}

void updateUI() {
    Timer t;
    t.reset();

    // Simulate UI update (fast)
    volatile int dummy = 0;
    for (int i = 0; i < 10000; ++i) {
        dummy += i;
    }

    std::cout << "  UI:     " << t.elapsedMilliseconds() << " ms\n";
}

void runFrameSlow() {
    Timer frameTimer;
    frameTimer.reset();

    std::cout << "\n--- Frame (SLOW) ---\n";
    processInput();
    updatePhysics();
    renderSceneSlow();  // BOTTLENECK!
    updateUI();

    double frameTime = frameTimer.elapsedMilliseconds();
    std::cout << "  TOTAL:  " << frameTime << " ms\n";
    std::cout << "  FPS:    " << (1000.0 / frameTime) << "\n";
}

void runFrameFast() {
    Timer frameTimer;
    frameTimer.reset();

    std::cout << "\n--- Frame (FAST) ---\n";
    processInput();
    updatePhysics();
    renderSceneFast();  // OPTIMIZED!
    updateUI();

    double frameTime = frameTimer.elapsedMilliseconds();
    std::cout << "  TOTAL:  " << frameTime << " ms\n";
    std::cout << "  FPS:    " << (1000.0 / frameTime) << "\n";
}

int main() {
    std::cout << "=== Bottleneck Identification Example ===\n";
    std::cout << "\nThis example simulates a game loop with a rendering bottleneck.\n";

    // Profile slow version
    std::cout << "\n========== BEFORE OPTIMIZATION ==========\n";
    Timer totalTimer;
    totalTimer.reset();

    for (int frame = 0; frame < 3; ++frame) {
        runFrameSlow();
    }

    double slowTime = totalTimer.elapsedMilliseconds();
    std::cout << "\n3 frames took: " << slowTime << " ms\n";

    // Profile fast version
    std::cout << "\n\n========== AFTER OPTIMIZATION ==========\n";
    totalTimer.reset();

    for (int frame = 0; frame < 3; ++frame) {
        runFrameFast();
    }

    double fastTime = totalTimer.elapsedMilliseconds();
    std::cout << "\n3 frames took: " << fastTime << " ms\n";

    // Show improvement
    std::cout << "\n========== RESULTS ==========\n";
    std::cout << "Speedup: " << (slowTime / fastTime) << "x faster\n";
    std::cout << "Time saved: " << (slowTime - fastTime) << " ms\n";
    std::cout << "Improvement: " << ((slowTime - fastTime) / slowTime * 100) << "%\n";

    std::cout << "\n========== LESSON ==========\n";
    std::cout << "By profiling, we identified that rendering was the bottleneck.\n";
    std::cout << "It took ~90% of frame time with 10,000 draw calls.\n";
    std::cout << "By batching draw calls (100 objects per call), we reduced\n";
    std::cout << "the number of calls from 10,000 to 100, achieving "
              << (slowTime / fastTime) << "x speedup!\n";

    return 0;
}
