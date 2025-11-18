/*
 * Lesson 91 - Example 06: Custom Profiler Class
 *
 * Demonstrates building a custom profiling system that tracks
 * function calls, execution times, and generates a report.
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>

struct ProfileResult {
    std::string name;
    double totalMicroseconds;
    int callCount;

    double averageMicroseconds() const {
        return totalMicroseconds / callCount;
    }
};

class Profiler {
private:
    static std::vector<ProfileResult> results;

    static ProfileResult* findResult(const std::string& name) {
        for (auto& result : results) {
            if (result.name == name) {
                return &result;
            }
        }
        return nullptr;
    }

public:
    static void recordResult(const std::string& name, double microseconds) {
        ProfileResult* existing = findResult(name);

        if (existing) {
            existing->totalMicroseconds += microseconds;
            existing->callCount++;
        } else {
            results.push_back({name, microseconds, 1});
        }
    }

    static void printReport() {
        if (results.empty()) {
            std::cout << "No profiling data collected.\n";
            return;
        }

        // Sort by total time (descending)
        std::sort(results.begin(), results.end(),
            [](const ProfileResult& a, const ProfileResult& b) {
                return a.totalMicroseconds > b.totalMicroseconds;
            });

        // Calculate total time
        double totalTime = 0;
        for (const auto& result : results) {
            totalTime += result.totalMicroseconds;
        }

        std::cout << "\n";
        std::cout << "===========================================================================================================\n";
        std::cout << "                                    PROFILING REPORT                                                       \n";
        std::cout << "===========================================================================================================\n";
        std::cout << std::left << std::setw(30) << "Function"
                  << std::right << std::setw(12) << "Calls"
                  << std::setw(15) << "Total (us)"
                  << std::setw(15) << "Average (us)"
                  << std::setw(15) << "Total (ms)"
                  << std::setw(12) << "% of Total\n";
        std::cout << "-----------------------------------------------------------------------------------------------------------\n";

        for (const auto& result : results) {
            double percent = (result.totalMicroseconds / totalTime) * 100.0;

            std::cout << std::left << std::setw(30) << result.name
                      << std::right << std::setw(12) << result.callCount
                      << std::setw(15) << std::fixed << std::setprecision(2) << result.totalMicroseconds
                      << std::setw(15) << std::fixed << std::setprecision(2) << result.averageMicroseconds()
                      << std::setw(15) << std::fixed << std::setprecision(2) << (result.totalMicroseconds / 1000.0)
                      << std::setw(11) << std::fixed << std::setprecision(1) << percent << "%\n";
        }

        std::cout << "-----------------------------------------------------------------------------------------------------------\n";
        std::cout << std::left << std::setw(30) << "TOTAL"
                  << std::right << std::setw(12) << ""
                  << std::setw(15) << std::fixed << std::setprecision(2) << totalTime
                  << std::setw(15) << ""
                  << std::setw(15) << std::fixed << std::setprecision(2) << (totalTime / 1000.0)
                  << std::setw(12) << "100.0%\n";
        std::cout << "===========================================================================================================\n\n";
    }

    static void clear() {
        results.clear();
    }
};

// Static member initialization
std::vector<ProfileResult> Profiler::results;

// RAII profiling helper
class ScopedProfiler {
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point startTime;

public:
    ScopedProfiler(const std::string& name) : name(name) {
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~ScopedProfiler() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        Profiler::recordResult(name, duration.count());
    }
};

// Define PROFILE macro for easy profiling
#define PROFILE_FUNCTION() ScopedProfiler profiler_##__LINE__(__FUNCTION__)
#define PROFILE_SCOPE(name) ScopedProfiler profiler_##__LINE__(name)

// Test functions
void fastFunction() {
    PROFILE_FUNCTION();

    volatile int sum = 0;
    for (int i = 0; i < 1000; ++i) {
        sum += i;
    }
}

void mediumFunction() {
    PROFILE_FUNCTION();

    volatile int sum = 0;
    for (int i = 0; i < 100000; ++i) {
        sum += i;
    }
}

void slowFunction() {
    PROFILE_FUNCTION();

    volatile int sum = 0;
    for (int i = 0; i < 10000000; ++i) {
        sum += i;
    }
}

void compositeFunction() {
    PROFILE_FUNCTION();

    {
        PROFILE_SCOPE("compositeFunction::Part1");
        fastFunction();
        fastFunction();
    }

    {
        PROFILE_SCOPE("compositeFunction::Part2");
        mediumFunction();
    }
}

void gameLoop() {
    PROFILE_FUNCTION();

    {
        PROFILE_SCOPE("Input");
        fastFunction();
    }

    {
        PROFILE_SCOPE("Update");
        mediumFunction();
    }

    {
        PROFILE_SCOPE("Render");
        slowFunction();
    }
}

int main() {
    std::cout << "=== Custom Profiler Example ===\n";
    std::cout << "Running profiled functions...\n";

    // Simulate game loop
    for (int frame = 0; frame < 10; ++frame) {
        gameLoop();
    }

    // Call other functions
    for (int i = 0; i < 100; ++i) {
        fastFunction();
    }

    for (int i = 0; i < 50; ++i) {
        mediumFunction();
    }

    for (int i = 0; i < 5; ++i) {
        slowFunction();
    }

    compositeFunction();

    // Print profiling report
    Profiler::printReport();

    std::cout << "Analysis:\n";
    std::cout << "  - Functions are sorted by total time\n";
    std::cout << "  - Focus optimization on functions with high % of total\n";
    std::cout << "  - Check if high call counts are necessary\n";

    return 0;
}
