# Lesson 14: Exception Handling

**Duration**: 2.5-3 hours

## Overview

Exception handling is a mechanism in C++ to handle runtime errors gracefully. Instead of crashing the program, exceptions allow you to catch errors, handle them appropriately, and continue execution or terminate cleanly. This lesson covers try-catch-throw syntax, standard exceptions, custom exceptions, and exception safety patterns.

## Topics Covered

1. Introduction to Exception Handling
2. Try-Catch-Throw Basics
3. Standard Exception Types
4. Custom Exception Classes
5. Multiple Catch Blocks
6. Exception Safety and RAII
7. Rethrowing Exceptions
8. Best Practices

## 1. Introduction to Exception Handling

Exception handling separates error handling code from normal code flow, making programs more readable and maintainable. When an error occurs, an exception is "thrown" and can be "caught" by appropriate handlers.

**Benefits:**
- Separates error handling from normal logic
- Propagates errors up the call stack
- Cannot be ignored (unlike return codes)
- Supports RAII for automatic resource cleanup

## 2. Try-Catch-Throw Basics

### Example 1: Basic Exception Handling - Complete Program

**File: `basic_exception.cpp`**

```cpp
#include <iostream>
#include <stdexcept>

// Function that throws an exception
double divide(double numerator, double denominator) {
    if (denominator == 0.0) {
        throw std::runtime_error("Division by zero error!");
    }
    return numerator / denominator;
}

int main() {
    std::cout << "=== Basic Exception Handling Demo ===" << std::endl;

    // Example 1: Successful division
    try {
        double result = divide(10.0, 2.0);
        std::cout << "10 / 2 = " << result << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Example 2: Division by zero
    try {
        double result = divide(10.0, 0.0);
        std::cout << "10 / 0 = " << result << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error caught: " << e.what() << std::endl;
    }

    // Example 3: Multiple operations in try block
    try {
        std::cout << "\nPerforming multiple divisions:" << std::endl;
        std::cout << "20 / 4 = " << divide(20.0, 4.0) << std::endl;
        std::cout << "15 / 3 = " << divide(15.0, 3.0) << std::endl;
        std::cout << "100 / 0 = " << divide(100.0, 0.0) << std::endl;  // This will throw
        std::cout << "This line will not execute" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error in operation sequence: " << e.what() << std::endl;
    }

    std::cout << "\nProgram continues after exception handling..." << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall basic_exception.cpp -o basic_exception
./basic_exception
```

## 3. Standard Exception Types

C++ provides a hierarchy of standard exception classes in `<stdexcept>`:

- `std::exception` - Base class for all exceptions
- `std::logic_error` - Logic errors (preventable)
  - `std::invalid_argument`
  - `std::domain_error`
  - `std::length_error`
  - `std::out_of_range`
- `std::runtime_error` - Runtime errors (not preventable)
  - `std::range_error`
  - `std::overflow_error`
  - `std::underflow_error`

### Example 2: Standard Exception Types - Complete Program

**File: `standard_exceptions.cpp`**

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

// Demonstrates invalid_argument
int parseAge(const std::string& input) {
    int age;
    try {
        age = std::stoi(input);
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Invalid age format: not a number");
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Age value out of range");
    }

    if (age < 0 || age > 150) {
        throw std::domain_error("Age must be between 0 and 150");
    }

    return age;
}

// Demonstrates out_of_range
int getElement(const std::vector<int>& vec, size_t index) {
    if (index >= vec.size()) {
        throw std::out_of_range("Index out of bounds");
    }
    return vec[index];
}

// Demonstrates overflow_error
int calculateFactorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial undefined for negative numbers");
    }
    if (n > 12) {
        throw std::overflow_error("Factorial too large for int type");
    }

    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    std::cout << "=== Standard Exception Types Demo ===" << std::endl;

    // Test 1: invalid_argument
    std::cout << "\n--- Test 1: Invalid Argument ---" << std::endl;
    std::vector<std::string> ageInputs = {"25", "abc", "200", "-5"};

    for (const auto& input : ageInputs) {
        try {
            int age = parseAge(input);
            std::cout << "Valid age: " << age << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        } catch (const std::domain_error& e) {
            std::cerr << "Domain error: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << std::endl;
        }
    }

    // Test 2: out_of_range
    std::cout << "\n--- Test 2: Out of Range ---" << std::endl;
    std::vector<int> numbers = {10, 20, 30, 40, 50};
    std::vector<size_t> indices = {0, 2, 4, 5, 10};

    for (size_t idx : indices) {
        try {
            int value = getElement(numbers, idx);
            std::cout << "numbers[" << idx << "] = " << value << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: " << e.what() << " (index: " << idx << ")" << std::endl;
        }
    }

    // Test 3: overflow_error
    std::cout << "\n--- Test 3: Overflow Error ---" << std::endl;
    std::vector<int> factorialInputs = {5, 10, 12, 13, 15, -1};

    for (int n : factorialInputs) {
        try {
            int result = calculateFactorial(n);
            std::cout << n << "! = " << result << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid: " << e.what() << " (n = " << n << ")" << std::endl;
        } catch (const std::overflow_error& e) {
            std::cerr << "Overflow: " << e.what() << " (n = " << n << ")" << std::endl;
        }
    }

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall standard_exceptions.cpp -o standard_exceptions
./standard_exceptions
```

## 4. Custom Exception Classes

You can create custom exception classes by inheriting from `std::exception` or its derived classes.

### Example 3: Custom Exceptions - Complete Program

**File: `custom_exceptions.cpp`**

```cpp
#include <iostream>
#include <exception>
#include <string>
#include <cmath>

// Custom exception for mathematical errors
class MathException : public std::exception {
private:
    std::string message;
public:
    explicit MathException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Custom exception for negative square root
class NegativeSqrtException : public MathException {
private:
    double value;
public:
    explicit NegativeSqrtException(double val)
        : MathException("Cannot calculate square root of negative number"),
          value(val) {}

    double getValue() const { return value; }
};

// Custom exception for division by zero
class DivisionByZeroException : public MathException {
private:
    double numerator;
public:
    explicit DivisionByZeroException(double num)
        : MathException("Division by zero"),
          numerator(num) {}

