# Lesson 66: Prototype Pattern

**Duration**: 5 hours
**Difficulty**: Advanced
**Pattern Type**: Creational
**Part**: 4 - Optimization & Advanced Techniques

## Table of Contents
1. What is the Prototype Pattern?
2. Problem and Solution
3. Clone Method
4. Shallow vs Deep Copy
5. Prototype Registry
6. Real-World Examples
7. Complete Working Examples
8. Advantages and Disadvantages
9. When to Use
10. Exercises

---

## 1. What is the Prototype Pattern?

The **Prototype Pattern** creates new objects by **cloning existing objects** (prototypes) rather than creating them from scratch.

### Intent:
- Specify kinds of objects using prototypical instance
- Create new objects by copying prototype
- Avoid expensive initialization
- Reduce subclassing

### Real-World Analogy:

**Cell Division**: Cells reproduce by dividing and copying their DNA - they clone themselves rather than being built from scratch.

**Document Templates**: Instead of creating a new document from scratch, you copy a template and modify it.

**Photocopier**: Make copies of an existing document rather than retyping it.

---

## 2. Problem and Solution

### The Problem:

```cpp
// ❌ Creating objects is expensive
class ComplexObject {
    vector<int> largeData;
    DatabaseConnection* dbConn;

public:
    ComplexObject() {
        // Expensive operations
        LoadFromDatabase();        // 5 seconds
        ProcessData();             // 3 seconds
        InitializeConnections();   // 2 seconds
        // Total: 10 seconds per object!
    }

    void LoadFromDatabase() { /* expensive */ }
    void ProcessData() { /* expensive */ }
    void InitializeConnections() { /* expensive */ }
};

// Creating 100 objects = 1000 seconds!
for (int i = 0; i < 100; ++i) {
    ComplexObject* obj = new ComplexObject();  // 10 seconds each
}
```

### The Solution:

```cpp
#include <iostream>
#include <memory>
using namespace std;

class ComplexObject {
    vector<int> largeData;
    string config;

public:
    ComplexObject() {
        cout << "Expensive initialization...\n";
        // Simulate expensive operation
    }

    // Copy constructor for cloning
    ComplexObject(const ComplexObject& other) {
        largeData = other.largeData;
        config = other.config;
        cout << "Quick clone operation!\n";
    }

    // Clone method
    unique_ptr<ComplexObject> Clone() const {
        return make_unique<ComplexObject>(*this);
    }

    void SetConfig(const string& cfg) { config = cfg; }
    string GetConfig() const { return config; }
};

int main() {
    // Create prototype once (expensive)
    ComplexObject prototype;
    prototype.SetConfig("Initial Config");

    // Clone many times (cheap!)
    auto obj1 = prototype.Clone();
    auto obj2 = prototype.Clone();
    auto obj3 = prototype.Clone();

    return 0;
}
```

**Output:**
```
Expensive initialization...
Quick clone operation!
Quick clone operation!
Quick clone operation!
```

---

## 3. Clone Method

### Basic Clone Interface:

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Prototype interface
class Prototype {
public:
    virtual unique_ptr<Prototype> Clone() const = 0;
    virtual void Display() const = 0;
    virtual ~Prototype() = default;
};

// Concrete Prototype 1
class ConcretePrototype1 : public Prototype {
private:
    string data;
    int value;

public:
    ConcretePrototype1(const string& d, int v) : data(d), value(v) {}

    unique_ptr<Prototype> Clone() const override {
        return make_unique<ConcretePrototype1>(*this);
    }

    void Display() const override {
        cout << "ConcretePrototype1: " << data << ", " << value << "\n";
    }
};

// Concrete Prototype 2
class ConcretePrototype2 : public Prototype {
private:
    double price;
    string name;

public:
    ConcretePrototype2(const string& n, double p) : name(n), price(p) {}

    unique_ptr<Prototype> Clone() const override {
        return make_unique<ConcretePrototype2>(*this);
    }

