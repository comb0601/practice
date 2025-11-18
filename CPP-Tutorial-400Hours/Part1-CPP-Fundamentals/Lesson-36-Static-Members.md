# Lesson 36: Static Members

**Duration**: 7 hours
**Difficulty**: Intermediate

## Table of Contents
1. What are Static Members?
2. Static Data Members
3. Static Member Functions
4. Static Constants
5. Counting Objects with Static
6. Singleton Pattern
7. Static vs Instance Members
8. Exercises
9. Practice Projects

---

## 1. What are Static Members?

**Static members** belong to the class itself, not to individual objects. They are shared among all instances of the class.

### Key Concepts:

- **Static Data Members**: Shared by all objects
- **Static Member Functions**: Can be called without an object
- **Class-Level**: Belong to the class, not instances
- **Single Copy**: Only one copy exists regardless of object count

### Visualization:

```
Regular Members:          Static Members:
Object1: [data]          ┌──────────────┐
Object2: [data]    vs    │ Class: [data]│
Object3: [data]          └──────────────┘
```

---

## 2. Static Data Members

**Static data members** are shared by all objects of the class.

### Syntax:
```cpp
class ClassName {
private:
    static int staticVariable;  // Declaration

public:
    // Methods using static variable
};

// Definition and initialization (outside class)
int ClassName::staticVariable = 0;
```

### Example 1: Basic Static Member

```cpp
#include <iostream>
using namespace std;

class Counter {
private:
    static int objectCount;  // Shared by all objects
    int instanceID;

public:
    Counter() {
        objectCount++;
        instanceID = objectCount;
        cout << "Object " << instanceID << " created. "
             << "Total objects: " << objectCount << endl;
    }

    ~Counter() {
        objectCount--;
        cout << "Object " << instanceID << " destroyed. "
             << "Remaining objects: " << objectCount << endl;
    }

    static int getObjectCount() {
        return objectCount;
    }
};

// Initialize static member
int Counter::objectCount = 0;

int main() {
    cout << "Creating objects..." << endl;

    Counter c1;
    Counter c2;
    Counter c3;

    cout << "\nTotal objects: " << Counter::getObjectCount() << endl;

    cout << "\nExiting scope..." << endl;
    return 0;
}
```

**Output**:
```
Creating objects...
Object 1 created. Total objects: 1
Object 2 created. Total objects: 2
Object 3 created. Total objects: 3

Total objects: 3

Exiting scope...
Object 3 destroyed. Remaining objects: 2
Object 2 destroyed. Remaining objects: 1
Object 1 destroyed. Remaining objects: 0
```

### Example 2: Shared Configuration

```cpp
#include <iostream>
#include <string>
using namespace std;

class DatabaseConnection {
private:
    static string serverAddress;
    static int port;
    static int activeConnections;

    int connectionID;

public:
    DatabaseConnection() {
        activeConnections++;
        connectionID = activeConnections;
        cout << "Connection " << connectionID << " established to "
             << serverAddress << ":" << port << endl;
    }

    ~DatabaseConnection() {
        cout << "Connection " << connectionID << " closed" << endl;
        activeConnections--;
    }

    // Static methods to configure shared settings
    static void setServerAddress(const string& addr) {
        serverAddress = addr;
    }

    static void setPort(int p) {
        port = p;
    }

    static int getActiveConnections() {
        return activeConnections;
    }

    static void displayConfig() {
        cout << "Server: " << serverAddress << ":" << port << endl;
        cout << "Active Connections: " << activeConnections << endl;
    }
};

// Initialize static members
string DatabaseConnection::serverAddress = "localhost";
int DatabaseConnection::port = 3306;
int DatabaseConnection::activeConnections = 0;

int main() {
    // Configure shared settings
    DatabaseConnection::setServerAddress("db.example.com");
    DatabaseConnection::setPort(5432);

    DatabaseConnection::displayConfig();

    DatabaseConnection conn1;
    DatabaseConnection conn2;

    cout << "\n";
    DatabaseConnection::displayConfig();

    return 0;
}
```

---

## 3. Static Member Functions

**Static member functions** can be called without creating an object and can only access static members.

### Characteristics:
- Called using class name: `ClassName::functionName()`
- Cannot access non-static members
- No `this` pointer
- Cannot be `const` or `volatile`

### Example 1: Utility Functions

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class MathUtils {
public:
    // Static utility functions
    static double square(double x) {
        return x * x;
    }

    static double cube(double x) {
        return x * x * x;
    }

    static double power(double base, int exponent) {
        return pow(base, exponent);
    }

    static bool isPrime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i <= sqrt(n); i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    static int factorial(int n) {
        if (n <= 1) return 1;
        return n * factorial(n - 1);
    }
};

