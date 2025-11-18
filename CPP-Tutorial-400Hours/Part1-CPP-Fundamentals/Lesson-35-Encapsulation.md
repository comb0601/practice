# Lesson 35: Encapsulation

**Duration**: 7 hours
**Difficulty**: Intermediate

## Table of Contents
1. What is Encapsulation?
2. Benefits of Encapsulation
3. Implementing Encapsulation
4. Getters and Setters Design
5. Data Validation
6. Encapsulation Patterns
7. Real-World Applications
8. Exercises
9. Practice Projects

---

## 1. What is Encapsulation?

**Encapsulation** is the bundling of data and methods that operate on that data within a single unit (class), and restricting direct access to some of the object's components.

### Core Principles:

1. **Data Hiding**: Make data members private
2. **Controlled Access**: Provide public methods for controlled access
3. **Implementation Hiding**: Hide how things work internally
4. **Interface Design**: Expose only what's necessary

### Encapsulation Visualization:

```
┌────────────────────────────────┐
│         Public Interface        │
│  ┌──────────────────────────┐  │
│  │  Public Methods (API)     │  │
│  └──────────────────────────┘  │
│  ┌──────────────────────────┐  │
│  │  Private Data             │  │
│  │  Private Methods          │  │
│  │  (Hidden Implementation)  │  │
│  └──────────────────────────┘  │
└────────────────────────────────┘
```

### Example 1: Without Encapsulation (BAD)

```cpp
#include <iostream>
using namespace std;

class BankAccountBad {
public:
    double balance;  // Direct access - NO validation!
    string accountNumber;
};

int main() {
    BankAccountBad account;
    account.balance = 1000;

    // Problems: No validation, no control
    account.balance = -5000;  // Negative balance allowed!
    account.balance = 999999999;  // No limits!

    cout << "Balance: " << account.balance << endl;
    return 0;
}
```

### Example 2: With Encapsulation (GOOD)

```cpp
#include <iostream>
#include <string>
using namespace std;

class BankAccountGood {
private:
    double balance;
    string accountNumber;

public:
    BankAccountGood(string accNum, double initial)
        : accountNumber(accNum), balance(initial) {
        if (balance < 0) balance = 0;
    }

    bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            return true;
        }
        cout << "Invalid deposit amount!" << endl;
        return false;
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        cout << "Invalid withdrawal!" << endl;
        return false;
    }

    double getBalance() const {
        return balance;
    }
};

int main() {
    BankAccountGood account("ACC001", 1000);

    account.deposit(500);
    account.withdraw(200);

    // Cannot set negative balance
    // account.balance = -5000;  // ERROR: balance is private

    cout << "Balance: " << account.getBalance() << endl;
    return 0;
}
```

---

## 2. Benefits of Encapsulation

### Benefit 1: Data Protection

```cpp
#include <iostream>
using namespace std;

class Counter {
private:
    int count;
    const int MIN_VALUE = 0;
    const int MAX_VALUE = 100;

public:
    Counter() : count(0) {}

    void increment() {
        if (count < MAX_VALUE) {
            count++;
        } else {
            cout << "Maximum value reached!" << endl;
        }
    }

    void decrement() {
        if (count > MIN_VALUE) {
            count--;
        } else {
            cout << "Minimum value reached!" << endl;
        }
    }

    int getValue() const { return count; }
};

int main() {
    Counter counter;

    for (int i = 0; i < 105; i++) {
        counter.increment();  // Safely capped at 100
    }

    cout << "Count: " << counter.getValue() << endl;
    return 0;
}
```

### Benefit 2: Flexible Implementation Changes

```cpp
#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    // Implementation can change without affecting users
    string firstName;
    string lastName;

public:
    Person(string first, string last)
        : firstName(first), lastName(last) {}

    // Public interface remains same even if implementation changes
    string getFullName() const {
        // Could change to: return lastName + ", " + firstName;
        return firstName + " " + lastName;
    }

    void setName(string first, string last) {
        firstName = first;
        lastName = last;
    }
};

int main() {
    Person person("John", "Doe");
    cout << person.getFullName() << endl;
    return 0;
}
```