    void Display() const override {
        cout << "ConcretePrototype2: " << name << ", $" << price << "\n";
    }
};

int main() {
    // Create prototypes
    unique_ptr<Prototype> proto1 = make_unique<ConcretePrototype1>("Data", 42);
    unique_ptr<Prototype> proto2 = make_unique<ConcretePrototype2>("Product", 99.99);

    // Clone prototypes
    auto clone1 = proto1->Clone();
    auto clone2 = proto2->Clone();

    cout << "Original 1: ";
    proto1->Display();
    cout << "Clone 1: ";
    clone1->Display();

    cout << "Original 2: ";
    proto2->Display();
    cout << "Clone 2: ";
    clone2->Display();

    return 0;
}
```

---

## 4. Shallow vs Deep Copy

### Shallow Copy (Default):

```cpp
#include <iostream>
using namespace std;

class ShallowExample {
private:
    int* data;

public:
    ShallowExample(int value) {
        data = new int(value);
        cout << "Created object with data at: " << data << "\n";
    }

    // Default copy constructor (shallow copy)
    ShallowExample(const ShallowExample& other) = default;

    int GetData() const { return *data; }
    void SetData(int value) { *data = value; }

    ~ShallowExample() {
        delete data;
    }
};

void TestShallowCopy() {
    ShallowExample original(42);
    ShallowExample copy = original;  // Shallow copy!

    cout << "Original data: " << original.GetData() << "\n";
    cout << "Copy data: " << copy.GetData() << "\n";

    copy.SetData(100);

    cout << "After modifying copy:\n";
    cout << "Original data: " << original.GetData() << "\n";  // ALSO CHANGED!
    cout << "Copy data: " << copy.GetData() << "\n";
    // Both point to same memory!
}
```

### Deep Copy (Correct):

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class DeepExample {
private:
    int* data;
    char* name;

public:
    DeepExample(int value, const char* n) {
        data = new int(value);
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        cout << "Created object\n";
    }

    // Deep copy constructor
    DeepExample(const DeepExample& other) {
        // Allocate new memory
        data = new int(*other.data);
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        cout << "Deep copy created\n";
    }

    // Clone method
    DeepExample* Clone() const {
        return new DeepExample(*this);
    }

    int GetData() const { return *data; }
    void SetData(int value) { *data = value; }
    const char* GetName() const { return name; }

    ~DeepExample() {
        delete data;
        delete[] name;
        cout << "Object destroyed\n";
    }
};

int main() {
    DeepExample original(42, "Original");
    DeepExample* copy = original.Clone();

    cout << "Original data: " << original.GetData() << " - " << original.GetName() << "\n";
    cout << "Copy data: " << copy->GetData() << " - " << copy->GetName() << "\n";

    copy->SetData(100);

    cout << "\nAfter modifying copy:\n";
    cout << "Original data: " << original.GetData() << "\n";  // Unchanged!
    cout << "Copy data: " << copy->GetData() << "\n";         // Changed

    delete copy;

    return 0;
}
```

---

## 5. Prototype Registry

### Centralized Prototype Management:

