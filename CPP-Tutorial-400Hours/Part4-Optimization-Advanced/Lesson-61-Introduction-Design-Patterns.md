# Lesson 61: Introduction to Design Patterns

**Duration**: 6 hours
**Difficulty**: Advanced
**Part**: 4 - Optimization & Advanced Techniques

## Table of Contents
1. What Are Design Patterns?
2. History and Origins
3. Categories of Design Patterns
4. Benefits and When to Use Them
5. SOLID Principles
6. Pattern Selection Guidelines
7. Anti-Patterns to Avoid
8. Complete Example: Evolution of Code Design
9. Exercises

---

## 1. What Are Design Patterns?

Design patterns are **reusable solutions to commonly occurring problems** in software design. They represent best practices evolved over time by experienced object-oriented software developers.

### Key Characteristics:

**NOT a finished design** - You can't just copy-paste a pattern into your code. A pattern is a description or template for how to solve a problem that can be used in many different situations.

**Language-independent** - While we implement them in C++, the concepts apply to any OOP language.

**Proven solutions** - These patterns have been tested and refined through real-world usage.

### Real-World Analogy:

Think of design patterns like architectural blueprints:
- A blueprint for a house isn't the actual house
- You can use the same blueprint in different locations with modifications
- The blueprint solves common problems (plumbing, electrical, structural support)
- Experienced architects recognize these patterns instantly

---

## 2. History and Origins

### The Gang of Four (GoF)

In 1994, four authors published the seminal book **"Design Patterns: Elements of Reusable Object-Oriented Software"**:

1. **Erich Gamma**
2. **Richard Helm**
3. **Ralph Johnson**
4. **John Vlissides**

This book cataloged **23 classic design patterns** that form the foundation of modern software design.

### Before Design Patterns:

```cpp
// Typical 1980s C++ code - procedural with global state
int globalCounter = 0;
ConfigData* globalConfig = nullptr;

void InitSystem() {
    globalConfig = new ConfigData();
    globalConfig->LoadFromFile("config.ini");
}

void DoSomething() {
    if (globalConfig == nullptr) {
        InitSystem();  // Implicit initialization
    }
    globalCounter++;
    // Use globalConfig...
}

// Problems: Global state, unclear initialization order, not thread-safe
```

### After Design Patterns:

```cpp
// Modern approach using Singleton pattern (we'll learn this in Lesson 62)
class ConfigManager {
private:
    static ConfigManager* instance;
    ConfigData config;

    ConfigManager() {  // Private constructor
        config.LoadFromFile("config.ini");
    }

public:
    static ConfigManager& GetInstance() {
        if (instance == nullptr) {
            instance = new ConfigManager();
        }
        return *instance;
    }

    ConfigData& GetConfig() { return config; }
};

// Usage is clean and explicit
void DoSomething() {
    ConfigManager::GetInstance().GetConfig().DoWork();
}
```

---

## 3. Categories of Design Patterns

The Gang of Four organized patterns into **three main categories**:

### A. Creational Patterns (5 patterns)

**Purpose**: Object creation mechanisms

| Pattern | Purpose | Lesson |
|---------|---------|--------|
| **Singleton** | Ensure only one instance exists | 62 |
| **Factory Method** | Create objects without specifying exact class | 63 |
| **Abstract Factory** | Families of related objects | 64 |
| **Builder** | Construct complex objects step-by-step | 65 |
| **Prototype** | Clone existing objects | 66 |

### B. Structural Patterns (7 patterns)

**Purpose**: Object composition and relationships

| Pattern | Purpose | Lesson |
|---------|---------|--------|
| **Adapter** | Make incompatible interfaces work together | 67 |
| **Bridge** | Separate abstraction from implementation | 68 |
| **Composite** | Tree structures of objects | 69 |
| **Decorator** | Add responsibilities dynamically | 70 |
| **Facade** | Simplified interface to complex system | 71 |
| **Proxy** | Placeholder for another object | 72 |
| **Flyweight** | Share common state efficiently | (Bonus) |

### C. Behavioral Patterns (11 patterns)

**Purpose**: Object interaction and responsibility distribution

| Pattern | Purpose | Lesson |
|---------|---------|--------|
| **Chain of Responsibility** | Pass requests along handler chain | 73 |
| **Command** | Encapsulate requests as objects | 74 |
| **Iterator** | Access elements sequentially | 75 |
| **Mediator** | Centralize complex communications | (Bonus) |
| **Memento** | Capture and restore object state | (Bonus) |
| **Observer** | Notify dependents of state changes | 76 |
| **State** | Change behavior when state changes | 77 |
| **Strategy** | Encapsulate interchangeable algorithms | 78 |
| **Template Method** | Define algorithm skeleton | 79 |
| **Visitor** | Operations on object structure | 80 |