### Benefit 3: Debugging and Maintenance

```cpp
#include <iostream>
using namespace std;

class DebugCounter {
private:
    int value;
    int accessCount;  // Track how many times accessed

public:
    DebugCounter() : value(0), accessCount(0) {}

    void setValue(int v) {
        value = v;
        accessCount++;
        cout << "[DEBUG] Value set to " << v
             << " (access #" << accessCount << ")" << endl;
    }

    int getValue() const {
        return value;
    }

    int getAccessCount() const {
        return accessCount;
    }
};

int main() {
    DebugCounter counter;
    counter.setValue(10);
    counter.setValue(20);
    counter.setValue(30);

    cout << "Value: " << counter.getValue() << endl;
    cout << "Total accesses: " << counter.getAccessCount() << endl;
    return 0;
}
```

---

## 3. Implementing Encapsulation

### Complete Example: Student Class

```cpp
#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    // Private data
    int studentID;
    string name;
    double gpa;
    int age;

    // Private validation methods
    bool isValidGPA(double g) const {
        return g >= 0.0 && g <= 4.0;
    }

    bool isValidAge(int a) const {
        return a >= 16 && a <= 100;
    }

    bool isValidName(const string& n) const {
        return !n.empty() && n.length() >= 2;
    }

public:
    // Constructor
    Student(int id, const string& n, double g, int a)
        : studentID(id), name("Unknown"), gpa(0.0), age(0) {
        // Use setters for validation
        setName(n);
        setGPA(g);
        setAge(a);
    }

    // Getters (read-only access)
    int getStudentID() const { return studentID; }
    string getName() const { return name; }
    double getGPA() const { return gpa; }
    int getAge() const { return age; }

    // Setters (controlled modification)
    bool setName(const string& n) {
        if (isValidName(n)) {
            name = n;
            return true;
        }
        cout << "Invalid name!" << endl;
        return false;
    }

    bool setGPA(double g) {
        if (isValidGPA(g)) {
            gpa = g;
            return true;
        }
        cout << "Invalid GPA (must be 0.0-4.0)!" << endl;
        return false;
    }

    bool setAge(int a) {
        if (isValidAge(a)) {
            age = a;
            return true;
        }
        cout << "Invalid age!" << endl;
        return false;
    }

    // Business logic
    char getGrade() const {
        if (gpa >= 3.7) return 'A';
        if (gpa >= 3.0) return 'B';
        if (gpa >= 2.0) return 'C';
        if (gpa >= 1.0) return 'D';
        return 'F';
    }

    bool isHonorStudent() const {
        return gpa >= 3.5;
    }

    void display() const {
        cout << "\n=== Student Information ===" << endl;
        cout << "ID: " << studentID << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "GPA: " << gpa << endl;
        cout << "Grade: " << getGrade() << endl;
        cout << "Honor Student: " << (isHonorStudent() ? "Yes" : "No") << endl;
    }
};

int main() {
    Student student(1001, "Alice Johnson", 3.8, 20);
    student.display();

    // Try invalid operations
    student.setGPA(5.0);  // Rejected
    student.setAge(10);   // Rejected

    // Valid operations
    student.setGPA(3.9);
    student.display();

    return 0;
}
```

---

## 4. Getters and Setters Design

### Design Patterns for Getters/Setters:

### Pattern 1: Basic Get/Set

```cpp
class SimpleClass {
private:
    int value;

public:
    int getValue() const { return value; }
    void setValue(int v) { value = v; }
};
```

### Pattern 2: Validated Set

```cpp
class ValidatedClass {
private:
    int value;
    const int MIN = 0;
    const int MAX = 100;

public:
    int getValue() const { return value; }

    bool setValue(int v) {
        if (v >= MIN && v <= MAX) {
            value = v;
            return true;
        }
        return false;
    }
};
```

### Pattern 3: Const Reference Return

```cpp
class StringClass {
private:
    string data;

public:
    const string& getData() const { return data; }
    void setData(const string& d) { data = d; }
};
```

