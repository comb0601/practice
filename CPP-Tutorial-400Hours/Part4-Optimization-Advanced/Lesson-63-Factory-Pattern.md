# Lesson 63: Factory Pattern (Factory Method)

**Duration**: 6 hours
**Difficulty**: Advanced
**Pattern Type**: Creational
**Part**: 4 - Optimization & Advanced Techniques

## Table of Contents
1. What is the Factory Pattern?
2. Problem: Direct Object Creation
3. Factory Method Solution
4. Structure and Components
5. Implementation Variations
6. Real-World Examples
7. Factory vs Direct Construction
8. Complete Working Examples
9. Advantages and Disadvantages
10. Exercises

---

## 1. What is the Factory Pattern?

The **Factory Method Pattern** defines an interface for creating objects, but lets **subclasses decide which class to instantiate**. It lets a class defer instantiation to subclasses.

### Intent:
- Define an interface for creating an object
- Let subclasses decide which class to instantiate
- Defer instantiation to subclasses
- Promote loose coupling

### Also Known As:
- **Virtual Constructor**
- **Factory Method**

### Real-World Analogy:

**Vehicle Manufacturing**: A car factory can produce different types of vehicles (sedan, SUV, truck) based on customer orders. The factory knows **how** to build vehicles, but the **specific type** is determined by the order.

**Restaurant Kitchen**: A kitchen can prepare different dishes. The chef (factory) knows how to cook, but what dish gets made depends on the customer's order.

---

## 2. Problem: Direct Object Creation

### The Problem:

```cpp
#include <iostream>
#include <string>
using namespace std;

// Different document types
class WordDocument {
public:
    void Open() { cout << "Opening Word document\n"; }
    void Save() { cout << "Saving Word document\n"; }
};

class PDFDocument {
public:
    void Open() { cout << "Opening PDF document\n"; }
    void Save() { cout << "Saving PDF document\n"; }
};

class ExcelDocument {
public:
    void Open() { cout << "Opening Excel document\n"; }
    void Save() { cout << "Saving Excel document\n"; }
};

// ❌ PROBLEM: Client code must know all concrete classes
class Application {
public:
    void CreateDocument(const string& type) {
        if (type == "word") {
            WordDocument* doc = new WordDocument();
            doc->Open();
            // ...
            delete doc;
        } else if (type == "pdf") {
            PDFDocument* doc = new PDFDocument();
            doc->Open();
            // ...
            delete doc;
        } else if (type == "excel") {
            ExcelDocument* doc = new ExcelDocument();
            doc->Open();
            // ...
            delete doc;
        }
        // Adding new document type requires modifying this code!
    }
};

// Issues:
// 1. Violates Open/Closed Principle
// 2. Tight coupling to concrete classes
// 3. Code duplication
// 4. Hard to maintain and extend
```

---

## 3. Factory Method Solution

### Basic Structure:

```cpp
#include <iostream>
#include <memory>
using namespace std;

// Product interface
class Document {
public:
    virtual void Open() = 0;
    virtual void Save() = 0;
    virtual void Close() = 0;
    virtual ~Document() = default;
};

// Concrete Products
class WordDocument : public Document {
public:
    void Open() override { cout << "Opening Word document\n"; }
    void Save() override { cout << "Saving Word document\n"; }
    void Close() override { cout << "Closing Word document\n"; }
};

class PDFDocument : public Document {
public:
    void Open() override { cout << "Opening PDF document\n"; }
    void Save() override { cout << "Saving PDF document\n"; }
    void Close() override { cout << "Closing PDF document\n"; }
};

// Creator (Abstract Factory)
class Application {
public:
    virtual ~Application() = default;

    // Factory Method (virtual constructor)
    virtual unique_ptr<Document> CreateDocument() = 0;

    // Template method using factory method
    void NewDocument() {
        auto doc = CreateDocument();  // Use factory method
        doc->Open();
        cout << "Document is ready to edit\n";
    }
};

// Concrete Creators
class WordApplication : public Application {
public:
    unique_ptr<Document> CreateDocument() override {
        return make_unique<WordDocument>();
    }
};

class PDFApplication : public Application {
public:
    unique_ptr<Document> CreateDocument() override {
        return make_unique<PDFDocument>();
    }
};

// Usage
int main() {
    cout << "=== Factory Method Pattern ===\n\n";

    WordApplication wordApp;
    wordApp.NewDocument();

    cout << "\n";

    PDFApplication pdfApp;
    pdfApp.NewDocument();

    return 0;
}
```