int main() {
    // Call static functions without creating object
    cout << "5 squared: " << MathUtils::square(5) << endl;
    cout << "3 cubed: " << MathUtils::cube(3) << endl;
    cout << "2^10: " << MathUtils::power(2, 10) << endl;
    cout << "Is 17 prime? " << (MathUtils::isPrime(17) ? "Yes" : "No") << endl;
    cout << "5! = " << MathUtils::factorial(5) << endl;

    return 0;
}
```

### Example 2: Static with Non-Static

```cpp
#include <iostream>
using namespace std;

class BankAccount {
private:
    static int totalAccounts;
    static double totalBalance;

    int accountID;
    double balance;

public:
    BankAccount(double initialBalance) {
        totalAccounts++;
        accountID = totalAccounts;
        balance = initialBalance;
        totalBalance += balance;

        cout << "Account " << accountID << " created with $"
             << balance << endl;
    }

    ~BankAccount() {
        totalBalance -= balance;
        totalAccounts--;
    }

    void deposit(double amount) {
        balance += amount;
        totalBalance += amount;
        cout << "Account " << accountID << ": Deposited $" << amount << endl;
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            totalBalance -= amount;
            cout << "Account " << accountID << ": Withdrawn $" << amount << endl;
        }
    }

    // Instance method
    double getBalance() const {
        return balance;
    }

    // Static methods - class-level information
    static int getTotalAccounts() {
        return totalAccounts;
    }

    static double getTotalBalance() {
        return totalBalance;
    }

    static void displayBankSummary() {
        cout << "\n=== Bank Summary ===" << endl;
        cout << "Total Accounts: " << totalAccounts << endl;
        cout << "Total Balance: $" << totalBalance << endl;
        cout << "Average Balance: $"
             << (totalAccounts > 0 ? totalBalance / totalAccounts : 0) << endl;
    }
};

// Initialize static members
int BankAccount::totalAccounts = 0;
double BankAccount::totalBalance = 0.0;

int main() {
    BankAccount acc1(1000);
    BankAccount acc2(2000);
    BankAccount acc3(1500);

    BankAccount::displayBankSummary();

    acc1.deposit(500);
    acc2.withdraw(200);

    BankAccount::displayBankSummary();

    return 0;
}
```

---

## 4. Static Constants

**Static const members** are constants shared by all objects.

### Example: Configuration Constants

```cpp
#include <iostream>
#include <string>
using namespace std;

class GameConfig {
public:
    // Static constants
    static const int MAX_PLAYERS = 10;
    static const int MAX_LEVEL = 100;
    static const double GRAVITY = 9.81;
    static const string GAME_VERSION;  // Initialized outside

private:
    static int currentPlayers;

public:
    static bool addPlayer() {
        if (currentPlayers < MAX_PLAYERS) {
            currentPlayers++;
            cout << "Player added. Total: " << currentPlayers << endl;
            return true;
        }
        cout << "Maximum players reached!" << endl;
        return false;
    }

    static void displayConfig() {
        cout << "\n=== Game Configuration ===" << endl;
        cout << "Version: " << GAME_VERSION << endl;
        cout << "Max Players: " << MAX_PLAYERS << endl;
        cout << "Max Level: " << MAX_LEVEL << endl;
        cout << "Gravity: " << GRAVITY << " m/s²" << endl;
        cout << "Current Players: " << currentPlayers << endl;
    }
};

// Initialize static members
const string GameConfig::GAME_VERSION = "1.0.5";
int GameConfig::currentPlayers = 0;

int main() {
    GameConfig::displayConfig();

    for (int i = 0; i < 12; i++) {
        GameConfig::addPlayer();
    }

    GameConfig::displayConfig();

    return 0;
}
```

---

## 5. Counting Objects with Static

### Example: Object Lifecycle Tracking

```cpp
#include <iostream>
#include <string>
using namespace std;

class Monster {
private:
    static int totalCreated;
    static int currentlyAlive;
    static int totalKilled;

    string name;
    int health;
    int monsterID;

public:
    Monster(const string& n) : name(n), health(100) {
        totalCreated++;
        currentlyAlive++;
        monsterID = totalCreated;
        cout << "Monster #" << monsterID << " (" << name
             << ") spawned! Alive: " << currentlyAlive << endl;
    }

    ~Monster() {
        currentlyAlive--;
        totalKilled++;
        cout << "Monster #" << monsterID << " (" << name
             << ") destroyed! Alive: " << currentlyAlive << endl;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health <= 0) {
            cout << name << " defeated!" << endl;
        }
    }

    static void displayStats() {
        cout << "\n=== Monster Statistics ===" << endl;
        cout << "Total Created: " << totalCreated << endl;
        cout << "Currently Alive: " << currentlyAlive << endl;
        cout << "Total Killed: " << totalKilled << endl;
    }
};

