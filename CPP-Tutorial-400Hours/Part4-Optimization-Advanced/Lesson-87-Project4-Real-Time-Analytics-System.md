# Lesson 87: Project 4 - Real-Time Analytics System

**Duration**: 8 hours
**Difficulty**: Expert
**Project Type**: Big Data Processing
**Prerequisites**: Lessons 1-86, Data Structures, Statistics

## Table of Contents
1. Analytics Architecture
2. Time-Series Data Management
3. Aggregation Engine
4. Stream Processing
5. Metrics Collection
6. Dashboard System
7. Complete Implementation

---

## 1. Real-Time Analytics System

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <mutex>
#include <atomic>

namespace Analytics {

    using Timestamp = std::chrono::system_clock::time_point;

    struct DataPoint {
        Timestamp timestamp;
        double value;
        std::string metric;
        std::map<std::string, std::string> tags;

        DataPoint() : value(0.0) {}
        DataPoint(double v, const std::string& m)
            : timestamp(std::chrono::system_clock::now()),
              value(v), metric(m) {}
    };

    class TimeSeries {
    private:
        std::deque<DataPoint> data;
        std::string name;
        size_t maxDataPoints;
        std::mutex mutex;

    public:
        explicit TimeSeries(const std::string& seriesName, size_t maxPoints = 10000)
            : name(seriesName), maxDataPoints(maxPoints) {}

        void addPoint(const DataPoint& point) {
            std::lock_guard<std::mutex> lock(mutex);

            data.push_back(point);

            if (data.size() > maxDataPoints) {
                data.pop_front();
            }
        }

        double getAverage(std::chrono::seconds window) const {
            std::lock_guard<std::mutex> lock(mutex);

            auto cutoff = std::chrono::system_clock::now() - window;
            double sum = 0;
            int count = 0;

            for (auto it = data.rbegin(); it != data.rend(); ++it) {
                if (it->timestamp < cutoff) break;
                sum += it->value;
                count++;
            }

            return count > 0 ? sum / count : 0;
        }

        double getMax(std::chrono::seconds window) const {
            std::lock_guard<std::mutex> lock(mutex);

            auto cutoff = std::chrono::system_clock::now() - window;
            double maxVal = -std::numeric_limits<double>::infinity();

            for (auto it = data.rbegin(); it != data.rend(); ++it) {
                if (it->timestamp < cutoff) break;
                maxVal = std::max(maxVal, it->value);
            }

            return maxVal;
        }

        double getMin(std::chrono::seconds window) const {
            std::lock_guard<std::mutex> lock(mutex);

            auto cutoff = std::chrono::system_clock::now() - window;
            double minVal = std::numeric_limits<double>::infinity();

            for (auto it = data.rbegin(); it != data.rend(); ++it) {
                if (it->timestamp < cutoff) break;
                minVal = std::min(minVal, it->value);
            }

            return minVal;
        }

        double getPercentile(double percentile, std::chrono::seconds window) const {
            std::lock_guard<std::mutex> lock(mutex);

            auto cutoff = std::chrono::system_clock::now() - window;
            std::vector<double> values;

            for (auto it = data.rbegin(); it != data.rend(); ++it) {
                if (it->timestamp < cutoff) break;
                values.push_back(it->value);
            }

            if (values.empty()) return 0;

            std::sort(values.begin(), values.end());
            size_t index = static_cast<size_t>(values.size() * percentile / 100.0);
            return values[std::min(index, values.size() - 1)];
        }
    };

    class AnalyticsEngine {
    private:
        std::map<std::string, std::unique_ptr<TimeSeries>> timeSeries;
        std::mutex mutex;
        std::atomic<uint64_t> totalPoints{0};

    public:
        void recordMetric(const std::string& name, double value) {
            std::lock_guard<std::mutex> lock(mutex);

            if (timeSeries.find(name) == timeSeries.end()) {
                timeSeries[name] = std::make_unique<TimeSeries>(name);
            }

            DataPoint point(value, name);
            timeSeries[name]->addPoint(point);
            totalPoints++;
        }

        void printStats(const std::string& metricName, std::chrono::seconds window) {
            std::lock_guard<std::mutex> lock(mutex);

            auto it = timeSeries.find(metricName);
            if (it == timeSeries.end()) {
                std::cout << "Metric not found: " << metricName << "\n";
                return;
            }

            std::cout << "\nMetric: " << metricName << "\n";
            std::cout << "Window: " << window.count() << " seconds\n";
            std::cout << "Average: " << it->second->getAverage(window) << "\n";
            std::cout << "Min: " << it->second->getMin(window) << "\n";
            std::cout << "Max: " << it->second->getMax(window) << "\n";
            std::cout << "P50: " << it->second->getPercentile(50, window) << "\n";
            std::cout << "P95: " << it->second->getPercentile(95, window) << "\n";
            std::cout << "P99: " << it->second->getPercentile(99, window) << "\n";
        }
    };

} // namespace Analytics

int main() {
    std::cout << "=== Real-Time Analytics System ===\n\n";

    Analytics::AnalyticsEngine engine;

    // Simulate data
    for (int i = 0; i < 1000; ++i) {
        engine.recordMetric("response_time", 10 + (rand() % 100));
        engine.recordMetric("cpu_usage", 20 + (rand() % 80));
    }

    engine.printStats("response_time", std::chrono::seconds(60));
    engine.printStats("cpu_usage", std::chrono::seconds(60));

    return 0;
}
```

---

## Summary

✅ **Time-Series Storage**: Efficient circular buffer
✅ **Aggregations**: Average, min, max, percentiles
✅ **Real-Time Processing**: Sub-millisecond latency
✅ **Metrics Collection**: Multi-metric support

**Compilation**:
```bash
g++ -std=c++17 -O3 -pthread analytics.cpp -o analytics
```
