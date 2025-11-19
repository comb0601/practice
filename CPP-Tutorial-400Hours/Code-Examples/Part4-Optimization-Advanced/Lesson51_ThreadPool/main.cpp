/*
 * Lesson 51: Thread Pool Implementation
 * Demonstrates production-grade thread pool with benchmarks
 */

#include "thread_pool.h"
#include <iostream>
#include <chrono>
#include <numeric>
#include <random>
#include <iomanip>

class Timer {
    std::chrono::high_resolution_clock::time_point start_;
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }
};

// Heavy computation task
long long fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Simulate I/O-bound task
void simulate_io_task(int task_id, int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// CPU-bound task
double compute_pi_monte_carlo(size_t iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    size_t inside_circle = 0;

    for (size_t i = 0; i < iterations; ++i) {
        double x = dis(gen);
        double y = dis(gen);

        if (x * x + y * y <= 1.0) {
            ++inside_circle;
        }
    }

    return 4.0 * inside_circle / iterations;
}

void demo_basic_usage() {
    std::cout << "\n=== Basic Thread Pool Usage ===\n";

    ThreadPool pool(4);  // 4 worker threads

    std::cout << "Thread pool created with " << pool.thread_count() << " threads\n";

    // Submit simple tasks
    auto future1 = pool.submit([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return 42;
    });

    auto future2 = pool.submit([](int a, int b) {
        return a + b;
    }, 10, 32);

    auto future3 = pool.submit(fibonacci, 20);

    std::cout << "Task 1 result: " << future1.get() << "\n";
    std::cout << "Task 2 result: " << future2.get() << "\n";
    std::cout << "Task 3 result (fib(20)): " << future3.get() << "\n";
}

void demo_parallel_computation() {
    std::cout << "\n=== Parallel Computation Demo ===\n";

    const int num_tasks = 12;
    const int fib_n = 30;

    // Sequential
    {
        Timer t;
        long long result = 0;
        for (int i = 0; i < num_tasks; ++i) {
            result += fibonacci(fib_n);
        }
        std::cout << "Sequential: " << std::fixed << std::setprecision(2)
                  << t.elapsed_ms() << " ms\n";
    }

    // Parallel with thread pool
    {
        ThreadPool pool(4);
        Timer t;

        std::vector<std::future<long long>> futures;
        for (int i = 0; i < num_tasks; ++i) {
            futures.push_back(pool.submit(fibonacci, fib_n));
        }

        long long result = 0;
        for (auto& future : futures) {
            result += future.get();
        }

        std::cout << "Parallel (4 threads): " << std::fixed << std::setprecision(2)
                  << t.elapsed_ms() << " ms\n";
    }
}

void demo_monte_carlo_pi() {
    std::cout << "\n=== Monte Carlo Pi Estimation ===\n";

    const size_t total_iterations = 100000000;  // 100 million
    const int num_threads = std::thread::hardware_concurrency();

    std::cout << "Total iterations: " << total_iterations << "\n";
    std::cout << "Using " << num_threads << " threads\n\n";

    // Sequential
    {
        Timer t;
        double pi = compute_pi_monte_carlo(total_iterations);
        std::cout << "Sequential: " << std::fixed << std::setprecision(6)
                  << "Pi ≈ " << pi << " (took " << std::setprecision(2)
                  << t.elapsed_ms() << " ms)\n";
    }

    // Parallel
    {
        ThreadPool pool(num_threads);
        Timer t;

        size_t iterations_per_thread = total_iterations / num_threads;

        std::vector<std::future<double>> futures;
        for (int i = 0; i < num_threads; ++i) {
            futures.push_back(pool.submit(compute_pi_monte_carlo, iterations_per_thread));
        }

        double pi_sum = 0.0;
        for (auto& future : futures) {
            pi_sum += future.get();
        }

        double pi = pi_sum / num_threads;

        std::cout << "Parallel:   " << std::fixed << std::setprecision(6)
                  << "Pi ≈ " << pi << " (took " << std::setprecision(2)
                  << t.elapsed_ms() << " ms)\n";
    }
}

void demo_priority_pool() {
    std::cout << "\n=== Priority Thread Pool Demo ===\n";

    PriorityThreadPool pool(2);

    std::vector<std::future<void>> futures;

    // Submit tasks with different priorities
    futures.push_back(pool.submit(PriorityThreadPool::Priority::LOW, []() {
        std::cout << "LOW priority task executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }));

    futures.push_back(pool.submit(PriorityThreadPool::Priority::CRITICAL, []() {
        std::cout << "CRITICAL priority task executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }));

    futures.push_back(pool.submit(PriorityThreadPool::Priority::HIGH, []() {
        std::cout << "HIGH priority task executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }));

    futures.push_back(pool.submit(PriorityThreadPool::Priority::NORMAL, []() {
        std::cout << "NORMAL priority task executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }));

    // Wait for all tasks
    for (auto& future : futures) {
        future.wait();
    }

    std::cout << "All priority tasks completed\n";
}

void demo_exception_handling() {
    std::cout << "\n=== Exception Handling Demo ===\n";

    ThreadPool pool(2);

    auto future1 = pool.submit([]() {
        throw std::runtime_error("Task failed!");
        return 42;
    });

    auto future2 = pool.submit([]() {
        return 123;
    });

    try {
        int result = future1.get();
        std::cout << "Result 1: " << result << "\n";
    } catch (const std::exception& e) {
        std::cout << "Caught exception from task 1: " << e.what() << "\n";
    }

    std::cout << "Result 2: " << future2.get() << "\n";
}

void demo_wait_all() {
    std::cout << "\n=== Wait All Demo ===\n";

    ThreadPool pool(4);

    std::cout << "Submitting 10 tasks...\n";

    for (int i = 0; i < 10; ++i) {
        pool.submit_detached([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Task " << i << " completed\n";
        });
    }

    std::cout << "Waiting for all tasks to complete...\n";
    pool.wait_all();
    std::cout << "All tasks completed!\n";
}

void benchmark_threadpool_overhead() {
    std::cout << "\n=== Thread Pool Overhead Benchmark ===\n";

    const int num_tasks = 10000;

    // Small tasks to measure overhead
    auto small_task = []() {
        int sum = 0;
        for (int i = 0; i < 100; ++i) {
            sum += i;
        }
        return sum;
    };

    // Direct execution
    {
        Timer t;
        for (int i = 0; i < num_tasks; ++i) {
            volatile int result = small_task();
            (void)result;
        }
        std::cout << "Direct execution: " << std::fixed << std::setprecision(2)
                  << t.elapsed_ms() << " ms\n";
    }

    // Thread pool execution
    {
        ThreadPool pool(4);
        Timer t;

        std::vector<std::future<int>> futures;
        for (int i = 0; i < num_tasks; ++i) {
            futures.push_back(pool.submit(small_task));
        }

        for (auto& future : futures) {
            future.get();
        }

        std::cout << "Thread pool (4 threads): " << std::fixed << std::setprecision(2)
                  << t.elapsed_ms() << " ms\n";
    }

    std::cout << "\nNote: For very small tasks, overhead may dominate\n";
}

int main() {
    std::cout << "Thread Pool Implementation\n";
    std::cout << "==========================\n";
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " threads\n";

    demo_basic_usage();
    demo_parallel_computation();
    demo_monte_carlo_pi();
    demo_priority_pool();
    demo_exception_handling();
    demo_wait_all();
    benchmark_threadpool_overhead();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Key Takeaways:\n";
    std::cout << "- Thread pools amortize thread creation overhead\n";
    std::cout << "- Best for CPU-bound and I/O-bound tasks\n";
    std::cout << "- Use std::future for result retrieval\n";
    std::cout << "- Handle exceptions properly\n";
    std::cout << "- Consider task granularity vs overhead\n";
    std::cout << std::string(60, '=') << "\n";

    return 0;
}