// Initialize static members
int Monster::totalCreated = 0;
int Monster::currentlyAlive = 0;
int Monster::totalKilled = 0;

int main() {
    {
        Monster m1("Goblin");
        Monster m2("Orc");

        Monster::displayStats();

        {
            Monster m3("Dragon");
            Monster::displayStats();
        }

        Monster::displayStats();
    }

    Monster::displayStats();

    return 0;
}
```

---

## 6. Singleton Pattern

**Singleton** ensures only one instance of a class exists.

### Example: Logger Singleton

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

class Logger {
private:
    static Logger* instance;
    ofstream logFile;
    int logCount;

    // Private constructor
    Logger() : logCount(0) {
        logFile.open("app.log", ios::app);
        if (logFile.is_open()) {
            log("Logger initialized");
        }
    }

    // Delete copy constructor and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    string getCurrentTime() {
        time_t now = time(0);
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&now));
        return string(buffer);
    }

public:
    ~Logger() {
        log("Logger shutting down");
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    static Logger* getInstance() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

    void log(const string& message) {
        logCount++;
        string logEntry = "[" + getCurrentTime() + "] " + message;

        cout << logEntry << endl;

        if (logFile.is_open()) {
            logFile << logEntry << endl;
            logFile.flush();
        }
    }

    int getLogCount() const {
        return logCount;
    }

    static void destroy() {
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
        }
    }
};

// Initialize static pointer
Logger* Logger::instance = nullptr;

int main() {
    // Get singleton instance
    Logger* logger = Logger::getInstance();

    logger->log("Application started");
    logger->log("Processing data...");
    logger->log("Operation completed");

    cout << "\nTotal logs: " << logger->getLogCount() << endl;

    // Same instance everywhere
    Logger* logger2 = Logger::getInstance();
    logger2->log("Another message");

    cout << "Total logs: " << logger2->getLogCount() << endl;

    Logger::destroy();

    return 0;
}
```

---

## 7. Static vs Instance Members

### Comparison Example:

```cpp
#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    // Instance members - each object has its own
    string name;
    int rollNumber;
    double gpa;

    // Static members - shared by all objects
    static string schoolName;
    static int totalStudents;
    static double totalGPA;

public:
    Student(const string& n, int roll, double g)
        : name(n), rollNumber(roll), gpa(g) {
        totalStudents++;
        totalGPA += gpa;
    }

    ~Student() {
        totalStudents--;
        totalGPA -= gpa;
    }

    // Instance method - operates on specific object
    void displayInfo() const {
        cout << "Name: " << name << endl;
        cout << "Roll: " << rollNumber << endl;
        cout << "GPA: " << gpa << endl;
        cout << "School: " << schoolName << endl;  // Can access static
    }

    // Static method - operates on class level
    static void displaySchoolInfo() {
        cout << "\n=== School Information ===" << endl;
        cout << "School Name: " << schoolName << endl;
        cout << "Total Students: " << totalStudents << endl;
        cout << "Average GPA: "
             << (totalStudents > 0 ? totalGPA / totalStudents : 0) << endl;

        // Cannot access instance members
        // cout << name << endl;  // ERROR!
    }

    static void setSchoolName(const string& school) {
        schoolName = school;
    }
};

// Initialize static members
string Student::schoolName = "Central High School";
int Student::totalStudents = 0;
double Student::totalGPA = 0.0;

int main() {
    Student::displaySchoolInfo();

    Student s1("Alice", 101, 3.8);
    Student s2("Bob", 102, 3.5);
    Student s3("Charlie", 103, 3.9);

    s1.displayInfo();

    Student::displaySchoolInfo();

    return 0;
}
```

---

## 8. Exercises

### Exercise 1: ID Generator
Create a class that generates unique IDs.

```cpp
class IDGenerator {
private:
    static int nextID;

public:
    static int generateID();
    static int getCurrentID();
    static void reset();
};
```

### Exercise 2: Statistics Tracker
Track statistics across all objects.

```cpp
class TestScore {
private:
    static int totalTests;
    static double totalScore;
    static double highestScore;

    double score;

public:
    TestScore(double s);
    static double getAverageScore();
    static double getHighestScore();
    static void displayStats();
};
```

### Exercise 3: Resource Pool
Create a resource pool manager.

```cpp
class ResourcePool {
private:
    static const int MAX_RESOURCES = 10;
    static int availableResources;
    static int usedResources;

public:
    static bool acquireResource();
    static void releaseResource();
    static void displayStatus();
};
```

---

## 9. Practice Projects

### Project: Complete Employee Management System