**Output:**
```
=== Factory Method Pattern ===

Opening Word document
Document is ready to edit

Opening PDF document
Document is ready to edit
```

---

## 4. Structure and Components

### UML Structure:

```
┌─────────────────────┐
│     Creator         │  ← Abstract creator
├─────────────────────┤
│ + FactoryMethod()   │  ← Returns Product
│ + Operation()       │  ← Uses factory method
└─────────────────────┘
          △
          │ inherits
          │
┌─────────┴──────────┐
│                    │
┌─────────────────────┐  ┌─────────────────────┐
│  ConcreteCreatorA   │  │  ConcreteCreatorB   │
├─────────────────────┤  ├─────────────────────┤
│ + FactoryMethod()   │  │ + FactoryMethod()   │
│   return ProductA   │  │   return ProductB   │
└─────────────────────┘  └─────────────────────┘
          │                        │
          │ creates                │ creates
          ▼                        ▼
┌─────────────────────┐  ┌─────────────────────┐
│     ProductA        │  │     ProductB        │
└─────────────────────┘  └─────────────────────┘
          △                        △
          └────────────┬───────────┘
                       │
               ┌───────────────┐
               │    Product    │  ← Abstract product
               └───────────────┘
```

### Components:

**1. Product (Interface)**
```cpp
class Product {
public:
    virtual void Operation() = 0;
    virtual ~Product() = default;
};
```

**2. Concrete Products**
```cpp
class ConcreteProductA : public Product {
public:
    void Operation() override { cout << "Product A operation\n"; }
};

class ConcreteProductB : public Product {
public:
    void Operation() override { cout << "Product B operation\n"; }
};
```

**3. Creator (Abstract Factory)**
```cpp
class Creator {
public:
    virtual unique_ptr<Product> FactoryMethod() = 0;  // Factory method

    void SomeOperation() {
        auto product = FactoryMethod();  // Use factory method
        product->Operation();
    }

    virtual ~Creator() = default;
};
```

**4. Concrete Creators**
```cpp
class ConcreteCreatorA : public Creator {
public:
    unique_ptr<Product> FactoryMethod() override {
        return make_unique<ConcreteProductA>();
    }
};

class ConcreteCreatorB : public Creator {
public:
    unique_ptr<Product> FactoryMethod() override {
        return make_unique<ConcreteProductB>();
    }
};
```

---

## 5. Implementation Variations

### Variation 1: Simple Factory (Not a Pattern, but Useful)

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Product interface
class Shape {
public:
    virtual void Draw() = 0;
    virtual ~Shape() = default;
};

// Concrete products
class Circle : public Shape {
public:
    void Draw() override { cout << "Drawing Circle\n"; }
};

class Rectangle : public Shape {
public:
    void Draw() override { cout << "Drawing Rectangle\n"; }
};

class Triangle : public Shape {
public:
    void Draw() override { cout << "Drawing Triangle\n"; }
};

// Simple Factory (not a design pattern, just a programming idiom)
class ShapeFactory {
public:
    static unique_ptr<Shape> CreateShape(const string& type) {
        if (type == "circle") {
            return make_unique<Circle>();
        } else if (type == "rectangle") {
            return make_unique<Rectangle>();
        } else if (type == "triangle") {
            return make_unique<Triangle>();
        }
        return nullptr;
    }
};