---

## 4. Benefits and When to Use Them

### Benefits:

**1. Code Reusability**
```cpp
// Instead of reinventing the wheel every time
class FileLogger { /* ... */ };
class ConsoleLogger { /* ... */ };
class NetworkLogger { /* ... */ };

// Use Strategy pattern to make loggers interchangeable
```

**2. Improved Communication**
```cpp
// "We need a Singleton for the database connection"
// Everyone immediately understands: single instance, global access
```

**3. Proven Solutions**
```cpp
// Don't waste time debugging a custom solution
// Use tested patterns that others have validated
```

**4. Easier Maintenance**
```cpp
// Developers familiar with patterns can understand code faster
// "Oh, this is the Observer pattern for event handling"
```

**5. Better Architecture**
```cpp
// Patterns encourage:
// - Loose coupling
// - High cohesion
// - Single Responsibility Principle
```

### When NOT to Use Patterns:

**❌ Don't force patterns where they don't fit**
```cpp
// Overkill: Using Abstract Factory for 2 simple classes
// Better: Just use simple inheritance
```

**❌ Don't use patterns to show off**
```cpp
// Bad motivation: "Let's use the Visitor pattern because it's cool"
// Good motivation: "We need to add operations without changing classes"
```

**❌ Keep it simple first**
```cpp
// Start simple, refactor to patterns when complexity grows
// YAGNI principle: You Aren't Gonna Need It
```

---

## 5. SOLID Principles

Design patterns implement **SOLID principles** - five fundamental OOP design principles:

### S - Single Responsibility Principle

**A class should have only one reason to change**

```cpp
// ❌ BAD: Multiple responsibilities
class Employee {
    string name;
    double salary;

    void CalculateSalary() { /* ... */ }
    void SaveToDatabase() { /* ... */ }  // Database concern
    void GenerateReport() { /* ... */ }   // Reporting concern
};

// ✅ GOOD: Single responsibility
class Employee {
    string name;
    double salary;
    void CalculateSalary() { /* ... */ }
};

class EmployeeRepository {
    void SaveToDatabase(const Employee& emp) { /* ... */ }
};

class EmployeeReportGenerator {
    void GenerateReport(const Employee& emp) { /* ... */ }
};
```

### O - Open/Closed Principle

**Open for extension, closed for modification**

```cpp
// ❌ BAD: Must modify class to add new shapes
class AreaCalculator {
public:
    double Calculate(const string& shape, double param1, double param2) {
        if (shape == "circle") {
            return 3.14159 * param1 * param1;
        } else if (shape == "rectangle") {
            return param1 * param2;
        }
        // Must add more if-else for new shapes!
    }
};

// ✅ GOOD: Can extend without modifying
class Shape {
public:
    virtual double GetArea() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) {}
    double GetArea() const override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double GetArea() const override {
        return width * height;
    }
};

// Add Triangle without modifying existing code
class Triangle : public Shape {
    double base, height;
public:
    Triangle(double b, double h) : base(b), height(h) {}
    double GetArea() const override {
        return 0.5 * base * height;
    }
};
```

### L - Liskov Substitution Principle

**Derived classes must be substitutable for their base classes**

```cpp
// ✅ GOOD: Square IS-A Rectangle (mathematically)
class Rectangle {
protected:
    double width, height;
public:
    virtual void SetWidth(double w) { width = w; }
    virtual void SetHeight(double h) { height = h; }
    virtual double GetArea() const { return width * height; }
};

class Square : public Rectangle {
public:
    void SetWidth(double w) override {
        width = height = w;  // Maintain square invariant
    }
    void SetHeight(double h) override {
        width = height = h;
    }
};

// Usage works correctly
void ProcessRectangle(Rectangle* rect) {
    rect->SetWidth(5);
    rect->SetHeight(10);
    cout << "Area: " << rect->GetArea() << endl;
}

// Both work correctly
Rectangle rect;
ProcessRectangle(&rect);  // Area: 50

Square square;
ProcessRectangle(&square);  // Area: 100 (10x10, last set value)
```

### I - Interface Segregation Principle

**Clients shouldn't depend on interfaces they don't use**

