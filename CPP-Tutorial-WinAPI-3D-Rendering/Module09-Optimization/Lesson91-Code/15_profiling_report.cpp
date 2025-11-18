/*
 * Lesson 91 - Example 15: Complete Profiling Report Generator
 *
 * Demonstrates a complete profiling system with hierarchical timing,
 * call graphs, and formatted reports. This is a production-ready
 * profiling tool for real applications.
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <map>

struct ProfileEntry {
    std::string name;
    std::string parent;
    double totalMicroseconds;
    int callCount;
    int depth;

    double averageMicroseconds() const {
        return callCount > 0 ? totalMicroseconds / callCount : 0.0;
    }
};

class ProfilerSystem {
private:
    static std::map<std::string, ProfileEntry> entries;
    static std::vector<std::string> callStack;

public:
    static void recordEntry(const std::string& name, double microseconds) {
        std::string parent = callStack.empty() ? "" : callStack.back();
        int depth = callStack.size();

        auto it = entries.find(name);
        if (it != entries.end()) {
            it->second.totalMicroseconds += microseconds;
            it->second.callCount++;
        } else {
            entries[name] = {name, parent, microseconds, 1, depth};
        }
    }

    static void pushCall(const std::string& name) {
        callStack.push_back(name);
    }

    static void popCall() {
        if (!callStack.empty()) {
            callStack.pop_back();
        }
    }

    static void generateReport() {
        if (entries.empty()) {
            std::cout << "No profiling data.\n";
            return;
        }

        // Convert to vector for sorting
        std::vector<ProfileEntry> sortedEntries;
        for (const auto& pair : entries) {
            sortedEntries.push_back(pair.second);
        }

        // Sort by total time
        std::sort(sortedEntries.begin(), sortedEntries.end(),
            [](const ProfileEntry& a, const ProfileEntry& b) {
                return a.totalMicroseconds > b.totalMicroseconds;
            });

        // Calculate total time
        double totalTime = 0;
        for (const auto& entry : sortedEntries) {
            if (entry.depth == 0) {  // Only count root-level functions
                totalTime += entry.totalMicroseconds;
            }
        }

        // Print header
        std::cout << "\n";
        std::cout << "========================================================================================================\n";
        std::cout << "                                     PROFILING REPORT                                                   \n";
        std::cout << "========================================================================================================\n\n";

        // Print flat profile
        std::cout << "FLAT PROFILE (sorted by total time):\n";
        std::cout << "--------------------------------------------------------------------------------------------------------\n";
        std::cout << std::left << std::setw(35) << "Function"
                  << std::right << std::setw(10) << "Calls"
                  << std::setw(15) << "Total (ms)"
                  << std::setw(15) << "Avg (us)"
                  << std::setw(12) << "% of Total"
                  << "\n";
        std::cout << "--------------------------------------------------------------------------------------------------------\n";

        for (const auto& entry : sortedEntries) {
            double percent = totalTime > 0 ? (entry.totalMicroseconds / totalTime) * 100.0 : 0.0;

            std::cout << std::left << std::setw(35) << entry.name
                      << std::right << std::setw(10) << entry.callCount
                      << std::setw(15) << std::fixed << std::setprecision(3) << (entry.totalMicroseconds / 1000.0)
                      << std::setw(15) << std::fixed << std::setprecision(2) << entry.averageMicroseconds()
                      << std::setw(11) << std::fixed << std::setprecision(1) << percent << "%"
                      << "\n";
        }

        // Print hierarchical profile
        std::cout << "\n\nHIERARCHICAL PROFILE (call tree):\n";
        std::cout << "--------------------------------------------------------------------------------------------------------\n";

        std::sort(sortedEntries.begin(), sortedEntries.end(),
            [](const ProfileEntry& a, const ProfileEntry& b) {
                if (a.depth != b.depth) return a.depth < b.depth;
                return a.totalMicroseconds > b.totalMicroseconds;
            });

        for (const auto& entry : sortedEntries) {
            // Indentation based on depth
            std::string indent(entry.depth * 2, ' ');

            std::cout << indent << entry.name
                      << " (" << (entry.totalMicroseconds / 1000.0) << " ms, "
                      << entry.callCount << " calls)\n";
        }

        // Print summary
        std::cout << "\n\nSUMMARY:\n";
        std::cout << "--------------------------------------------------------------------------------------------------------\n";
        std::cout << "Total profiled time: " << (totalTime / 1000.0) << " ms\n";
        std::cout << "Total function calls: ";
        int totalCalls = 0;
        for (const auto& entry : sortedEntries) {
            totalCalls += entry.callCount;
        }
        std::cout << totalCalls << "\n";
        std::cout << "Unique functions: " << sortedEntries.size() << "\n";

        // Find hotspots
        std::cout << "\nTOP 3 HOTSPOTS:\n";
        for (size_t i = 0; i < std::min(size_t(3), sortedEntries.size()); ++i) {
            const auto& entry = sortedEntries[i];
            double percent = totalTime > 0 ? (entry.totalMicroseconds / totalTime) * 100.0 : 0.0;
            std::cout << (i + 1) << ". " << entry.name
                      << " - " << percent << "% of total time\n";
        }

        std::cout << "========================================================================================================\n\n";
    }

    static void clear() {
        entries.clear();
        callStack.clear();
    }
};

std::map<std::string, ProfileEntry> ProfilerSystem::entries;
std::vector<std::string> ProfilerSystem::callStack;

class ScopedProfile {
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point startTime;

public:
    ScopedProfile(const std::string& name) : name(name) {
        ProfilerSystem::pushCall(name);
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~ScopedProfile() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        ProfilerSystem::recordEntry(name, duration.count());
        ProfilerSystem::popCall();
    }
};

#define PROFILE_SCOPE(name) ScopedProfile profile_##__LINE__(name)
#define PROFILE_FUNCTION() ScopedProfile profile_##__LINE__(__FUNCTION__)

// ===== Sample Application =====

void renderMesh() {
    PROFILE_FUNCTION();

    volatile int sum = 0;
    for (int i = 0; i < 500000; ++i) {
        sum += i;
    }
}

void renderTerrain() {
    PROFILE_FUNCTION();

    volatile int sum = 0;
    for (int i = 0; i < 1000000; ++i) {
        sum += i;
    }
}

void renderSky() {
    PROFILE_FUNCTION();

    volatile int sum = 0;
    for (int i = 0; i < 200000; ++i) {
        sum += i;
    }
}

void renderScene() {
    PROFILE_FUNCTION();

    {
        PROFILE_SCOPE("RenderScene::Meshes");
        for (int i = 0; i < 100; ++i) {
            renderMesh();
        }
    }

    renderTerrain();
    renderSky();
}

void updatePhysics() {
    PROFILE_FUNCTION();

    volatile double result = 0;
    for (int i = 0; i < 2000000; ++i) {
        result += i * 0.5;
    }
}

void updateAI() {
    PROFILE_FUNCTION();

    volatile int sum = 0;
    for (int i = 0; i < 500000; ++i) {
        sum += i * i;
    }
}

void gameLoop() {
    PROFILE_FUNCTION();

    {
        PROFILE_SCOPE("GameLoop::Update");
        updatePhysics();
        updateAI();
    }

    {
        PROFILE_SCOPE("GameLoop::Render");
        renderScene();
    }
}

int main() {
    std::cout << "=== Complete Profiling Report Example ===\n";
    std::cout << "Running profiled game loop for 10 frames...\n";

    // Run game loop
    for (int frame = 0; frame < 10; ++frame) {
        gameLoop();
    }

    // Generate and print report
    ProfilerSystem::generateReport();

    std::cout << "This profiling system provides:\n";
    std::cout << "  1. Flat profile - See all functions sorted by time\n";
    std::cout << "  2. Hierarchical profile - See call tree structure\n";
    std::cout << "  3. Statistical data - Calls, averages, percentages\n";
    std::cout << "  4. Hotspot identification - Top time consumers\n\n";

    std::cout << "Use this to:\n";
    std::cout << "  - Find bottlenecks in your application\n";
    std::cout << "  - Verify optimizations actually help\n";
    std::cout << "  - Make data-driven optimization decisions\n";

    return 0;
}
