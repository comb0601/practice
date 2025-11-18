/*
 * Lesson 21: Move Semantics
 * File: 12_real_world_examples.cpp
 *
 * Real-World Move Semantics Examples
 *
 * This program demonstrates practical, real-world applications of move
 * semantics in various scenarios.
 *
 * Compile: g++ -std=c++17 -Wall -O2 12_real_world_examples.cpp -o 12_real_world_examples
 */

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <sstream>

// Example 1: Image Buffer (Large Data Structure)
class ImageBuffer {
private:
    std::vector<uint8_t> pixels;
    int width;
    int height;

public:
    ImageBuffer(int w, int h)
        : pixels(w * h * 4), width(w), height(h) {  // RGBA
        std::cout << "  ImageBuffer created (" << width << "x" << height << ")\n";
    }

    // Move constructor (cheap for large images)
    ImageBuffer(ImageBuffer&& other) noexcept
        : pixels(std::move(other.pixels)),
          width(other.width),
          height(other.height) {
        other.width = 0;
        other.height = 0;
        std::cout << "  ImageBuffer moved (" << width << "x" << height << ")\n";
    }

    ImageBuffer& operator=(ImageBuffer&& other) noexcept {
        if (this != &other) {
            pixels = std::move(other.pixels);
            width = other.width;
            height = other.height;
            other.width = 0;
            other.height = 0;
        }
        return *this;
    }

    size_t sizeInBytes() const { return pixels.size(); }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

// Example 2: Document with History (Undo/Redo)
class Document {
private:
    std::string content;
    std::vector<std::string> history;

public:
    Document(std::string initial = "") : content(std::move(initial)) {
        history.push_back(content);
    }

    void setText(std::string text) {
        content = std::move(text);  // Move instead of copy
        history.push_back(content);
    }

    // Return by value, enables move
    std::string getText() const {
        return content;
    }

    // Move entire document
    Document(Document&& other) noexcept
        : content(std::move(other.content)),
          history(std::move(other.history)) {
        std::cout << "  Document moved (history size: " << history.size() << ")\n";
    }

    size_t historySize() const { return history.size(); }
};

// Example 3: Network Message (Builder Pattern with Move)
class Message {
private:
    std::string header;
    std::string body;
    std::vector<std::string> attachments;

public:
    Message() = default;

    // Builder methods that return *this for chaining
    Message& setHeader(std::string h) {
        header = std::move(h);
        return *this;
    }

    Message& setBody(std::string b) {
        body = std::move(b);
        return *this;
    }

    Message& addAttachment(std::string att) {
        attachments.push_back(std::move(att));
        return *this;
    }

    // Move constructor for efficient transfer
    Message(Message&& other) noexcept = default;
    Message& operator=(Message&& other) noexcept = default;

    std::string serialize() const {
        std::ostringstream oss;
        oss << "Header: " << header << "\n"
            << "Body: " << body << "\n"
            << "Attachments: " << attachments.size();
        return oss.str();
    }
};

// Example 4: Database Query Result
class QueryResult {
private:
    std::vector<std::vector<std::string>> rows;
    std::vector<std::string> columnNames;

public:
    QueryResult() = default;

    void addRow(std::vector<std::string> row) {
        rows.push_back(std::move(row));  // Move row into container
    }

    void setColumns(std::vector<std::string> cols) {
        columnNames = std::move(cols);
    }

    // Move-enabled accessors
    QueryResult(QueryResult&&) noexcept = default;
    QueryResult& operator=(QueryResult&&) noexcept = default;

    size_t rowCount() const { return rows.size(); }
    size_t columnCount() const { return columnNames.size(); }

    void print() const {
        std::cout << "  Columns: ";
        for (const auto& col : columnNames) {
            std::cout << col << " ";
        }
        std::cout << "\n  Rows: " << rows.size() << "\n";
    }
};

// Example 5: Cache with LRU eviction
template<typename Key, typename Value>
class Cache {
private:
    std::map<Key, Value> data;
    size_t maxSize;

public:
    Cache(size_t max) : maxSize(max) {}

