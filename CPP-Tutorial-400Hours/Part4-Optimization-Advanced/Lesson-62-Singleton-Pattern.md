# Lesson 62: Singleton Pattern

**Duration**: 6 hours
**Difficulty**: Advanced
**Pattern Type**: Creational
**Part**: 4 - Optimization & Advanced Techniques

## Table of Contents
1. What is the Singleton Pattern?
2. When to Use Singleton
3. Basic Implementation
4. Thread-Safe Implementations
5. Meyer's Singleton (C++11)
6. Lazy vs Eager Initialization
7. Real-World Examples
8. Singleton Pros and Cons
9. Common Mistakes and Solutions
10. Complete Working Examples
11. Exercises

---

## 1. What is the Singleton Pattern?

The **Singleton Pattern** ensures that **a class has only one instance** and provides a **global point of access** to that instance.

### Intent:
- Ensure a class has only one instance
- Provide global access to that instance
- Control instantiation (prevent new instances)

### Structure:

```
┌─────────────────────┐
│    Singleton        │
├─────────────────────┤
│ - instance: static  │  ← Static instance
│ - constructor()     │  ← Private constructor
├─────────────────────┤
│ + getInstance()     │  ← Public static method
│ + businessLogic()   │
└─────────────────────┘
```

### Real-World Analogy:

**Government Example**: A country has only one president at a time. Everyone knows how to access the president (through official channels), but there's only ever one instance of the president role.

**Office Example**: A single printer shared by all employees. Everyone can send print jobs to it, but there's only one printer instance.

---

## 2. When to Use Singleton

### ✅ Use Singleton When:

1. **Database Connection Pool**
   - Only one pool managing all connections
   - Expensive to create multiple pools

2. **Logger System**
   - Single log file, coordinated writes
   - Prevent file access conflicts

3. **Configuration Manager**
   - One source of truth for settings
   - Load config file once

4. **Hardware Interface Controller**
   - GPU, printer, or device driver
   - Only one instance can control hardware

5. **Cache Manager**
   - Centralized cache across application
   - Coordinate cache invalidation

### ❌ Don't Use Singleton When:

1. **You just want a global variable** (use namespace instead)
2. **Testing needs multiple instances** (makes unit testing hard)
3. **State should be isolated** (creates hidden dependencies)
4. **You might need multiple instances later** (hard to refactor)

---

## 3. Basic Implementation

### Naive Implementation (Not Thread-Safe):

```cpp
#include <iostream>
using namespace std;

class Singleton {
private:
    // Private static instance
    static Singleton* instance;

    // Private constructor (prevents external instantiation)
    Singleton() {
        cout << "Singleton instance created!\n";
    }

    // Delete copy constructor and assignment operator
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    // Static method to get the single instance
    static Singleton* GetInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    void DoSomething() {
        cout << "Doing something...\n";
    }

    // Cleanup (optional)
    static void Destroy() {
        delete instance;
        instance = nullptr;
    }
};

// Initialize static member
Singleton* Singleton::instance = nullptr;

int main() {
    // Get the singleton instance
    Singleton* s1 = Singleton::GetInstance();
    Singleton* s2 = Singleton::GetInstance();

    cout << "s1 address: " << s1 << endl;
    cout << "s2 address: " << s2 << endl;
    cout << "Same instance? " << (s1 == s2 ? "Yes" : "No") << endl;

    s1->DoSomething();
    s2->DoSomething();

    Singleton::Destroy();

    return 0;
}
```

**Output:**
```
Singleton instance created!
s1 address: 0x1a2b3c4d
s2 address: 0x1a2b3c4d
Same instance? Yes
Doing something...
Doing something...
```

### Problems with This Implementation:

1. **❌ Not thread-safe** - Multiple threads can create multiple instances
2. **❌ Memory leak** - Relies on manual Destroy() call
3. **❌ No control over destruction order**

---

## 4. Thread-Safe Implementations

