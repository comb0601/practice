# Lesson 16: Namespaces

**Duration**: 2.5-3 hours

## Overview

Namespaces are a feature in C++ that allows you to group related code under a named scope to avoid name conflicts. They are essential for organizing large codebases and preventing naming collisions between different libraries or modules.

## Topics Covered

1. Introduction to Namespaces
2. Declaring and Using Namespaces
3. Nested Namespaces
4. using Declarations and Directives
5. Anonymous (Unnamed) Namespaces
6. Namespace Aliases
7. Argument-Dependent Lookup (ADL)
8. Best Practices

## 1. Introduction to Namespaces

**Problem**: In large projects, different libraries might define functions, classes, or variables with the same name, causing conflicts.

**Solution**: Namespaces provide a way to organize code and prevent name collisions.

```cpp
// Without namespaces - name collision
int count;  // From library A
int count;  // From library B - ERROR!

// With namespaces - no collision
namespace LibraryA {
    int count;
}
namespace LibraryB {
    int count;
}
```

## 2. Declaring and Using Namespaces

### Example 1: Basic Namespace Usage - Complete Program

**File: `basic_namespaces.cpp`**

```cpp
#include <iostream>
#include <string>

// Declaring a namespace
namespace Math {
    const double PI = 3.14159265359;

    double add(double a, double b) {
        return a + b;
    }

    double subtract(double a, double b) {
        return a - b;
    }

    double multiply(double a, double b) {
        return a * b;
    }

    double divide(double a, double b) {
        if (b == 0) {
            std::cerr << "Error: Division by zero" << std::endl;
            return 0;
        }
        return a / b;
    }
}

// Another namespace
namespace String {
    std::string toUpper(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            c = std::toupper(c);
        }
        return result;
    }

    std::string toLower(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            c = std::tolower(c);
        }
        return result;
    }

    std::string repeat(const std::string& str, int times) {
        std::string result;
        for (int i = 0; i < times; ++i) {
            result += str;
        }
        return result;
    }
}

int main() {
    std::cout << "=== Basic Namespaces Demo ===" << std::endl;

    // Using namespace members with :: operator
    std::cout << "\n--- Math Namespace ---" << std::endl;
    std::cout << "PI = " << Math::PI << std::endl;
    std::cout << "5 + 3 = " << Math::add(5, 3) << std::endl;
    std::cout << "5 - 3 = " << Math::subtract(5, 3) << std::endl;
    std::cout << "5 * 3 = " << Math::multiply(5, 3) << std::endl;
    std::cout << "10 / 2 = " << Math::divide(10, 2) << std::endl;

    // Using String namespace
    std::cout << "\n--- String Namespace ---" << std::endl;
    std::string text = "Hello World";
    std::cout << "Original: " << text << std::endl;
    std::cout << "Upper: " << String::toUpper(text) << std::endl;
    std::cout << "Lower: " << String::toLower(text) << std::endl;
    std::cout << "Repeat 3x: " << String::repeat("Ha", 3) << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall basic_namespaces.cpp -o basic_namespaces
./basic_namespaces
```

## 3. Nested Namespaces

### Example 2: Nested Namespaces - Complete Program

**File: `nested_namespaces.cpp`**