```cpp
#include <iostream>
#include <map>
#include <memory>
#include <string>
using namespace std;

// Prototype interface
class Shape {
public:
    virtual unique_ptr<Shape> Clone() const = 0;
    virtual void Draw() const = 0;
    virtual ~Shape() = default;
};

// Concrete Prototypes
class Circle : public Shape {
private:
    int radius;

public:
    Circle(int r = 10) : radius(r) {}

    unique_ptr<Shape> Clone() const override {
        return make_unique<Circle>(*this);
    }

    void Draw() const override {
        cout << "Drawing Circle with radius " << radius << "\n";
    }

    void SetRadius(int r) { radius = r; }
};

class Rectangle : public Shape {
private:
    int width, height;

public:
    Rectangle(int w = 20, int h = 10) : width(w), height(h) {}

    unique_ptr<Shape> Clone() const override {
        return make_unique<Rectangle>(*this);
    }

    void Draw() const override {
        cout << "Drawing Rectangle " << width << "x" << height << "\n";
    }

    void SetDimensions(int w, int h) {
        width = w;
        height = h;
    }
};

class Triangle : public Shape {
private:
    int base, height;

public:
    Triangle(int b = 15, int h = 12) : base(b), height(h) {}

    unique_ptr<Shape> Clone() const override {
        return make_unique<Triangle>(*this);
    }

    void Draw() const override {
        cout << "Drawing Triangle base=" << base << " height=" << height << "\n";
    }
};

// Prototype Registry
class ShapeRegistry {
private:
    map<string, unique_ptr<Shape>> prototypes;

public:
    void RegisterPrototype(const string& key, unique_ptr<Shape> prototype) {
        prototypes[key] = move(prototype);
    }

    unique_ptr<Shape> Create(const string& key) {
        auto it = prototypes.find(key);
        if (it != prototypes.end()) {
            return it->second->Clone();
        }
        return nullptr;
    }

    void ListPrototypes() const {
        cout << "Registered prototypes:\n";
        for (const auto& pair : prototypes) {
            cout << "  - " << pair.first << "\n";
        }
    }
};

int main() {
    ShapeRegistry registry;

    // Register prototypes
    registry.RegisterPrototype("SmallCircle", make_unique<Circle>(5));
    registry.RegisterPrototype("LargeCircle", make_unique<Circle>(20));
    registry.RegisterPrototype("Square", make_unique<Rectangle>(10, 10));
    registry.RegisterPrototype("WideRectangle", make_unique<Rectangle>(30, 10));
    registry.RegisterPrototype("Triangle", make_unique<Triangle>());

    registry.ListPrototypes();

    cout << "\n=== Creating shapes from prototypes ===\n";

    auto shape1 = registry.Create("SmallCircle");
    auto shape2 = registry.Create("LargeCircle");
    auto shape3 = registry.Create("Square");
    auto shape4 = registry.Create("WideRectangle");
    auto shape5 = registry.Create("Triangle");

    shape1->Draw();
    shape2->Draw();
    shape3->Draw();
    shape4->Draw();
    shape5->Draw();

    return 0;
}
```

**Output:**
```
Registered prototypes:
  - LargeCircle
  - SmallCircle
  - Square
  - Triangle
  - WideRectangle

=== Creating shapes from prototypes ===
Drawing Circle with radius 5
Drawing Circle with radius 20
Drawing Rectangle 10x10
Drawing Rectangle 30x10
Drawing Triangle base=15 height=12
```

---

## 6. Real-World Examples

### Example 1: Game Enemy Factory using Prototypes