int main() {
    auto circle = ShapeFactory::CreateShape("circle");
    auto rectangle = ShapeFactory::CreateShape("rectangle");
    auto triangle = ShapeFactory::CreateShape("triangle");

    circle->Draw();
    rectangle->Draw();
    triangle->Draw();

    return 0;
}
```

**Output:**
```
Drawing Circle
Drawing Rectangle
Drawing Triangle
```

**Note**: Simple Factory is **not** the Factory Method pattern. It's a single static method that creates objects, violating Open/Closed Principle.

### Variation 2: Parameterized Factory Method

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

enum class VehicleType {
    CAR,
    TRUCK,
    MOTORCYCLE
};

class Vehicle {
public:
    virtual void Drive() = 0;
    virtual ~Vehicle() = default;
};

class Car : public Vehicle {
public:
    void Drive() override { cout << "Driving a car\n"; }
};

class Truck : public Vehicle {
public:
    void Drive() override { cout << "Driving a truck\n"; }
};

class Motorcycle : public Vehicle {
public:
    void Drive() override { cout << "Riding a motorcycle\n"; }
};

// Creator with parameterized factory method
class VehicleFactory {
public:
    virtual unique_ptr<Vehicle> CreateVehicle(VehicleType type) = 0;
    virtual ~VehicleFactory() = default;
};

// Concrete creator
class ConcreteVehicleFactory : public VehicleFactory {
public:
    unique_ptr<Vehicle> CreateVehicle(VehicleType type) override {
        switch (type) {
            case VehicleType::CAR:
                return make_unique<Car>();
            case VehicleType::TRUCK:
                return make_unique<Truck>();
            case VehicleType::MOTORCYCLE:
                return make_unique<Motorcycle>();
            default:
                return nullptr;
        }
    }
};

int main() {
    ConcreteVehicleFactory factory;

    auto car = factory.CreateVehicle(VehicleType::CAR);
    auto truck = factory.CreateVehicle(VehicleType::TRUCK);
    auto motorcycle = factory.CreateVehicle(VehicleType::MOTORCYCLE);

    car->Drive();
    truck->Drive();
    motorcycle->Drive();

    return 0;
}
```

---

## 6. Real-World Examples

### Example 1: UI Component Factory (Cross-Platform)

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Abstract Products
class Button {
public:
    virtual void Render() = 0;
    virtual void Click() = 0;
    virtual ~Button() = default;
};

class Checkbox {
public:
    virtual void Render() = 0;
    virtual void Toggle() = 0;
    virtual ~Checkbox() = default;
};

// Windows Products
class WindowsButton : public Button {
public:
    void Render() override {
        cout << "Rendering Windows-style button [OK]\n";
    }
    void Click() override {
        cout << "Windows button clicked\n";
    }
};

class WindowsCheckbox : public Checkbox {
public:
    void Render() override {
        cout << "Rendering Windows-style checkbox [☑]\n";
    }
    void Toggle() override {
        cout << "Windows checkbox toggled\n";
    }
};

// macOS Products
class MacButton : public Button {
public:
    void Render() override {
        cout << "Rendering macOS-style button (OK)\n";
    }
    void Click() override {
        cout << "macOS button clicked\n";
    }
};

class MacCheckbox : public Checkbox {
public:
    void Render() override {
        cout << "Rendering macOS-style checkbox (☑)\n";
    }
    void Toggle() override {
        cout << "macOS checkbox toggled\n";
    }
};

// Abstract Creator
class UIFactory {
public:
    virtual unique_ptr<Button> CreateButton() = 0;
    virtual unique_ptr<Checkbox> CreateCheckbox() = 0;
    virtual ~UIFactory() = default;

    void RenderUI() {
        auto button = CreateButton();
        auto checkbox = CreateCheckbox();

        button->Render();
        checkbox->Render();
    }
};

// Concrete Creators
class WindowsFactory : public UIFactory {
public:
    unique_ptr<Button> CreateButton() override {
        return make_unique<WindowsButton>();
    }
    unique_ptr<Checkbox> CreateCheckbox() override {
        return make_unique<WindowsCheckbox>();
    }
};

class MacFactory : public UIFactory {
public:
    unique_ptr<Button> CreateButton() override {
        return make_unique<MacButton>();
    }
    unique_ptr<Checkbox> CreateCheckbox() override {
        return make_unique<MacCheckbox>();
    }
};

// Client code
class Application {
    unique_ptr<UIFactory> factory;

public:
    Application(unique_ptr<UIFactory> f) : factory(move(f)) {}

    void CreateUI() {
        cout << "Creating UI components:\n";
        factory->RenderUI();
    }
};

int main() {
    cout << "=== Windows Application ===\n";
    Application windowsApp(make_unique<WindowsFactory>());
    windowsApp.CreateUI();

    cout << "\n=== macOS Application ===\n";
    Application macApp(make_unique<MacFactory>());
    macApp.CreateUI();

    return 0;
}
```

**Output:**
```
=== Windows Application ===
Creating UI components:
Rendering Windows-style button [OK]
Rendering Windows-style checkbox [☑]