```cpp
// ❌ BAD: Fat interface
class IWorker {
public:
    virtual void Work() = 0;
    virtual void Eat() = 0;
    virtual void Sleep() = 0;
};

class Robot : public IWorker {
    void Work() override { /* OK */ }
    void Eat() override { /* Robots don't eat! */ }
    void Sleep() override { /* Robots don't sleep! */ }
};

// ✅ GOOD: Segregated interfaces
class IWorkable {
public:
    virtual void Work() = 0;
    virtual ~IWorkable() = default;
};

class IFeedable {
public:
    virtual void Eat() = 0;
    virtual ~IFeedable() = default;
};

class ISleepable {
public:
    virtual void Sleep() = 0;
    virtual ~ISleepable() = default;
};

class Human : public IWorkable, public IFeedable, public ISleepable {
    void Work() override { cout << "Working...\n"; }
    void Eat() override { cout << "Eating...\n"; }
    void Sleep() override { cout << "Sleeping...\n"; }
};

class Robot : public IWorkable {
    void Work() override { cout << "Processing...\n"; }
    // Only implements what it needs!
};
```

### D - Dependency Inversion Principle

**Depend on abstractions, not concretions**

```cpp
// ❌ BAD: High-level module depends on low-level module
class MySQLDatabase {
public:
    void SaveData(const string& data) {
        cout << "Saving to MySQL: " << data << endl;
    }
};

class UserService {
    MySQLDatabase db;  // Tight coupling!
public:
    void CreateUser(const string& username) {
        db.SaveData(username);
        // Can't switch to PostgreSQL without modifying this class
    }
};

// ✅ GOOD: Both depend on abstraction
class IDatabase {
public:
    virtual void SaveData(const string& data) = 0;
    virtual ~IDatabase() = default;
};

class MySQLDatabase : public IDatabase {
public:
    void SaveData(const string& data) override {
        cout << "Saving to MySQL: " << data << endl;
    }
};

class PostgreSQLDatabase : public IDatabase {
public:
    void SaveData(const string& data) override {
        cout << "Saving to PostgreSQL: " << data << endl;
    }
};

class UserService {
    IDatabase* db;  // Depends on abstraction
public:
    UserService(IDatabase* database) : db(database) {}

    void CreateUser(const string& username) {
        db->SaveData(username);
        // Can use ANY database implementation!
    }
};

// Usage
int main() {
    MySQLDatabase mysql;
    PostgreSQLDatabase postgres;

    UserService service1(&mysql);
    service1.CreateUser("John");  // Uses MySQL

    UserService service2(&postgres);
    service2.CreateUser("Jane");  // Uses PostgreSQL
}
```

---

## 6. Pattern Selection Guidelines

### Decision Tree:

**Creating Objects?** → **Creational Patterns**
- Need only one instance? → **Singleton**
- Creating families of objects? → **Abstract Factory**
- Complex construction process? → **Builder**
- Want to clone objects? → **Prototype**
- Defer instantiation to subclasses? → **Factory Method**

**Composing Objects?** → **Structural Patterns**
- Incompatible interfaces? → **Adapter**
- Hide complexity? → **Facade**
- Represent part-whole hierarchies? → **Composite**
- Add responsibilities dynamically? → **Decorator**
- Control access? → **Proxy**
- Separate abstraction from implementation? → **Bridge**

**Object Behavior?** → **Behavioral Patterns**
- Communicate between objects? → **Observer**
- Encapsulate algorithms? → **Strategy**
- Encapsulate requests? → **Command**
- Change behavior based on state? → **State**
- Define algorithm skeleton? → **Template Method**
- Traverse elements? → **Iterator**
- Pass request along chain? → **Chain of Responsibility**
- Add operations without changing classes? → **Visitor**

---

## 7. Anti-Patterns to Avoid

### God Object

```cpp
// ❌ Anti-pattern: One class does everything
class Application {
    void InitializeDatabase() { /* ... */ }
    void CreateUI() { /* ... */ }
    void HandleNetwork() { /* ... */ }
    void ProcessBusinessLogic() { /* ... */ }
    void GenerateReports() { /* ... */ }
    void ManageUsers() { /* ... */ }
    // ... 50 more methods
};
```

### Spaghetti Code

