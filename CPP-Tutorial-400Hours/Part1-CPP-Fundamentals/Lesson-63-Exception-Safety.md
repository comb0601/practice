# Lesson 63: Exception Safety

**Duration**: 5 hours
**Difficulty**: Advanced

## Table of Contents
1. Exception Safety Guarantees
2. RAII Pattern
3. Strong Exception Guarantee
4. noexcept Specification
5. Best Practices
6. Exercises

---

## 1. Exception Safety Guarantees

### Three Levels:

```cpp
/*
1. Basic Guarantee:
   - No resource leaks
   - Object remains in valid state

2. Strong Guarantee:
   - Transaction semantics
   - Operation succeeds or no side effects

3. No-throw Guarantee:
   - Operation never throws
   - Mark with noexcept
*/
```

---

## 2. RAII Pattern

### Resource Acquisition Is Initialization:

```cpp
#include <iostream>
#include <fstream>
#include <stdexcept>

class FileHandler {
private:
    std::ofstream file;
public:
    FileHandler(const std::string& filename) {
        file.open(filename);
        if (!file) throw std::runtime_error("Cannot open file");
    }

    ~FileHandler() {
        if (file.is_open()) {
            file.close();
            std::cout << "File closed\n";
        }
    }

    void write(const std::string& data) {
        file << data;
        if (!file) throw std::runtime_error("Write failed");
    }
};

int main()
{
    try {
        FileHandler fh("output.txt");
        fh.write("Hello");
        throw std::runtime_error("Some error");
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        // File automatically closed by destructor
    }

    return 0;
}
```

---

## 3. noexcept Specification

### noexcept Keyword:

```cpp
#include <iostream>

class Resource {
public:
    Resource() noexcept {
        std::cout << "Resource acquired\n";
    }

    ~Resource() noexcept {
        std::cout << "Resource released\n";
    }

    void operation() noexcept {
        // Guaranteed not to throw
    }

    // Conditional noexcept
    void conditionalOp() noexcept(noexcept(operation())) {
        operation();
    }
};

int main()
{
    Resource r;
    r.operation();
    return 0;
}
```

---

## Key Takeaways

1. ✅ Three exception safety levels
2. ✅ RAII ensures cleanup in exceptions
3. ✅ Strong guarantee = commit or rollback
4. ✅ noexcept for non-throwing functions
5. ✅ Destructors should never throw
6. ✅ Use smart pointers for automatic cleanup

---

## Next Lesson Preview

In **Lesson 64**, we'll learn about:
- File input/output basics
- Reading and writing text files
- File streams
- File operations

**Great work on exception safety!**