```cpp
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Enemy {
protected:
    string name;
    int health;
    int damage;
    int speed;

public:
    Enemy(const string& n, int h, int d, int s)
        : name(n), health(h), damage(d), speed(s) {}

    virtual unique_ptr<Enemy> Clone() const = 0;
    virtual void Attack() const = 0;

    virtual void Display() const {
        cout << name << " [HP:" << health << " DMG:" << damage << " SPD:" << speed << "]\n";
    }

    virtual ~Enemy() = default;
};

class Goblin : public Enemy {
public:
    Goblin() : Enemy("Goblin", 50, 10, 15) {}

    unique_ptr<Enemy> Clone() const override {
        return make_unique<Goblin>(*this);
    }

    void Attack() const override {
        cout << name << " slashes with rusty knife!\n";
    }
};

class Orc : public Enemy {
public:
    Orc() : Enemy("Orc", 100, 20, 10) {}

    unique_ptr<Enemy> Clone() const override {
        return make_unique<Orc>(*this);
    }

    void Attack() const override {
        cout << name << " swings massive axe!\n";
    }
};

class Dragon : public Enemy {
public:
    Dragon() : Enemy("Dragon", 500, 75, 8) {}

    unique_ptr<Enemy> Clone() const override {
        return make_unique<Dragon>(*this);
    }

    void Attack() const override {
        cout << name << " breathes fire!\n";
    }
};

class EnemyFactory {
private:
    map<string, unique_ptr<Enemy>> prototypes;

public:
    void RegisterEnemy(const string& type, unique_ptr<Enemy> prototype) {
        prototypes[type] = move(prototype);
    }

    unique_ptr<Enemy> SpawnEnemy(const string& type) {
        auto it = prototypes.find(type);
        if (it != prototypes.end()) {
            return it->second->Clone();
        }
        return nullptr;
    }
};

class Level {
    vector<unique_ptr<Enemy>> enemies;

public:
    void AddEnemy(unique_ptr<Enemy> enemy) {
        enemies.push_back(move(enemy));
    }

    void DisplayEnemies() const {
        cout << "Enemies in level:\n";
        for (const auto& enemy : enemies) {
            cout << "  ";
            enemy->Display();
        }
    }

    void StartBattle() const {
        cout << "\n=== Battle Start ===\n";
        for (const auto& enemy : enemies) {
            enemy->Attack();
        }
    }
};

int main() {
    EnemyFactory factory;

    // Register enemy prototypes
    factory.RegisterEnemy("goblin", make_unique<Goblin>());
    factory.RegisterEnemy("orc", make_unique<Orc>());
    factory.RegisterEnemy("dragon", make_unique<Dragon>());

    // Create level with multiple enemies
    Level level1;

    // Spawn enemies by cloning prototypes
    level1.AddEnemy(factory.SpawnEnemy("goblin"));
    level1.AddEnemy(factory.SpawnEnemy("goblin"));
    level1.AddEnemy(factory.SpawnEnemy("orc"));
    level1.AddEnemy(factory.SpawnEnemy("goblin"));
    level1.AddEnemy(factory.SpawnEnemy("dragon"));

    level1.DisplayEnemies();
    level1.StartBattle();

    return 0;
}
```

**Output:**
```
Enemies in level:
  Goblin [HP:50 DMG:10 SPD:15]
  Goblin [HP:50 DMG:10 SPD:15]
  Orc [HP:100 DMG:20 SPD:10]
  Goblin [HP:50 DMG:10 SPD:15]
  Dragon [HP:500 DMG:75 SPD:8]

=== Battle Start ===
Goblin slashes with rusty knife!
Goblin slashes with rusty knife!
Orc swings massive axe!
Goblin slashes with rusty knife!
Dragon breathes fire!
```

### Example 2: Document Template System

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <map>
using namespace std;

class Document {
protected:
    string title;
    string header;
    string footer;
    string content;

public:
    virtual unique_ptr<Document> Clone() const = 0;

    void SetTitle(const string& t) { title = t; }
    void SetContent(const string& c) { content = c; }

    virtual void Display() const {
        cout << "======================\n";
        cout << header << "\n";
        cout << "Title: " << title << "\n";
        cout << "----------------------\n";
        cout << content << "\n";
        cout << "----------------------\n";
        cout << footer << "\n";
        cout << "======================\n";
    }

    virtual ~Document() = default;
};

class BusinessLetter : public Document {
public:
    BusinessLetter() {
        header = "ACME Corporation\n123 Business St.\nNew York, NY 10001";
        footer = "Sincerely,\n\n_____________\nJohn Doe\nCEO";
    }

    unique_ptr<Document> Clone() const override {
        return make_unique<BusinessLetter>(*this);
    }
};

class Invoice : public Document {
public:
    Invoice() {
        header = "INVOICE\nACME Corporation";
        footer = "Payment Terms: Net 30\nThank you for your business!";
    }

    unique_ptr<Document> Clone() const override {
        return make_unique<Invoice>(*this);
    }
};

class Resume : public Document {
public:
    Resume() {
        header = "PROFESSIONAL RESUME";
        footer = "References available upon request";
    }