### Pattern 4: Chaining Setters

```cpp
class ChainableClass {
private:
    int x, y;

public:
    ChainableClass& setX(int val) {
        x = val;
        return *this;
    }

    ChainableClass& setY(int val) {
        y = val;
        return *this;
    }

    // Usage: obj.setX(10).setY(20);
};
```

### Complete Example: Configuration Class

```cpp
#include <iostream>
#include <string>
using namespace std;

class Configuration {
private:
    string serverAddress;
    int port;
    int timeout;
    bool useSSL;

    bool isValidPort(int p) const {
        return p > 0 && p <= 65535;
    }

    bool isValidTimeout(int t) const {
        return t >= 0 && t <= 3600;
    }

public:
    Configuration()
        : serverAddress("localhost"),
          port(8080),
          timeout(30),
          useSSL(false) {}

    // Getters
    const string& getServerAddress() const { return serverAddress; }
    int getPort() const { return port; }
    int getTimeout() const { return timeout; }
    bool isSSLEnabled() const { return useSSL; }

    // Chainable setters
    Configuration& setServerAddress(const string& addr) {
        if (!addr.empty()) {
            serverAddress = addr;
        }
        return *this;
    }

    Configuration& setPort(int p) {
        if (isValidPort(p)) {
            port = p;
        }
        return *this;
    }

    Configuration& setTimeout(int t) {
        if (isValidTimeout(t)) {
            timeout = t;
        }
        return *this;
    }

    Configuration& enableSSL(bool enable) {
        useSSL = enable;
        return *this;
    }

    void display() const {
        cout << "Server: " << serverAddress << ":" << port << endl;
        cout << "Timeout: " << timeout << "s" << endl;
        cout << "SSL: " << (useSSL ? "Enabled" : "Disabled") << endl;
    }
};

int main() {
    Configuration config;

    // Method chaining
    config.setServerAddress("api.example.com")
          .setPort(443)
          .setTimeout(60)
          .enableSSL(true);

    config.display();

    return 0;
}
```

---

## 5. Data Validation

### Comprehensive Validation Example:

```cpp
#include <iostream>
#include <string>
#include <regex>
using namespace std;

class UserProfile {
private:
    string username;
    string email;
    int age;
    string phoneNumber;

    bool isValidUsername(const string& user) const {
        // 3-20 characters, alphanumeric and underscore only
        if (user.length() < 3 || user.length() > 20) return false;
        for (char c : user) {
            if (!isalnum(c) && c != '_') return false;
        }
        return true;
    }

    bool isValidEmail(const string& e) const {
        // Simple email validation
        regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return regex_match(e, emailPattern);
    }

    bool isValidAge(int a) const {
        return a >= 13 && a <= 120;
    }

    bool isValidPhone(const string& phone) const {
        // Simple: 10-15 digits
        if (phone.length() < 10 || phone.length() > 15) return false;
        for (char c : phone) {
            if (!isdigit(c) && c != '-' && c != ' ' && c != '+' && c != '(' && c != ')') {
                return false;
            }
        }
        return true;
    }

public:
    UserProfile() : username(""), email(""), age(0), phoneNumber("") {}

    bool setUsername(const string& user) {
        if (isValidUsername(user)) {
            username = user;
            return true;
        }
        cout << "Invalid username! (3-20 chars, alphanumeric + underscore)" << endl;
        return false;
    }

    bool setEmail(const string& e) {
        if (isValidEmail(e)) {
            email = e;
            return true;
        }
        cout << "Invalid email address!" << endl;
        return false;
    }

    bool setAge(int a) {
        if (isValidAge(a)) {
            age = a;
            return true;
        }
        cout << "Invalid age! (13-120)" << endl;
        return false;
    }

    bool setPhoneNumber(const string& phone) {
        if (isValidPhone(phone)) {
            phoneNumber = phone;
            return true;
        }
        cout << "Invalid phone number!" << endl;
        return false;
    }

    // Getters
    string getUsername() const { return username; }
    string getEmail() const { return email; }
    int getAge() const { return age; }
    string getPhoneNumber() const { return phoneNumber; }

    void display() const {
        cout << "\n=== User Profile ===" << endl;
        cout << "Username: " << username << endl;
        cout << "Email: " << email << endl;
        cout << "Age: " << age << endl;
        cout << "Phone: " << phoneNumber << endl;
    }
};

int main() {
    UserProfile user;

    // Test validation
    user.setUsername("john_doe123");
    user.setEmail("john@example.com");
    user.setAge(25);
    user.setPhoneNumber("+1-555-123-4567");

    user.display();

    // Test invalid inputs
    cout << "\nTesting invalid inputs:" << endl;
    user.setUsername("ab");  // Too short
    user.setEmail("invalid-email");  // Invalid format
    user.setAge(10);  // Too young

    return 0;
}
```

