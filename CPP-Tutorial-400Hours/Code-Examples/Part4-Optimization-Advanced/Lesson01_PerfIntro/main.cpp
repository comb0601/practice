/*
 * Lesson 01: Performance Optimization Introduction
 * Demonstrates basic performance measurement and optimization concepts
 */

#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <iomanip>

// Simple timer class for benchmarking
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    std::string name;
    bool stopped;

public:
    Timer(const std::string& timer_name = "Timer")
        : name(timer_name), stopped(false) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        if (!stopped) {
            stop();
        }
    }

    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
        stopped = true;
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time);
        std::cout << name << " took: " << duration.count() << " microseconds\n";
    }

    double elapsed_microseconds() {
        auto current = stopped ? end_time : std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(
            current - start_time).count();
    }
};

// Inefficient version - poor cache locality
double sum_array_inefficient(const std::vector<std::vector<int>>& matrix) {
    double sum = 0;
    if (matrix.empty() || matrix[0].empty()) return sum;

    size_t cols = matrix[0].size();
    size_t rows = matrix.size();

    // Column-major access (cache-unfriendly for row-major storage)
    for (size_t j = 0; j < cols; ++j) {
        for (size_t i = 0; i < rows; ++i) {
            sum += matrix[i][j];
        }
    }
    return sum;
}

// Efficient version - good cache locality
double sum_array_efficient(const std::vector<std::vector<int>>& matrix) {
    double sum = 0;
    // Row-major access (cache-friendly)
    for (const auto& row : matrix) {
        for (int val : row) {
            sum += val;
        }
    }
    return sum;
}

// Inefficient sorting comparison
void inefficient_sort_example(std::vector<int>& data) {
    // Bubble sort - O(n²)
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data.size() - 1; ++j) {
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
            }
        }
    }
}

// Efficient sorting
void efficient_sort_example(std::vector<int>& data) {
    // std::sort - O(n log n)
    std::sort(data.begin(), data.end());
}

// Memory allocation in loop (inefficient)
std::vector<int> allocate_in_loop(int iterations) {
    std::vector<int> result;
    for (int i = 0; i < iterations; ++i) {
        result.push_back(i);  // May cause multiple reallocations
    }
    return result;
}

// Pre-allocated memory (efficient)
std::vector<int> preallocate_memory(int iterations) {
    std::vector<int> result;
    result.reserve(iterations);  // Single allocation
    for (int i = 0; i < iterations; ++i) {
        result.push_back(i);
    }
    return result;
}

void print_header(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

void benchmark_cache_locality() {
    print_header("Cache Locality Benchmark");

    const size_t rows = 1000;
    const size_t cols = 1000;

    // Create matrix
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 1));

    double sum1, sum2;

    {
        Timer t("Inefficient (column-major)");
        sum1 = sum_array_inefficient(matrix);
    }

    {
        Timer t("Efficient (row-major)   ");
        sum2 = sum_array_efficient(matrix);
    }

    std::cout << "Both sums equal: " << (sum1 == sum2 ? "YES" : "NO") << "\n";
}

void benchmark_algorithms() {
    print_header("Algorithm Complexity Benchmark");

    const int size = 10000;

    // Test bubble sort
    std::vector<int> data1(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (int& val : data1) {
        val = dis(gen);
    }

    auto data2 = data1;  // Copy for fair comparison

    {
        Timer t("Bubble Sort (O(n²))    ");
        inefficient_sort_example(data1);
    }

    {
        Timer t("std::sort (O(n log n))");
        efficient_sort_example(data2);
    }

    std::cout << "Both sorted correctly: "
              << (data1 == data2 ? "YES" : "NO") << "\n";
}

void benchmark_memory_allocation() {
    print_header("Memory Allocation Benchmark");

    const int iterations = 1000000;

    {
        Timer t("Without reserve()");
        auto result = allocate_in_loop(iterations);
    }

    {
        Timer t("With reserve()   ");
        auto result = preallocate_memory(iterations);
    }
}

// Demonstrate branch prediction effects
long long branch_prediction_friendly(const std::vector<int>& data, int threshold) {
    long long sum = 0;
    for (int val : data) {
        if (val < threshold) {  // Predictable branch
            sum += val;
        }
    }
    return sum;
}

long long branch_prediction_unfriendly(const std::vector<int>& data, int threshold) {
    long long sum = 0;
    for (int val : data) {
        if (val < threshold) {  // Unpredictable branch
            sum += val;
        }
    }
    return sum;
}

void benchmark_branch_prediction() {
    print_header("Branch Prediction Benchmark");

    const int size = 1000000;
    std::vector<int> sorted_data(size);
    std::vector<int> random_data(size);

    std::iota(sorted_data.begin(), sorted_data.end(), 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, size - 1);

    for (int& val : random_data) {
        val = dis(gen);
    }

    int threshold = size / 2;
    long long sum;

    {
        Timer t("Sorted data (predictable)  ");
        sum = branch_prediction_friendly(sorted_data, threshold);
    }

    {
        Timer t("Random data (unpredictable)");
        sum = branch_prediction_unfriendly(random_data, threshold);
    }
}

int main() {
    std::cout << "Performance Optimization Introduction\n";
    std::cout << "======================================\n";
    std::cout << "\nThis lesson demonstrates fundamental performance concepts:\n";
    std::cout << "1. Cache locality and memory access patterns\n";
    std::cout << "2. Algorithm complexity (O(n²) vs O(n log n))\n";
    std::cout << "3. Memory allocation strategies\n";
    std::cout << "4. Branch prediction effects\n";

    benchmark_cache_locality();
    benchmark_algorithms();
    benchmark_memory_allocation();
    benchmark_branch_prediction();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Key Takeaways:\n";
    std::cout << "- Cache-friendly access patterns are much faster\n";
    std::cout << "- Choosing the right algorithm matters (O(n²) vs O(n log n))\n";
    std::cout << "- Pre-allocating memory reduces overhead\n";
    std::cout << "- Predictable branches are faster\n";
    std::cout << std::string(60, '=') << "\n";

    return 0;
}