    double getNumerator() const { return numerator; }
};

// Bank account exception hierarchy
class BankException : public std::exception {
protected:
    std::string message;
public:
    explicit BankException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InsufficientFundsException : public BankException {
private:
    double balance;
    double requested;
public:
    InsufficientFundsException(double bal, double req)
        : BankException("Insufficient funds"),
          balance(bal), requested(req) {}

    double getBalance() const { return balance; }
    double getRequested() const { return requested; }
};

class InvalidAmountException : public BankException {
public:
    explicit InvalidAmountException(const std::string& msg)
        : BankException(msg) {}
};

// Math calculator class
class Calculator {
public:
    static double safeSqrt(double x) {
        if (x < 0) {
            throw NegativeSqrtException(x);
        }
        return std::sqrt(x);
    }

    static double safeDivide(double numerator, double denominator) {
        if (denominator == 0.0) {
            throw DivisionByZeroException(numerator);
        }
        return numerator / denominator;
    }
};

// Bank account class
class BankAccount {
private:
    std::string accountNumber;
    double balance;

public:
    BankAccount(const std::string& accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance) {
        if (initialBalance < 0) {
            throw InvalidAmountException("Initial balance cannot be negative");
        }
    }

    void deposit(double amount) {
        if (amount <= 0) {
            throw InvalidAmountException("Deposit amount must be positive");
        }
        balance += amount;
        std::cout << "Deposited $" << amount << ". New balance: $" << balance << std::endl;
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            throw InvalidAmountException("Withdrawal amount must be positive");
        }
        if (amount > balance) {
            throw InsufficientFundsException(balance, amount);
        }
        balance -= amount;
        std::cout << "Withdrew $" << amount << ". New balance: $" << balance << std::endl;
    }