### 4.1 Double-Checked Locking Pattern (DCLP)

```cpp
#include <iostream>
#include <mutex>
using namespace std;

class ThreadSafeSingleton {
private:
    static ThreadSafeSingleton* instance;
    static mutex mtx;

    int value;

    ThreadSafeSingleton() : value(0) {
        cout << "ThreadSafeSingleton created\n";
    }

    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;

public:
    static ThreadSafeSingleton* GetInstance() {
        // First check (without locking) - for performance
        if (instance == nullptr) {
            lock_guard<mutex> lock(mtx);  // Lock only if needed

            // Second check (with locking) - for thread safety
            if (instance == nullptr) {
                instance = new ThreadSafeSingleton();
            }
        }
        return instance;
    }

    void SetValue(int v) { value = v; }
    int GetValue() const { return value; }

    static void Destroy() {
        lock_guard<mutex> lock(mtx);
        delete instance;
        instance = nullptr;
    }
};

// Initialize static members
ThreadSafeSingleton* ThreadSafeSingleton::instance = nullptr;
mutex ThreadSafeSingleton::mtx;

// Example usage with threads
#include <thread>
#include <vector>

void ThreadFunction(int id) {
    ThreadSafeSingleton* singleton = ThreadSafeSingleton::GetInstance();
    cout << "Thread " << id << " got instance at: " << singleton << endl;
    singleton->SetValue(id);
    cout << "Thread " << id << " set value to: " << singleton->GetValue() << endl;
}

int main() {
    cout << "=== Thread-Safe Singleton Test ===\n\n";

    vector<thread> threads;

    // Create 5 threads trying to get the singleton
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(ThreadFunction, i);
    }

    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }

    ThreadSafeSingleton::Destroy();

    return 0;
}
```

**Output:**
```
=== Thread-Safe Singleton Test ===

ThreadSafeSingleton created
Thread 0 got instance at: 0x1a2b3c4d
Thread 1 got instance at: 0x1a2b3c4d
Thread 0 set value to: 0
Thread 2 got instance at: 0x1a2b3c4d
Thread 1 set value to: 1
Thread 3 got instance at: 0x1a2b3c4d
Thread 2 set value to: 2
Thread 4 got instance at: 0x1a2b3c4d
Thread 3 set value to: 3
Thread 4 set value to: 4
```

---

## 5. Meyer's Singleton (C++11) - RECOMMENDED

**Best Practice**: Use Meyer's Singleton with static local variable:

```cpp
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

class MeyersSingleton {
private:
    int value;

    // Private constructor
    MeyersSingleton() : value(42) {
        cout << "MeyersSingleton created\n";
    }

    // Delete copy and assignment
    MeyersSingleton(const MeyersSingleton&) = delete;
    MeyersSingleton& operator=(const MeyersSingleton&) = delete;

public:
    // C++11 guarantees thread-safe initialization of static local variables
    static MeyersSingleton& GetInstance() {
        static MeyersSingleton instance;  // Created on first call
        return instance;
    }

    void SetValue(int v) { value = v; }
    int GetValue() const { return value; }

    ~MeyersSingleton() {
        cout << "MeyersSingleton destroyed\n";
    }
};

void TestThread(int id) {
    MeyersSingleton& singleton = MeyersSingleton::GetInstance();
    cout << "Thread " << id << " accessing singleton\n";
}

int main() {
    cout << "=== Meyer's Singleton Test ===\n\n";

    // First access
    MeyersSingleton& s1 = MeyersSingleton::GetInstance();
    cout << "Initial value: " << s1.GetValue() << "\n\n";

    s1.SetValue(100);

    // Second access
    MeyersSingleton& s2 = MeyersSingleton::GetInstance();
    cout << "Value from s2: " << s2.GetValue() << "\n\n";

    cout << "Are they the same? " << (&s1 == &s2 ? "Yes" : "No") << "\n\n";

    // Thread safety test
    cout << "Testing with multiple threads:\n";
    vector<thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(TestThread, i);
    }
    for (auto& t : threads) {
        t.join();
    }

    cout << "\nExiting main...\n";
    return 0;
    // Singleton automatically destroyed at program exit
}
```