    unique_ptr<Document> Clone() const override {
        return make_unique<Resume>(*this);
    }
};

class DocumentTemplateManager {
private:
    map<string, unique_ptr<Document>> templates;

public:
    void RegisterTemplate(const string& name, unique_ptr<Document> doc) {
        templates[name] = move(doc);
    }

    unique_ptr<Document> CreateFromTemplate(const string& name) {
        auto it = templates.find(name);
        if (it != templates.end()) {
            return it->second->Clone();
        }
        return nullptr;
    }
};

int main() {
    DocumentTemplateManager manager;

    // Register templates
    manager.RegisterTemplate("BusinessLetter", make_unique<BusinessLetter>());
    manager.RegisterTemplate("Invoice", make_unique<Invoice>());
    manager.RegisterTemplate("Resume", make_unique<Resume>());

    // Create documents from templates
    cout << "Creating business letter from template:\n";
    auto letter = manager.CreateFromTemplate("BusinessLetter");
    letter->SetTitle("Partnership Proposal");
    letter->SetContent("We would like to propose a strategic partnership...");
    letter->Display();

    cout << "\nCreating invoice from template:\n";
    auto invoice = manager.CreateFromTemplate("Invoice");
    invoice->SetTitle("Invoice #12345");
    invoice->SetContent("Services Rendered: $5,000.00\nTax: $500.00\nTotal: $5,500.00");
    invoice->Display();

    return 0;
}
```

---

## 7. Complete Working Examples

### Complete Example: Configuration Prototype System

```cpp
#include <iostream>
#include <memory>
#include <map>
#include <string>
using namespace std;

class ServerConfig {
private:
    string name;
    string ipAddress;
    int port;
    int maxConnections;
    int timeout;
    bool sslEnabled;
    string logLevel;

public:
    ServerConfig(const string& n = "Server")
        : name(n), ipAddress("127.0.0.1"), port(8080),
          maxConnections(100), timeout(30), sslEnabled(false),
          logLevel("INFO") {}

    // Copy constructor for cloning
    ServerConfig(const ServerConfig& other) = default;

    unique_ptr<ServerConfig> Clone() const {
        return make_unique<ServerConfig>(*this);
    }

    // Setters
    void SetName(const string& n) { name = n; }
    void SetIPAddress(const string& ip) { ipAddress = ip; }
    void SetPort(int p) { port = p; }
    void SetMaxConnections(int max) { maxConnections = max; }
    void SetTimeout(int t) { timeout = t; }
    void SetSSL(bool enabled) { sslEnabled = enabled; }
    void SetLogLevel(const string& level) { logLevel = level; }

    void Display() const {
        cout << "Server Configuration: " << name << "\n";
        cout << "  IP Address: " << ipAddress << "\n";
        cout << "  Port: " << port << "\n";
        cout << "  Max Connections: " << maxConnections << "\n";
        cout << "  Timeout: " << timeout << "s\n";
        cout << "  SSL: " << (sslEnabled ? "Enabled" : "Disabled") << "\n";
        cout << "  Log Level: " << logLevel << "\n";
    }
};

class ConfigurationManager {
private:
    map<string, unique_ptr<ServerConfig>> presets;

public:
    void CreatePresets() {
        // Development preset
        auto devConfig = make_unique<ServerConfig>("Development");
        devConfig->SetPort(3000);
        devConfig->SetMaxConnections(10);
        devConfig->SetLogLevel("DEBUG");
        presets["development"] = move(devConfig);

        // Production preset
        auto prodConfig = make_unique<ServerConfig>("Production");
        prodConfig->SetIPAddress("0.0.0.0");
        prodConfig->SetPort(443);
        prodConfig->SetMaxConnections(1000);
        prodConfig->SetTimeout(60);
        prodConfig->SetSSL(true);
        prodConfig->SetLogLevel("ERROR");
        presets["production"] = move(prodConfig);

        // Testing preset
        auto testConfig = make_unique<ServerConfig>("Testing");
        testConfig->SetPort(8080);
        testConfig->SetMaxConnections(50);
        testConfig->SetLogLevel("WARN");
        presets["testing"] = move(testConfig);
    }

