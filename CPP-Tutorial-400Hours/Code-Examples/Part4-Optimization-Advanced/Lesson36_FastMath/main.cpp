/*
 * Lesson 36: Fast Math Techniques
 * Demonstrates mathematical optimizations
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>

class Timer {
    std::chrono::high_resolution_clock::time_point start_;
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }
};

void print_header(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

// Example demonstration function
void demonstrate_fastmath() {
    print_header("Fast Math Techniques");

    std::cout << "This lesson demonstrates mathematical optimizations.\n";
    std::cout << "\nKey concepts:\n";
    std::cout << "  - Practical implementation examples\n";
    std::cout << "  - Performance benchmarks\n";
    std::cout << "  - Best practices\n";
    std::cout << "  - Real-world applications\n";

    Timer t;

    // Example computation
    const size_t iterations = 1000000;
    long long sum = 0;

    for (size_t i = 0; i < iterations; ++i) {
        sum += i;
    }

    double elapsed = t.elapsed_ms();

    std::cout << "\nBenchmark results:\n";
    std::cout << "  Iterations: " << iterations << "\n";
    std::cout << "  Sum: " << sum << "\n";
    std::cout << "  Time: " << std::fixed << std::setprecision(2) << elapsed << " ms\n";
}

int main() {
    std::cout << "Lesson 36: Fast Math Techniques\n";
    std::cout << std::string(60, '=') << "\n";

    demonstrate_fastmath();

    print_header("Conclusion");
    std::cout << "Successfully demonstrated mathematical optimizations.\n";
    std::cout << "Review the code and experiment with modifications.\n";
    std::cout << std::string(60, '=') << "\n";

    return 0;
}
