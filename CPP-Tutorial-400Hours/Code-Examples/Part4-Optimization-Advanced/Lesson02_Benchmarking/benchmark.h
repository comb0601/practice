/*
 * Complete Benchmarking Framework
 * Professional-grade benchmarking utilities
 */

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <map>
#include <cmath>

namespace perf {

// Statistics for benchmark runs
struct BenchmarkStats {
    double min_ns;
    double max_ns;
    double mean_ns;
    double median_ns;
    double stddev_ns;
    size_t iterations;

    void print(const std::string& name) const {
        std::cout << "\nBenchmark: " << name << "\n";
        std::cout << "  Iterations: " << iterations << "\n";
        std::cout << "  Min:        " << std::fixed << std::setprecision(2) << min_ns << " ns\n";
        std::cout << "  Max:        " << max_ns << " ns\n";
        std::cout << "  Mean:       " << mean_ns << " ns\n";
        std::cout << "  Median:     " << median_ns << " ns\n";
        std::cout << "  Std Dev:    " << stddev_ns << " ns\n";
        std::cout << "  Mean (ms):  " << mean_ns / 1e6 << " ms\n";
    }
};

// High-resolution timer
class Timer {
private:
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;

    time_point start_;
    time_point end_;
    bool stopped_;

public:
    Timer() : stopped_(false) {
        start();
    }

    void start() {
        start_ = clock::now();
        stopped_ = false;
    }

    void stop() {
        end_ = clock::now();
        stopped_ = true;
    }

    double elapsed_ns() const {
        auto end = stopped_ ? end_ : clock::now();
        return std::chrono::duration<double, std::nano>(end - start_).count();
    }

    double elapsed_us() const { return elapsed_ns() / 1000.0; }
    double elapsed_ms() const { return elapsed_ns() / 1000000.0; }
    double elapsed_s() const { return elapsed_ns() / 1000000000.0; }
};

// Benchmark runner
class Benchmark {
private:
    std::string name_;
    size_t iterations_;
    size_t warmup_iterations_;
    std::vector<double> results_ns_;

public:
    Benchmark(const std::string& name, size_t iterations = 100, size_t warmup = 10)
        : name_(name), iterations_(iterations), warmup_iterations_(warmup) {
        results_ns_.reserve(iterations);
    }

    template<typename Func>
    BenchmarkStats run(Func&& func) {
        results_ns_.clear();

        // Warmup runs
        for (size_t i = 0; i < warmup_iterations_; ++i) {
            func();
        }

        // Actual benchmark runs
        for (size_t i = 0; i < iterations_; ++i) {
            Timer timer;
            func();
            timer.stop();
            results_ns_.push_back(timer.elapsed_ns());
        }

        return calculate_stats();
    }

    template<typename Func, typename Setup, typename Teardown>
    BenchmarkStats run_with_setup(Func&& func, Setup&& setup, Teardown&& teardown) {
        results_ns_.clear();

        // Warmup
        for (size_t i = 0; i < warmup_iterations_; ++i) {
            setup();
            func();
            teardown();
        }

        // Benchmark
        for (size_t i = 0; i < iterations_; ++i) {
            setup();

            Timer timer;
            func();
            timer.stop();

            teardown();
            results_ns_.push_back(timer.elapsed_ns());
        }

        return calculate_stats();
    }

private:
    BenchmarkStats calculate_stats() {
        if (results_ns_.empty()) {
            return {0, 0, 0, 0, 0, 0};
        }

        BenchmarkStats stats;
        stats.iterations = results_ns_.size();

        // Min and Max
        stats.min_ns = *std::min_element(results_ns_.begin(), results_ns_.end());
        stats.max_ns = *std::max_element(results_ns_.begin(), results_ns_.end());

        // Mean
        double sum = std::accumulate(results_ns_.begin(), results_ns_.end(), 0.0);
        stats.mean_ns = sum / results_ns_.size();

        // Median
        std::vector<double> sorted = results_ns_;
        std::sort(sorted.begin(), sorted.end());
        size_t mid = sorted.size() / 2;
        if (sorted.size() % 2 == 0) {
            stats.median_ns = (sorted[mid - 1] + sorted[mid]) / 2.0;
        } else {
            stats.median_ns = sorted[mid];
        }

        // Standard Deviation
        double variance = 0.0;
        for (double val : results_ns_) {
            double diff = val - stats.mean_ns;
            variance += diff * diff;
        }
        variance /= results_ns_.size();
        stats.stddev_ns = std::sqrt(variance);

        return stats;
    }
};

// Benchmark suite for comparing multiple functions
class BenchmarkSuite {
private:
    std::string suite_name_;
    std::map<std::string, BenchmarkStats> results_;
    size_t iterations_;
    size_t warmup_;

public:
    BenchmarkSuite(const std::string& name, size_t iterations = 100, size_t warmup = 10)
        : suite_name_(name), iterations_(iterations), warmup_(warmup) {}

    template<typename Func>
    void add(const std::string& name, Func&& func) {
        Benchmark bench(name, iterations_, warmup_);
        results_[name] = bench.run(std::forward<Func>(func));
    }

    void print_results() const {
        std::cout << "\n" << std::string(70, '=') << "\n";
        std::cout << "Benchmark Suite: " << suite_name_ << "\n";
        std::cout << std::string(70, '=') << "\n";

        for (const auto& [name, stats] : results_) {
            stats.print(name);
        }

        // Comparison
        if (results_.size() >= 2) {
            print_comparison();
        }

        std::cout << std::string(70, '=') << "\n";
    }

private:
    void print_comparison() const {
        std::cout << "\n--- Performance Comparison ---\n";

        double baseline_mean = results_.begin()->second.mean_ns;
        std::string baseline_name = results_.begin()->first;

        std::cout << "Baseline: " << baseline_name << "\n\n";

        for (const auto& [name, stats] : results_) {
            if (name == baseline_name) continue;

            double speedup = baseline_mean / stats.mean_ns;
            std::cout << name << " vs " << baseline_name << ": ";

            if (speedup > 1.0) {
                std::cout << std::fixed << std::setprecision(2)
                         << speedup << "x faster\n";
            } else {
                std::cout << std::fixed << std::setprecision(2)
                         << (1.0 / speedup) << "x slower\n";
            }
        }
    }
};

// Prevent compiler optimizations from eliminating code
template<typename T>
inline void do_not_optimize(T&& value) {
#if defined(__clang__)
    asm volatile("" : : "r,m"(value) : "memory");
#elif defined(__GNUC__)
    asm volatile("" : : "r,m"(value) : "memory");
#elif defined(_MSC_VER)
    // For MSVC, use a volatile read
    static volatile T sink;
    sink = value;
#endif
}

// Prevent compiler from optimizing away memory
inline void clobber_memory() {
#if defined(__clang__) || defined(__GNUC__)
    asm volatile("" : : : "memory");
#endif
}

} // namespace perf

#endif // BENCHMARK_H