---

## 6. Encapsulation Patterns

### Pattern 1: Immutable Class

```cpp
#include <iostream>
#include <string>
using namespace std;

class ImmutablePoint {
private:
    const int x;
    const int y;

public:
    ImmutablePoint(int x, int y) : x(x), y(y) {}

    int getX() const { return x; }
    int getY() const { return y; }

    // No setters - object cannot be modified after creation

    // Instead, create new objects
    ImmutablePoint moveTo(int newX, int newY) const {
        return ImmutablePoint(newX, newY);
    }

    ImmutablePoint moveBy(int dx, int dy) const {
        return ImmutablePoint(x + dx, y + dy);
    }

    void display() const {
        cout << "(" << x << ", " << y << ")" << endl;
    }
};

int main() {
    ImmutablePoint p1(10, 20);
    p1.display();

    ImmutablePoint p2 = p1.moveBy(5, 5);
    p2.display();

    p1.display();  // p1 unchanged

    return 0;
}
```

### Pattern 2: Lazy Initialization

```cpp
#include <iostream>
#include <string>
using namespace std;

class DataProcessor {
private:
    mutable string cachedResult;  // mutable allows modification in const methods
    mutable bool isCached;
    string inputData;

    string expensiveComputation() const {
        cout << "Performing expensive computation..." << endl;
        // Simulate expensive operation
        string result = "Processed: " + inputData;
        return result;
    }

public:
    DataProcessor(const string& data)
        : inputData(data), isCached(false), cachedResult("") {}

    const string& getResult() const {
        if (!isCached) {
            cachedResult = expensiveComputation();
            isCached = true;
        }
        return cachedResult;
    }

    void setInputData(const string& data) {
        inputData = data;
        isCached = false;  // Invalidate cache
    }
};

int main() {
    DataProcessor processor("Hello");

    cout << processor.getResult() << endl;  // Computes
    cout << processor.getResult() << endl;  // Uses cache

    processor.setInputData("World");
    cout << processor.getResult() << endl;  // Computes again

    return 0;
}
```

---

## 7. Real-World Applications

### Application: Smart Home Temperature Controller