```cpp
#include <iostream>
#include <string>
#include <vector>

// Nested namespaces - traditional syntax
namespace Company {
    namespace Product {
        namespace Version1 {
            void display() {
                std::cout << "Company::Product::Version1" << std::endl;
            }
        }

        namespace Version2 {
            void display() {
                std::cout << "Company::Product::Version2" << std::endl;
            }
        }
    }
}

// Nested namespaces - C++17 compact syntax
namespace Graphics::2D {
    struct Point {
        double x, y;
    };

    void printPoint(const Point& p) {
        std::cout << "2D Point(" << p.x << ", " << p.y << ")" << std::endl;
    }
}

namespace Graphics::3D {
    struct Point {
        double x, y, z;
    };

    void printPoint(const Point& p) {
        std::cout << "3D Point(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
    }
}

// Real-world example: Game engine structure
namespace GameEngine {
    namespace Rendering {
        namespace OpenGL {
            void initialize() {
                std::cout << "Initializing OpenGL renderer" << std::endl;
            }

            void render() {
                std::cout << "Rendering with OpenGL" << std::endl;
            }
        }

        namespace Vulkan {
            void initialize() {
                std::cout << "Initializing Vulkan renderer" << std::endl;
            }

            void render() {
                std::cout << "Rendering with Vulkan" << std::endl;
            }
        }
    }

    namespace Physics {
        void update(double deltaTime) {
            std::cout << "Updating physics (dt=" << deltaTime << ")" << std::endl;
        }

        void applyGravity() {
            std::cout << "Applying gravity" << std::endl;
        }
    }

    namespace Audio {
        void playSound(const std::string& soundName) {
            std::cout << "Playing sound: " << soundName << std::endl;
        }

        void stopAllSounds() {
            std::cout << "Stopping all sounds" << std::endl;
        }
    }
}

int main() {
    std::cout << "=== Nested Namespaces Demo ===" << std::endl;

    // Using nested namespaces
    std::cout << "\n--- Product Versions ---" << std::endl;
    Company::Product::Version1::display();
    Company::Product::Version2::display();

    // Using Graphics namespaces
    std::cout << "\n--- Graphics Points ---" << std::endl;
    Graphics::2D::Point point2d = {10.5, 20.3};
    Graphics::3D::Point point3d = {1.0, 2.0, 3.0};

    Graphics::2D::printPoint(point2d);
    Graphics::3D::printPoint(point3d);

    // Using Game Engine namespaces
    std::cout << "\n--- Game Engine ---" << std::endl;

    std::cout << "Rendering subsystem:" << std::endl;
    GameEngine::Rendering::OpenGL::initialize();
    GameEngine::Rendering::OpenGL::render();

    std::cout << "\nPhysics subsystem:" << std::endl;
    GameEngine::Physics::update(0.016);
    GameEngine::Physics::applyGravity();

    std::cout << "\nAudio subsystem:" << std::endl;
    GameEngine::Audio::playSound("explosion.wav");
    GameEngine::Audio::playSound("music.mp3");
    GameEngine::Audio::stopAllSounds();

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall nested_namespaces.cpp -o nested_namespaces
./nested_namespaces
```

## 4. using Declarations and Directives

### Example 3: using Keyword - Complete Program

**File: `using_keyword.cpp`**

```cpp
#include <iostream>
#include <string>
#include <vector>

namespace Math {
    double sqrt(double x) {
        // Simple approximation for demo
        return x / 2.0;  // Not accurate, just for demo
    }

    double power(double base, int exp) {
        double result = 1.0;
        for (int i = 0; i < exp; ++i) {
            result *= base;
        }
        return result;
    }

    double abs(double x) {
        return (x < 0) ? -x : x;
    }
}

namespace Database {
    class Connection {
    public:
        void connect() {
            std::cout << "Database connected" << std::endl;
        }

        void disconnect() {
            std::cout << "Database disconnected" << std::endl;
        }

        void executeQuery(const std::string& query) {
            std::cout << "Executing: " << query << std::endl;
        }
    };

    class Table {
    private:
        std::string name;
    public:
        Table(const std::string& n) : name(n) {}

        void insert(const std::string& data) {
            std::cout << "Inserting into " << name << ": " << data << std::endl;
        }

        void select() {
            std::cout << "Selecting from " << name << std::endl;
        }
    };
}

// Function demonstrating 'using declaration'
void demoUsingDeclaration() {
    std::cout << "\n--- using Declaration ---" << std::endl;

    // Bring specific names into current scope
    using Math::sqrt;
    using Math::power;

    // Can now use without Math:: prefix
    std::cout << "sqrt(16) = " << sqrt(16) << std::endl;
    std::cout << "power(2, 10) = " << power(2, 10) << std::endl;

    // Still need prefix for abs
    std::cout << "Math::abs(-42) = " << Math::abs(-42) << std::endl;
}

// Function demonstrating 'using directive'
void demoUsingDirective() {
    std::cout << "\n--- using Directive ---" << std::endl;

    // Bring all names from namespace into current scope
    using namespace Math;

    // All Math functions available without prefix
    std::cout << "sqrt(25) = " << sqrt(25) << std::endl;
    std::cout << "power(3, 4) = " << power(3, 4) << std::endl;
    std::cout << "abs(-100) = " << abs(-100) << std::endl;
}

// Function demonstrating scoped using
void demoScopedUsing() {
    std::cout << "\n--- Scoped using ---" << std::endl;

    {
        using namespace Database;
        Connection conn;
        conn.connect();
        conn.executeQuery("SELECT * FROM users");
        conn.disconnect();
    }

    // Connection not available here (out of scope)
    // Connection conn;  // ERROR: Connection not in scope
}

int main() {
    std::cout << "=== using Keyword Demo ===" << std::endl;

    // Demo 1: Explicit namespace qualification
    std::cout << "\n--- Without using ---" << std::endl;
    std::cout << "Math::sqrt(9) = " << Math::sqrt(9) << std::endl;
    std::cout << "Math::power(2, 8) = " << Math::power(2, 8) << std::endl;

    // Demo 2: using declaration
    demoUsingDeclaration();

    // Demo 3: using directive
    demoUsingDirective();

    // Demo 4: Scoped using
    demoScopedUsing();

    // Demo 5: using with classes
    std::cout << "\n--- using with Classes ---" << std::endl;
    using Database::Table;

    Table users("users");
    users.insert("John Doe");
    users.insert("Jane Smith");
    users.select();

    // Demonstration of why 'using namespace std' can be problematic
    std::cout << "\n--- Why Avoid 'using namespace std' in Headers ---" << std::endl;
    std::cout << "It can cause name collisions and ambiguity" << std::endl;
    std::cout << "Example: std::count vs custom count function" << std::endl;

    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "Vector size: " << numbers.size() << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall using_keyword.cpp -o using_keyword
./using_keyword
```