    unique_ptr<ServerConfig> GetConfig(const string& preset) {
        auto it = presets.find(preset);
        if (it != presets.end()) {
            return it->second->Clone();
        }
        return nullptr;
    }

    void ListPresets() const {
        cout << "Available configuration presets:\n";
        for (const auto& pair : presets) {
            cout << "  - " << pair.first << "\n";
        }
    }
};

int main() {
    ConfigurationManager configMgr;
    configMgr.CreatePresets();

    configMgr.ListPresets();
    cout << "\n";

    // Create configurations from presets
    cout << "=== Development Environment ===\n";
    auto devConfig = configMgr.GetConfig("development");
    devConfig->Display();

    cout << "\n=== Production Environment ===\n";
    auto prodConfig = configMgr.GetConfig("production");
    prodConfig->Display();

    cout << "\n=== Testing Environment ===\n";
    auto testConfig = configMgr.GetConfig("testing");
    testConfig->Display();

    // Customize cloned configuration
    cout << "\n=== Custom Configuration (based on development) ===\n";
    auto customConfig = configMgr.GetConfig("development");
    customConfig->SetName("Custom Dev Server");
    customConfig->SetPort(5000);
    customConfig->Display();

    return 0;
}
```

---

## 8. Advantages and Disadvantages

### Advantages:

1. ✅ **Performance**: Faster than creating from scratch
2. ✅ **Reduced Initialization**: Expensive operations done once
3. ✅ **Runtime Flexibility**: Can create objects dynamically
4. ✅ **Hides Complexity**: Client doesn't know concrete classes
5. ✅ **Reduces Subclassing**: Alternative to Factory hierarchy

### Disadvantages:

1. ❌ **Deep Copy Complexity**: Hard to implement for complex objects
2. ❌ **Circular References**: Difficult to clone objects with circular references
3. ❌ **Memory**: Maintaining prototypes uses memory

---

## 9. When to Use

### Use Prototype When:
- ✅ Object creation is expensive
- ✅ System needs to be independent of how objects are created
- ✅ Classes to instantiate are specified at runtime
- ✅ Avoiding parallel class hierarchies

### Don't Use When:
- ❌ Objects are simple to construct
- ❌ Deep copy is too complex
- ❌ Prototypes change frequently

---

## 10. Exercises

### Exercise 1: Clone Graphics Elements
Create clonable graphics elements (line, circle, text) with a drawing canvas that can duplicate selected elements.

### Exercise 2: User Profile Templates
Implement user profile templates (Admin, Regular User, Guest) that can be cloned and customized.

### Exercise 3: Email Template System
Create email templates with predefined formatting that users can clone and customize.

---

## Key Takeaways

1. ✅ Prototype creates objects by **cloning** existing ones
2. ✅ **Clone method** is core to the pattern
3. ✅ **Deep copy** vs **shallow copy** is crucial
4. ✅ **Prototype Registry** manages available prototypes
5. ✅ Great for **expensive object creation**
6. ✅ Provides **runtime flexibility**
7. ✅ Alternative to **large factory hierarchies**
8. ✅ Used in game development, document systems

---

## Next Lesson Preview

In **Lesson 67**, we'll learn about **Adapter Pattern**:
- Making incompatible interfaces work together
- Class adapter vs object adapter
- Wrapping legacy code
- Third-party library integration

---

## Summary Checklist

Before moving to Lesson 67, ensure you can:

- [ ] Explain Prototype pattern
- [ ] Implement Clone method
- [ ] Understand shallow vs deep copy
- [ ] Create prototype registry
- [ ] Identify when to use prototypes
- [ ] Handle complex object cloning
- [ ] Use prototypes for performance

**Excellent work!** Prototype pattern mastered!

Next: **Adapter Pattern** - interface compatibility!
