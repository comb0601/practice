/*
 * Lesson 21: Move Semantics
 * File: 11_move_only_types.cpp
 *
 * Move-Only Types
 *
 * This program demonstrates move-only types that cannot be copied,
 * only moved. Examples include unique_ptr, thread, and custom types.
 *
 * Compile: g++ -std=c++17 -Wall 11_move_only_types.cpp -o 11_move_only_types
 */

#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <fstream>
#include <mutex>

// Custom move-only file wrapper
class FileHandle {
private:
    std::string filename;
    FILE* handle;

public:
    explicit FileHandle(const std::string& fname)
        : filename(fname), handle(std::fopen(fname.c_str(), "w")) {
        if (handle) {
            std::cout << "  [" << filename << "] File opened\n";
        } else {
            std::cout << "  [" << filename << "] Failed to open\n";
        }
    }

    // Delete copy operations
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Move constructor
    FileHandle(FileHandle&& other) noexcept
        : filename(std::move(other.filename)), handle(other.handle) {
        other.handle = nullptr;
        std::cout << "  [" << filename << "] File handle moved\n";
    }

    // Move assignment
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            close();
            filename = std::move(other.filename);
            handle = other.handle;
            other.handle = nullptr;
            std::cout << "  [" << filename << "] File handle move assigned\n";
        }
        return *this;
    }

    ~FileHandle() {
        close();
    }

    void write(const std::string& data) {
        if (handle) {
            std::fputs(data.c_str(), handle);
        }
    }

    bool isOpen() const { return handle != nullptr; }

private:
    void close() {
        if (handle) {
            std::fclose(handle);
            std::cout << "  [" << filename << "] File closed\n";
            handle = nullptr;
        }
    }
};

// Move-only resource manager
class ResourceManager {
private:
    std::unique_ptr<int[]> buffer;
    size_t size;
    std::string name;

public:
    ResourceManager(const std::string& n, size_t s)
        : buffer(std::make_unique<int[]>(s)), size(s), name(n) {
        std::cout << "  [" << name << "] ResourceManager created (size=" << size << ")\n";
    }

    // Delete copy operations
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Default move operations
    ResourceManager(ResourceManager&&) noexcept = default;
    ResourceManager& operator=(ResourceManager&&) noexcept = default;

    ~ResourceManager() {
        std::cout << "  [" << name << "] ResourceManager destroyed\n";
    }

    void fillBuffer(int value) {
        for (size_t i = 0; i < size; ++i) {
            buffer[i] = value;
        }
    }

    const std::string& getName() const { return name; }
    size_t getSize() const { return size; }
};

// Connection pool using move-only connections
class Connection {
private:
    int id;
    bool active;
    static int nextId;

public:
    Connection() : id(nextId++), active(true) {
        std::cout << "  Connection " << id << " established\n";
    }

    // Move-only
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&& other) noexcept : id(other.id), active(other.active) {
        other.active = false;
        std::cout << "  Connection " << id << " moved\n";
    }

    Connection& operator=(Connection&& other) noexcept {
        if (this != &other) {
            close();
            id = other.id;
            active = other.active;
            other.active = false;
        }
        return *this;
    }

    ~Connection() {
        close();
    }

    void send(const std::string& msg) {
        if (active) {
            std::cout << "  [Conn " << id << "] Sending: " << msg << "\n";
        }
    }

    bool isActive() const { return active; }
    int getId() const { return id; }

private:
    void close() {
        if (active) {
            std::cout << "  Connection " << id << " closed\n";
            active = false;
        }
    }
};

int Connection::nextId = 1;

// Factory function for move-only types
std::unique_ptr<int> createResource(int value) {
    return std::make_unique<int>(value);
}

FileHandle createFile(const std::string& filename) {
    return FileHandle(filename);
}