## 5. Anonymous (Unnamed) Namespaces

### Example 4: Anonymous Namespaces - Complete Program

**File: `anonymous_namespaces.cpp`**

```cpp
#include <iostream>
#include <string>

// Anonymous namespace - contents have internal linkage
// Similar to 'static' at file scope
namespace {
    // These functions are only visible in this translation unit
    int internalCounter = 0;

    void incrementCounter() {
        internalCounter++;
    }

    int getCounter() {
        return internalCounter;
    }

    // Helper function not meant to be exposed
    std::string formatMessage(const std::string& msg) {
        return "[INTERNAL] " + msg;
    }
}

// Public interface
namespace PublicAPI {
    void processData(const std::string& data) {
        std::cout << formatMessage("Processing: " + data) << std::endl;
        incrementCounter();
    }

    void showStats() {
        std::cout << "Operations performed: " << getCounter() << std::endl;
    }
}

// Another anonymous namespace (separate from the first)
namespace {
    class Logger {
    private:
        std::string prefix;

    public:
        Logger(const std::string& p) : prefix(p) {}

        void log(const std::string& message) {
            std::cout << "[" << prefix << "] " << message << std::endl;
        }
    };

    Logger globalLogger("APP");
}

// Demonstrating constants in anonymous namespace
namespace {
    const int MAX_CONNECTIONS = 100;
    const double TIMEOUT_SECONDS = 30.0;
    const std::string APP_VERSION = "1.0.0";
}

class Application {
public:
    void start() {
        globalLogger.log("Application starting");
        globalLogger.log("Version: " + APP_VERSION);
        globalLogger.log("Max connections: " + std::to_string(MAX_CONNECTIONS));
        globalLogger.log("Timeout: " + std::to_string(TIMEOUT_SECONDS) + "s");
    }

    void processItems() {
        PublicAPI::processData("Item 1");
        PublicAPI::processData("Item 2");
        PublicAPI::processData("Item 3");
    }

    void stop() {
        PublicAPI::showStats();
        globalLogger.log("Application stopping");
    }
};

int main() {
    std::cout << "=== Anonymous Namespaces Demo ===" << std::endl;

    Application app;

    app.start();

    std::cout << "\n--- Processing Data ---" << std::endl;
    app.processItems();

    std::cout << "\n--- Shutdown ---" << std::endl;
    app.stop();

    // Demonstration of anonymous namespace scope
    std::cout << "\n--- Anonymous Namespace Scope ---" << std::endl;
    std::cout << "Anonymous namespace contents are file-scoped" << std::endl;
    std::cout << "Similar to static linkage in C" << std::endl;
    std::cout << "Prevents naming conflicts across translation units" << std::endl;

    // Direct access to anonymous namespace members
    std::cout << "\n--- Direct Access ---" << std::endl;
    incrementCounter();
    incrementCounter();
    std::cout << "Counter value: " << getCounter() << std::endl;

    globalLogger.log("Direct log message");

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall anonymous_namespaces.cpp -o anonymous_namespaces
./anonymous_namespaces
```