```cpp
#include <iostream>
#include <string>
using namespace std;

class Employee {
private:
    // Static members - company-wide
    static int totalEmployees;
    static double totalSalary;
    static string companyName;
    static const double MIN_SALARY;
    static const double MAX_SALARY;

    // Instance members - per employee
    int employeeID;
    string name;
    string department;
    double salary;

    bool isValidSalary(double s) const {
        return s >= MIN_SALARY && s <= MAX_SALARY;
    }

public:
    Employee(const string& n, const string& dept, double sal)
        : name(n), department(dept) {
        totalEmployees++;
        employeeID = totalEmployees;

        if (isValidSalary(sal)) {
            salary = sal;
        } else {
            salary = MIN_SALARY;
        }

        totalSalary += salary;

        cout << "Employee " << employeeID << " (" << name
             << ") hired!" << endl;
    }

    ~Employee() {
        totalSalary -= salary;
        totalEmployees--;
        cout << "Employee " << employeeID << " (" << name
             << ") left the company" << endl;
    }

    // Instance methods
    void displayInfo() const {
        cout << "\n=== Employee Info ===" << endl;
        cout << "ID: " << employeeID << endl;
        cout << "Name: " << name << endl;
        cout << "Department: " << department << endl;
        cout << "Salary: $" << salary << endl;
        cout << "Company: " << companyName << endl;
    }

    bool raiseSalary(double percentage) {
        double newSalary = salary * (1 + percentage / 100.0);
        if (isValidSalary(newSalary)) {
            totalSalary -= salary;
            salary = newSalary;
            totalSalary += salary;
            cout << name << "'s salary raised to $" << salary << endl;
            return true;
        }
        cout << "Cannot raise salary beyond maximum!" << endl;
        return false;
    }

    // Static methods
    static void setCompanyName(const string& name) {
        companyName = name;
    }

    static string getCompanyName() {
        return companyName;
    }

    static int getTotalEmployees() {
        return totalEmployees;
    }

    static double getAverageSalary() {
        return totalEmployees > 0 ? totalSalary / totalEmployees : 0;
    }

    static void displayCompanyStats() {
        cout << "\n=== Company Statistics ===" << endl;
        cout << "Company: " << companyName << endl;
        cout << "Total Employees: " << totalEmployees << endl;
        cout << "Total Payroll: $" << totalSalary << endl;
        cout << "Average Salary: $" << getAverageSalary() << endl;
        cout << "Salary Range: $" << MIN_SALARY
             << " - $" << MAX_SALARY << endl;
    }
};

// Initialize static members
int Employee::totalEmployees = 0;
double Employee::totalSalary = 0.0;
string Employee::companyName = "Tech Corp";
const double Employee::MIN_SALARY = 30000.0;
const double Employee::MAX_SALARY = 200000.0;

int main() {
    Employee::setCompanyName("Awesome Tech Inc");
    Employee::displayCompanyStats();

    {
        Employee emp1("Alice Johnson", "Engineering", 75000);
        Employee emp2("Bob Smith", "Marketing", 60000);
        Employee emp3("Charlie Brown", "Engineering", 80000);

        Employee::displayCompanyStats();

        emp1.displayInfo();

        emp1.raiseSalary(10);
        emp2.raiseSalary(5);

        Employee::displayCompanyStats();
    }

    cout << "\nAll employees left scope..." << endl;
    Employee::displayCompanyStats();

    return 0;
}
```

---

## Key Takeaways

1. **Static members** belong to the class, not objects
2. **Static data** is shared by all instances
3. **Static functions** can be called without objects
4. **Must initialize** static data members outside class
5. **Static functions** cannot access non-static members
6. **No `this` pointer** in static functions
7. **Singleton pattern** uses private constructor + static instance
8. **Use for** counters, configuration, utilities

---

## Common Mistakes

### Mistake 1: Not Initializing Static Members
```cpp
class Wrong {
    static int value;  // Declared
};
// ERROR: Not initialized! Must add:
// int Wrong::value = 0;
```

### Mistake 2: Accessing Non-Static from Static
```cpp
class Wrong {
    int instanceVar;
    static void staticFunc() {
        instanceVar = 10;  // ERROR: Can't access
    }
};
```

### Mistake 3: Making `this` in Static Function
```cpp
class Wrong {
    static void func() {
        this->value = 10;  // ERROR: No 'this' in static
    }
};
```

---

## Summary Checklist

Before moving to Lesson 37, ensure you can:
- [ ] Declare and initialize static data members
- [ ] Create static member functions
- [ ] Understand static vs instance members
- [ ] Implement object counting with static
- [ ] Use static for configuration
- [ ] Implement singleton pattern
- [ ] Use static constants effectively

---

## Next Lesson Preview

In **Lesson 37: Friend Functions and Classes**, we'll learn:
- What friend functions are
- Friend classes
- When to use friends
- Friend vs member functions
- Best practices for friends

Great job! You now master static members in C++!