    void put(Key key, Value value) {
        if (data.size() >= maxSize) {
            data.erase(data.begin());  // Simple eviction
        }
        data[std::move(key)] = std::move(value);  // Move both key and value
    }

    Value* get(const Key& key) {
        auto it = data.find(key);
        return (it != data.end()) ? &it->second : nullptr;
    }

    size_t size() const { return data.size(); }
};

// Example 6: Task Queue
class Task {
private:
    std::string name;
    std::function<void()> work;

public:
    Task(std::string n, std::function<void()> w)
        : name(std::move(n)), work(std::move(w)) {}

    void execute() const {
        std::cout << "  Executing task: " << name << "\n";
        work();
    }

    const std::string& getName() const { return name; }

    // Move-only (functions are move-enabled)
    Task(Task&&) noexcept = default;
    Task& operator=(Task&&) noexcept = default;
};

class TaskQueue {
private:
    std::vector<Task> tasks;

public:
    void addTask(Task task) {
        tasks.push_back(std::move(task));  // Move task into queue
    }

    void executAll() {
        for (const auto& task : tasks) {
            task.execute();
        }
        tasks.clear();
    }

    size_t size() const { return tasks.size(); }
};

// Example 7: Resource Pool
template<typename T>
class ResourcePool {
private:
    std::vector<std::unique_ptr<T>> available;
    std::vector<std::unique_ptr<T>> inUse;

public:
    void addResource(std::unique_ptr<T> resource) {
        available.push_back(std::move(resource));
    }

    std::unique_ptr<T> acquire() {
        if (available.empty()) {
            return nullptr;
        }
        auto resource = std::move(available.back());
        available.pop_back();
        return resource;  // Return ownership
    }

    void release(std::unique_ptr<T> resource) {
        available.push_back(std::move(resource));
    }