```cpp
// ❌ Anti-pattern: Tangled control flow
void ProcessOrder(Order* order) {
    if (order != nullptr) {
        if (order->IsValid()) {
            if (CheckInventory(order)) {
                if (ProcessPayment(order)) {
                    if (ShipOrder(order)) {
                        // Nested 5 levels deep!
                        if (SendConfirmation(order)) {
                            // Success
                        }
                    }
                }
            }
        }
    }
}
```

### Lava Flow

```cpp
// ❌ Anti-pattern: Dead code nobody dares remove
class OldSystem {
    void LegacyMethod() {
        // TODO: This might be used somewhere?
        // Written in 2005, nobody knows what it does
        // Too scared to delete it
    }
};
```

---

## 8. Complete Example: Evolution of Code Design

### Problem: Game Character System

Let's see how design evolves from naive to pattern-based:

### Version 1: No Patterns (Bad)

```cpp
#include <iostream>
#include <string>
using namespace std;

class Character {
public:
    string name;
    string type;  // "warrior", "mage", "archer"
    int health;
    int damage;

    void Attack(Character& target) {
        if (type == "warrior") {
            cout << name << " slashes with sword!\n";
            target.health -= damage * 2;
        } else if (type == "mage") {
            cout << name << " casts fireball!\n";
            target.health -= damage * 3;
        } else if (type == "archer") {
            cout << name << " shoots arrow!\n";
            target.health -= damage;
        }
        // Adding new character type requires modifying this method!
    }

    void SpecialAbility() {
        if (type == "warrior") {
            cout << name << " uses Shield Bash!\n";
        } else if (type == "mage") {
            cout << name << " teleports!\n";
        } else if (type == "archer") {
            cout << name << " shoots multiple arrows!\n";
        }
    }
};

// Problems:
// - Violates Open/Closed Principle
// - Violates Single Responsibility Principle
// - Hard to maintain and extend
```

### Version 2: Using Strategy Pattern (Better)

```cpp
#include <iostream>
#include <string>
#include <memory>
using namespace std;

// Strategy interface
class AttackStrategy {
public:
    virtual void Execute(const string& attacker, int baseDamage, int& targetHealth) = 0;
    virtual ~AttackStrategy() = default;
};

// Concrete strategies
class SwordAttack : public AttackStrategy {
public:
    void Execute(const string& attacker, int baseDamage, int& targetHealth) override {
        cout << attacker << " slashes with sword!\n";
        targetHealth -= baseDamage * 2;
    }
};

class FireballAttack : public AttackStrategy {
public:
    void Execute(const string& attacker, int baseDamage, int& targetHealth) override {
        cout << attacker << " casts fireball!\n";
        targetHealth -= baseDamage * 3;
    }
};

class ArrowAttack : public AttackStrategy {
public:
    void Execute(const string& attacker, int baseDamage, int& targetHealth) override {
        cout << attacker << " shoots arrow!\n";
        targetHealth -= baseDamage;
    }
};

// Character class using Strategy
class Character {
private:
    string name;
    int health;
    int baseDamage;
    unique_ptr<AttackStrategy> attackStrategy;

public:
    Character(const string& n, int h, int dmg, unique_ptr<AttackStrategy> strategy)
        : name(n), health(h), baseDamage(dmg), attackStrategy(move(strategy)) {}

    void Attack(Character& target) {
        attackStrategy->Execute(name, baseDamage, target.health);
    }

    void SetAttackStrategy(unique_ptr<AttackStrategy> newStrategy) {
        attackStrategy = move(newStrategy);
    }

    int GetHealth() const { return health; }
    string GetName() const { return name; }
};

int main() {
    // Create characters with different attack strategies
    Character warrior("Conan", 100, 10, make_unique<SwordAttack>());
    Character mage("Merlin", 80, 8, make_unique<FireballAttack>());
    Character archer("Legolas", 90, 12, make_unique<ArrowAttack>());

    cout << "=== Battle Begins ===\n\n";

    warrior.Attack(mage);
    cout << "Merlin's health: " << mage.GetHealth() << "\n\n";

    mage.Attack(archer);
    cout << "Legolas's health: " << archer.GetHealth() << "\n\n";

    archer.Attack(warrior);
    cout << "Conan's health: " << warrior.GetHealth() << "\n\n";

    // Can change strategy at runtime!
    cout << "=== Warrior picks up a bow ===\n\n";
    warrior.SetAttackStrategy(make_unique<ArrowAttack>());
    warrior.Attack(mage);

    return 0;
}

// Benefits:
// ✅ Easy to add new attack types without modifying Character
// ✅ Can change attack strategy at runtime
// ✅ Each strategy is a separate class (Single Responsibility)
// ✅ Open/Closed Principle satisfied
```