    double getBalance() const { return balance; }
    std::string getAccountNumber() const { return accountNumber; }
};

int main() {
    std::cout << "=== Custom Exceptions Demo ===" << std::endl;

    // Test 1: Math exceptions
    std::cout << "\n--- Test 1: Math Operations ---" << std::endl;
    double mathValues[] = {16.0, -4.0, 25.0, -9.0};

    for (double val : mathValues) {
        try {
            double result = Calculator::safeSqrt(val);
            std::cout << "sqrt(" << val << ") = " << result << std::endl;
        } catch (const NegativeSqrtException& e) {
            std::cerr << "Error: " << e.what()
                      << " (value: " << e.getValue() << ")" << std::endl;
        }
    }

    std::cout << "\n--- Division Operations ---" << std::endl;
    double divisions[][2] = {{10.0, 2.0}, {15.0, 0.0}, {20.0, 4.0}, {30.0, 0.0}};

    for (auto& pair : divisions) {
        try {
            double result = Calculator::safeDivide(pair[0], pair[1]);
            std::cout << pair[0] << " / " << pair[1] << " = " << result << std::endl;
        } catch (const DivisionByZeroException& e) {
            std::cerr << "Error: " << e.what()
                      << " (numerator: " << e.getNumerator() << ")" << std::endl;
        }
    }

    // Test 2: Bank account exceptions
    std::cout << "\n--- Test 2: Bank Account Operations ---" << std::endl;

    try {
        BankAccount account("ACC-12345", 1000.0);
        std::cout << "Account created: " << account.getAccountNumber()
                  << " with balance $" << account.getBalance() << std::endl;

        // Valid operations
        account.deposit(500.0);
        account.withdraw(300.0);

        // Invalid operations
        try {
            account.deposit(-100.0);
        } catch (const InvalidAmountException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        try {
            account.withdraw(2000.0);
        } catch (const InsufficientFundsException& e) {
            std::cerr << "Error: " << e.what()
                      << " (balance: $" << e.getBalance()
                      << ", requested: $" << e.getRequested() << ")" << std::endl;
        }

        try {
            account.withdraw(0.0);
        } catch (const InvalidAmountException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        std::cout << "Final balance: $" << account.getBalance() << std::endl;

    } catch (const BankException& e) {
        std::cerr << "Bank error: " << e.what() << std::endl;
    }

    // Test 3: Creating account with negative balance
    std::cout << "\n--- Test 3: Invalid Account Creation ---" << std::endl;
    try {
        BankAccount invalidAccount("ACC-99999", -500.0);
    } catch (const InvalidAmountException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall custom_exceptions.cpp -o custom_exceptions
./custom_exceptions
```

## 5. Multiple Catch Blocks and Exception Hierarchy

### Example 4: Multiple Catch Blocks - Complete Program

**File: `multiple_catch.cpp`**

```cpp
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Function that can throw different exception types
void processData(int choice) {
    switch (choice) {
        case 1:
            throw std::invalid_argument("Invalid argument exception");
        case 2:
            throw std::out_of_range("Out of range exception");
        case 3:
            throw std::runtime_error("Runtime error exception");
        case 4:
            throw std::logic_error("Logic error exception");
        case 5:
            throw 42;  // Throwing an integer
        case 6:
            throw "String exception";  // Throwing a const char*
        default:
            std::cout << "No exception thrown" << std::endl;
    }
}

// Array access with different exception types
class SafeArray {
private:
    std::vector<int> data;

public:
    SafeArray(const std::vector<int>& initial) : data(initial) {}

    int& at(size_t index) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void resize(size_t newSize) {
        if (newSize > 1000000) {
            throw std::length_error("Requested size too large");
        }
        data.resize(newSize);
    }

    size_t size() const { return data.size(); }
};

int main() {
    std::cout << "=== Multiple Catch Blocks Demo ===" << std::endl;

    // Test 1: Catching different exception types
    std::cout << "\n--- Test 1: Different Exception Types ---" << std::endl;

    for (int i = 0; i <= 7; ++i) {
        std::cout << "\nTest case " << i << ": ";
        try {
            processData(i);
        }
        // Specific exceptions first, then more general
        catch (const std::invalid_argument& e) {
            std::cerr << "Caught invalid_argument: " << e.what() << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Caught out_of_range: " << e.what() << std::endl;
        }
        catch (const std::logic_error& e) {
            std::cerr << "Caught logic_error: " << e.what() << std::endl;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Caught runtime_error: " << e.what() << std::endl;
        }
        catch (int value) {
            std::cerr << "Caught integer: " << value << std::endl;
        }
        catch (const char* msg) {
            std::cerr << "Caught string: " << msg << std::endl;
        }
        catch (...) {
            std::cerr << "Caught unknown exception" << std::endl;
        }
    }

    // Test 2: Exception hierarchy - catching base class
    std::cout << "\n--- Test 2: Catching Base Class ---" << std::endl;

    for (int i = 1; i <= 4; ++i) {
        std::cout << "\nTest case " << i << ": ";
        try {
            processData(i);
        }
        catch (const std::exception& e) {
            // This catches all standard exceptions
            std::cerr << "Caught std::exception: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "Caught non-standard exception" << std::endl;
        }
    }

    // Test 3: SafeArray with multiple exceptions
    std::cout << "\n--- Test 3: SafeArray Operations ---" << std::endl;

    try {
        SafeArray arr({10, 20, 30, 40, 50});
        std::cout << "Array created with " << arr.size() << " elements" << std::endl;

        // Valid access
        std::cout << "arr[2] = " << arr.at(2) << std::endl;

        // Modify value
        arr.at(1) = 25;
        std::cout << "Modified arr[1] = " << arr.at(1) << std::endl;

        // Invalid access
        try {
            std::cout << "Trying to access arr[10]..." << std::endl;
            std::cout << "arr[10] = " << arr.at(10) << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Access error: " << e.what() << std::endl;
        }

        // Valid resize
        arr.resize(100);
        std::cout << "Resized to " << arr.size() << " elements" << std::endl;

        // Invalid resize
        try {
            std::cout << "Trying to resize to 2000000..." << std::endl;
            arr.resize(2000000);
        } catch (const std::length_error& e) {
            std::cerr << "Resize error: " << e.what() << std::endl;
        }

        std::cout << "Final array size: " << arr.size() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }

    std::cout << "\nProgram completed successfully" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall multiple_catch.cpp -o multiple_catch
./multiple_catch
```

## 6. Exception Safety and RAII

RAII (Resource Acquisition Is Initialization) ensures resources are properly cleaned up even when exceptions occur.

### Example 5: RAII and Exception Safety - Complete Program

**File: `raii_exception_safety.cpp`**

```cpp
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <stdexcept>

// Manual resource management (BAD - can leak)
class UnsafeResource {
private:
    int* data;
    size_t size;

public:
    UnsafeResource(size_t s) : size(s) {
        std::cout << "Allocating " << size << " integers (UNSAFE)" << std::endl;
        data = new int[size];
        if (size > 1000) {
            // Memory leak! Exception thrown before we can clean up
            throw std::runtime_error("Size too large");
        }
    }

    ~UnsafeResource() {
        std::cout << "Deallocating unsafe resource" << std::endl;
        delete[] data;
    }

    // Prevent copying
    UnsafeResource(const UnsafeResource&) = delete;
    UnsafeResource& operator=(const UnsafeResource&) = delete;
};

// RAII resource management (GOOD - exception safe)
class SafeResource {
private:
    std::unique_ptr<int[]> data;
    size_t size;

public:
    SafeResource(size_t s) : size(s) {
        std::cout << "Allocating " << size << " integers (SAFE with RAII)" << std::endl;
        data = std::make_unique<int[]>(size);
        if (size > 1000) {
            // No leak! unique_ptr automatically cleans up
            throw std::runtime_error("Size too large");
        }
        // Initialize data
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<int>(i);
        }
    }

    ~SafeResource() {
        std::cout << "Destroying safe resource (auto cleanup)" << std::endl;
        // unique_ptr automatically deletes the array
    }

    int& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    size_t getSize() const { return size; }
};

// File handler with RAII
class FileHandler {
private:
    std::unique_ptr<std::ifstream> file;
    std::string filename;

public:
    explicit FileHandler(const std::string& fname) : filename(fname) {
        std::cout << "Opening file: " << filename << std::endl;
        file = std::make_unique<std::ifstream>(filename);
        if (!file->is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }
    }

    ~FileHandler() {
        std::cout << "Closing file: " << filename << std::endl;
        // File automatically closed when ifstream is destroyed
    }

    std::string readLine() {
        std::string line;
        if (std::getline(*file, line)) {
            return line;
        }
        throw std::runtime_error("Failed to read from file");
    }

    bool hasMore() {
        return file->good() && !file->eof();
    }
};

// Transaction class with rollback capability
class Transaction {
private:
    std::string name;
    bool committed;
    int* tempData;

public:
    explicit Transaction(const std::string& n)
        : name(n), committed(false), tempData(nullptr) {
        std::cout << "BEGIN transaction: " << name << std::endl;
        tempData = new int[10];  // Allocate temporary resources
    }

    ~Transaction() {
        if (!committed) {
            std::cout << "ROLLBACK transaction: " << name
                      << " (exception occurred)" << std::endl;
        } else {
            std::cout << "Transaction completed: " << name << std::endl;
        }
        delete[] tempData;
    }

    void doWork() {
        std::cout << "  Doing work in transaction: " << name << std::endl;
        // Simulate some work
        for (int i = 0; i < 10; ++i) {
            tempData[i] = i * 2;
        }
    }

    void commit() {
        std::cout << "COMMIT transaction: " << name << std::endl;
        committed = true;
    }
};

// Function demonstrating exception safety
void performOperations(bool shouldFail) {
    SafeResource resource(10);
    std::cout << "Resource created successfully" << std::endl;

    resource[0] = 100;
    resource[5] = 200;
    std::cout << "Resource[0] = " << resource[0] << std::endl;
    std::cout << "Resource[5] = " << resource[5] << std::endl;

    if (shouldFail) {
        throw std::runtime_error("Simulated failure");
    }

    std::cout << "Operations completed successfully" << std::endl;
}

int main() {
    std::cout << "=== RAII and Exception Safety Demo ===" << std::endl;

    // Test 1: Unsafe resource (demonstrates memory leak)
    std::cout << "\n--- Test 1: Unsafe Resource ---" << std::endl;
    try {
        UnsafeResource unsafe(2000);  // This will throw and leak memory
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        std::cerr << "WARNING: Memory may have leaked!" << std::endl;
    }

    // Test 2: Safe resource with RAII
    std::cout << "\n--- Test 2: Safe Resource with RAII ---" << std::endl;
    try {
        SafeResource safe(2000);  // This will throw but NOT leak
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        std::cerr << "No memory leak - RAII cleaned up automatically!" << std::endl;
    }

    // Test 3: Successful resource usage
    std::cout << "\n--- Test 3: Successful Resource Usage ---" << std::endl;
    try {
        SafeResource safe(50);
        for (size_t i = 0; i < 5; ++i) {
            std::cout << "safe[" << i << "] = " << safe[i] << std::endl;
        }
        safe[10] = 999;
        std::cout << "safe[10] = " << safe[10] << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    // Test 4: Operations with exception safety
    std::cout << "\n--- Test 4: Exception-Safe Operations ---" << std::endl;

    std::cout << "Case A: Success" << std::endl;
    try {
        performOperations(false);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\nCase B: Failure" << std::endl;
    try {
        performOperations(true);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Resources cleaned up automatically!" << std::endl;
    }

    // Test 5: Transaction rollback
    std::cout << "\n--- Test 5: Transaction with Rollback ---" << std::endl;

    std::cout << "Successful transaction:" << std::endl;
    try {
        Transaction t1("TXN-001");
        t1.doWork();
        t1.commit();
    } catch (const std::exception& e) {
        std::cerr << "Transaction failed: " << e.what() << std::endl;
    }

    std::cout << "\nFailed transaction (automatic rollback):" << std::endl;
    try {
        Transaction t2("TXN-002");
        t2.doWork();
        throw std::runtime_error("Simulated transaction failure");
        t2.commit();  // This line won't execute
    } catch (const std::exception& e) {
        std::cerr << "Transaction failed: " << e.what() << std::endl;
    }

    std::cout << "\nProgram completed" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall raii_exception_safety.cpp -o raii_exception_safety
./raii_exception_safety
```

## 7. Rethrowing Exceptions

### Example 6: Rethrowing Exceptions - Complete Program

**File: `rethrow_exceptions.cpp`**

```cpp
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Logger class for tracking operations
class Logger {
public:
    static void log(const std::string& message) {
        std::cout << "[LOG] " << message << std::endl;
    }

    static void error(const std::string& message) {
        std::cerr << "[ERROR] " << message << std::endl;
    }
};

// Database connection simulator
class Database {
private:
    bool connected;
    std::string connectionString;

public:
    explicit Database(const std::string& connStr)
        : connected(false), connectionString(connStr) {}

    void connect() {
        Logger::log("Attempting to connect to database...");
        if (connectionString.empty()) {
            throw std::invalid_argument("Empty connection string");
        }
        if (connectionString == "invalid") {
            throw std::runtime_error("Invalid connection string");
        }
        connected = true;
        Logger::log("Connected successfully");
    }

    void executeQuery(const std::string& query) {
        if (!connected) {
            throw std::logic_error("Not connected to database");
        }
        Logger::log("Executing query: " + query);

        if (query.empty()) {
            throw std::invalid_argument("Empty query");
        }
        if (query.find("ERROR") != std::string::npos) {
            throw std::runtime_error("Query execution failed");
        }

        Logger::log("Query executed successfully");
    }

    bool isConnected() const { return connected; }
};

// Data access layer - catches and rethrows with context
class DataAccessLayer {
private:
    Database& db;

public:
    explicit DataAccessLayer(Database& database) : db(database) {}

    void initialize() {
        try {
            db.connect();
        } catch (const std::exception& e) {
            Logger::error("DAL initialization failed: " + std::string(e.what()));
            throw;  // Rethrow the same exception
        }
    }

    std::vector<std::string> fetchRecords(const std::string& table) {
        try {
            std::string query = "SELECT * FROM " + table;
            db.executeQuery(query);
            return {"Record1", "Record2", "Record3"};
        } catch (const std::exception& e) {
            Logger::error("Failed to fetch records from " + table + ": " + e.what());
            throw;  // Rethrow for caller to handle
        }
    }

    void updateRecord(const std::string& table, int id, const std::string& data) {
        try {
            std::string query = "UPDATE " + table + " SET data='" + data + "' WHERE id=" + std::to_string(id);
            db.executeQuery(query);
        } catch (const std::invalid_argument& e) {
            Logger::error("Invalid update parameters");
            throw;  // Rethrow specific exception
        } catch (const std::exception& e) {
            Logger::error("Update failed - wrapping in runtime_error");
            // Rethrow as different exception with more context
            throw std::runtime_error("DataAccessLayer::updateRecord failed: " + std::string(e.what()));
        }
    }
};

// Business logic layer - may catch, log, and rethrow
class BusinessLogic {
private:
    DataAccessLayer& dal;

public:
    explicit BusinessLogic(DataAccessLayer& dataAccess) : dal(dataAccess) {}

    void processUserData(const std::string& username) {
        Logger::log("Processing data for user: " + username);

        try {
            auto records = dal.fetchRecords("users");
            Logger::log("Found " + std::to_string(records.size()) + " records");

            // Simulate processing
            for (const auto& record : records) {
                Logger::log("  Processing: " + record);
            }

        } catch (const std::exception& e) {
            Logger::error("Business logic error in processUserData");
            throw;  // Rethrow to caller
        }
    }

    void updateUserProfile(int userId, const std::string& newData) {
        Logger::log("Updating profile for user ID: " + std::to_string(userId));

        try {
            dal.updateRecord("users", userId, newData);
            Logger::log("Profile updated successfully");
        } catch (...) {
            Logger::error("Failed to update user profile");
            throw;  // Rethrow unknown exception
        }
    }
};

// Demonstration function with nested try-catch
void demonstrateNestedTryCatch() {
    Logger::log("\n=== Nested Try-Catch Demo ===");

    try {
        Logger::log("Outer try block started");

        try {
            Logger::log("Inner try block started");
            throw std::runtime_error("Exception from inner try");
        } catch (const std::runtime_error& e) {
            Logger::error("Caught in inner catch: " + std::string(e.what()));
            Logger::log("Rethrowing from inner catch...");
            throw;  // Rethrow to outer catch
        }

        Logger::log("This line will not execute");

    } catch (const std::exception& e) {
        Logger::error("Caught in outer catch: " + std::string(e.what()));
    }
}

// Function showing partial exception handling
void partiallyHandleException(int value) {
    try {
        if (value < 0) {
            throw std::invalid_argument("Negative value not allowed");
        }
        if (value == 0) {
            throw std::logic_error("Zero value not allowed");
        }
        if (value > 100) {
            throw std::out_of_range("Value exceeds maximum");
        }

        Logger::log("Value " + std::to_string(value) + " processed successfully");

    } catch (const std::invalid_argument& e) {
        // Handle this specific exception completely
        Logger::error("Handled invalid_argument: " + std::string(e.what()));
        Logger::log("Using default value instead");
        // Don't rethrow - exception fully handled
    } catch (...) {
        // Log but don't fully handle other exceptions
        Logger::error("Unexpected exception - rethrowing");
        throw;  // Rethrow for caller to handle
    }
}

int main() {
    std::cout << "=== Rethrowing Exceptions Demo ===" << std::endl;

    // Test 1: Successful database operations
    std::cout << "\n--- Test 1: Successful Operations ---" << std::endl;
    try {
        Database db("server=localhost;database=test");
        DataAccessLayer dal(db);
        BusinessLogic logic(dal);

        dal.initialize();
        logic.processUserData("john_doe");
        logic.updateUserProfile(123, "Updated data");

    } catch (const std::exception& e) {
        Logger::error("Top-level catch: " + std::string(e.what()));
    }

    // Test 2: Failed database connection
    std::cout << "\n--- Test 2: Failed Connection ---" << std::endl;
    try {
        Database db("invalid");
        DataAccessLayer dal(db);

        dal.initialize();  // This will fail and rethrow

    } catch (const std::exception& e) {
        Logger::error("Top-level catch: " + std::string(e.what()));
    }

    // Test 3: Failed query execution
    std::cout << "\n--- Test 3: Failed Query ---" << std::endl;
    try {
        Database db("server=localhost;database=test");
        DataAccessLayer dal(db);
        BusinessLogic logic(dal);

        dal.initialize();
        dal.updateRecord("users", 1, "ERROR");  // Will trigger error

    } catch (const std::exception& e) {
        Logger::error("Top-level catch: " + std::string(e.what()));
    }

    // Test 4: Nested try-catch
    demonstrateNestedTryCatch();

    // Test 5: Partial exception handling
    std::cout << "\n--- Test 5: Partial Exception Handling ---" << std::endl;

    int testValues[] = {50, -10, 0, 150};
    for (int val : testValues) {
        std::cout << "\nTesting value: " << val << std::endl;
        try {
            partiallyHandleException(val);
            Logger::log("Value processed without exception");
        } catch (const std::exception& e) {
            Logger::error("Main caught rethrown exception: " + std::string(e.what()));
        }
    }

    std::cout << "\nProgram completed" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall rethrow_exceptions.cpp -o rethrow_exceptions
./rethrow_exceptions
```

## Complete Project 1: Contact Management System with Exception Handling

**File: `contact_manager_exceptions.cpp`**

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <sstream>

// Custom exceptions
class ContactException : public std::exception {
protected:
    std::string message;
public:
    explicit ContactException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class DuplicateContactException : public ContactException {
public:
    explicit DuplicateContactException(const std::string& name)
        : ContactException("Contact already exists: " + name) {}
};

class ContactNotFoundException : public ContactException {
public:
    explicit ContactNotFoundException(const std::string& name)
        : ContactException("Contact not found: " + name) {}
};

class InvalidContactDataException : public ContactException {
public:
    explicit InvalidContactDataException(const std::string& msg)
        : ContactException("Invalid contact data: " + msg) {}
};

class FileOperationException : public ContactException {
public:
    explicit FileOperationException(const std::string& msg)
        : ContactException("File operation failed: " + msg) {}
};

// Contact structure
struct Contact {
    std::string name;
    std::string phone;
    std::string email;
    std::string address;

    Contact() = default;
    Contact(const std::string& n, const std::string& p,
            const std::string& e, const std::string& a)
        : name(n), phone(p), email(e), address(a) {
        validate();
    }

    void validate() const {
        if (name.empty()) {
            throw InvalidContactDataException("Name cannot be empty");
        }
        if (phone.empty()) {
            throw InvalidContactDataException("Phone cannot be empty");
        }
        if (phone.length() < 10) {
            throw InvalidContactDataException("Phone number too short");
        }
        if (!email.empty() && email.find('@') == std::string::npos) {
            throw InvalidContactDataException("Invalid email format");
        }
    }

    std::string toString() const {
        std::stringstream ss;
        ss << "Name: " << name << "\n"
           << "Phone: " << phone << "\n"
           << "Email: " << email << "\n"
           << "Address: " << address;
        return ss.str();
    }
};

// Contact Manager class
class ContactManager {
private:
    std::vector<Contact> contacts;
    std::string filename;
    bool modified;

    auto findContact(const std::string& name) {
        return std::find_if(contacts.begin(), contacts.end(),
            [&name](const Contact& c) { return c.name == name; });
    }

    auto findContact(const std::string& name) const {
        return std::find_if(contacts.begin(), contacts.end(),
            [&name](const Contact& c) { return c.name == name; });
    }

public:
    explicit ContactManager(const std::string& file)
        : filename(file), modified(false) {
        try {
            loadFromFile();
        } catch (const FileOperationException& e) {
            std::cerr << "Warning: " << e.what() << std::endl;
            std::cerr << "Starting with empty contact list" << std::endl;
        }
    }

    ~ContactManager() {
        try {
            if (modified) {
                saveToFile();
            }
        } catch (const std::exception& e) {
            std::cerr << "Error saving contacts: " << e.what() << std::endl;
        }
    }

    void addContact(const Contact& contact) {
        // Validate contact
        contact.validate();

        // Check for duplicates
        if (findContact(contact.name) != contacts.end()) {
            throw DuplicateContactException(contact.name);
        }

        contacts.push_back(contact);
        modified = true;
        std::cout << "Contact added successfully: " << contact.name << std::endl;
    }

    void removeContact(const std::string& name) {
        auto it = findContact(name);
        if (it == contacts.end()) {
            throw ContactNotFoundException(name);
        }

        contacts.erase(it);
        modified = true;
        std::cout << "Contact removed: " << name << std::endl;
    }

    const Contact& getContact(const std::string& name) const {
        auto it = findContact(name);
        if (it == contacts.end()) {
            throw ContactNotFoundException(name);
        }
        return *it;
    }

    void updateContact(const std::string& name, const Contact& newData) {
        auto it = findContact(name);
        if (it == contacts.end()) {
            throw ContactNotFoundException(name);
        }

        // Validate new data
        newData.validate();

        *it = newData;
        modified = true;
        std::cout << "Contact updated: " << name << std::endl;
    }

    std::vector<Contact> searchContacts(const std::string& query) const {
        std::vector<Contact> results;
        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(),
                      lowerQuery.begin(), ::tolower);

        for (const auto& contact : contacts) {
            std::string lowerName = contact.name;
            std::transform(lowerName.begin(), lowerName.end(),
                          lowerName.begin(), ::tolower);

            if (lowerName.find(lowerQuery) != std::string::npos ||
                contact.phone.find(query) != std::string::npos ||
                contact.email.find(query) != std::string::npos) {
                results.push_back(contact);
            }
        }

        return results;
    }

    void listAllContacts() const {
        if (contacts.empty()) {
            std::cout << "No contacts in the system" << std::endl;
            return;
        }

        std::cout << "\n=== All Contacts ===" << std::endl;
        for (size_t i = 0; i < contacts.size(); ++i) {
            std::cout << "\n[" << (i + 1) << "] " << contacts[i].name << std::endl;
            std::cout << "    Phone: " << contacts[i].phone << std::endl;
            std::cout << "    Email: " << contacts[i].email << std::endl;
        }
    }

    void saveToFile() {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw FileOperationException("Cannot open file for writing: " + filename);
        }

        try {
            for (const auto& contact : contacts) {
                file << contact.name << "|"
                     << contact.phone << "|"
                     << contact.email << "|"
                     << contact.address << "\n";

                if (file.fail()) {
                    throw FileOperationException("Write error occurred");
                }
            }

            file.close();
            modified = false;
            std::cout << "Contacts saved to file: " << filename << std::endl;

        } catch (const std::exception& e) {
            file.close();
            throw FileOperationException("Error while saving: " + std::string(e.what()));
        }
    }

    void loadFromFile() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw FileOperationException("Cannot open file for reading: " + filename);
        }

        contacts.clear();
        std::string line;
        int lineNumber = 0;

        try {
            while (std::getline(file, line)) {
                lineNumber++;
                if (line.empty()) continue;

                std::stringstream ss(line);
                std::string name, phone, email, address;

                if (!std::getline(ss, name, '|') ||
                    !std::getline(ss, phone, '|') ||
                    !std::getline(ss, email, '|') ||
                    !std::getline(ss, address, '|')) {
                    throw InvalidContactDataException(
                        "Malformed data at line " + std::to_string(lineNumber));
                }

                Contact contact(name, phone, email, address);
                contacts.push_back(contact);
            }

            file.close();
            modified = false;
            std::cout << "Loaded " << contacts.size() << " contacts from file" << std::endl;

        } catch (const std::exception& e) {
            file.close();
            throw FileOperationException("Error at line " + std::to_string(lineNumber) +
                                        ": " + e.what());
        }
    }

    size_t getContactCount() const {
        return contacts.size();
    }
};

// Menu system
void displayMenu() {
    std::cout << "\n=== Contact Manager Menu ===" << std::endl;
    std::cout << "1. Add Contact" << std::endl;
    std::cout << "2. Remove Contact" << std::endl;
    std::cout << "3. Search Contact" << std::endl;
    std::cout << "4. Update Contact" << std::endl;
    std::cout << "5. List All Contacts" << std::endl;
    std::cout << "6. Save to File" << std::endl;
    std::cout << "7. Exit" << std::endl;
    std::cout << "Enter choice: ";
}

void addContactInteractive(ContactManager& manager) {
    std::string name, phone, email, address;

    std::cout << "\nEnter contact details:" << std::endl;
    std::cout << "Name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Phone: ";
    std::getline(std::cin, phone);
    std::cout << "Email: ";
    std::getline(std::cin, email);
    std::cout << "Address: ";
    std::getline(std::cin, address);

    try {
        Contact contact(name, phone, email, address);
        manager.addContact(contact);
    } catch (const ContactException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void removeContactInteractive(ContactManager& manager) {
    std::string name;
    std::cout << "\nEnter name of contact to remove: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    try {
        manager.removeContact(name);
    } catch (const ContactException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void searchContactInteractive(ContactManager& manager) {
    std::string query;
    std::cout << "\nEnter search query: ";
    std::cin.ignore();
    std::getline(std::cin, query);

    try {
        auto results = manager.searchContacts(query);
        if (results.empty()) {
            std::cout << "No contacts found matching: " << query << std::endl;
        } else {
            std::cout << "\nFound " << results.size() << " contact(s):" << std::endl;
            for (const auto& contact : results) {
                std::cout << "\n" << contact.toString() << std::endl;
                std::cout << "---" << std::endl;
            }
        }
    } catch (const ContactException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== Contact Management System ===" << std::endl;
    std::cout << "Exception Handling Demonstration" << std::endl;

    try {
        ContactManager manager("contacts.txt");

        // Automated demo
        std::cout << "\n--- Running Automated Demo ---" << std::endl;

        // Add valid contacts
        try {
            manager.addContact(Contact("John Doe", "1234567890", "john@email.com", "123 Main St"));
            manager.addContact(Contact("Jane Smith", "0987654321", "jane@email.com", "456 Oak Ave"));
            manager.addContact(Contact("Bob Johnson", "5555555555", "bob@email.com", "789 Pine Rd"));
        } catch (const ContactException& e) {
            std::cerr << "Error adding contact: " << e.what() << std::endl;
        }

        // Try to add duplicate
        try {
            std::cout << "\nTrying to add duplicate contact..." << std::endl;
            manager.addContact(Contact("John Doe", "9999999999", "john2@email.com", "Different address"));
        } catch (const DuplicateContactException& e) {
            std::cerr << "Expected error: " << e.what() << std::endl;
        }

        // Try to add invalid contact
        try {
            std::cout << "\nTrying to add contact with invalid data..." << std::endl;
            manager.addContact(Contact("", "123", "invalid-email", ""));
        } catch (const InvalidContactDataException& e) {
            std::cerr << "Expected error: " << e.what() << std::endl;
        }

        // Search for contact
        std::cout << "\nSearching for 'John'..." << std::endl;
        auto results = manager.searchContacts("John");
        for (const auto& contact : results) {
            std::cout << contact.toString() << std::endl;
        }

        // Update contact
        try {
            std::cout << "\nUpdating Jane's phone number..." << std::endl;
            Contact updated = manager.getContact("Jane Smith");
            updated.phone = "1112223333";
            manager.updateContact("Jane Smith", updated);
        } catch (const ContactException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        // Try to update non-existent contact
        try {
            std::cout << "\nTrying to update non-existent contact..." << std::endl;
            Contact dummy("Dummy", "0000000000", "dummy@email.com", "Nowhere");
            manager.updateContact("Non Existent", dummy);
        } catch (const ContactNotFoundException& e) {
            std::cerr << "Expected error: " << e.what() << std::endl;
        }

        // List all contacts
        manager.listAllContacts();

        // Remove a contact
        try {
            std::cout << "\nRemoving Bob Johnson..." << std::endl;
            manager.removeContact("Bob Johnson");
        } catch (const ContactException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        // Try to remove non-existent contact
        try {
            std::cout << "\nTrying to remove non-existent contact..." << std::endl;
            manager.removeContact("Ghost Person");
        } catch (const ContactNotFoundException& e) {
            std::cerr << "Expected error: " << e.what() << std::endl;
        }

        // List final contacts
        std::cout << "\nFinal contact list:" << std::endl;
        manager.listAllContacts();

        // Manually save
        try {
            manager.saveToFile();
        } catch (const FileOperationException& e) {
            std::cerr << "Save error: " << e.what() << std::endl;
        }

        std::cout << "\n=== Demo Completed ===" << std::endl;
        std::cout << "Total contacts: " << manager.getContactCount() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall contact_manager_exceptions.cpp -o contact_manager_exceptions
./contact_manager_exceptions
```

## Complete Project 2: Safe Calculator with Expression Parser

**File: `safe_calculator.cpp`**

```cpp
#include <iostream>
#include <string>
#include <stack>
#include <stdexcept>
#include <cmath>
#include <cctype>
#include <sstream>
#include <vector>

// Custom exceptions
class CalculatorException : public std::exception {
protected:
    std::string message;
public:
    explicit CalculatorException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class DivisionByZeroException : public CalculatorException {
public:
    DivisionByZeroException() : CalculatorException("Division by zero") {}
};

class InvalidExpressionException : public CalculatorException {
public:
    explicit InvalidExpressionException(const std::string& msg)
        : CalculatorException("Invalid expression: " + msg) {}
};

class MathDomainException : public CalculatorException {
public:
    explicit MathDomainException(const std::string& msg)
        : CalculatorException("Math domain error: " + msg) {}
};

class StackUnderflowException : public CalculatorException {
public:
    StackUnderflowException() : CalculatorException("Insufficient operands") {}
};

// Safe math operations
class SafeMath {
public:
    static double add(double a, double b) {
        double result = a + b;
        if (std::isinf(result)) {
            throw std::overflow_error("Addition overflow");
        }
        return result;
    }

    static double subtract(double a, double b) {
        double result = a - b;
        if (std::isinf(result)) {
            throw std::overflow_error("Subtraction overflow");
        }
        return result;
    }

    static double multiply(double a, double b) {
        double result = a * b;
        if (std::isinf(result)) {
            throw std::overflow_error("Multiplication overflow");
        }
        return result;
    }

    static double divide(double a, double b) {
        if (b == 0.0) {
            throw DivisionByZeroException();
        }
        double result = a / b;
        if (std::isinf(result)) {
            throw std::overflow_error("Division overflow");
        }
        return result;
    }

    static double power(double base, double exponent) {
        if (base == 0.0 && exponent < 0) {
            throw MathDomainException("0 to negative power");
        }
        if (base < 0 && std::floor(exponent) != exponent) {
            throw MathDomainException("Negative base with non-integer exponent");
        }
        double result = std::pow(base, exponent);
        if (std::isnan(result) || std::isinf(result)) {
            throw std::overflow_error("Power operation overflow");
        }
        return result;
    }

    static double squareRoot(double x) {
        if (x < 0) {
            throw MathDomainException("Square root of negative number");
        }
        return std::sqrt(x);
    }

    static double logarithm(double x) {
        if (x <= 0) {
            throw MathDomainException("Logarithm of non-positive number");
        }
        return std::log(x);
    }
};

// Expression parser and evaluator
class Calculator {
private:
    std::stack<double> operands;
    std::stack<char> operators;

    int getPrecedence(char op) {
        switch (op) {
            case '+':
            case '-':
                return 1;
            case '*':
            case '/':
                return 2;
            case '^':
                return 3;
            default:
                return 0;
        }
    }

    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }

    double applyOperation(char op, double a, double b) {
        switch (op) {
            case '+': return SafeMath::add(a, b);
            case '-': return SafeMath::subtract(a, b);
            case '*': return SafeMath::multiply(a, b);
            case '/': return SafeMath::divide(a, b);
            case '^': return SafeMath::power(a, b);
            default:
                throw InvalidExpressionException("Unknown operator: " + std::string(1, op));
        }
    }

    void processOperator(char op) {
        if (operands.size() < 2) {
            throw StackUnderflowException();
        }

        double b = operands.top(); operands.pop();
        double a = operands.top(); operands.pop();

        double result = applyOperation(op, a, b);
        operands.push(result);
    }

public:
    double evaluate(const std::string& expression) {
        // Clear stacks
        while (!operands.empty()) operands.pop();
        while (!operators.empty()) operators.pop();

        if (expression.empty()) {
            throw InvalidExpressionException("Empty expression");
        }

        std::string expr = expression;
        // Remove spaces
        expr.erase(std::remove_if(expr.begin(), expr.end(), ::isspace), expr.end());

        for (size_t i = 0; i < expr.length(); ++i) {
            char c = expr[i];

            // Handle numbers
            if (std::isdigit(c) || c == '.') {
                std::string numStr;
                while (i < expr.length() && (std::isdigit(expr[i]) || expr[i] == '.')) {
                    numStr += expr[i++];
                }
                --i;

                try {
                    double num = std::stod(numStr);
                    operands.push(num);
                } catch (const std::exception& e) {
                    throw InvalidExpressionException("Invalid number: " + numStr);
                }
            }
            // Handle opening parenthesis
            else if (c == '(') {
                operators.push(c);
            }
            // Handle closing parenthesis
            else if (c == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    processOperator(operators.top());
                    operators.pop();
                }

                if (operators.empty()) {
                    throw InvalidExpressionException("Mismatched parentheses");
                }
                operators.pop();  // Remove '('
            }
            // Handle operators
            else if (isOperator(c)) {
                // Handle negative numbers at start or after operators/parentheses
                if (c == '-' && (i == 0 || expr[i-1] == '(' || isOperator(expr[i-1]))) {
                    operands.push(0);
                }

                while (!operators.empty() && operators.top() != '(' &&
                       getPrecedence(operators.top()) >= getPrecedence(c)) {
                    processOperator(operators.top());
                    operators.pop();
                }
                operators.push(c);
            }
            else {
                throw InvalidExpressionException("Invalid character: " + std::string(1, c));
            }
        }

        // Process remaining operators
        while (!operators.empty()) {
            if (operators.top() == '(' || operators.top() == ')') {
                throw InvalidExpressionException("Mismatched parentheses");
            }
            processOperator(operators.top());
            operators.pop();
        }

        if (operands.empty()) {
            throw InvalidExpressionException("No result");
        }
        if (operands.size() > 1) {
            throw InvalidExpressionException("Too many operands");
        }

        return operands.top();
    }

    // Special functions
    double sqrt(double x) {
        return SafeMath::squareRoot(x);
    }

    double log(double x) {
        return SafeMath::logarithm(x);
    }

    double pow(double base, double exp) {
        return SafeMath::power(base, exp);
    }
};

// Test suite
void runTests() {
    Calculator calc;

    struct TestCase {
        std::string expression;
        double expectedResult;
        bool shouldThrow;
        std::string description;
    };

    std::vector<TestCase> tests = {
        {"2 + 3", 5.0, false, "Simple addition"},
        {"10 - 4", 6.0, false, "Simple subtraction"},
        {"5 * 6", 30.0, false, "Simple multiplication"},
        {"20 / 4", 5.0, false, "Simple division"},
        {"2 + 3 * 4", 14.0, false, "Operator precedence"},
        {"(2 + 3) * 4", 20.0, false, "Parentheses"},
        {"10 / 2 + 3 * 4", 17.0, false, "Mixed operations"},
        {"2 ^ 3", 8.0, false, "Power operation"},
        {"(5 + 3) * (2 + 2)", 32.0, false, "Multiple parentheses"},
        {"-5 + 3", -2.0, false, "Negative number"},
        {"-(5 + 3)", -8.0, false, "Negative expression"},
        {"10 / 0", 0.0, true, "Division by zero"},
        {"2 + + 3", 0.0, true, "Invalid expression"},
        {"((2 + 3)", 0.0, true, "Mismatched parentheses"},
        {"", 0.0, true, "Empty expression"},
    };

    std::cout << "\n=== Running Calculator Tests ===" << std::endl;
    int passed = 0;
    int failed = 0;

    for (const auto& test : tests) {
        std::cout << "\nTest: " << test.description << std::endl;
        std::cout << "Expression: \"" << test.expression << "\"" << std::endl;

        try {
            double result = calc.evaluate(test.expression);

            if (test.shouldThrow) {
                std::cout << "FAILED: Expected exception but got result: " << result << std::endl;
                failed++;
            } else if (std::abs(result - test.expectedResult) < 0.0001) {
                std::cout << "PASSED: Result = " << result << std::endl;
                passed++;
            } else {
                std::cout << "FAILED: Expected " << test.expectedResult
                         << " but got " << result << std::endl;
                failed++;
            }
        } catch (const CalculatorException& e) {
            if (test.shouldThrow) {
                std::cout << "PASSED: Caught expected exception: " << e.what() << std::endl;
                passed++;
            } else {
                std::cout << "FAILED: Unexpected exception: " << e.what() << std::endl;
                failed++;
            }
        } catch (const std::exception& e) {
            std::cout << "FAILED: Unexpected standard exception: " << e.what() << std::endl;
            failed++;
        }
    }

    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Passed: " << passed << std::endl;
    std::cout << "Failed: " << failed << std::endl;
    std::cout << "Total: " << (passed + failed) << std::endl;
}

int main() {
    std::cout << "=== Safe Calculator with Exception Handling ===" << std::endl;

    // Run automated tests
    runTests();

    // Interactive demo
    std::cout << "\n=== Interactive Calculator ===" << std::endl;
    Calculator calc;

    std::vector<std::string> expressions = {
        "5 + 3 * 2",
        "(10 + 5) / 3",
        "2 ^ 10",
        "100 / 4 + 25",
        "10 / 0",  // Will throw
        "sqrt(invalid)",  // Will throw
    };

    for (const auto& expr : expressions) {
        std::cout << "\nEvaluating: " << expr << std::endl;
        try {
            double result = calc.evaluate(expr);
            std::cout << "Result: " << result << std::endl;
        } catch (const DivisionByZeroException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (const MathDomainException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (const InvalidExpressionException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (const CalculatorException& e) {
            std::cerr << "Calculator error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected error: " << e.what() << std::endl;
        }
    }

    // Special functions demo
    std::cout << "\n=== Special Functions ===" << std::endl;

    try {
        std::cout << "sqrt(25) = " << calc.sqrt(25) << std::endl;
        std::cout << "sqrt(2) = " << calc.sqrt(2) << std::endl;
        std::cout << "log(10) = " << calc.log(10) << std::endl;
        std::cout << "pow(2, 8) = " << calc.pow(2, 8) << std::endl;

        // This will throw
        std::cout << "sqrt(-1) = " << calc.sqrt(-1) << std::endl;

    } catch (const MathDomainException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    try {
        // This will throw
        std::cout << "log(-5) = " << calc.log(-5) << std::endl;
    } catch (const MathDomainException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n=== Calculator Demo Complete ===" << std::endl;

    return 0;
}
```

**Compile and run:**
```bash
g++ -std=c++20 -Wall safe_calculator.cpp -o safe_calculator
./safe_calculator
```

## Best Practices for Exception Handling

1. **Use exceptions for exceptional cases** - Not for normal control flow
2. **Catch by const reference** - `catch (const std::exception& e)`
3. **Order catch blocks properly** - Most specific first
4. **Use RAII** - Resources automatically cleaned up
5. **Don't throw from destructors** - Mark destructors `noexcept`
6. **Provide strong exception guarantee** - Use RAII and smart pointers
7. **Document what exceptions can be thrown** - In function comments
8. **Create meaningful exception hierarchies** - Group related errors
9. **Include context in exception messages** - Help debugging
10. **Use `noexcept` where appropriate** - For performance-critical code

## Summary

In this lesson, you learned:

- **try-catch-throw syntax** - Basic exception handling mechanism
- **Standard exception types** - `std::exception` hierarchy
- **Custom exceptions** - Creating domain-specific error types
- **Multiple catch blocks** - Handling different exception types
- **Exception safety** - RAII and automatic resource management
- **Rethrowing exceptions** - Propagating errors with context
- **Best practices** - Writing robust exception-safe code

Exception handling is essential for writing reliable C++ programs that can gracefully handle errors and maintain program stability.

## Next Lesson

Lesson 15: Introduction to Object-Oriented Programming

## Additional Resources

- C++ Reference: Exception Handling
- Effective C++ by Scott Meyers (Exception Safety chapter)
- C++ Core Guidelines: Error Handling
- Practice: Implement exception handling in all previous projects

---

**Exercise**: Modify the Grade Manager from Lesson 13 to use comprehensive exception handling with custom exception classes for file errors, validation errors, and data errors.
