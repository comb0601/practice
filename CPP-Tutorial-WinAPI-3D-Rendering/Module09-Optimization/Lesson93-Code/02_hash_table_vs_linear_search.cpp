/*
 * Lesson 93 - Example 02: Hash Table vs Linear Search
 *
 * Demonstrates the power of O(1) hash table lookups vs O(n) linear search.
 * Shows when and why to use unordered_set/unordered_map.
 */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <random>

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

// O(n) - Linear search
bool linearSearch(const std::vector<int>& vec, int value) {
    for (int v : vec) {
        if (v == value) return true;
    }
    return false;
}

// O(1) average - Hash table lookup
bool hashLookup(const std::unordered_set<int>& set, int value) {
    return set.find(value) != set.end();
}

int main() {
    std::cout << "=== Hash Table vs Linear Search ===\n\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000000);

    // Test with different data sizes
    std::vector<int> sizes = {1000, 10000, 100000};

    for (int size : sizes) {
        std::cout << "Data size: " << size << " elements\n";

        // Create test data
        std::vector<int> vec;
        std::unordered_set<int> set;

        for (int i = 0; i < size; ++i) {
            int val = dis(gen);
            vec.push_back(val);
            set.insert(val);
        }

        // Perform 10,000 lookups
        const int LOOKUPS = 10000;
        std::vector<int> lookupValues(LOOKUPS);
        for (int& val : lookupValues) {
            val = dis(gen);
        }

        Timer timer;

        // Test linear search
        timer.reset();
        int foundLinear = 0;
        for (int val : lookupValues) {
            if (linearSearch(vec, val)) {
                ++foundLinear;
            }
        }
        double timeLinear = timer.elapsedMs();

        // Test hash table
        timer.reset();
        int foundHash = 0;
        for (int val : lookupValues) {
            if (hashLookup(set, val)) {
                ++foundHash;
            }
        }
        double timeHash = timer.elapsedMs();

        std::cout << "  Linear search: " << timeLinear << " ms (" << foundLinear << " found)\n";
        std::cout << "  Hash lookup:   " << timeHash << " ms (" << foundHash << " found)\n";
        std::cout << "  Speedup:       " << (timeLinear / timeHash) << "x\n\n";
    }

    std::cout << "========== ANALYSIS ==========\n\n";
    std::cout << "Time Complexity:\n";
    std::cout << "  Linear Search:  O(n) per lookup\n";
    std::cout << "  Hash Table:     O(1) per lookup (average)\n\n";

    std::cout << "For k lookups on n elements:\n";
    std::cout << "  Linear: k * n operations\n";
    std::cout << "  Hash:   k operations\n\n";

    std::cout << "Example (n=100,000, k=10,000):\n";
    std::cout << "  Linear: 1,000,000,000 operations (~1 second)\n";
    std::cout << "  Hash:   10,000 operations (~0.01 seconds)\n";
    std::cout << "  Speedup: 100x!\n\n";

    std::cout << "WHEN TO USE HASH TABLES:\n";
    std::cout << "  ✓ Frequent lookups (> 10 times)\n";
    std::cout << "  ✓ Large datasets (> 100 elements)\n";
    std::cout << "  ✓ Need O(1) access\n";
    std::cout << "  ✓ Duplicate detection\n";
    std::cout << "  ✓ Counting frequencies\n\n";

    std::cout << "WHEN TO USE LINEAR SEARCH:\n";
    std::cout << "  ✓ Small datasets (< 100 elements)\n";
    std::cout << "  ✓ Single lookup\n";
    std::cout << "  ✓ Memory constrained\n";

    return 0;
}