int main() {
    std::cout << "=== Move-Only Types ===\n\n";

    // 1. unique_ptr (built-in move-only type)
    std::cout << "1. UNIQUE_PTR (MOVE-ONLY):\n";
    {
        std::unique_ptr<int> p1 = std::make_unique<int>(42);
        std::cout << "p1 points to: " << *p1 << "\n";

        // std::unique_ptr<int> p2 = p1;  // ERROR: Can't copy

        std::cout << "\nMoving unique_ptr:\n";
        std::unique_ptr<int> p2 = std::move(p1);

        std::cout << "p1 is " << (p1 ? "valid" : "nullptr") << "\n";
        std::cout << "p2 points to: " << *p2 << "\n";
    }
    std::cout << "\n";

    // 2. Custom move-only file handle
    std::cout << "2. CUSTOM MOVE-ONLY TYPE (FILE HANDLE):\n";
    {
        FileHandle file1("test1.txt");
        file1.write("Hello, World!\n");

        // FileHandle file2 = file1;  // ERROR: Can't copy

        std::cout << "\nMoving file handle:\n";
        FileHandle file2 = std::move(file1);

        std::cout << "file1 is " << (file1.isOpen() ? "open" : "closed") << "\n";
        std::cout << "file2 is " << (file2.isOpen() ? "open" : "closed") << "\n";

        file2.write("More data\n");
    }
    std::cout << "\n";

    // 3. Move-only types in containers
    std::cout << "3. MOVE-ONLY TYPES IN CONTAINERS:\n";
    {
        std::vector<std::unique_ptr<int>> vec;

        std::cout << "Adding unique_ptrs to vector:\n";
        vec.push_back(std::make_unique<int>(10));
        vec.push_back(std::make_unique<int>(20));
        vec.emplace_back(std::make_unique<int>(30));

        auto ptr = std::make_unique<int>(40);
        vec.push_back(std::move(ptr));  // Must use std::move

        std::cout << "Vector contents:\n";
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << "  [" << i << "]: " << *vec[i] << "\n";
        }
    }
    std::cout << "\n";

    // 4. Resource manager
    std::cout << "4. MOVE-ONLY RESOURCE MANAGER:\n";
    {
        ResourceManager rm1("RM1", 100);
        rm1.fillBuffer(42);

        std::cout << "\nMoving resource manager:\n";
        ResourceManager rm2 = std::move(rm1);

        std::cout << "rm2: " << rm2.getName() << ", size=" << rm2.getSize() << "\n";
    }
    std::cout << "\n";

    // 5. Factory functions returning move-only types
    std::cout << "5. FACTORY FUNCTIONS:\n";
    {
        std::cout << "Creating resource via factory:\n";
        auto resource = createResource(99);
        std::cout << "Resource value: " << *resource << "\n";

        std::cout << "\nCreating file via factory:\n";
        FileHandle file = createFile("factory.txt");
        file.write("Created by factory\n");
    }
    std::cout << "\n";

    // 6. Connection example
    std::cout << "6. MOVE-ONLY CONNECTION:\n";
    {
        Connection conn1;
        conn1.send("Message 1");

        std::cout << "\nMoving connection:\n";
        Connection conn2 = std::move(conn1);

        std::cout << "conn1 is " << (conn1.isActive() ? "active" : "inactive") << "\n";
        std::cout << "conn2 is " << (conn2.isActive() ? "active" : "inactive") << "\n";

        conn2.send("Message 2");
    }
    std::cout << "\n";

    // 7. Vector of move-only connections
    std::cout << "7. VECTOR OF CONNECTIONS:\n";
    {
        std::vector<Connection> pool;

        std::cout << "Creating connection pool:\n";
        for (int i = 0; i < 3; ++i) {
            pool.push_back(Connection());
        }

        std::cout << "\nUsing connections:\n";
        for (auto& conn : pool) {
            conn.send("Data");
        }
    }
    std::cout << "\n";

    // 8. Transferring ownership
    std::cout << "8. TRANSFERRING OWNERSHIP:\n";
    {
        auto processResource = [](std::unique_ptr<int> ptr) {
            std::cout << "  Processing: " << *ptr << "\n";
            *ptr = 100;
            return ptr;  // Return ownership
        };

        std::cout << "Creating resource:\n";
        auto res = std::make_unique<int>(42);

        std::cout << "\nTransferring to function:\n";
        res = processResource(std::move(res));

        std::cout << "After processing: " << *res << "\n";
    }
    std::cout << "\n";

    // 9. Move-only lambda captures
    std::cout << "9. MOVE-ONLY LAMBDA CAPTURES:\n";
    {
        auto ptr = std::make_unique<int>(42);

        // Capture by move
        auto lambda = [p = std::move(ptr)]() {
            std::cout << "  Lambda has ownership: " << *p << "\n";
        };

        std::cout << "ptr is " << (ptr ? "valid" : "nullptr") << "\n";
        lambda();
    }
    std::cout << "\n";

    // 10. std::thread (move-only)
    std::cout << "10. STD::THREAD (MOVE-ONLY):\n";
    {
        auto threadFunc = []() {
            std::cout << "  Thread executing\n";
        };

        std::thread t1(threadFunc);

        // std::thread t2 = t1;  // ERROR: Can't copy thread

        std::cout << "Moving thread:\n";
        std::thread t2 = std::move(t1);

        std::cout << "t1 is " << (t1.joinable() ? "joinable" : "not joinable") << "\n";
        std::cout << "t2 is " << (t2.joinable() ? "joinable" : "not joinable") << "\n";

        t2.join();
    }
    std::cout << "\n";

    // 11. When to use move-only types
    std::cout << "11. WHEN TO USE MOVE-ONLY TYPES:\n";
    std::cout << "Use move-only types when:\n";
    std::cout << "  - Copying doesn't make sense (file handles, connections)\n";
    std::cout << "  - Unique ownership is required (unique_ptr)\n";
    std::cout << "  - Resource is expensive to copy\n";
    std::cout << "  - Want to prevent accidental copies\n";
    std::cout << "  - Implementing RAII wrappers\n\n";

    // 12. How to make types move-only
    std::cout << "12. MAKING TYPES MOVE-ONLY:\n";
    std::cout << "class MoveOnly {\n";
    std::cout << "public:\n";
    std::cout << "    // Delete copy operations\n";
    std::cout << "    MoveOnly(const MoveOnly&) = delete;\n";
    std::cout << "    MoveOnly& operator=(const MoveOnly&) = delete;\n\n";
    std::cout << "    // Define or default move operations\n";
    std::cout << "    MoveOnly(MoveOnly&&) noexcept = default;\n";
    std::cout << "    MoveOnly& operator=(MoveOnly&&) noexcept = default;\n";
    std::cout << "};\n\n";

    // 13. Benefits of move-only types
    std::cout << "13. BENEFITS:\n";
    std::cout << "1. Clear ownership semantics\n";
    std::cout << "2. Prevent accidental copies\n";
    std::cout << "3. Zero-cost abstraction (no copying overhead)\n";
    std::cout << "4. Compile-time enforcement of unique ownership\n";
    std::cout << "5. Better resource management\n";
    std::cout << "6. Explicit ownership transfer\n";

    return 0;
}