    size_t availableCount() const { return available.size(); }
};

int main() {
    std::cout << "=== Real-World Move Semantics Examples ===\n\n";

    // 1. Image processing pipeline
    std::cout << "1. IMAGE PROCESSING PIPELINE:\n";
    {
        ImageBuffer img(1920, 1080);
        std::cout << "Original image: " << img.sizeInBytes() / (1024*1024) << " MB\n";

        std::cout << "\nPassing through pipeline:\n";
        auto processImage = [](ImageBuffer img) {
            std::cout << "  Processing image: " << img.getWidth() << "x" << img.getHeight() << "\n";
            return img;  // Move on return
        };

        ImageBuffer processed = processImage(std::move(img));
        std::cout << "Processed: " << processed.getWidth() << "x" << processed.getHeight() << "\n";
    }
    std::cout << "\n";

    // 2. Document with undo history
    std::cout << "2. DOCUMENT WITH HISTORY:\n";
    {
        Document doc("Initial text");
        doc.setText("Modified text");
        doc.setText("Final text");

        std::cout << "History size: " << doc.historySize() << "\n";

        std::cout << "\nMoving document:\n";
        Document backup = std::move(doc);
        std::cout << "Backup history: " << backup.historySize() << "\n";
    }
    std::cout << "\n";

    // 3. Building network message
    std::cout << "3. BUILDING NETWORK MESSAGE:\n";
    {
        std::cout << "Using builder pattern:\n";
        Message msg;
        msg.setHeader("Content-Type: application/json")
           .setBody("{\"data\": \"example\"}")
           .addAttachment("file1.txt")
           .addAttachment("file2.pdf");

        std::cout << msg.serialize() << "\n";

        std::cout << "\nMoving message:\n";
        Message sent = std::move(msg);
        std::cout << sent.serialize() << "\n";
    }
    std::cout << "\n";

    // 4. Database query results
    std::cout << "4. DATABASE QUERY RESULTS:\n";
    {
        QueryResult result;
        result.setColumns({"id", "name", "email"});

        std::vector<std::string> row1 = {"1", "Alice", "alice@example.com"};
        std::vector<std::string> row2 = {"2", "Bob", "bob@example.com"};

        result.addRow(std::move(row1));  // Move rows
        result.addRow(std::move(row2));

        result.print();

        std::cout << "\nMoving result set:\n";
        QueryResult cached = std::move(result);
        cached.print();
    }
    std::cout << "\n";

    // 5. Cache with move semantics
    std::cout << "5. CACHE WITH MOVE:\n";
    {
        Cache<std::string, std::vector<int>> cache(3);

        std::cout << "Adding items to cache:\n";
        std::vector<int> data1 = {1, 2, 3, 4, 5};
        cache.put("key1", std::move(data1));  // Move into cache

        std::vector<int> data2 = {6, 7, 8, 9, 10};
        cache.put("key2", std::move(data2));

        std::cout << "Cache size: " << cache.size() << "\n";

        if (auto* value = cache.get("key1")) {
            std::cout << "key1 size: " << value->size() << "\n";
        }
    }
    std::cout << "\n";

    // 6. Task queue with lambdas
    std::cout << "6. TASK QUEUE:\n";
    {
        TaskQueue queue;

        std::cout << "Adding tasks:\n";
        queue.addTask(Task("Task1", []() {
            std::cout << "    Doing work 1\n";
        }));

        queue.addTask(Task("Task2", []() {
            std::cout << "    Doing work 2\n";
        }));

        queue.addTask(Task("Task3", []() {
            std::cout << "    Doing work 3\n";
        }));

        std::cout << "Queue size: " << queue.size() << "\n";

        std::cout << "\nExecuting all tasks:\n";
        queue.executAll();
    }
    std::cout << "\n";

    // 7. Resource pool
    std::cout << "7. RESOURCE POOL:\n";
    {
        ResourcePool<int> pool;

        std::cout << "Adding resources to pool:\n";
        for (int i = 1; i <= 3; ++i) {
            pool.addResource(std::make_unique<int>(i * 10));
        }

        std::cout << "Available: " << pool.availableCount() << "\n";

        std::cout << "\nAcquiring resource:\n";
        auto resource = pool.acquire();
        std::cout << "Acquired: " << *resource << "\n";
        std::cout << "Available: " << pool.availableCount() << "\n";

        std::cout << "\nReleasing resource:\n";
        pool.release(std::move(resource));
        std::cout << "Available: " << pool.availableCount() << "\n";
    }
    std::cout << "\n";

    // 8. String concatenation optimization
    std::cout << "8. STRING CONCATENATION:\n";
    {
        auto buildString = []() {
            std::string result;
            result.reserve(100);  // Preallocate

            std::string part1 = "Hello ";
            std::string part2 = "World ";
            std::string part3 = "from Move Semantics!";

            // Move strings into concatenation
            result = std::move(part1);
            result += std::move(part2);
            result += std::move(part3);

            return result;  // RVO or move
        };

        std::string str = buildString();
        std::cout << "Built string: " << str << "\n";
        std::cout << "Length: " << str.length() << "\n";
    }
    std::cout << "\n";

    // 9. Factory with configuration
    std::cout << "9. FACTORY WITH CONFIGURATION:\n";
    {
        class Config {
        public:
            std::string name;
            std::vector<std::string> options;

            Config(std::string n) : name(std::move(n)) {}

            Config& addOption(std::string opt) {
                options.push_back(std::move(opt));
                return *this;
            }
        };

        auto createWithConfig = [](Config config) {
            std::cout << "  Creating with config: " << config.name << "\n";
            std::cout << "  Options: " << config.options.size() << "\n";
        };

        Config cfg("MyApp");
        cfg.addOption("option1").addOption("option2").addOption("option3");

        createWithConfig(std::move(cfg));  // Move config
    }
    std::cout << "\n";

    // 10. Best practices summary
    std::cout << "10. BEST PRACTICES:\n";
    std::cout << "1. Use move for large data structures (images, buffers)\n";
    std::cout << "2. Move into containers to avoid copies\n";
    std::cout << "3. Return by value and let compiler optimize\n";
    std::cout << "4. Use builder pattern with move\n";
    std::cout << "5. Move temporary results in pipelines\n";
    std::cout << "6. Cache/pool with move semantics\n";
    std::cout << "7. Task queues with move-only tasks\n";
    std::cout << "8. Move configuration objects\n";
    std::cout << "9. Enable RVO by returning locals\n";
    std::cout << "10. Profile to verify performance gains\n";

    return 0;
}