```cpp
#include <iostream>
#include <string>
using namespace std;

class SmartThermostat {
private:
    double currentTemp;
    double targetTemp;
    bool isOn;
    string mode;  // "heat", "cool", "auto"

    const double MIN_TEMP = 10.0;
    const double MAX_TEMP = 35.0;
    const double TEMP_TOLERANCE = 0.5;

    bool isValidTemp(double temp) const {
        return temp >= MIN_TEMP && temp <= MAX_TEMP;
    }

    bool isValidMode(const string& m) const {
        return m == "heat" || m == "cool" || m == "auto";
    }

    void adjustHeating() {
        if (!isOn) return;

        if (mode == "heat" && currentTemp < targetTemp - TEMP_TOLERANCE) {
            cout << "[HEATING] Current: " << currentTemp
                 << "°C, Target: " << targetTemp << "°C" << endl;
        } else if (mode == "cool" && currentTemp > targetTemp + TEMP_TOLERANCE) {
            cout << "[COOLING] Current: " << currentTemp
                 << "°C, Target: " << targetTemp << "°C" << endl;
        } else {
            cout << "[IDLE] Temperature at target" << endl;
        }
    }

public:
    SmartThermostat()
        : currentTemp(20.0), targetTemp(22.0), isOn(false), mode("auto") {}

    // Controlled access methods
    bool setTargetTemp(double temp) {
        if (isValidTemp(temp)) {
            targetTemp = temp;
            cout << "Target temperature set to " << temp << "°C" << endl;
            adjustHeating();
            return true;
        }
        cout << "Invalid temperature! (Range: " << MIN_TEMP
             << "-" << MAX_TEMP << "°C)" << endl;
        return false;
    }

    bool setMode(const string& m) {
        if (isValidMode(m)) {
            mode = m;
            cout << "Mode set to: " << m << endl;
            adjustHeating();
            return true;
        }
        cout << "Invalid mode! (Options: heat, cool, auto)" << endl;
        return false;
    }

    void turnOn() {
        isOn = true;
        cout << "Thermostat turned ON" << endl;
        adjustHeating();
    }

    void turnOff() {
        isOn = false;
        cout << "Thermostat turned OFF" << endl;
    }

    void updateCurrentTemp(double temp) {
        if (isValidTemp(temp)) {
            currentTemp = temp;
            adjustHeating();
        }
    }

    // Read-only access
    double getCurrentTemp() const { return currentTemp; }
    double getTargetTemp() const { return targetTemp; }
    bool isPoweredOn() const { return isOn; }
    string getMode() const { return mode; }

    void displayStatus() const {
        cout << "\n=== Thermostat Status ===" << endl;
        cout << "Power: " << (isOn ? "ON" : "OFF") << endl;
        cout << "Mode: " << mode << endl;
        cout << "Current Temp: " << currentTemp << "°C" << endl;
        cout << "Target Temp: " << targetTemp << "°C" << endl;
    }
};

int main() {
    SmartThermostat thermostat;

    thermostat.displayStatus();

    thermostat.turnOn();
    thermostat.setMode("heat");
    thermostat.setTargetTemp(24.0);

    // Simulate temperature changes
    thermostat.updateCurrentTemp(20.0);
    thermostat.updateCurrentTemp(22.0);
    thermostat.updateCurrentTemp(24.0);

    thermostat.displayStatus();

    return 0;
}
```

---

## 8. Exercises

### Exercise 1: Email Validator
Create a class that validates and stores email addresses.

```cpp
class EmailAddress {
private:
    string email;
    bool isValid(const string& e) const;

public:
    bool setEmail(const string& e);
    string getEmail() const;
    string getDomain() const;
    string getUsername() const;
};
```

### Exercise 2: Shopping Cart
Create an encapsulated shopping cart.

```cpp
class ShoppingCart {
private:
    struct Item {
        string name;
        double price;
        int quantity;
    };
    Item items[100];
    int itemCount;

public:
    bool addItem(const string& name, double price, int quantity);
    bool removeItem(const string& name);
    double calculateTotal() const;
    void displayCart() const;
};
```

### Exercise 3: Time Class
Create a time class with validation.

```cpp
class Time {
private:
    int hours;
    int minutes;
    int seconds;

public:
    Time(int h = 0, int m = 0, int s = 0);
    bool setHours(int h);
    bool setMinutes(int m);
    bool setSeconds(int s);
    void tick();  // Add one second
    string toString() const;
};
```

---

## 9. Practice Projects

### Project: Complete ATM System

