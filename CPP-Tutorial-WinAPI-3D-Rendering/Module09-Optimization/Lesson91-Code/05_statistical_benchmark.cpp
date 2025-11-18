/*
 * Lesson 91 - Example 05: Statistical Benchmarking
 *
 * Demonstrates proper benchmarking with statistical analysis.
 * Shows mean, median, std deviation, min, max, and percentiles.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;

public:
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsedMicroseconds() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count();
    }
};

struct BenchmarkStats {
    double mean;
    double median;
    double stddev;
    double min;
    double max;
    double p95;  // 95th percentile
    double p99;  // 99th percentile
};

BenchmarkStats calculateStatistics(std::vector<double> samples) {
    BenchmarkStats stats;

    // Sort for percentiles and median
    std::sort(samples.begin(), samples.end());

    // Min and Max
    stats.min = samples.front();
    stats.max = samples.back();

    // Median
    stats.median = samples[samples.size() / 2];

    // Mean
    double sum = 0;
    for (double sample : samples) {
        sum += sample;
    }
    stats.mean = sum / samples.size();

    // Standard Deviation
    double variance = 0;
    for (double sample : samples) {
        double diff = sample - stats.mean;
        variance += diff * diff;
    }
    variance /= samples.size();
    stats.stddev = std::sqrt(variance);

    // Percentiles
    stats.p95 = samples[static_cast<size_t>(samples.size() * 0.95)];
    stats.p99 = samples[static_cast<size_t>(samples.size() * 0.99)];

    return stats;
}

void printStatistics(const std::string& name, const BenchmarkStats& stats) {
    std::cout << "\n=== " << name << " ===\n";
    std::cout << "  Mean:         " << stats.mean << " us\n";
    std::cout << "  Median:       " << stats.median << " us\n";
    std::cout << "  Std Dev:      " << stats.stddev << " us\n";
    std::cout << "  Min:          " << stats.min << " us\n";
    std::cout << "  Max:          " << stats.max << " us\n";
    std::cout << "  95th percentile: " << stats.p95 << " us\n";
    std::cout << "  99th percentile: " << stats.p99 << " us\n";
    std::cout << "  Coefficient of variation: " << (stats.stddev / stats.mean * 100) << "%\n";
}

// Function to benchmark
int processData(const std::vector<int>& data) {
    int sum = 0;
    for (int value : data) {
        sum += value * value;  // Some computation
    }
    return sum;
}

int main() {
    std::cout << "=== Statistical Benchmarking Example ===\n";

    const int DATA_SIZE = 100000;
    const int NUM_RUNS = 1000;

    // Prepare test data
    std::vector<int> data(DATA_SIZE);
    for (int i = 0; i < DATA_SIZE; ++i) {
        data[i] = i % 100;
    }

    std::vector<double> samples;
    samples.reserve(NUM_RUNS);

    Timer timer;

    std::cout << "\nRunning " << NUM_RUNS << " benchmark iterations...\n";

    // Warm up
    for (int i = 0; i < 10; ++i) {
        processData(data);
    }

    // Collect samples
    for (int i = 0; i < NUM_RUNS; ++i) {
        timer.reset();
        int result = processData(data);
        double elapsed = timer.elapsedMicroseconds();
        samples.push_back(elapsed);

        // Prevent compiler optimization
        if (result == -1) {
            std::cout << "Unexpected result\n";
        }
    }

    // Calculate and print statistics
    BenchmarkStats stats = calculateStatistics(samples);
    printStatistics("processData() Performance", stats);

    // Additional analysis
    std::cout << "\n=== Analysis ===\n";

    if (stats.stddev / stats.mean < 0.1) {
        std::cout << "  Consistency: EXCELLENT (low variance)\n";
    } else if (stats.stddev / stats.mean < 0.2) {
        std::cout << "  Consistency: GOOD\n";
    } else {
        std::cout << "  Consistency: POOR (high variance - check system load)\n";
    }

    double outlierThreshold = stats.mean + 3 * stats.stddev;
    int outlierCount = 0;
    for (double sample : samples) {
        if (sample > outlierThreshold) {
            outlierCount++;
        }
    }

    std::cout << "  Outliers (> 3σ): " << outlierCount << " ("
              << (outlierCount * 100.0 / samples.size()) << "%)\n";

    std::cout << "\nInterpretation:\n";
    std::cout << "  - Use MEDIAN for typical performance\n";
    std::cout << "  - Use P95/P99 for worst-case scenarios\n";
    std::cout << "  - Low std dev = consistent performance\n";
    std::cout << "  - High std dev = investigate system noise\n";

    return 0;
}
