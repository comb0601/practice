/*
 * Lesson 02: Complete Benchmarking Framework
 * Demonstrates professional benchmarking techniques
 */

#include "benchmark.h"
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

using namespace perf;

// Test functions for benchmarking
namespace test_functions {

// Vector operations
std::vector<int> create_vector(size_t size) {
    std::vector<int> v(size);
    for (size_t i = 0; i < size; ++i) {
        v[i] = static_cast<int>(i);
    }
    return v;
}

int sum_vector_loop(const std::vector<int>& v) {
    int sum = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        sum += v[i];
    }
    return sum;
}

int sum_vector_range(const std::vector<int>& v) {
    int sum = 0;
    for (int val : v) {
        sum += val;
    }
    return sum;
}

int sum_vector_accumulate(const std::vector<int>& v) {
    return std::accumulate(v.begin(), v.end(), 0);
}

// Math operations
double slow_sqrt(double x) {
    // Newton-Raphson method (multiple iterations)
    double guess = x / 2.0;
    for (int i = 0; i < 10; ++i) {
        guess = (guess + x / guess) / 2.0;
    }
    return guess;
}

double fast_sqrt(double x) {
    return std::sqrt(x);
}

// String operations
std::string concat_strings_operator(const std::vector<std::string>& strings) {
    std::string result;
    for (const auto& s : strings) {
        result += s;
    }
    return result;
}

std::string concat_strings_reserve(const std::vector<std::string>& strings) {
    size_t total_size = 0;
    for (const auto& s : strings) {
        total_size += s.length();
    }

    std::string result;
    result.reserve(total_size);

    for (const auto& s : strings) {
        result += s;
    }
    return result;
}

// Memory allocation
void allocate_small_objects() {
    for (int i = 0; i < 1000; ++i) {
        auto* p = new int(i);
        delete p;
    }
}

void allocate_pooled_objects() {
    // Simulate object pool
    std::vector<int> pool(1000);
    for (int i = 0; i < 1000; ++i) {
        pool[i] = i;
    }
}

} // namespace test_functions

void demo_basic_benchmark() {
    std::cout << "\n=== Basic Benchmark Demo ===\n";

    Benchmark bench("Vector Sum", 1000, 50);

    auto vec = test_functions::create_vector(10000);

    auto stats = bench.run([&]() {
        int sum = test_functions::sum_vector_range(vec);
        do_not_optimize(sum);
    });

    stats.print("Vector Sum (10,000 elements)");
}

void demo_benchmark_suite_vector_sum() {
    BenchmarkSuite suite("Vector Summation Comparison", 1000, 50);

    auto vec = test_functions::create_vector(10000);

    suite.add("Index Loop", [&]() {
        int sum = test_functions::sum_vector_loop(vec);
        do_not_optimize(sum);
    });

    suite.add("Range-based For", [&]() {
        int sum = test_functions::sum_vector_range(vec);
        do_not_optimize(sum);
    });

    suite.add("std::accumulate", [&]() {
        int sum = test_functions::sum_vector_accumulate(vec);
        do_not_optimize(sum);
    });

    suite.print_results();
}

void demo_benchmark_suite_sqrt() {
    BenchmarkSuite suite("Square Root Comparison", 10000, 100);

    double x = 123.456;

    suite.add("Custom Newton-Raphson", [&]() {
        double result = test_functions::slow_sqrt(x);
        do_not_optimize(result);
    });

    suite.add("std::sqrt", [&]() {
        double result = test_functions::fast_sqrt(x);
        do_not_optimize(result);
    });

    suite.print_results();
}

void demo_benchmark_suite_string_concat() {
    BenchmarkSuite suite("String Concatenation", 1000, 50);

    std::vector<std::string> strings;
    for (int i = 0; i < 100; ++i) {
        strings.push_back("String_" + std::to_string(i) + "_");
    }

    suite.add("Operator += (no reserve)", [&]() {
        std::string result = test_functions::concat_strings_operator(strings);
        do_not_optimize(result);
    });

    suite.add("Operator += (with reserve)", [&]() {
        std::string result = test_functions::concat_strings_reserve(strings);
        do_not_optimize(result);
    });

    suite.print_results();
}