**Output:**
```
=== Meyer's Singleton Test ===

MeyersSingleton created
Initial value: 42

Value from s2: 100

Are they the same? Yes

Testing with multiple threads:
Thread 0 accessing singleton
Thread 1 accessing singleton
Thread 2 accessing singleton
Thread 3 accessing singleton
Thread 4 accessing singleton

Exiting main...
MeyersSingleton destroyed
```

### Why Meyer's Singleton is Best:

1. ✅ **Thread-safe** (guaranteed by C++11 standard)
2. ✅ **Lazy initialization** (created on first use)
3. ✅ **Automatic destruction** (no memory leaks)
4. ✅ **Simple and clean** (no manual locking code)
5. ✅ **No heap allocation** (static local variable)

---

## 6. Lazy vs Eager Initialization

### Lazy Initialization (On-Demand):

```cpp
class LazySingleton {
public:
    static LazySingleton& GetInstance() {
        static LazySingleton instance;  // Created on first call
        return instance;
    }
private:
    LazySingleton() { cout << "Lazy: Created when first needed\n"; }
};

int main() {
    cout << "Program started\n";
    cout << "Before accessing singleton...\n";

    LazySingleton::GetInstance();  // Created HERE

    cout << "After accessing singleton\n";
}
```

**Output:**
```
Program started
Before accessing singleton...
Lazy: Created when first needed
After accessing singleton
```

### Eager Initialization (At Program Start):

```cpp
class EagerSingleton {
private:
    static EagerSingleton instance;  // Created at program start

    EagerSingleton() { cout << "Eager: Created at program start\n"; }

public:
    static EagerSingleton& GetInstance() {
        return instance;
    }
};

// Initialize immediately
EagerSingleton EagerSingleton::instance;

int main() {
    cout << "Program started\n";
    cout << "Before accessing singleton...\n";

    EagerSingleton::GetInstance();  // Already exists

    cout << "After accessing singleton\n";
}
```

**Output:**
```
Eager: Created at program start
Program started
Before accessing singleton...
After accessing singleton
```

### Comparison:

| Aspect | Lazy | Eager |
|--------|------|-------|
| **Created** | On first use | At program start |
| **Memory** | Only if used | Always allocated |
| **Startup** | Faster | Slower |
| **Complexity** | Slightly more complex | Simple |
| **Best for** | Heavy objects | Lightweight, always needed |

---

## 7. Real-World Examples

