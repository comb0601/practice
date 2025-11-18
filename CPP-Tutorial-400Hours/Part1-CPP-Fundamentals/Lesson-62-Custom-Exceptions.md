# Lesson 62: Custom Exceptions

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Creating Custom Exception Classes
2. Inheriting from std::exception
3. Exception Hierarchies
4. Adding Custom Data
5. Best Practices
6. Exercises and Projects

---

## 1. Creating Custom Exception Classes

### Basic Custom Exception:

```cpp
#include <exception>
#include <string>
#include <iostream>

class MyException : public std::exception {
private:
    std::string message;
public:
    MyException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

int main()
{
    try {
        throw MyException("Custom error occurred");
    }
    catch (const MyException& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    return 0;
}
```

---

## 2. Exception Hierarchies

### Domain-Specific Exceptions:

```cpp
#include <exception>
#include <string>
#include <iostream>

class DatabaseException : public std::exception {
protected:
    std::string message;
public:
    DatabaseException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ConnectionException : public DatabaseException {
public:
    ConnectionException(const std::string& msg)
        : DatabaseException("Connection Error: " + msg) {}
};

class QueryException : public DatabaseException {
public:
    QueryException(const std::string& msg)
        : DatabaseException("Query Error: " + msg) {}
};

int main()
{
    try {
        throw QueryException("Invalid SQL syntax");
    }
    catch (const QueryException& e) {
        std::cout << "Query issue: " << e.what() << "\n";
    }
    catch (const DatabaseException& e) {
        std::cout << "DB issue: " << e.what() << "\n";
    }

    return 0;
}
```

---

## 3. Adding Custom Data

### Rich Exception Information:

```cpp
#include <exception>
#include <string>
#include <iostream>

class FileException : public std::exception {
private:
    std::string filename;
    std::string operation;
    int errorCode;
    std::string message;

public:
    FileException(const std::string& file, const std::string& op, int code)
        : filename(file), operation(op), errorCode(code) {
        message = "File: " + filename + ", Op: " + operation +
                  ", Code: " + std::to_string(errorCode);
    }

    const char* what() const noexcept override {
        return message.c_str();
    }

    std::string getFilename() const { return filename; }
    std::string getOperation() const { return operation; }
    int getErrorCode() const { return errorCode; }
};

int main()
{
    try {
        throw FileException("data.txt", "read", 404);
    }
    catch (const FileException& e) {
        std::cout << e.what() << "\n";
        std::cout << "File: " << e.getFilename() << "\n";
        std::cout << "Error code: " << e.getErrorCode() << "\n";
    }

    return 0;
}
```

---

## Key Takeaways

1. ✅ Inherit from std::exception for custom exceptions
2. ✅ Override what() method
3. ✅ Create exception hierarchies for organization
4. ✅ Add custom data members for rich information
5. ✅ Use noexcept on what() method
6. ✅ Catch specific exceptions before general ones

---

## Next Lesson Preview

In **Lesson 63**, we'll learn about:
- Exception safety guarantees
- RAII and exceptions
- Strong exception guarantee
- noexcept specifications

**Excellent work!**