## 6. Namespace Aliases

### Example 5: Namespace Aliases - Complete Program

**File: `namespace_aliases.cpp`**

```cpp
#include <iostream>
#include <string>
#include <vector>

// Long nested namespace names
namespace VeryLongCompanyName {
    namespace ProductLine {
        namespace ComponentVersion3 {
            namespace Utilities {
                class StringHelper {
                public:
                    static std::string reverse(const std::string& str) {
                        return std::string(str.rbegin(), str.rend());
                    }

                    static int count(const std::string& str, char c) {
                        int cnt = 0;
                        for (char ch : str) {
                            if (ch == c) cnt++;
                        }
                        return cnt;
                    }
                };

                class MathHelper {
                public:
                    static double average(const std::vector<double>& values) {
                        if (values.empty()) return 0.0;
                        double sum = 0.0;
                        for (double v : values) sum += v;
                        return sum / values.size();
                    }

                    static double max(const std::vector<double>& values) {
                        if (values.empty()) return 0.0;
                        double maxVal = values[0];
                        for (double v : values) {
                            if (v > maxVal) maxVal = v;
                        }
                        return maxVal;
                    }
                };
            }
        }
    }
}

// Create an alias for the long namespace
namespace Utils = VeryLongCompanyName::ProductLine::ComponentVersion3::Utilities;

// Another example with multiple versions
namespace MyLibrary {
    namespace v1 {
        void processData() {
            std::cout << "Processing with version 1 algorithm" << std::endl;
        }
    }

    namespace v2 {
        void processData() {
            std::cout << "Processing with version 2 algorithm (improved)" << std::endl;
        }
    }

    namespace v3 {
        void processData() {
            std::cout << "Processing with version 3 algorithm (optimized)" << std::endl;
        }
    }
}

// Alias to current version (easy to change)
namespace Current = MyLibrary::v3;

int main() {
    std::cout << "=== Namespace Aliases Demo ===" << std::endl;

    // Without alias - very verbose
    std::cout << "\n--- Without Alias (Verbose) ---" << std::endl;
    std::string text1 = "Hello";
    std::string reversed1 = VeryLongCompanyName::ProductLine::ComponentVersion3::Utilities::StringHelper::reverse(text1);
    std::cout << "Reversed: " << reversed1 << std::endl;

    // With alias - much cleaner
    std::cout << "\n--- With Alias (Clean) ---" << std::endl;
    std::string text2 = "World";
    std::string reversed2 = Utils::StringHelper::reverse(text2);
    std::cout << "Reversed: " << reversed2 << std::endl;

    // Using MathHelper through alias
    std::cout << "\n--- Math Helper ---" << std::endl;
    std::vector<double> values = {10.5, 20.3, 15.7, 8.2, 12.9};

    std::cout << "Values: ";
    for (double v : values) std::cout << v << " ";
    std::cout << std::endl;

    std::cout << "Average: " << Utils::MathHelper::average(values) << std::endl;
    std::cout << "Maximum: " << Utils::MathHelper::max(values) << std::endl;

    // String operations
    std::cout << "\n--- String Operations ---" << std::endl;
    std::string sentence = "programming";
    std::cout << "String: " << sentence << std::endl;
    std::cout << "Reversed: " << Utils::StringHelper::reverse(sentence) << std::endl;
    std::cout << "Count 'g': " << Utils::StringHelper::count(sentence, 'g') << std::endl;
    std::cout << "Count 'm': " << Utils::StringHelper::count(sentence, 'm') << std::endl;

    // Version selection with alias
    std::cout << "\n--- Version Selection ---" << std::endl;
    std::cout << "Version 1: ";
    MyLibrary::v1::processData();

    std::cout << "Version 2: ";
    MyLibrary::v2::processData();

    std::cout << "Version 3: ";
    MyLibrary::v3::processData();

    std::cout << "Current (aliased to v3): ";
    Current::processData();

    // Creating local aliases
    std::cout << "\n--- Local Aliases ---" << std::endl;
    {
        namespace Str = Utils::StringHelper;
        std::string test = "abcdef";
        std::cout << "Original: " << test << std::endl;
        std::cout << "Reversed: " << Str::reverse(test) << std::endl;
    }

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall namespace_aliases.cpp -o namespace_aliases
./namespace_aliases
```

## Complete Project 1: Module System with Namespaces

**File: `logger.h`**