### Example 1: Logger System

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <ctime>
using namespace std;

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    ofstream logFile;
    mutex logMutex;
    LogLevel minLevel;

    Logger() : minLevel(LogLevel::INFO) {
        logFile.open("application.log", ios::app);
        if (logFile.is_open()) {
            Log(LogLevel::INFO, "Logger initialized");
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            Log(LogLevel::INFO, "Logger shutting down");
            logFile.close();
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    string GetCurrentTime() {
        time_t now = time(nullptr);
        char buf[26];
        ctime_s(buf, sizeof(buf), &now);
        string timeStr(buf);
        timeStr.pop_back();  // Remove newline
        return timeStr;
    }

    string LevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

public:
    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    void SetMinLevel(LogLevel level) {
        minLevel = level;
    }

    void Log(LogLevel level, const string& message) {
        if (level < minLevel) return;

        lock_guard<mutex> lock(logMutex);

        string logEntry = "[" + GetCurrentTime() + "] " +
                         "[" + LevelToString(level) + "] " +
                         message;

        if (logFile.is_open()) {
            logFile << logEntry << endl;
        }

        // Also print to console
        cout << logEntry << endl;
    }

    void Info(const string& message) { Log(LogLevel::INFO, message); }
    void Warning(const string& message) { Log(LogLevel::WARNING, message); }
    void Error(const string& message) { Log(LogLevel::ERROR, message); }
};

// Example usage
int main() {
    Logger& logger = Logger::GetInstance();

    logger.Info("Application started");
    logger.Warning("Low memory warning");
    logger.Error("Failed to connect to database");

    // Can access from anywhere
    Logger::GetInstance().Info("Processing data...");

    logger.Info("Application shutting down");

    return 0;
}
```

**Output:**
```
[Mon Nov 18 10:30:45 2025] [INFO] Logger initialized
[Mon Nov 18 10:30:45 2025] [INFO] Application started
[Mon Nov 18 10:30:45 2025] [WARNING] Low memory warning
[Mon Nov 18 10:30:45 2025] [ERROR] Failed to connect to database
[Mon Nov 18 10:30:45 2025] [INFO] Processing data...
[Mon Nov 18 10:30:45 2025] [INFO] Application shutting down
[Mon Nov 18 10:30:45 2025] [INFO] Logger shutting down
```

### Example 2: Configuration Manager

```cpp
#include <iostream>
#include <map>
#include <string>
#include <fstream>
using namespace std;

class ConfigManager {
private:
    map<string, string> settings;

    ConfigManager() {
        LoadDefaults();
        // In real app, would load from file
    }

    void LoadDefaults() {
        settings["app_name"] = "MyApplication";
        settings["version"] = "1.0.0";
        settings["max_connections"] = "100";
        settings["timeout"] = "30";
        settings["debug_mode"] = "false";
    }

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

public:
    static ConfigManager& GetInstance() {
        static ConfigManager instance;
        return instance;
    }

    string GetSetting(const string& key) const {
        auto it = settings.find(key);
        if (it != settings.end()) {
            return it->second;
        }
        return "";
    }

    int GetSettingAsInt(const string& key) const {
        string value = GetSetting(key);
        return value.empty() ? 0 : stoi(value);
    }

    bool GetSettingAsBool(const string& key) const {
        string value = GetSetting(key);
        return value == "true" || value == "1";
    }

    void SetSetting(const string& key, const string& value) {
        settings[key] = value;
    }

    void PrintAllSettings() const {
        cout << "=== Configuration Settings ===\n";
        for (const auto& pair : settings) {
            cout << pair.first << " = " << pair.second << endl;
        }
        cout << "==============================\n";
    }
};

// Usage example
class DatabaseConnection {
public:
    void Connect() {
        ConfigManager& config = ConfigManager::GetInstance();

        int maxConn = config.GetSettingAsInt("max_connections");
        int timeout = config.GetSettingAsInt("timeout");

        cout << "Connecting to database...\n";
        cout << "Max connections: " << maxConn << "\n";
        cout << "Timeout: " << timeout << " seconds\n";
    }
};

class Application {
public:
    void Run() {
        ConfigManager& config = ConfigManager::GetInstance();

        cout << "Starting " << config.GetSetting("app_name")
             << " v" << config.GetSetting("version") << "\n\n";

        if (config.GetSettingAsBool("debug_mode")) {
            cout << "Debug mode enabled\n";
        }

        DatabaseConnection db;
        db.Connect();
    }
};

int main() {
    // Get config instance
    ConfigManager& config = ConfigManager::GetInstance();

    // Show all settings
    config.PrintAllSettings();
    cout << "\n";

    // Change a setting
    config.SetSetting("debug_mode", "true");

    // Run application
    Application app;
    app.Run();

    return 0;
}
```

**Output:**
```
=== Configuration Settings ===
app_name = MyApplication
debug_mode = false
max_connections = 100
timeout = 30
version = 1.0.0
==============================

Starting MyApplication v1.0.0

Debug mode enabled
Connecting to database...
Max connections: 100
Timeout: 30 seconds
```

### Example 3: Resource Manager (Game Development)

```cpp
#include <iostream>
#include <map>
#include <string>
#include <memory>
using namespace std;

// Simulated texture class
class Texture {
    string filename;
    int width, height;

public:
    Texture(const string& file, int w, int h)
        : filename(file), width(w), height(h) {
        cout << "Loading texture: " << filename << " (" << width << "x" << height << ")\n";
    }

    ~Texture() {
        cout << "Unloading texture: " << filename << "\n";
    }

    string GetFilename() const { return filename; }
};

class ResourceManager {
private:
    map<string, shared_ptr<Texture>> textures;
    int totalLoaded;

    ResourceManager() : totalLoaded(0) {
        cout << "ResourceManager initialized\n";
    }

    ~ResourceManager() {
        cout << "ResourceManager shutting down. Total resources loaded: " << totalLoaded << "\n";
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

public:
    static ResourceManager& GetInstance() {
        static ResourceManager instance;
        return instance;
    }

    shared_ptr<Texture> LoadTexture(const string& filename, int width, int height) {
        // Check if already loaded
        auto it = textures.find(filename);
        if (it != textures.end()) {
            cout << "Texture '" << filename << "' already loaded. Reusing.\n";
            return it->second;
        }

        // Load new texture
        auto texture = make_shared<Texture>(filename, width, height);
        textures[filename] = texture;
        totalLoaded++;

        return texture;
    }

    void UnloadTexture(const string& filename) {
        auto it = textures.find(filename);
        if (it != textures.end()) {
            textures.erase(it);
            cout << "Texture '" << filename << "' unloaded from cache\n";
        }
    }

    void PrintLoadedResources() {
        cout << "\n=== Loaded Resources (" << textures.size() << ") ===\n";
        for (const auto& pair : textures) {
            cout << "  - " << pair.first << "\n";
        }
        cout << "===========================\n\n";
    }
};

int main() {
    ResourceManager& rm = ResourceManager::GetInstance();

    cout << "\n=== Game Starting ===\n\n";

    // Load textures
    auto playerTexture = rm.LoadTexture("player.png", 64, 64);
    auto enemyTexture = rm.LoadTexture("enemy.png", 32, 32);
    auto backgroundTexture = rm.LoadTexture("background.jpg", 1920, 1080);

    rm.PrintLoadedResources();

    // Try to load player texture again (should reuse)
    auto playerTexture2 = rm.LoadTexture("player.png", 64, 64);

    rm.PrintLoadedResources();

    // Unload enemy texture
    rm.UnloadTexture("enemy.png");

    rm.PrintLoadedResources();

    cout << "=== Game Ending ===\n\n";

    return 0;
}
```

**Output:**
```
ResourceManager initialized

=== Game Starting ===

Loading texture: player.png (64x64)
Loading texture: enemy.png (32x32)
Loading texture: background.jpg (1920x1080)

=== Loaded Resources (3) ===
  - background.jpg
  - enemy.png
  - player.png
===========================

Texture 'player.png' already loaded. Reusing.

=== Loaded Resources (3) ===
  - background.jpg
  - enemy.png
  - player.png
===========================

Unloading texture: enemy.png
Texture 'enemy.png' unloaded from cache

=== Loaded Resources (2) ===
  - background.jpg
  - player.png
===========================

=== Game Ending ===

Unloading texture: background.jpg
Unloading texture: player.png
ResourceManager shutting down. Total resources loaded: 3
```

---

## 8. Singleton Pros and Cons

### Advantages:

1. ✅ **Controlled access** - Only one instance guaranteed
2. ✅ **Lazy initialization** - Created when needed
3. ✅ **Global access point** - Easy to access from anywhere
4. ✅ **Reduced namespace pollution** - No global variables
5. ✅ **Permits refinement** - Can subclass the singleton

### Disadvantages:

1. ❌ **Difficult to test** - Hard to mock or replace
2. ❌ **Hidden dependencies** - Classes depend on global state
3. ❌ **Violates Single Responsibility** - Controls both instance and business logic
4. ❌ **Concurrency issues** - If not implemented correctly
5. ❌ **Difficult to extend** - Subclassing can be problematic

---

## 9. Common Mistakes and Solutions

### Mistake 1: Not Deleting Copy Constructor

```cpp
// ❌ BAD: Can be copied
class BadSingleton {
private:
    static BadSingleton* instance;
    BadSingleton() {}

public:
    static BadSingleton* GetInstance() {
        if (instance == nullptr) {
            instance = new BadSingleton();
        }
        return instance;
    }
    // Missing: deleted copy constructor!
};

// ✅ GOOD: Delete copy operations
class GoodSingleton {
private:
    GoodSingleton() {}

    // Prevent copying
    GoodSingleton(const GoodSingleton&) = delete;
    GoodSingleton& operator=(const GoodSingleton&) = delete;

public:
    static GoodSingleton& GetInstance() {
        static GoodSingleton instance;
        return instance;
    }
};
```

### Mistake 2: Returning Pointer Instead of Reference

```cpp
// ❌ BAD: Pointer can be deleted by user
class BadSingleton {
public:
    static BadSingleton* GetInstance() {
        static BadSingleton instance;
        return &instance;
    }
};

// User can (incorrectly) do:
// delete BadSingleton::GetInstance();  // Disaster!

// ✅ GOOD: Return reference
class GoodSingleton {
public:
    static GoodSingleton& GetInstance() {
        static GoodSingleton instance;
        return instance;  // Can't delete a reference
    }
};
```

### Mistake 3: Not Thread-Safe

```cpp
// ❌ BAD: Race condition possible
class BadSingleton {
private:
    static BadSingleton* instance;

public:
    static BadSingleton* GetInstance() {
        if (instance == nullptr) {  // Thread A and B both see nullptr
            instance = new BadSingleton();  // Both create instances!
        }
        return instance;
    }
};

// ✅ GOOD: Use Meyer's Singleton (thread-safe in C++11+)
class GoodSingleton {
public:
    static GoodSingleton& GetInstance() {
        static GoodSingleton instance;  // Thread-safe initialization
        return instance;
    }
};
```

---

## 10. Complete Working Examples

### Complete Example: Print Spooler System

```cpp
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

struct PrintJob {
    int id;
    string document;
    int pages;

    PrintJob(int i, const string& doc, int p)
        : id(i), document(doc), pages(p) {}
};

class PrintSpooler {
private:
    queue<PrintJob> printQueue;
    mutex queueMutex;
    int jobCounter;
    bool isPrinting;

    PrintSpooler() : jobCounter(0), isPrinting(false) {
        cout << "Print Spooler initialized\n";
    }

    ~PrintSpooler() {
        cout << "Print Spooler shutting down\n";
    }

    PrintSpooler(const PrintSpooler&) = delete;
    PrintSpooler& operator=(const PrintSpooler&) = delete;

    void ProcessPrintJob(const PrintJob& job) {
        cout << "\n>>> Printing Job #" << job.id << ": " << job.document << "\n";
        cout << "    Pages: " << job.pages << "\n";

        // Simulate printing time
        this_thread::sleep_for(chrono::milliseconds(job.pages * 100));

        cout << "<<< Job #" << job.id << " completed\n";
    }

public:
    static PrintSpooler& GetInstance() {
        static PrintSpooler instance;
        return instance;
    }

    int AddJob(const string& document, int pages) {
        lock_guard<mutex> lock(queueMutex);

        int jobId = ++jobCounter;
        printQueue.emplace(jobId, document, pages);

        cout << "[Added] Job #" << jobId << ": " << document
             << " (" << pages << " pages) - Queue size: " << printQueue.size() << "\n";

        return jobId;
    }

    void ProcessQueue() {
        while (true) {
            PrintJob job(0, "", 0);
            bool hasJob = false;

            {
                lock_guard<mutex> lock(queueMutex);
                if (!printQueue.empty()) {
                    job = printQueue.front();
                    printQueue.pop();
                    hasJob = true;
                }
            }

            if (hasJob) {
                ProcessPrintJob(job);
            } else {
                break;  // No more jobs
            }
        }
    }

    int GetQueueSize() {
        lock_guard<mutex> lock(queueMutex);
        return printQueue.size();
    }
};

int main() {
    cout << "=== Print Spooler System ===\n\n";

    PrintSpooler& spooler = PrintSpooler::GetInstance();

    // Multiple users adding print jobs
    spooler.AddJob("Report.pdf", 10);
    spooler.AddJob("Presentation.pptx", 25);
    spooler.AddJob("Invoice.docx", 3);
    spooler.AddJob("Contract.pdf", 15);

    cout << "\nTotal jobs in queue: " << spooler.GetQueueSize() << "\n";

    cout << "\n=== Starting Print Processing ===\n";
    spooler.ProcessQueue();

    cout << "\n=== All Jobs Completed ===\n";
    cout << "Jobs remaining: " << spooler.GetQueueSize() << "\n";

    return 0;
}
```

**Output:**
```
=== Print Spooler System ===

Print Spooler initialized
[Added] Job #1: Report.pdf (10 pages) - Queue size: 1
[Added] Job #2: Presentation.pptx (25 pages) - Queue size: 2
[Added] Job #3: Invoice.docx (3 pages) - Queue size: 3
[Added] Job #4: Contract.pdf (15 pages) - Queue size: 4

Total jobs in queue: 4

=== Starting Print Processing ===

>>> Printing Job #1: Report.pdf
    Pages: 10
<<< Job #1 completed

>>> Printing Job #2: Presentation.pptx
    Pages: 25
<<< Job #2 completed

>>> Printing Job #3: Invoice.docx
    Pages: 3
<<< Job #3 completed

>>> Printing Job #4: Contract.pdf
    Pages: 15
<<< Job #4 completed

=== All Jobs Completed ===
Jobs remaining: 0
Print Spooler shutting down
```

---

## 11. Exercises

### Exercise 1: Implement Database Connection Pool

Create a Singleton `ConnectionPool` class that:
- Maintains a pool of 5 database connections
- Allows acquiring and releasing connections
- Thread-safe operations

### Exercise 2: Application Settings

Create a Singleton `AppSettings` class that:
- Loads settings from a file (simulate with hardcoded values)
- Provides getters for different setting types (int, string, bool)
- Allows runtime modification of settings

### Exercise 3: Thread-Safety Testing

Modify the naive singleton to test thread safety:
- Create 100 threads simultaneously accessing GetInstance()
- Count how many instances get created
- Fix the issue using Meyer's Singleton

---

## Key Takeaways

1. ✅ Singleton ensures **only one instance** of a class exists
2. ✅ Use **Meyer's Singleton** (static local variable) in C++11+
3. ✅ Meyer's Singleton is **thread-safe** by standard guarantee
4. ✅ Always **delete copy constructor and assignment operator**
5. ✅ Return **reference**, not pointer, to prevent deletion
6. ✅ Use for **Logger, Config, Resource Manager, Device Drivers**
7. ✅ **Don't overuse** - makes testing difficult
8. ✅ Singleton is a **creational pattern**

---

## Next Lesson Preview

In **Lesson 63**, we'll learn about the **Factory Pattern**:
- Creating objects without specifying exact class
- Encapsulating object creation
- Product hierarchies
- Virtual constructors
- Real-world examples (UI components, document types)

---

## Summary Checklist

Before moving to Lesson 63, ensure you can:

- [ ] Explain what the Singleton pattern is
- [ ] Implement Meyer's Singleton
- [ ] Understand thread-safety concerns
- [ ] Delete copy constructor and assignment operator
- [ ] Identify when to use (and not use) Singleton
- [ ] Create a Logger using Singleton
- [ ] Understand lazy vs eager initialization

**Excellent work!** You now master the Singleton pattern!

Next up: **Factory Pattern** - creating objects without specifying exact classes!