```cpp
#include <iostream>
#include <string>
using namespace std;

class ATMAccount {
private:
    const string accountNumber;
    string accountHolder;
    double balance;
    string pin;
    bool isLocked;
    int failedAttempts;

    const int MAX_FAILED_ATTEMPTS = 3;
    const double DAILY_WITHDRAWAL_LIMIT = 1000.0;
    double todayWithdrawn;

    bool verifyPIN(const string& inputPin) const {
        return inputPin == pin && !isLocked;
    }

    void lockAccount() {
        isLocked = true;
        cout << "Account locked due to multiple failed attempts!" << endl;
    }

public:
    ATMAccount(const string& accNum, const string& holder,
               const string& pinCode, double initialBalance)
        : accountNumber(accNum), accountHolder(holder),
          pin(pinCode), balance(initialBalance),
          isLocked(false), failedAttempts(0), todayWithdrawn(0) {}

    bool authenticate(const string& inputPin) {
        if (isLocked) {
            cout << "Account is locked!" << endl;
            return false;
        }

        if (verifyPIN(inputPin)) {
            failedAttempts = 0;
            cout << "Authentication successful!" << endl;
            return true;
        } else {
            failedAttempts++;
            cout << "Invalid PIN! Attempts left: "
                 << (MAX_FAILED_ATTEMPTS - failedAttempts) << endl;

            if (failedAttempts >= MAX_FAILED_ATTEMPTS) {
                lockAccount();
            }
            return false;
        }
    }

    bool withdraw(double amount, const string& inputPin) {
        if (!authenticate(inputPin)) return false;

        if (amount <= 0) {
            cout << "Invalid amount!" << endl;
            return false;
        }

        if (todayWithdrawn + amount > DAILY_WITHDRAWAL_LIMIT) {
            cout << "Daily withdrawal limit exceeded!" << endl;
            return false;
        }

        if (amount > balance) {
            cout << "Insufficient funds!" << endl;
            return false;
        }

        balance -= amount;
        todayWithdrawn += amount;
        cout << "Withdrawn: $" << amount << endl;
        cout << "New balance: $" << balance << endl;
        return true;
    }

    bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: $" << amount << endl;
            cout << "New balance: $" << balance << endl;
            return true;
        }
        return false;
    }

    double checkBalance(const string& inputPin) {
        if (authenticate(inputPin)) {
            return balance;
        }
        return -1;
    }

    bool changePIN(const string& oldPin, const string& newPin) {
        if (!authenticate(oldPin)) return false;

        if (newPin.length() != 4) {
            cout << "PIN must be 4 digits!" << endl;
            return false;
        }

        pin = newPin;
        cout << "PIN changed successfully!" << endl;
        return true;
    }

    void displayInfo(const string& inputPin) {
        if (!authenticate(inputPin)) return;

        cout << "\n=== Account Information ===" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Balance: $" << balance << endl;
        cout << "Today's Withdrawals: $" << todayWithdrawn << endl;
        cout << "Status: " << (isLocked ? "LOCKED" : "ACTIVE") << endl;
    }
};

int main() {
    ATMAccount account("1234567890", "John Doe", "1234", 5000.0);

    account.displayInfo("1234");

    account.withdraw(200, "1234");
    account.deposit(100);

    double balance = account.checkBalance("1234");
    cout << "\nCurrent balance: $" << balance << endl;

    // Test wrong PIN
    cout << "\n=== Testing wrong PIN ===" << endl;
    account.withdraw(100, "0000");
    account.withdraw(100, "1111");
    account.withdraw(100, "2222");

    return 0;
}
```

---

## Key Takeaways

1. **Encapsulation** bundles data and methods together
2. **Data hiding** protects internal state
3. **Validation** ensures data integrity
4. **Getters/Setters** provide controlled access
5. **Private** for data, **Public** for interface
6. **Implementation** can change without affecting users
7. **Design clean interfaces** with minimal exposure
8. **Validate all inputs** in setter methods

---

## Summary Checklist

Before moving to Lesson 36, ensure you can:
- [ ] Implement proper encapsulation
- [ ] Create effective getters and setters
- [ ] Validate data in all setters
- [ ] Hide implementation details
- [ ] Design clean public interfaces
- [ ] Use const correctness
- [ ] Implement validation patterns
- [ ] Apply encapsulation in real projects

---

## Next Lesson Preview

In **Lesson 36: Static Members**, we'll learn:
- Static data members
- Static member functions
- Class-level vs instance-level data
- Static member initialization
- Use cases for static members

Excellent work! You now understand encapsulation deeply!
