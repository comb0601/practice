/*
 * Lesson 91 - Example 11: Frame Timing for Games
 *
 * Demonstrates tracking frame times and FPS in game applications.
 * Shows moving average, min/max tracking, and FPS stability analysis.
 */

#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

class FrameTimer {
private:
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    std::vector<double> frameTimes;  // Milliseconds
    size_t maxSamples;

public:
    FrameTimer(size_t maxSamples = 60) : maxSamples(maxSamples) {
        lastFrameTime = std::chrono::high_resolution_clock::now();
        frameTimes.reserve(maxSamples);
    }

    void markFrame() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            currentTime - lastFrameTime);

        double frameTimeMs = duration.count() / 1000.0;

        frameTimes.push_back(frameTimeMs);

        // Keep only last N samples
        if (frameTimes.size() > maxSamples) {
            frameTimes.erase(frameTimes.begin());
        }

        lastFrameTime = currentTime;
    }

    double getLastFrameTime() const {
        if (frameTimes.empty()) return 0.0;
        return frameTimes.back();
    }

    double getAverageFrameTime() const {
        if (frameTimes.empty()) return 0.0;

        double sum = 0;
        for (double time : frameTimes) {
            sum += time;
        }
        return sum / frameTimes.size();
    }

    double getMinFrameTime() const {
        if (frameTimes.empty()) return 0.0;
        return *std::min_element(frameTimes.begin(), frameTimes.end());
    }

    double getMaxFrameTime() const {
        if (frameTimes.empty()) return 0.0;
        return *std::max_element(frameTimes.begin(), frameTimes.end());
    }

    double getFPS() const {
        double avgTime = getAverageFrameTime();
        if (avgTime == 0.0) return 0.0;
        return 1000.0 / avgTime;
    }

    double getStdDev() const {
        if (frameTimes.size() < 2) return 0.0;

        double mean = getAverageFrameTime();
        double variance = 0.0;

        for (double time : frameTimes) {
            double diff = time - mean;
            variance += diff * diff;
        }

        variance /= frameTimes.size();
        return std::sqrt(variance);
    }

    void printStats() const {
        double avgTime = getAverageFrameTime();
        double fps = getFPS();
        double minTime = getMinFrameTime();
        double maxTime = getMaxFrameTime();
        double stdDev = getStdDev();

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Frame Stats (last " << frameTimes.size() << " frames):\n";
        std::cout << "  Average: " << avgTime << " ms (" << fps << " FPS)\n";
        std::cout << "  Min:     " << minTime << " ms (" << (1000.0/minTime) << " FPS)\n";
        std::cout << "  Max:     " << maxTime << " ms (" << (1000.0/maxTime) << " FPS)\n";
        std::cout << "  StdDev:  " << stdDev << " ms\n";

        // Frame time budget analysis
        double targetFPS60 = 16.67;
        double targetFPS30 = 33.33;

        if (avgTime <= targetFPS60) {
            std::cout << "  Status:  ✓ Hitting 60 FPS target\n";
        } else if (avgTime <= targetFPS30) {
            std::cout << "  Status:  ~ Hitting 30 FPS, missing 60 FPS\n";
        } else {
            std::cout << "  Status:  ✗ Below 30 FPS - optimization needed!\n";
        }

        // Stability analysis
        double coefficientOfVariation = (stdDev / avgTime) * 100.0;
        std::cout << "  Stability: ";
        if (coefficientOfVariation < 5.0) {
            std::cout << "EXCELLENT (CV: " << coefficientOfVariation << "%)\n";
        } else if (coefficientOfVariation < 10.0) {
            std::cout << "GOOD (CV: " << coefficientOfVariation << "%)\n";
        } else if (coefficientOfVariation < 20.0) {
            std::cout << "MODERATE (CV: " << coefficientOfVariation << "%)\n";
        } else {
            std::cout << "POOR (CV: " << coefficientOfVariation << "%) - stuttering likely!\n";
        }
    }
};

// Simulate variable frame workload
void simulateFrame(int complexity) {
    volatile double result = 0;
    for (int i = 0; i < complexity; ++i) {
        result += std::sin(i * 0.001);
    }
}

int main() {
    std::cout << "=== Frame Timing Example ===\n\n";

    FrameTimer frameTimer(60);  // Track last 60 frames

    std::cout << "Simulating 200 frames with variable complexity...\n\n";

    // Simulate 200 frames
    for (int frame = 0; frame < 200; ++frame) {
        // Variable complexity simulating different frame workloads
        int complexity;

        if (frame < 60) {
            // First 60 frames: stable, light workload
            complexity = 100000;
        } else if (frame < 120) {
            // Next 60 frames: stable, medium workload
            complexity = 200000;
        } else if (frame < 180) {
            // Next 60 frames: heavy workload
            complexity = 400000;
        } else {
            // Last 20 frames: variable (stuttering)
            complexity = (frame % 2 == 0) ? 100000 : 500000;
        }

        simulateFrame(complexity);
        frameTimer.markFrame();

        // Print stats every 60 frames
        if ((frame + 1) % 60 == 0) {
            std::cout << "After " << (frame + 1) << " frames:\n";
            frameTimer.printStats();
            std::cout << "\n";
        }
    }

    // Final stats
    std::cout << "Final statistics:\n";
    frameTimer.printStats();

    std::cout << "\n========== KEY CONCEPTS ==========\n\n";

    std::cout << "Frame Time Budget:\n";
    std::cout << "  60 FPS = 16.67 ms per frame\n";
    std::cout << "  30 FPS = 33.33 ms per frame\n";
    std::cout << "  If your frame takes > 16.67ms, you drop below 60 FPS\n\n";

    std::cout << "Coefficient of Variation (CV):\n";
    std::cout << "  CV = (StdDev / Mean) * 100%\n";
    std::cout << "  Low CV = consistent frame times = smooth gameplay\n";
    std::cout << "  High CV = variable frame times = stuttering\n\n";

    std::cout << "Best Practices:\n";
    std::cout << "  1. Track frame times in a ring buffer\n";
    std::cout << "  2. Display average FPS (not instantaneous)\n";
    std::cout << "  3. Monitor min/max to catch frame spikes\n";
    std::cout << "  4. Check std dev for frame time consistency\n";
    std::cout << "  5. Budget time for each subsystem (render, physics, etc.)\n";

    return 0;
}