=== macOS Application ===
Creating UI components:
Rendering macOS-style button (OK)
Rendering macOS-style checkbox (☑)
```

### Example 2: Logger Factory

```cpp
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <ctime>
using namespace std;

// Abstract Product
class Logger {
public:
    virtual void Log(const string& message) = 0;
    virtual ~Logger() = default;

protected:
    string GetTimestamp() {
        time_t now = time(nullptr);
        char buf[26];
        ctime_s(buf, sizeof(buf), &now);
        string timeStr(buf);
        timeStr.pop_back();  // Remove newline
        return timeStr;
    }
};

// Concrete Products
class ConsoleLogger : public Logger {
public:
    void Log(const string& message) override {
        cout << "[CONSOLE] [" << GetTimestamp() << "] " << message << endl;
    }
};

class FileLogger : public Logger {
    ofstream file;

public:
    FileLogger(const string& filename) {
        file.open(filename, ios::app);
    }

    void Log(const string& message) override {
        if (file.is_open()) {
            file << "[FILE] [" << GetTimestamp() << "] " << message << endl;
            cout << "(Logged to file)\n";
        }
    }

    ~FileLogger() {
        if (file.is_open()) {
            file.close();
        }
    }
};

class NetworkLogger : public Logger {
    string serverAddress;

public:
    NetworkLogger(const string& server) : serverAddress(server) {}

    void Log(const string& message) override {
        cout << "[NETWORK] Sending to " << serverAddress << ": "
             << "[" << GetTimestamp() << "] " << message << endl;
    }
};

// Abstract Creator
class LoggerFactory {
public:
    virtual unique_ptr<Logger> CreateLogger() = 0;
    virtual ~LoggerFactory() = default;
};

// Concrete Creators
class ConsoleLoggerFactory : public LoggerFactory {
public:
    unique_ptr<Logger> CreateLogger() override {
        return make_unique<ConsoleLogger>();
    }
};

class FileLoggerFactory : public LoggerFactory {
    string filename;

public:
    FileLoggerFactory(const string& file) : filename(file) {}

    unique_ptr<Logger> CreateLogger() override {
        return make_unique<FileLogger>(filename);
    }
};

class NetworkLoggerFactory : public LoggerFactory {
    string serverAddress;

public:
    NetworkLoggerFactory(const string& server) : serverAddress(server) {}

    unique_ptr<Logger> CreateLogger() override {
        return make_unique<NetworkLogger>(serverAddress);
    }
};

// Application using logger
class Application {
    unique_ptr<Logger> logger;

public:
    Application(unique_ptr<LoggerFactory> factory) {
        logger = factory->CreateLogger();
    }

    void Run() {
        logger->Log("Application started");
        logger->Log("Processing data...");
        logger->Log("Operation completed");
    }
};

int main() {
    cout << "=== Console Logger ===\n";
    Application app1(make_unique<ConsoleLoggerFactory>());
    app1.Run();

    cout << "\n=== File Logger ===\n";
    Application app2(make_unique<FileLoggerFactory>("app.log"));
    app2.Run();

    cout << "\n=== Network Logger ===\n";
    Application app3(make_unique<NetworkLoggerFactory>("192.168.1.100:5000"));
    app3.Run();

    return 0;
}
```

**Output:**
```
=== Console Logger ===
[CONSOLE] [Mon Nov 18 11:45:30 2025] Application started
[CONSOLE] [Mon Nov 18 11:45:30 2025] Processing data...
[CONSOLE] [Mon Nov 18 11:45:30 2025] Operation completed

=== File Logger ===
(Logged to file)
(Logged to file)
(Logged to file)

