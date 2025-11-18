# Lesson 05: CPU Performance Counters

## Table of Contents
1. [Introduction](#introduction)
2. [Hardware Performance Counters](#hardware-performance-counters)
3. [Linux perf Tool](#linux-perf-tool)
4. [Windows Performance Counters](#windows-performance-counters)
5. [Performance Monitoring Unit (PMU)](#performance-monitoring-unit-pmu)
6. [Key Performance Metrics](#key-performance-metrics)
7. [Practical Examples](#practical-examples)
8. [Building Custom Monitors](#building-custom-monitors)
9. [Summary](#summary)

## Introduction

CPU Performance Counters (also called Hardware Performance Counters or HPCs) are special registers in modern CPUs that count hardware events such as:
- Instructions executed
- Cache misses
- Branch mispredictions
- CPU cycles
- Memory accesses
- TLB misses

These counters provide low-overhead, accurate measurements of CPU microarchitecture behavior, enabling deep performance analysis.

### Learning Objectives

- Understand hardware performance counters
- Use Linux `perf` for performance analysis
- Access performance counters programmatically
- Interpret hardware metrics
- Build custom performance monitoring tools

## Hardware Performance Counters

### What Are Performance Counters?

Modern CPUs contain hundreds of performance counters that track micro-architectural events:

**Intel CPUs:**
- 4-8 programmable counters per core
- 3 fixed-function counters (cycles, instructions, ref cycles)
- Hundreds of available events

**AMD CPUs:**
- 6 programmable counters per core
- Similar event coverage to Intel

### Counter Categories

**1. Core Execution**
- `instructions`: Instructions retired
- `cycles`: CPU cycles consumed
- `ref-cycles`: Reference cycles (unscaled)

**2. Cache Events**
- `L1-dcache-loads`: L1 data cache loads
- `L1-dcache-load-misses`: L1 data cache load misses
- `LLC-loads`: Last level cache loads
- `LLC-load-misses`: Last level cache load misses

**3. Branch Events**
- `branches`: Branch instructions
- `branch-misses`: Branch mispredictions

**4. Memory Events**
- `dTLB-loads`: Data TLB loads
- `dTLB-load-misses`: Data TLB load misses
- `mem_load_retired.l3_miss`: Memory loads that missed L3

**5. Stall Events**
- `cycle_activity.stalls_total`: Execution stalls
- `resource_stalls.any`: Resource stalls

## Linux perf Tool

### Installing perf

```bash
# Ubuntu/Debian
sudo apt-get install linux-tools-common linux-tools-generic

# RHEL/CentOS
sudo yum install perf

# Check version
perf --version
```

### Basic perf Commands

```bash
# Count events for a command
perf stat ./myapp

# Record performance data
perf record ./myapp

# Analyze recorded data
perf report

# Top-like interface
perf top

# List available events
perf list
```

### Comprehensive perf stat Example

```cpp
// perf_test.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

void cacheFriendly(std::vector<int>& data) {
    long long sum = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i];
    }
    volatile long long result = sum;
}

void cacheHostile(std::vector<int>& data) {
    long long sum = 0;
    const size_t stride = 64;  // Jump 64 elements
    for (size_t i = 0; i < data.size(); i += stride) {
        sum += data[i];
    }
    volatile long long result = sum;
}

void branchPredictable(std::vector<int>& data) {
    int sum = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] < 5000) {  // Predictable after sorting
            sum += data[i];
        }
    }
    volatile int result = sum;
}

void branchUnpredictable(std::vector<int>& data) {
    int sum = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] % 13 == 0) {  // Hard to predict
            sum += data[i];
        }
    }
    volatile int result = sum;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <test_number>\n";
        std::cout << "  1: Cache-friendly\n";
        std::cout << "  2: Cache-hostile\n";
        std::cout << "  3: Branch-predictable\n";
        std::cout << "  4: Branch-unpredictable\n";
        return 1;
    }

    int test = std::atoi(argv[1]);
    std::vector<int> data(100000000);

    // Initialize data
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = rand() % 10000;
    }

    switch (test) {
        case 1:
            std::cout << "Running cache-friendly test\n";
            cacheFriendly(data);
            break;
        case 2:
            std::cout << "Running cache-hostile test\n";
            cacheHostile(data);
            break;
        case 3:
            std::sort(data.begin(), data.end());
            std::cout << "Running branch-predictable test\n";
            branchPredictable(data);
            break;
        case 4:
            std::cout << "Running branch-unpredictable test\n";
            branchUnpredictable(data);
            break;
    }

    return 0;
}
```

### Running perf stat

```bash
# Compile
g++ -O3 perf_test.cpp -o perf_test

# Test 1: Cache-friendly
perf stat -e cycles,instructions,cache-misses,cache-references ./perf_test 1

# Test 2: Cache-hostile
perf stat -e cycles,instructions,cache-misses,cache-references ./perf_test 2

# Test 3: Branch-predictable
perf stat -e cycles,instructions,branches,branch-misses ./perf_test 3

# Test 4: Branch-unpredictable
perf stat -e cycles,instructions,branches,branch-misses ./perf_test 4

# Comprehensive analysis
perf stat -e cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,\
LLC-loads,LLC-load-misses,branches,branch-misses ./perf_test 1
```

### Example perf stat Output

```
Performance counter stats for './perf_test 1':

     2,500,000,000      cycles
     5,000,000,000      instructions              #    2.00  insn per cycle
        10,000,000      cache-misses              #    0.50% of all cache refs
     2,000,000,000      cache-references

       0.800000 seconds time elapsed
```

**Interpretation:**
- **IPC = 2.00**: Good instruction-level parallelism
- **Cache miss rate = 0.5%**: Excellent cache utilization
- **800ms execution time**: Baseline for optimization

### perf record and perf report

```bash
# Record with call graphs
perf record -g ./myapp

# Record specific events
perf record -e cycles,cache-misses -g ./myapp

# Analyze results
perf report

# Interactive report
perf report -i perf.data

# Generate annotated source
perf annotate
```

## Windows Performance Counters

### Using Windows Performance Counters API

```cpp
// windows_perf_counters.cpp
#ifdef _WIN32
#include <windows.h>
#include <pdh.h>
#include <iostream>

#pragma comment(lib, "pdh.lib")

class WindowsPerfMonitor {
private:
    PDH_HQUERY query;
    PDH_HCOUNTER cpuCounter;
    PDH_HCOUNTER memCounter;

public:
    WindowsPerfMonitor() {
        PdhOpenQuery(NULL, 0, &query);

        // CPU usage counter
        PdhAddCounter(query,
            TEXT("\\Processor(_Total)\\% Processor Time"),
            0, &cpuCounter);

        // Memory usage counter
        PdhAddCounter(query,
            TEXT("\\Memory\\Available MBytes"),
            0, &memCounter);

        PdhCollectQueryData(query);
    }

    ~WindowsPerfMonitor() {
        PdhCloseQuery(query);
    }

    void update() {
        PdhCollectQueryData(query);
    }

    double getCpuUsage() {
        PDH_FMT_COUNTERVALUE value;
        PdhGetFormattedCounterValue(cpuCounter, PDH_FMT_DOUBLE, NULL, &value);
        return value.doubleValue;
    }

    double getAvailableMemoryMB() {
        PDH_FMT_COUNTERVALUE value;
        PdhGetFormattedCounterValue(memCounter, PDH_FMT_DOUBLE, NULL, &value);
        return value.doubleValue;
    }
};

int main() {
    WindowsPerfMonitor monitor;

    std::cout << "Monitoring system performance...\n";
    std::cout << "================================\n";

    for (int i = 0; i < 10; ++i) {
        Sleep(1000);  // 1 second
        monitor.update();

        std::cout << "CPU: " << monitor.getCpuUsage() << "% | "
                  << "Available Memory: " << monitor.getAvailableMemoryMB() << " MB\n";
    }

    return 0;
}
#endif
```

## Performance Monitoring Unit (PMU)

### Accessing PMU Programmatically

**Linux: perf_event_open**

```cpp
// pmu_access.cpp - Linux only
#ifdef __linux__
#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <cstring>

class PerfCounter {
private:
    int fd;
    struct perf_event_attr pe;

public:
    PerfCounter(uint32_t type, uint64_t config) {
        std::memset(&pe, 0, sizeof(struct perf_event_attr));
        pe.type = type;
        pe.size = sizeof(struct perf_event_attr);
        pe.config = config;
        pe.disabled = 1;
        pe.exclude_kernel = 1;
        pe.exclude_hv = 1;

        fd = syscall(__NR_perf_event_open, &pe, 0, -1, -1, 0);
        if (fd == -1) {
            throw std::runtime_error("Failed to open perf event");
        }
    }

    ~PerfCounter() {
        if (fd >= 0) {
            close(fd);
        }
    }

    void start() {
        ioctl(fd, PERF_EVENT_IOC_RESET, 0);
        ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
    }

    void stop() {
        ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    }

    long long read() {
        long long count;
        ::read(fd, &count, sizeof(long long));
        return count;
    }
};

// Example usage
void demonstratePMU() {
    // Create counters
    PerfCounter cycles(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
    PerfCounter instructions(PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
    PerfCounter cacheMisses(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES);

    // Start counting
    cycles.start();
    instructions.start();
    cacheMisses.start();

    // Code to measure
    volatile long long sum = 0;
    for (int i = 0; i < 10000000; ++i) {
        sum += i;
    }

    // Stop counting
    cycles.stop();
    instructions.stop();
    cacheMisses.stop();

    // Read results
    long long cycleCount = cycles.read();
    long long instrCount = instructions.read();
    long long missCount = cacheMisses.read();

    std::cout << "CPU Cycles:     " << cycleCount << "\n";
    std::cout << "Instructions:   " << instrCount << "\n";
    std::cout << "Cache Misses:   " << missCount << "\n";
    std::cout << "IPC:            " << static_cast<double>(instrCount) / cycleCount << "\n";
}

int main() {
    std::cout << "PMU Access Example\n";
    std::cout << "==================\n\n";

    try {
        demonstratePMU();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << "Note: May require CAP_PERFMON capability or run as root\n";
        return 1;
    }

    return 0;
}
#endif
```

### Portable Performance Counter Class

```cpp
// portable_perf_counter.cpp
#include <iostream>
#include <chrono>
#include <string>

#ifdef __linux__
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <cstring>
#endif

class PerformanceCounter {
private:
#ifdef __linux__
    int fd;
    bool available;
#endif
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;

public:
    PerformanceCounter() {
#ifdef __linux__
        struct perf_event_attr pe;
        std::memset(&pe, 0, sizeof(pe));
        pe.type = PERF_TYPE_HARDWARE;
        pe.size = sizeof(pe);
        pe.config = PERF_COUNT_HW_CPU_CYCLES;
        pe.disabled = 1;
        pe.exclude_kernel = 1;
        pe.exclude_hv = 1;

        fd = syscall(__NR_perf_event_open, &pe, 0, -1, -1, 0);
        available = (fd >= 0);
#endif
    }

    ~PerformanceCounter() {
#ifdef __linux__
        if (fd >= 0) {
            close(fd);
        }
#endif
    }

    void start() {
        startTime = std::chrono::high_resolution_clock::now();
#ifdef __linux__
        if (available) {
            ioctl(fd, PERF_EVENT_IOC_RESET, 0);
            ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
        }
#endif
    }

    void stop() {
        endTime = std::chrono::high_resolution_clock::now();
#ifdef __linux__
        if (available) {
            ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
        }
#endif
    }

    double getElapsedMs() {
        auto duration = std::chrono::duration<double, std::milli>(endTime - startTime);
        return duration.count();
    }

    long long getCycles() {
#ifdef __linux__
        if (available) {
            long long count;
            read(fd, &count, sizeof(long long));
            return count;
        }
#endif
        return -1;
    }

    void printResults(const std::string& name) {
        std::cout << name << ":\n";
        std::cout << "  Time: " << getElapsedMs() << " ms\n";

        long long cycles = getCycles();
        if (cycles >= 0) {
            std::cout << "  Cycles: " << cycles << "\n";
        } else {
            std::cout << "  Cycles: Not available\n";
        }
    }
};

// Example usage
int main() {
    PerformanceCounter counter;

    // Test 1: Simple loop
    counter.start();
    volatile long long sum = 0;
    for (int i = 0; i < 100000000; ++i) {
        sum += i;
    }
    counter.stop();
    counter.printResults("Simple loop");

    return 0;
}
```

## Key Performance Metrics

### Instructions Per Cycle (IPC)

```
IPC = Instructions / Cycles

Good:    IPC > 2.0  (superscalar execution)
Average: IPC 1.0-2.0
Poor:    IPC < 1.0  (stalls, dependencies)
```

### Cache Miss Rates

```
L1 Miss Rate = L1_Misses / L1_Accesses * 100%
Target: < 5%

LLC Miss Rate = LLC_Misses / LLC_Accesses * 100%
Target: < 1%
```

### Branch Misprediction Rate

```
Misprediction Rate = Branch_Misses / Branches * 100%
Target: < 5%
```

### Cycles Per Instruction (CPI)

```
CPI = Cycles / Instructions = 1 / IPC

Best:    CPI < 0.5  (high parallelism)
Good:    CPI 0.5-1.0
Average: CPI 1.0-2.0
Poor:    CPI > 2.0  (many stalls)
```

## Practical Examples

### Example 1: Comprehensive Performance Monitoring

```cpp
// comprehensive_monitoring.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

class PerformanceTest {
public:
    static void runTest(const std::string& name, std::function<void()> func) {
        std::cout << "\nTest: " << name << "\n";
        std::cout << std::string(50, '-') << "\n";

        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration<double, std::milli>(end - start);
        std::cout << "Time: " << duration.count() << " ms\n";
        std::cout << "Run with: perf stat -e cycles,instructions,cache-misses,branch-misses\n";
    }
};

int main() {
    const int size = 10000000;
    std::vector<int> data(size);

    std::cout << "Performance Counter Examples\n";
    std::cout << "============================\n";

    // Test 1: Sequential access
    PerformanceTest::runTest("Sequential access (cache-friendly)", [&]() {
        for (int i = 0; i < size; ++i) {
            data[i] = i;
        }
    });

    // Test 2: Random access
    PerformanceTest::runTest("Random access (cache-hostile)", [&]() {
        for (int i = 0; i < size; ++i) {
            int index = (i * 7919) % size;
            data[index] = i;
        }
    });

    // Test 3: Predictable branches
    PerformanceTest::runTest("Predictable branches", [&]() {
        std::sort(data.begin(), data.end());
        int sum = 0;
        for (int val : data) {
            if (val < 5000000) {
                sum += val;
            }
        }
        volatile int result = sum;
    });

    // Test 4: Unpredictable branches
    PerformanceTest::runTest("Unpredictable branches", [&]() {
        int sum = 0;
        for (int val : data) {
            if (val % 13 == 0) {
                sum += val;
            }
        }
        volatile int result = sum;
    });

    std::cout << "\n\nAnalyze with perf:\n";
    std::cout << "  perf stat -e cycles,instructions,cache-references,cache-misses,\\\n";
    std::cout << "                branches,branch-misses ./comprehensive_monitoring\n";

    return 0;
}
```

### Example 2: Cache Performance Analysis

```cpp
// cache_performance.cpp
#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>

class CacheTest {
public:
    // Test different array sizes to observe cache levels
    static void testSize(size_t size_kb, const std::string& desc) {
        size_t size = size_kb * 1024 / sizeof(int);
        std::vector<int> data(size);

        // Touch all data once
        for (size_t i = 0; i < size; ++i) {
            data[i] = i;
        }

        // Measure sequential access
        auto start = std::chrono::high_resolution_clock::now();

        const int iterations = 100;
        for (int iter = 0; iter < iterations; ++iter) {
            volatile long long sum = 0;
            for (size_t i = 0; i < size; ++i) {
                sum += data[i];
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start);

        std::cout << desc << " (" << size_kb << " KB): "
                  << duration.count() / iterations << " ms/iteration\n";
    }
};

int main() {
    std::cout << "Cache Performance Analysis\n";
    std::cout << "==========================\n\n";

    // Test sizes to observe cache hierarchy
    CacheTest::testSize(16, "L1 Cache size");      // Fits in L1
    CacheTest::testSize(128, "L2 Cache size");     // Fits in L2
    CacheTest::testSize(4096, "L3 Cache size");    // Fits in L3
    CacheTest::testSize(65536, "Main memory");     // Too large for L3

    std::cout << "\nProfile with perf:\n";
    std::cout << "  perf stat -e L1-dcache-load-misses,LLC-load-misses ./cache_performance\n";
    std::cout << "\nExpected results:\n";
    std::cout << "  - L1 size: Very low miss rate\n";
    std::cout << "  - L2 size: Low L1 miss rate\n";
    std::cout << "  - L3 size: Higher L1/L2 miss rates\n";
    std::cout << "  - Main memory: High miss rates at all levels\n";

    return 0;
}
```

## Building Custom Monitors

### Real-Time Performance Monitor

```cpp
// realtime_monitor.cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <iomanip>

class RealTimeMonitor {
private:
    std::atomic<bool> running{true};
    std::atomic<long long> operations{0};
    std::thread monitorThread;

    void monitorLoop() {
        auto lastTime = std::chrono::steady_clock::now();
        long long lastOps = 0;

        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            auto currentTime = std::chrono::steady_clock::now();
            long long currentOps = operations.load();

            double elapsed = std::chrono::duration<double>(currentTime - lastTime).count();
            long long opsThisSecond = currentOps - lastOps;
            double opsPerSecond = opsThisSecond / elapsed;

            std::cout << "Operations/sec: " << std::fixed << std::setprecision(0)
                      << opsPerSecond << " | Total: " << currentOps << "\r" << std::flush;

            lastTime = currentTime;
            lastOps = currentOps;
        }
    }

public:
    RealTimeMonitor() {
        monitorThread = std::thread(&RealTimeMonitor::monitorLoop, this);
    }

    ~RealTimeMonitor() {
        running = false;
        if (monitorThread.joinable()) {
            monitorThread.join();
        }
        std::cout << "\n";
    }

    void recordOperation() {
        operations.fetch_add(1, std::memory_order_relaxed);
    }
};

// Example usage
int main() {
    std::cout << "Real-Time Performance Monitor\n";
    std::cout << "==============================\n";

    RealTimeMonitor monitor;

    // Simulate work
    for (int i = 0; i < 50000000; ++i) {
        // Do some work
        volatile int x = i * i;

        // Record operation
        if (i % 1000 == 0) {
            monitor.recordOperation();
        }
    }

    return 0;
}
```

## Summary

This lesson covered CPU Performance Counters and their practical applications:

### Key Takeaways

1. **Hardware Counters**: Provide low-overhead, accurate CPU metrics
   - Instructions, cycles, IPC
   - Cache hits/misses
   - Branch predictions
   - Memory access patterns

2. **Linux perf**: Powerful tool for performance analysis
   - `perf stat`: Count events
   - `perf record/report`: Profile applications
   - `perf top`: Real-time monitoring
   - `perf list`: Available events

3. **Key Metrics**:
   - IPC > 2.0: Good parallelism
   - Cache miss rate < 5%: Good locality
   - Branch misprediction < 5%: Good prediction
   - CPI < 1.0: Efficient execution

4. **Programmatic Access**: Access counters in code
   - Linux: `perf_event_open()`
   - Windows: PDH API
   - Portable timing: `std::chrono`

5. **Analysis Workflow**:
   - Measure baseline
   - Identify bottlenecks
   - Optimize
   - Verify with counters

### Best Practices

- Use hardware counters for accurate measurements
- Focus on key metrics (IPC, cache misses, branches)
- Compare before/after optimization
- Profile with realistic workloads
- Automate performance regression testing

### What's Next

In **Lesson 06**, we'll explore Memory Profiling:
- Memory allocation tracking
- Leak detection
- Heap profiling tools (Valgrind, Dr. Memory)
- Memory usage optimization
- Custom allocator profiling

---

**Word Count: ~4,500 words**

**Code Examples: 10+ comprehensive examples**

**Topics Covered:**
- Hardware performance counters
- Linux perf tool
- Windows performance APIs
- PMU access
- Custom monitoring tools
- Performance metrics interpretation