```cpp
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>

namespace Logging {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    class Logger {
    private:
        LogLevel minLevel;
        std::ofstream logFile;
        bool consoleOutput;

        std::string getCurrentTime() {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
            return ss.str();
        }

        std::string levelToString(LogLevel level) {
            switch (level) {
                case LogLevel::DEBUG: return "DEBUG";
                case LogLevel::INFO: return "INFO";
                case LogLevel::WARNING: return "WARNING";
                case LogLevel::ERROR: return "ERROR";
                default: return "UNKNOWN";
            }
        }

    public:
        Logger(LogLevel level = LogLevel::INFO, bool console = true)
            : minLevel(level), consoleOutput(console) {}

        void setLogFile(const std::string& filename) {
            logFile.open(filename, std::ios::app);
        }

        void log(LogLevel level, const std::string& message) {
            if (level < minLevel) return;

            std::string logMessage = "[" + getCurrentTime() + "] "
                                   + "[" + levelToString(level) + "] "
                                   + message;

            if (consoleOutput) {
                if (level == LogLevel::ERROR || level == LogLevel::WARNING) {
                    std::cerr << logMessage << std::endl;
                } else {
                    std::cout << logMessage << std::endl;
                }
            }

            if (logFile.is_open()) {
                logFile << logMessage << std::endl;
                logFile.flush();
            }
        }

        void debug(const std::string& message) { log(LogLevel::DEBUG, message); }
        void info(const std::string& message) { log(LogLevel::INFO, message); }
        void warning(const std::string& message) { log(LogLevel::WARNING, message); }
        void error(const std::string& message) { log(LogLevel::ERROR, message); }

        ~Logger() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }
    };
}

#endif // LOGGER_H
```

**File: `database.h`**

```cpp
#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <map>
#include "logger.h"

namespace Database {
    namespace Internal {
        // Internal helper functions
        inline bool validateQuery(const std::string& query) {
            return !query.empty();
        }

        inline std::string sanitize(const std::string& input) {
            // Simple sanitization (just for demo)
            std::string result = input;
            // Remove semicolons
            result.erase(std::remove(result.begin(), result.end(), ';'), result.end());
            return result;
        }
    }

    class Connection {
    private:
        std::string connectionString;
        bool connected;
        Logging::Logger& logger;

    public:
        Connection(const std::string& connStr, Logging::Logger& log)
            : connectionString(connStr), connected(false), logger(log) {}

        bool connect() {
            logger.info("Connecting to database: " + connectionString);
            connected = true;
            logger.info("Successfully connected");
            return true;
        }

        void disconnect() {
            if (connected) {
                logger.info("Disconnecting from database");
                connected = false;
                logger.info("Disconnected");
            }
        }

        bool executeQuery(const std::string& query) {
            if (!connected) {
                logger.error("Not connected to database");
                return false;
            }

            if (!Internal::validateQuery(query)) {
                logger.error("Invalid query");
                return false;
            }

            std::string safeQuery = Internal::sanitize(query);
            logger.info("Executing query: " + safeQuery);

            // Simulate query execution
            logger.info("Query executed successfully");
            return true;
        }

        bool isConnected() const { return connected; }

        ~Connection() {
            disconnect();
        }
    };

    class Table {
    private:
        std::string name;
        std::vector<std::map<std::string, std::string>> rows;
        Logging::Logger& logger;

    public:
        Table(const std::string& tableName, Logging::Logger& log)
            : name(tableName), logger(log) {}

        void insert(const std::map<std::string, std::string>& row) {
            logger.info("Inserting row into table: " + name);
            rows.push_back(row);
            logger.info("Row inserted successfully");
        }

        void select() {
            logger.info("Selecting from table: " + name);
            logger.info("Found " + std::to_string(rows.size()) + " rows");

            for (size_t i = 0; i < rows.size(); ++i) {
                std::string rowStr = "Row " + std::to_string(i) + ": ";
                for (const auto& [key, value] : rows[i]) {
                    rowStr += key + "=" + value + " ";
                }
                logger.debug(rowStr);
            }
        }

        size_t count() const {
            return rows.size();
        }
    };
}

#endif // DATABASE_H
```

**File: `module_system.cpp`**