=== Network Logger ===
[NETWORK] Sending to 192.168.1.100:5000: [Mon Nov 18 11:45:30 2025] Application started
[NETWORK] Sending to 192.168.1.100:5000: [Mon Nov 18 11:45:30 2025] Processing data...
[NETWORK] Sending to 192.168.1.100:5000: [Mon Nov 18 11:45:30 2025] Operation completed
```

---

## 7. Factory vs Direct Construction

### Comparison:

| Aspect | Direct Construction | Factory Method |
|--------|-------------------|----------------|
| **Coupling** | Tight (knows concrete classes) | Loose (depends on interface) |
| **Flexibility** | Low (hard to change) | High (easy to extend) |
| **Testing** | Difficult (hard to mock) | Easy (can inject test doubles) |
| **Code Location** | Scattered everywhere | Centralized in factory |
| **Maintenance** | Hard (changes ripple) | Easy (changes localized) |

### Example Comparison:

**Direct Construction:**
```cpp
void ProcessDocument(const string& type) {
    Document* doc;

    if (type == "word") {
        doc = new WordDocument();
    } else if (type == "pdf") {
        doc = new PDFDocument();
    }

    doc->Open();
    doc->Save();
    delete doc;

    // Adding new type requires modifying this code
}
```

**Factory Method:**
```cpp
void ProcessDocument(unique_ptr<DocumentFactory> factory) {
    auto doc = factory->CreateDocument();

    doc->Open();
    doc->Save();

    // Adding new type only requires new factory subclass
    // This code remains unchanged!
}
```

---

## 8. Complete Working Examples

### Complete Example: Game Character Factory

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

// Abstract Product: Character
class Character {
protected:
    string name;
    int health;
    int attackPower;

public:
    Character(const string& n, int h, int ap)
        : name(n), health(h), attackPower(ap) {}

    virtual void Attack() = 0;
    virtual void SpecialAbility() = 0;
    virtual void DisplayStats() {
        cout << "Name: " << name << "\n";
        cout << "Health: " << health << "\n";
        cout << "Attack Power: " << attackPower << "\n";
    }

    virtual ~Character() = default;
};

// Concrete Products
class Warrior : public Character {
public:
    Warrior(const string& name)
        : Character(name, 150, 20) {}

    void Attack() override {
        cout << name << " swings sword! Damage: " << attackPower << "\n";
    }

    void SpecialAbility() override {
        cout << name << " uses SHIELD BASH! (Stun enemy)\n";
    }
};

class Mage : public Character {
public:
    Mage(const string& name)
        : Character(name, 80, 35) {}

    void Attack() override {
        cout << name << " casts fireball! Damage: " << attackPower << "\n";
    }

    void SpecialAbility() override {
        cout << name << " uses TELEPORT! (Escape danger)\n";
    }
};

class Archer : public Character {
public:
    Archer(const string& name)
        : Character(name, 100, 25) {}

    void Attack() override {
        cout << name << " shoots arrow! Damage: " << attackPower << "\n";
    }

    void SpecialAbility() override {
        cout << name << " uses MULTI-SHOT! (Hit multiple targets)\n";
    }
};

// Abstract Creator
class CharacterFactory {
public:
    virtual unique_ptr<Character> CreateCharacter(const string& name) = 0;
    virtual ~CharacterFactory() = default;

    // Template method
    void CreateAndShowCharacter(const string& name) {
        cout << "\n=== Creating Character ===\n";
        auto character = CreateCharacter(name);
        character->DisplayStats();
        cout << "\nAbilities:\n";
        character->Attack();
        character->SpecialAbility();
        cout << "========================\n";
    }
};

// Concrete Creators
class WarriorFactory : public CharacterFactory {
public:
    unique_ptr<Character> CreateCharacter(const string& name) override {
        return make_unique<Warrior>(name);
    }
};

class MageFactory : public CharacterFactory {
public:
    unique_ptr<Character> CreateCharacter(const string& name) override {
        return make_unique<Mage>(name);
    }
};

class ArcherFactory : public CharacterFactory {
public:
    unique_ptr<Character> CreateCharacter(const string& name) override {
        return make_unique<Archer>(name);
    }
};

// Game class
class Game {
    vector<unique_ptr<Character>> party;

public:
    void CreateParty() {
        WarriorFactory warriorFactory;
        MageFactory mageFactory;
        ArcherFactory archerFactory;

        party.push_back(warriorFactory.CreateCharacter("Conan"));
        party.push_back(mageFactory.CreateCharacter("Gandalf"));
        party.push_back(archerFactory.CreateCharacter("Legolas"));
    }

    void Battle() {
        cout << "\n\n=== BATTLE BEGINS ===\n";
        for (auto& character : party) {
            character->Attack();
        }

        cout << "\n=== SPECIAL ABILITIES ===\n";
        for (auto& character : party) {
            character->SpecialAbility();
        }
    }
};

int main() {
    cout << "=== Character Factory Pattern Demo ===\n";

    // Show character creation
    WarriorFactory warriorFactory;
    warriorFactory.CreateAndShowCharacter("Conan");

    MageFactory mageFactory;
    mageFactory.CreateAndShowCharacter("Gandalf");

    ArcherFactory archerFactory;
    archerFactory.CreateAndShowCharacter("Legolas");

    // Create game party and battle
    Game game;
    game.CreateParty();
    game.Battle();

    return 0;
}
```

