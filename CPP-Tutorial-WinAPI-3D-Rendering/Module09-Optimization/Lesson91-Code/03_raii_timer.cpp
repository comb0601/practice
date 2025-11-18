/*
 * Lesson 91 - Example 03: RAII Scoped Timer
 *
 * Demonstrates automatic timing using RAII (Resource Acquisition Is Initialization).
 * Timer automatically reports when it goes out of scope.
 */

#include <iostream>
#include <chrono>
#include <string>
#include <vector>

class ScopedTimer {
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point startTime;
    bool reportOnDestroy;

public:
    ScopedTimer(const std::string& name, bool reportOnDestroy = true)
        : name(name), reportOnDestroy(reportOnDestroy) {
        startTime = std::chrono::high_resolution_clock::now();
        std::cout << "[" << name << "] Started...\n";
    }

    ~ScopedTimer() {
        if (reportOnDestroy) {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

            std::cout << "[" << name << "] Completed in " << duration.count() << " us";

            // Also show in milliseconds if > 1000 us
            if (duration.count() >= 1000) {
                std::cout << " (" << (duration.count() / 1000.0) << " ms)";
            }

            std::cout << "\n";
        }
    }

    // Manual time check without destroying timer
    double elapsedMicroseconds() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime);
        return duration.count();
    }
};

void initializeData() {
    ScopedTimer timer("InitializeData");

    std::vector<int> data(1000000);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = i * 2;
    }

    // Timer automatically reports when function exits
}

void processData() {
    ScopedTimer timer("ProcessData");

    {
        ScopedTimer subTimer("  Allocation");
        std::vector<double> data(5000000);
    }

    {
        ScopedTimer subTimer("  Calculation");
        volatile double sum = 0;
        for (int i = 0; i < 10000000; ++i) {
            sum += i * 0.5;
        }
    }

    // Both sub-timers and main timer report automatically
}

void quickOperation() {
    ScopedTimer timer("QuickOperation");

    int sum = 0;
    for (int i = 0; i < 1000; ++i) {
        sum += i;
    }
}

void nestedFunctions() {
    ScopedTimer timer("NestedFunctions");

    {
        ScopedTimer t1("  Level 1");
        {
            ScopedTimer t2("    Level 2");
            {
                ScopedTimer t3("      Level 3");

                volatile int sum = 0;
                for (int i = 0; i < 1000000; ++i) {
                    sum += i;
                }
            }
        }
    }

    // All timers report in reverse order (LIFO - Last In First Out)
}

int main() {
    std::cout << "=== RAII Scoped Timer Example ===\n\n";

    {
        ScopedTimer timer("Main Program");

        initializeData();
        std::cout << "\n";

        processData();
        std::cout << "\n";

        quickOperation();
        std::cout << "\n";

        nestedFunctions();
        std::cout << "\n";

        // Main timer reports here when it goes out of scope
    }

    std::cout << "\n=== Program Completed ===\n";

    return 0;
}
