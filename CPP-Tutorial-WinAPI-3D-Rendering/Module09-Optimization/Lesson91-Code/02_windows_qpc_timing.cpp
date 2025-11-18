/*
 * Lesson 91 - Example 02: Windows QueryPerformanceCounter
 *
 * Demonstrates high-precision timing using Windows-specific API.
 * QueryPerformanceCounter provides the most accurate timing on Windows.
 */

#include <iostream>
#include <windows.h>
#include <vector>

class HighPrecisionTimer {
private:
    LARGE_INTEGER frequency;
    LARGE_INTEGER startTime;

public:
    HighPrecisionTimer() {
        // Get timer frequency (ticks per second)
        QueryPerformanceFrequency(&frequency);
        std::cout << "Timer frequency: " << frequency.QuadPart << " Hz\n";
        std::cout << "Timer resolution: " << (1.0 / frequency.QuadPart * 1000000000) << " ns\n\n";
    }

    void start() {
        QueryPerformanceCounter(&startTime);
    }

    double getElapsedMicroseconds() {
        LARGE_INTEGER endTime;
        QueryPerformanceCounter(&endTime);

        LARGE_INTEGER elapsed;
        elapsed.QuadPart = endTime.QuadPart - startTime.QuadPart;

        // Convert to microseconds
        return (elapsed.QuadPart * 1000000.0) / frequency.QuadPart;
    }

    double getElapsedMilliseconds() {
        return getElapsedMicroseconds() / 1000.0;
    }

    double getElapsedSeconds() {
        return getElapsedMicroseconds() / 1000000.0;
    }
};

// Test function: Fill and sum a vector
long long sumVector(int size) {
    std::vector<int> data(size);

    // Fill with data
    for (int i = 0; i < size; ++i) {
        data[i] = i;
    }

    // Sum elements
    long long sum = 0;
    for (int value : data) {
        sum += value;
    }

    return sum;
}

int main() {
    std::cout << "=== Windows QueryPerformanceCounter Example ===\n\n";

    HighPrecisionTimer timer;

    // Test different vector sizes
    int sizes[] = { 1000, 10000, 100000, 1000000 };

    for (int size : sizes) {
        timer.start();
        long long result = sumVector(size);
        double elapsed = timer.getElapsedMicroseconds();

        std::cout << "Size: " << size << "\n";
        std::cout << "  Result: " << result << "\n";
        std::cout << "  Time: " << elapsed << " us\n";
        std::cout << "  Time per element: " << (elapsed / size) << " us\n\n";
    }

    return 0;
}