**Output:**
```
=== Character Factory Pattern Demo ===

=== Creating Character ===
Name: Conan
Health: 150
Attack Power: 20

Abilities:
Conan swings sword! Damage: 20
Conan uses SHIELD BASH! (Stun enemy)
========================

=== Creating Character ===
Name: Gandalf
Health: 80
Attack Power: 35

Abilities:
Gandalf casts fireball! Damage: 35
Gandalf uses TELEPORT! (Escape danger)
========================

=== Creating Character ===
Name: Legolas
Health: 100
Attack Power: 25

Abilities:
Legolas shoots arrow! Damage: 25
Legolas uses MULTI-SHOT! (Hit multiple targets)
========================


=== BATTLE BEGINS ===
Conan swings sword! Damage: 20
Gandalf casts fireball! Damage: 35
Legolas shoots arrow! Damage: 25

=== SPECIAL ABILITIES ===
Conan uses SHIELD BASH! (Stun enemy)
Gandalf uses TELEPORT! (Escape danger)
Legolas uses MULTI-SHOT! (Hit multiple targets)
```

---

## 9. Advantages and Disadvantages

### Advantages:

1. ✅ **Loose Coupling**
   - Client code doesn't depend on concrete classes
   - Changes to products don't affect clients

2. ✅ **Open/Closed Principle**
   - Can add new products without modifying existing code
   - Just create new factory subclass

3. ✅ **Single Responsibility**
   - Object creation code is in one place
   - Easy to maintain and modify

4. ✅ **Testability**
   - Easy to inject mock factories for testing
   - Can test with different implementations

5. ✅ **Flexibility**
   - Can change which objects are created at runtime
   - Can configure factories via dependency injection

### Disadvantages:

1. ❌ **Increased Complexity**
   - More classes to maintain
   - More abstract code

2. ❌ **Indirection**
   - Extra layer between client and product
   - Harder to trace code flow

3. ❌ **Overkill for Simple Cases**
   - Too much for creating simple objects
   - Use direct construction if only one type

---

## 10. Exercises

### Exercise 1: Weapon Factory

Create a weapon factory system:
- Abstract `Weapon` class with `Attack()` and `GetDamage()`
- Concrete weapons: `Sword`, `Bow`, `Staff`
- Factory method to create weapons based on character class

### Exercise 2: Database Connection Factory

Implement:
- Abstract `DatabaseConnection` class
- Concrete: `MySQLConnection`, `PostgreSQLConnection`, `SQLiteConnection`
- Factory to create appropriate connection based on config

### Exercise 3: Notification System

Create:
- Abstract `Notification` class with `Send(message)`
- Concrete: `EmailNotification`, `SMSNotification`, `PushNotification`
- Factory to create based on user preference

---

## Key Takeaways

1. ✅ Factory Method **defines interface for object creation**
2. ✅ **Subclasses decide** which class to instantiate
3. ✅ Also called **Virtual Constructor**
4. ✅ Promotes **loose coupling** between client and products
5. ✅ Follows **Open/Closed Principle** - easy to extend
6. ✅ Use when object creation logic is complex or varies
7. ✅ **Simple Factory** is not the Factory Method pattern
8. ✅ Best for creating **families of related objects**

---

## Next Lesson Preview

In **Lesson 64**, we'll learn about the **Abstract Factory Pattern**:
- Creating families of related objects
- Multiple product types
- Consistency among products
- Platform-independent code
- Real examples: UI toolkits, document readers

---

## Summary Checklist

Before moving to Lesson 64, ensure you can:

- [ ] Explain the Factory Method pattern
- [ ] Identify problems solved by Factory Method
- [ ] Implement factory method in C++
- [ ] Distinguish Simple Factory from Factory Method
- [ ] Create product hierarchies
- [ ] Use factory method for loose coupling
- [ ] Understand when to use (and not use) factories

**Great job!** You've mastered the Factory Method pattern!

Next: **Abstract Factory** - families of related objects!