void demo_benchmark_suite_allocation() {
    BenchmarkSuite suite("Memory Allocation", 100, 10);

    suite.add("Individual Allocations", []() {
        test_functions::allocate_small_objects();
    });

    suite.add("Pooled Allocation", []() {
        test_functions::allocate_pooled_objects();
    });

    suite.print_results();
}

void demo_benchmark_with_setup() {
    std::cout << "\n=== Benchmark with Setup/Teardown ===\n";

    Benchmark bench("Sorting Random Vector", 100, 10);

    std::vector<int> vec;
    std::random_device rd;
    std::mt19937 gen(42); // Fixed seed for reproducibility

    auto setup = [&]() {
        vec = test_functions::create_vector(1000);
        std::shuffle(vec.begin(), vec.end(), gen);
    };

    auto teardown = [&]() {
        vec.clear();
    };

    auto stats = bench.run_with_setup(
        [&]() {
            std::sort(vec.begin(), vec.end());
        },
        setup,
        teardown
    );

    stats.print("Sort 1000 Random Integers");
}

void demo_timer() {
    std::cout << "\n=== Timer Demo ===\n";

    Timer timer;

    // Simulate some work
    std::vector<int> vec = test_functions::create_vector(1000000);
    int sum = test_functions::sum_vector_accumulate(vec);
    do_not_optimize(sum);

    timer.stop();

    std::cout << "Operation took:\n";
    std::cout << "  " << timer.elapsed_ns() << " nanoseconds\n";
    std::cout << "  " << timer.elapsed_us() << " microseconds\n";
    std::cout << "  " << timer.elapsed_ms() << " milliseconds\n";
    std::cout << "  " << timer.elapsed_s() << " seconds\n";
}

void demo_scaling_analysis() {
    std::cout << "\n=== Scaling Analysis ===\n";
    std::cout << "Testing how performance scales with input size:\n\n";

    std::vector<size_t> sizes = {1000, 10000, 100000, 1000000};

    for (size_t size : sizes) {
        auto vec = test_functions::create_vector(size);

        Benchmark bench("Sum " + std::to_string(size) + " elements", 100, 10);

        auto stats = bench.run([&]() {
            int sum = test_functions::sum_vector_accumulate(vec);
            do_not_optimize(sum);
        });

        std::cout << std::setw(10) << size << " elements: "
                  << std::fixed << std::setprecision(2)
                  << stats.mean_ns / 1000.0 << " µs (mean)\n";
    }
}

int main() {
    std::cout << "Complete Benchmarking Framework\n";
    std::cout << "================================\n";
    std::cout << "\nThis lesson demonstrates:\n";
    std::cout << "1. Basic benchmarking\n";
    std::cout << "2. Benchmark suites for comparison\n";
    std::cout << "3. Setup/teardown support\n";
    std::cout << "4. Statistical analysis\n";
    std::cout << "5. Scaling analysis\n";

    demo_basic_benchmark();
    demo_timer();
    demo_benchmark_suite_vector_sum();
    demo_benchmark_suite_sqrt();
    demo_benchmark_suite_string_concat();
    demo_benchmark_suite_allocation();
    demo_benchmark_with_setup();
    demo_scaling_analysis();

    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "Key Takeaways:\n";
    std::cout << "- Always use warmup iterations\n";
    std::cout << "- Run multiple iterations for statistical significance\n";
    std::cout << "- Use do_not_optimize() to prevent compiler optimizations\n";
    std::cout << "- Compare multiple implementations\n";
    std::cout << "- Analyze scaling behavior\n";
    std::cout << std::string(70, '=') << "\n";

    return 0;
}