**Output:**
```
=== Battle Begins ===

Conan slashes with sword!
Merlin's health: 60

Merlin casts fireball!
Legolas's health: 66

Legolas shoots arrow!
Conan's health: 88

=== Warrior picks up a bow ===

Conan shoots arrow!
```

---

## 9. Exercises

### Exercise 1: Identify the Problem

Identify what's wrong with this code and suggest which pattern(s) could help:

```cpp
class ReportGenerator {
public:
    void GenerateReport(const string& type, const vector<string>& data) {
        if (type == "PDF") {
            // 200 lines of PDF generation code
        } else if (type == "HTML") {
            // 200 lines of HTML generation code
        } else if (type == "Excel") {
            // 200 lines of Excel generation code
        }
    }
};
```

**Questions:**
1. Which SOLID principle is violated?
2. What happens when you need to add CSV format?
3. Which pattern would you use to fix this?

### Exercise 2: Apply SOLID Principles

Refactor this code to follow SOLID principles:

```cpp
class EmailService {
    string smtpServer;
    int port;

public:
    void SendEmail(const string& to, const string& subject, const string& body) {
        // Connect to SMTP
        cout << "Connecting to " << smtpServer << ":" << port << endl;
        // Send email
        cout << "Sending email to " << to << endl;
        // Log to database
        cout << "INSERT INTO email_log VALUES ('" << to << "', '" << subject << "')" << endl;
        // Update statistics
        cout << "UPDATE statistics SET emails_sent = emails_sent + 1" << endl;
    }
};
```

### Exercise 3: Pattern Recognition

For each scenario, identify which pattern category (Creational, Structural, or Behavioral) would be most appropriate:

1. **Scenario A**: You need to ensure only one instance of a Logger class exists throughout your application.

2. **Scenario B**: You have a legacy FileReader class with a different interface than your new system expects.

3. **Scenario C**: You need to notify multiple UI components when data changes.

4. **Scenario D**: You want to build a complex Computer object with many optional components (RAM, GPU, SSD, etc.).

5. **Scenario E**: You need to add new features to existing objects at runtime without modifying their structure.

---

## Key Takeaways

1. ✅ **Design patterns** are reusable solutions to common problems
2. ✅ The **Gang of Four** cataloged 23 classic patterns in 1994
3. ✅ Patterns fall into **three categories**: Creational, Structural, Behavioral
4. ✅ **SOLID principles** guide good object-oriented design
5. ✅ **Don't force patterns** where they don't fit naturally
6. ✅ Patterns improve **communication** among developers
7. ✅ Start **simple**, refactor to patterns when complexity grows
8. ✅ Patterns help achieve **loose coupling** and **high cohesion**

---

## Common Mistakes

### ❌ Pattern Overuse:
```cpp
// Don't create a Factory for creating simple objects
SimpleObject* obj = SimpleObjectFactory::CreateSimpleObject();
// Just do: SimpleObject* obj = new SimpleObject();
```

### ❌ Premature Optimization:
```cpp
// Don't implement complex patterns for future requirements that may never come
// Wait until you actually need the flexibility
```

### ❌ Misunderstanding the Problem:
```cpp
// Using Singleton when you really just need a global variable
// Using Observer when a simple callback would suffice
```

---

## Next Lesson Preview

In **Lesson 62**, we'll dive deep into our first creational pattern:

**The Singleton Pattern**
- Thread-safe implementation
- Lazy vs eager initialization
- Meyer's Singleton
- Real-world examples (Logger, ConfigManager, ResourceManager)

---

## Additional Resources

- **Book**: "Design Patterns" by Gang of Four (essential reading)
- **Book**: "Head First Design Patterns" (beginner-friendly)
- **Website**: [Refactoring.Guru](https://refactoring.guru/design-patterns) (excellent visual explanations)
- **Book**: "Clean Code" by Robert C. Martin (SOLID principles)

---

## Summary Checklist

Before moving to Lesson 62, ensure you understand:

- [ ] What design patterns are and why they matter
- [ ] The three main categories of patterns
- [ ] All five SOLID principles
- [ ] When to use (and NOT use) patterns
- [ ] The difference between patterns and anti-patterns
- [ ] How patterns improve code maintainability
- [ ] Pattern selection guidelines

**Congratulations!** You're ready to learn specific design patterns!

Next up: **The Singleton Pattern** - ensuring only one instance exists!