```cpp
#include <iostream>
#include <string>
#include "logger.h"
#include "database.h"

// Application namespace
namespace Application {
    class App {
    private:
        Logging::Logger logger;
        Database::Connection* dbConnection;

    public:
        App() : logger(Logging::LogLevel::DEBUG, true), dbConnection(nullptr) {
            logger.setLogFile("app.log");
        }

        void initialize() {
            logger.info("Initializing application");

            // Create database connection
            dbConnection = new Database::Connection("localhost:5432/mydb", logger);

            if (!dbConnection->connect()) {
                logger.error("Failed to connect to database");
                return;
            }

            logger.info("Application initialized successfully");
        }

        void run() {
            logger.info("Running application");

            // Execute some queries
            dbConnection->executeQuery("SELECT * FROM users");
            dbConnection->executeQuery("SELECT * FROM products");

            // Work with tables
            Database::Table users("users", logger);
            users.insert({{"id", "1"}, {"name", "John"}, {"email", "john@example.com"}});
            users.insert({{"id", "2"}, {"name", "Jane"}, {"email", "jane@example.com"}});
            users.insert({{"id", "3"}, {"name", "Bob"}, {"email", "bob@example.com"}});

            users.select();
            logger.info("Total users: " + std::to_string(users.count()));

            Database::Table products("products", logger);
            products.insert({{"id", "101"}, {"name", "Laptop"}, {"price", "999.99"}});
            products.insert({{"id", "102"}, {"name", "Mouse"}, {"price", "29.99"}});

            products.select();
            logger.info("Total products: " + std::to_string(products.count()));

            logger.info("Application execution completed");
        }

        void shutdown() {
            logger.info("Shutting down application");

            if (dbConnection) {
                delete dbConnection;
                dbConnection = nullptr;
            }

            logger.info("Application shut down successfully");
        }

        ~App() {
            if (dbConnection) {
                delete dbConnection;
            }
        }
    };
}

int main() {
    std::cout << "=== Module System with Namespaces Demo ===" << std::endl;

    // Create namespace aliases for convenience
    namespace Log = Logging;
    namespace DB = Database;
    namespace App = Application;

    std::cout << "\nStarting application...\n" << std::endl;

    App::App application;

    application.initialize();
    application.run();
    application.shutdown();

    std::cout << "\nApplication finished. Check 'app.log' for detailed logs." << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall module_system.cpp -o module_system
./module_system
cat app.log
```

## Best Practices

1. **Use namespaces to organize code**
   - Group related functionality together
   - Prevents naming conflicts

2. **Avoid `using namespace` in headers**
   - Can cause naming conflicts for users of your header
   - Use explicit qualification or `using` declarations instead

3. **Use anonymous namespaces for internal linkage**
   - Better than `static` for file-scope items
   - Works with all types (functions, variables, classes)

4. **Create namespace aliases for long names**
   - Improves code readability
   - Easy to update when namespaces change

5. **Use nested namespaces for hierarchical organization**
   - Reflects project structure
   - Use C++17 compact syntax: `namespace A::B::C`

6. **Prefer `using` declarations over directives**
   - More specific and controlled
   - Reduces chance of conflicts

7. **Document your namespace structure**
   - Helps users understand organization
   - Makes codebase navigation easier

## Common Pitfalls

1. **Overusing `using namespace std`**
   - Can lead to name collisions
   - Especially problematic in headers

2. **Too many nested levels**
   - Makes code harder to read
   - Use namespace aliases to simplify

3. **Inconsistent namespace usage**
   - Stick to a naming convention
   - Be consistent across the project

4. **Forgetting anonymous namespaces**
   - Internal helpers should be in anonymous namespace
   - Prevents ODR violations

## Summary

In this lesson, you learned:

- **Namespace basics** - Declaring and using namespaces to avoid name conflicts
- **Nested namespaces** - Organizing code hierarchically
- **using keyword** - Declarations vs directives and their proper usage
- **Anonymous namespaces** - Internal linkage for file-scope entities
- **Namespace aliases** - Simplifying long namespace names
- **Best practices** - When and how to use namespaces effectively

Namespaces are essential for organizing large C++ projects and preventing naming conflicts.

## Next Lesson

Lesson 17: STL Introduction

## Additional Resources

- C++ Reference: Namespaces
- C++ Core Guidelines: Namespaces
- Effective C++: Item 57 (Use explicit qualification or using-declarations)
- Practice: Refactor existing code to use namespaces

---

**Exercise**: Create a multi-module application with separate namespaces for UI, Business Logic, and Data Access layers, demonstrating proper namespace organization and usage.
