# Lesson 64: Abstract Factory Pattern

**Duration**: 6 hours
**Difficulty**: Advanced
**Pattern Type**: Creational
**Part**: 4 - Optimization & Advanced Techniques

## Table of Contents
1. What is the Abstract Factory Pattern?
2. Abstract Factory vs Factory Method
3. Problem and Solution
4. Structure and Components
5. Implementation Steps
6. Real-World Examples
7. Complete Working Examples
8. Advantages and Disadvantages
9. When to Use
10. Exercises

---

## 1. What is the Abstract Factory Pattern?

The **Abstract Factory Pattern** provides an interface for creating **families of related or dependent objects** without specifying their concrete classes.

### Intent:
- Create families of related objects
- Ensure objects are compatible
- Hide concrete classes from client
- Provide consistent interface

### Key Difference from Factory Method:
- **Factory Method**: Creates **one** type of product
- **Abstract Factory**: Creates **families** of related products

### Real-World Analogy:

**Furniture Store**: A modern furniture store and a Victorian furniture store both sell chairs, sofas, and tables. The products from each store are designed to work together (same style), but you can't mix modern and Victorian furniture.

**Car Manufacturing**: BMW and Toyota both produce cars, engines, and tires. Each brand's parts are designed to work together, but you can't mix BMW parts with Toyota parts.

---

## 2. Abstract Factory vs Factory Method

### Factory Method:
```cpp
// Creates ONE type of product
class DocumentFactory {
public:
    virtual Document* CreateDocument() = 0;  // One product
};
```

### Abstract Factory:
```cpp
// Creates FAMILY of products
class UIFactory {
public:
    virtual Button* CreateButton() = 0;      // Product A
    virtual Checkbox* CreateCheckbox() = 0;  // Product B
    virtual TextBox* CreateTextBox() = 0;    // Product C
    // All products work together!
};
```

---

## 3. Problem and Solution

### The Problem:

```cpp
// ❌ Client needs to know about all concrete classes
void CreateUI(const string& platform) {
    Button* button;
    Checkbox* checkbox;

    if (platform == "Windows") {
        button = new WindowsButton();
        checkbox = new WindowsCheckbox();
    } else if (platform == "Mac") {
        button = new MacButton();
        checkbox = new MacCheckbox();
    }

    button->Render();
    checkbox->Render();

    // Problems:
    // 1. Client knows about all concrete classes
    // 2. Hard to add new platforms
    // 3. No guarantee button and checkbox are compatible
}
```

### The Solution:

```cpp
#include <iostream>
#include <memory>
using namespace std;

// Abstract Products
class Button {
public:
    virtual void Render() = 0;
    virtual ~Button() = default;
};

class Checkbox {
public:
    virtual void Render() = 0;
    virtual ~Checkbox() = default;
};

// Windows Products (Family 1)
class WindowsButton : public Button {
public:
    void Render() override {
        cout << "Rendering Windows button\n";
    }
};

class WindowsCheckbox : public Checkbox {
public:
    void Render() override {
        cout << "Rendering Windows checkbox\n";
    }
};

// Mac Products (Family 2)
class MacButton : public Button {
public:
    void Render() override {
        cout << "Rendering Mac button\n";
    }
};

class MacCheckbox : public Checkbox {
public:
    void Render() override {
        cout << "Rendering Mac checkbox\n";
    }
};

// Abstract Factory
class UIFactory {
public:
    virtual unique_ptr<Button> CreateButton() = 0;
    virtual unique_ptr<Checkbox> CreateCheckbox() = 0;
    virtual ~UIFactory() = default;
};

// Concrete Factories
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

// ✅ Client code - platform independent!
class Application {
    unique_ptr<UIFactory> factory;

public:
    Application(unique_ptr<UIFactory> f) : factory(move(f)) {}

    void CreateUI() {
        auto button = factory->CreateButton();
        auto checkbox = factory->CreateCheckbox();

        button->Render();
        checkbox->Render();
        // Guaranteed to be compatible!
    }
};

int main() {
    Application windowsApp(make_unique<WindowsFactory>());
    windowsApp.CreateUI();

    Application macApp(make_unique<MacFactory>());
    macApp.CreateUI();

    return 0;
}
```

---

## 4. Structure and Components

### UML Structure:

```
┌──────────────────────┐
│  AbstractFactory     │
├──────────────────────┤
│ + CreateProductA()   │
│ + CreateProductB()   │
└──────────────────────┘
          △
          │
    ┌─────┴─────┐
    │           │
┌───────────────────┐  ┌───────────────────┐
│ ConcreteFactory1  │  │ ConcreteFactory2  │
├───────────────────┤  ├───────────────────┤
│ + CreateProductA()│  │ + CreateProductA()│
│ + CreateProductB()│  │ + CreateProductB()│
└───────────────────┘  └───────────────────┘
    │      │               │      │
    │      │               │      │
    │      └───────┐       │      └───────┐
    │              │       │              │
    ▼              ▼       ▼              ▼
ProductA1      ProductB1  ProductA2    ProductB2
```

---

## 5. Implementation Steps

### Step 1: Define Abstract Products

```cpp
// Product A
class Chair {
public:
    virtual void SitOn() = 0;
    virtual ~Chair() = default;
};

// Product B
class Sofa {
public:
    virtual void LieOn() = 0;
    virtual ~Sofa() = default;
};

// Product C
class Table {
public:
    virtual void PutOn() = 0;
    virtual ~Table() = default;
};
```

### Step 2: Create Concrete Products for Each Family

```cpp
// Modern Family
class ModernChair : public Chair {
public:
    void SitOn() override {
        cout << "Sitting on modern minimalist chair\n";
    }
};

class ModernSofa : public Sofa {
public:
    void LieOn() override {
        cout << "Lying on modern sectional sofa\n";
    }
};

class ModernTable : public Table {
public:
    void PutOn() override {
        cout << "Putting items on glass modern table\n";
    }
};

// Victorian Family
class VictorianChair : public Chair {
public:
    void SitOn() override {
        cout << "Sitting on ornate Victorian chair\n";
    }
};

class VictorianSofa : public Sofa {
public:
    void LieOn() override {
        cout << "Lying on tufted Victorian sofa\n";
    }
};

class VictorianTable : public Table {
public:
    void PutOn() override {
        cout << "Putting items on carved wood Victorian table\n";
    }
};
```

### Step 3: Define Abstract Factory

```cpp
class FurnitureFactory {
public:
    virtual unique_ptr<Chair> CreateChair() = 0;
    virtual unique_ptr<Sofa> CreateSofa() = 0;
    virtual unique_ptr<Table> CreateTable() = 0;
    virtual ~FurnitureFactory() = default;
};
```

### Step 4: Implement Concrete Factories

```cpp
class ModernFurnitureFactory : public FurnitureFactory {
public:
    unique_ptr<Chair> CreateChair() override {
        return make_unique<ModernChair>();
    }
    unique_ptr<Sofa> CreateSofa() override {
        return make_unique<ModernSofa>();
    }
    unique_ptr<Table> CreateTable() override {
        return make_unique<ModernTable>();
    }
};

class VictorianFurnitureFactory : public FurnitureFactory {
public:
    unique_ptr<Chair> CreateChair() override {
        return make_unique<VictorianChair>();
    }
    unique_ptr<Sofa> CreateSofa() override {
        return make_unique<VictorianSofa>();
    }
    unique_ptr<Table> CreateTable() override {
        return make_unique<VictorianTable>();
    }
};
```

### Step 5: Use in Client Code

```cpp
class Room {
    unique_ptr<FurnitureFactory> factory;

public:
    Room(unique_ptr<FurnitureFactory> f) : factory(move(f)) {}

    void Furnish() {
        auto chair = factory->CreateChair();
        auto sofa = factory->CreateSofa();
        auto table = factory->CreateTable();

        chair->SitOn();
        sofa->LieOn();
        table->PutOn();
    }
};

int main() {
    cout << "=== Modern Room ===\n";
    Room modernRoom(make_unique<ModernFurnitureFactory>());
    modernRoom.Furnish();

    cout << "\n=== Victorian Room ===\n";
    Room victorianRoom(make_unique<VictorianFurnitureFactory>());
    victorianRoom.Furnish();
}
```

---

## 6. Real-World Examples

### Example 1: Cross-Platform GUI Framework

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Abstract Products
class Button {
public:
    virtual void Paint() = 0;
    virtual ~Button() = default;
};

class Scrollbar {
public:
    virtual void Paint() = 0;
    virtual ~Scrollbar() = default;
};

class Window {
public:
    virtual void Paint() = 0;
    virtual ~Window() = default;
};

// Windows Family
class WinButton : public Button {
public:
    void Paint() override {
        cout << "  [Button] Windows 11 style\n";
    }
};

class WinScrollbar : public Scrollbar {
public:
    void Paint() override {
        cout << "  [Scrollbar] Windows 11 style\n";
    }
};

class WinWindow : public Window {
public:
    void Paint() override {
        cout << "  [Window] Windows 11 style with rounded corners\n";
    }
};

// macOS Family
class MacButton : public Button {
public:
    void Paint() override {
        cout << "  [Button] macOS Big Sur style\n";
    }
};

class MacScrollbar : public Scrollbar {
public:
    void Paint() override {
        cout << "  [Scrollbar] macOS Big Sur style\n";
    }
};

class MacWindow : public Window {
public:
    void Paint() override {
        cout << "  [Window] macOS Big Sur style with traffic lights\n";
    }
};

// Linux Family
class LinuxButton : public Button {
public:
    void Paint() override {
        cout << "  [Button] GTK+ style\n";
    }
};

class LinuxScrollbar : public Scrollbar {
public:
    void Paint() override {
        cout << "  [Scrollbar] GTK+ style\n";
    }
};

class LinuxWindow : public Window {
public:
    void Paint() override {
        cout << "  [Window] GTK+ style\n";
    }
};

// Abstract Factory
class GUIFactory {
public:
    virtual unique_ptr<Button> CreateButton() = 0;
    virtual unique_ptr<Scrollbar> CreateScrollbar() = 0;
    virtual unique_ptr<Window> CreateWindow() = 0;
    virtual ~GUIFactory() = default;
};

// Concrete Factories
class WindowsFactory : public GUIFactory {
public:
    unique_ptr<Button> CreateButton() override {
        return make_unique<WinButton>();
    }
    unique_ptr<Scrollbar> CreateScrollbar() override {
        return make_unique<WinScrollbar>();
    }
    unique_ptr<Window> CreateWindow() override {
        return make_unique<WinWindow>();
    }
};

class MacFactory : public GUIFactory {
public:
    unique_ptr<Button> CreateButton() override {
        return make_unique<MacButton>();
    }
    unique_ptr<Scrollbar> CreateScrollbar() override {
        return make_unique<MacScrollbar>();
    }
    unique_ptr<Window> CreateWindow() override {
        return make_unique<MacWindow>();
    }
};

class LinuxFactory : public GUIFactory {
public:
    unique_ptr<Button> CreateButton() override {
        return make_unique<LinuxButton>();
    }
    unique_ptr<Scrollbar> CreateScrollbar() override {
        return make_unique<LinuxScrollbar>();
    }
    unique_ptr<Window> CreateWindow() override {
        return make_unique<LinuxWindow>();
    }
};

// Application
class Application {
    unique_ptr<GUIFactory> factory;

public:
    Application(unique_ptr<GUIFactory> f) : factory(move(f)) {}

    void CreateUI() {
        auto button = factory->CreateButton();
        auto scrollbar = factory->CreateScrollbar();
        auto window = factory->CreateWindow();

        cout << "Creating UI components:\n";
        button->Paint();
        scrollbar->Paint();
        window->Paint();
    }
};

// Platform detection (simplified)
unique_ptr<GUIFactory> CreateFactoryForPlatform(const string& platform) {
    if (platform == "Windows") {
        return make_unique<WindowsFactory>();
    } else if (platform == "macOS") {
        return make_unique<MacFactory>();
    } else {
        return make_unique<LinuxFactory>();
    }
}

int main() {
    cout << "=== Windows Application ===\n";
    Application winApp(CreateFactoryForPlatform("Windows"));
    winApp.CreateUI();

    cout << "\n=== macOS Application ===\n";
    Application macApp(CreateFactoryForPlatform("macOS"));
    macApp.CreateUI();

    cout << "\n=== Linux Application ===\n";
    Application linuxApp(CreateFactoryForPlatform("Linux"));
    linuxApp.CreateUI();

    return 0;
}
```

**Output:**
```
=== Windows Application ===
Creating UI components:
  [Button] Windows 11 style
  [Scrollbar] Windows 11 style
  [Window] Windows 11 style with rounded corners

=== macOS Application ===
Creating UI components:
  [Button] macOS Big Sur style
  [Scrollbar] macOS Big Sur style
  [Window] macOS Big Sur style with traffic lights

=== Linux Application ===
Creating UI components:
  [Button] GTK+ style
  [Scrollbar] GTK+ style
  [Window] GTK+ style
```

### Example 2: Database Access Layer

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

// Abstract Products
class Connection {
public:
    virtual void Connect(const string& connectionString) = 0;
    virtual void Disconnect() = 0;
    virtual ~Connection() = default;
};

class Command {
public:
    virtual void Execute(const string& sql) = 0;
    virtual ~Command() = default;
};

class DataReader {
public:
    virtual void Read() = 0;
    virtual ~DataReader() = default;
};

// MySQL Family
class MySQLConnection : public Connection {
public:
    void Connect(const string& connectionString) override {
        cout << "  [MySQL] Connecting to: " << connectionString << "\n";
        cout << "  [MySQL] Connected successfully\n";
    }
    void Disconnect() override {
        cout << "  [MySQL] Disconnected\n";
    }
};

class MySQLCommand : public Command {
public:
    void Execute(const string& sql) override {
        cout << "  [MySQL] Executing: " << sql << "\n";
    }
};

class MySQLDataReader : public DataReader {
public:
    void Read() override {
        cout << "  [MySQL] Reading data with mysql_fetch_row()\n";
    }
};

// PostgreSQL Family
class PostgreSQLConnection : public Connection {
public:
    void Connect(const string& connectionString) override {
        cout << "  [PostgreSQL] Connecting to: " << connectionString << "\n";
        cout << "  [PostgreSQL] Connected successfully\n";
    }
    void Disconnect() override {
        cout << "  [PostgreSQL] Disconnected\n";
    }
};

class PostgreSQLCommand : public Command {
public:
    void Execute(const string& sql) override {
        cout << "  [PostgreSQL] Executing: " << sql << "\n";
    }
};

class PostgreSQLDataReader : public DataReader {
public:
    void Read() override {
        cout << "  [PostgreSQL] Reading data with PQgetvalue()\n";
    }
};

// SQLite Family
class SQLiteConnection : public Connection {
public:
    void Connect(const string& connectionString) override {
        cout << "  [SQLite] Opening database: " << connectionString << "\n";
        cout << "  [SQLite] Opened successfully\n";
    }
    void Disconnect() override {
        cout << "  [SQLite] Closed\n";
    }
};

class SQLiteCommand : public Command {
public:
    void Execute(const string& sql) override {
        cout << "  [SQLite] Executing: " << sql << "\n";
    }
};

class SQLiteDataReader : public DataReader {
public:
    void Read() override {
        cout << "  [SQLite] Reading data with sqlite3_step()\n";
    }
};

// Abstract Factory
class DatabaseFactory {
public:
    virtual unique_ptr<Connection> CreateConnection() = 0;
    virtual unique_ptr<Command> CreateCommand() = 0;
    virtual unique_ptr<DataReader> CreateDataReader() = 0;
    virtual ~DatabaseFactory() = default;
};

// Concrete Factories
class MySQLFactory : public DatabaseFactory {
public:
    unique_ptr<Connection> CreateConnection() override {
        return make_unique<MySQLConnection>();
    }
    unique_ptr<Command> CreateCommand() override {
        return make_unique<MySQLCommand>();
    }
    unique_ptr<DataReader> CreateDataReader() override {
        return make_unique<MySQLDataReader>();
    }
};

class PostgreSQLFactory : public DatabaseFactory {
public:
    unique_ptr<Connection> CreateConnection() override {
        return make_unique<PostgreSQLConnection>();
    }
    unique_ptr<Command> CreateCommand() override {
        return make_unique<PostgreSQLCommand>();
    }
    unique_ptr<DataReader> CreateDataReader() override {
        return make_unique<PostgreSQLDataReader>();
    }
};

class SQLiteFactory : public DatabaseFactory {
public:
    unique_ptr<Connection> CreateConnection() override {
        return make_unique<SQLiteConnection>();
    }
    unique_ptr<Command> CreateCommand() override {
        return make_unique<SQLiteCommand>();
    }
    unique_ptr<DataReader> CreateDataReader() override {
        return make_unique<SQLiteDataReader>();
    }
};

// Data Access Layer
class DataAccess {
    unique_ptr<DatabaseFactory> factory;

public:
    DataAccess(unique_ptr<DatabaseFactory> f) : factory(move(f)) {}

    void PerformDatabaseOperations() {
        auto connection = factory->CreateConnection();
        auto command = factory->CreateCommand();
        auto reader = factory->CreateDataReader();

        connection->Connect("server=localhost;database=testdb");
        command->Execute("SELECT * FROM users");
        reader->Read();
        connection->Disconnect();
    }
};

int main() {
    cout << "=== Using MySQL ===\n";
    DataAccess mysqlDA(make_unique<MySQLFactory>());
    mysqlDA.PerformDatabaseOperations();

    cout << "\n=== Using PostgreSQL ===\n";
    DataAccess postgresDA(make_unique<PostgreSQLFactory>());
    postgresDA.PerformDatabaseOperations();

    cout << "\n=== Using SQLite ===\n";
    DataAccess sqliteDA(make_unique<SQLiteFactory>());
    sqliteDA.PerformDatabaseOperations();

    return 0;
}
```

---

## 7. Complete Working Examples

### Complete Example: Game Environment Factory

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Abstract Products
class Terrain {
public:
    virtual void Render() = 0;
    virtual ~Terrain() = default;
};

class Vegetation {
public:
    virtual void Render() = 0;
    virtual ~Vegetation() = default;
};

class Enemy {
public:
    virtual void Spawn() = 0;
    virtual ~Enemy() = default;
};

class Weather {
public:
    virtual void Apply() = 0;
    virtual ~Weather() = default;
};

// Desert Environment Family
class DesertTerrain : public Terrain {
public:
    void Render() override {
        cout << "  Terrain: Sandy dunes with rocks\n";
    }
};

class DesertVegetation : public Vegetation {
public:
    void Render() override {
        cout << "  Vegetation: Cacti and sparse shrubs\n";
    }
};

class DesertEnemy : public Enemy {
public:
    void Spawn() override {
        cout << "  Enemy: Scorpions and sand worms spawned\n";
    }
};

class DesertWeather : public Weather {
public:
    void Apply() override {
        cout << "  Weather: Hot sun with sandstorms\n";
    }
};

// Forest Environment Family
class ForestTerrain : public Terrain {
public:
    void Render() override {
        cout << "  Terrain: Grassy ground with dirt paths\n";
    }
};

class ForestVegetation : public Vegetation {
public:
    void Render() override {
        cout << "  Vegetation: Dense trees and bushes\n";
    }
};

class ForestEnemy : public Enemy {
public:
    void Spawn() override {
        cout << "  Enemy: Wolves and bears spawned\n";
    }
};

class ForestWeather : public Weather {
public:
    void Apply() override {
        cout << "  Weather: Foggy with light rain\n";
    }
};

// Arctic Environment Family
class ArcticTerrain : public Terrain {
public:
    void Render() override {
        cout << "  Terrain: Snow and ice sheets\n";
    }
};

class ArcticVegetation : public Vegetation {
public:
    void Render() override {
        cout << "  Vegetation: Ice crystals and frozen plants\n";
    }
};

class ArcticEnemy : public Enemy {
public:
    void Spawn() override {
        cout << "  Enemy: Yetis and ice elementals spawned\n";
    }
};

class ArcticWeather : public Weather {
public:
    void Apply() override {
        cout << "  Weather: Blizzard with freezing winds\n";
    }
};

// Abstract Factory
class EnvironmentFactory {
public:
    virtual unique_ptr<Terrain> CreateTerrain() = 0;
    virtual unique_ptr<Vegetation> CreateVegetation() = 0;
    virtual unique_ptr<Enemy> CreateEnemy() = 0;
    virtual unique_ptr<Weather> CreateWeather() = 0;
    virtual ~EnvironmentFactory() = default;
};

// Concrete Factories
class DesertFactory : public EnvironmentFactory {
public:
    unique_ptr<Terrain> CreateTerrain() override {
        return make_unique<DesertTerrain>();
    }
    unique_ptr<Vegetation> CreateVegetation() override {
        return make_unique<DesertVegetation>();
    }
    unique_ptr<Enemy> CreateEnemy() override {
        return make_unique<DesertEnemy>();
    }
    unique_ptr<Weather> CreateWeather() override {
        return make_unique<DesertWeather>();
    }
};

class ForestFactory : public EnvironmentFactory {
public:
    unique_ptr<Terrain> CreateTerrain() override {
        return make_unique<ForestTerrain>();
    }
    unique_ptr<Vegetation> CreateVegetation() override {
        return make_unique<ForestVegetation>();
    }
    unique_ptr<Enemy> CreateEnemy() override {
        return make_unique<ForestEnemy>();
    }
    unique_ptr<Weather> CreateWeather() override {
        return make_unique<ForestWeather>();
    }
};

class ArcticFactory : public EnvironmentFactory {
public:
    unique_ptr<Terrain> CreateTerrain() override {
        return make_unique<ArcticTerrain>();
    }
    unique_ptr<Vegetation> CreateVegetation() override {
        return make_unique<ArcticVegetation>();
    }
    unique_ptr<Enemy> CreateEnemy() override {
        return make_unique<ArcticEnemy>();
    }
    unique_ptr<Weather> CreateWeather() override {
        return make_unique<ArcticWeather>();
    }
};

// Game Level
class GameLevel {
    string name;
    unique_ptr<EnvironmentFactory> factory;

public:
    GameLevel(const string& n, unique_ptr<EnvironmentFactory> f)
        : name(n), factory(move(f)) {}

    void LoadLevel() {
        cout << "\n=== Loading Level: " << name << " ===\n";

        auto terrain = factory->CreateTerrain();
        auto vegetation = factory->CreateVegetation();
        auto enemy = factory->CreateEnemy();
        auto weather = factory->CreateWeather();

        terrain->Render();
        vegetation->Render();
        enemy->Spawn();
        weather->Apply();

        cout << "Level loaded successfully!\n";
    }
};

// Game
class Game {
public:
    void Run() {
        cout << "=== Adventure Game Starting ===\n";

        GameLevel level1("Scorching Desert", make_unique<DesertFactory>());
        level1.LoadLevel();

        GameLevel level2("Enchanted Forest", make_unique<ForestFactory>());
        level2.LoadLevel();

        GameLevel level3("Frozen Wasteland", make_unique<ArcticFactory>());
        level3.LoadLevel();

        cout << "\n=== All Levels Loaded ===\n";
    }
};

int main() {
    Game game;
    game.Run();

    return 0;
}
```

**Output:**
```
=== Adventure Game Starting ===

=== Loading Level: Scorching Desert ===
  Terrain: Sandy dunes with rocks
  Vegetation: Cacti and sparse shrubs
  Enemy: Scorpions and sand worms spawned
  Weather: Hot sun with sandstorms
Level loaded successfully!

=== Loading Level: Enchanted Forest ===
  Terrain: Grassy ground with dirt paths
  Vegetation: Dense trees and bushes
  Enemy: Wolves and bears spawned
  Weather: Foggy with light rain
Level loaded successfully!

=== Loading Level: Frozen Wasteland ===
  Terrain: Snow and ice sheets
  Vegetation: Ice crystals and frozen plants
  Enemy: Yetis and ice elementals spawned
  Weather: Blizzard with freezing winds
Level loaded successfully!

=== All Levels Loaded ===
```

---

## 8. Advantages and Disadvantages

### Advantages:

1. ✅ **Product Consistency**: Ensures related products work together
2. ✅ **Isolation**: Client code isolated from concrete classes
3. ✅ **Easy to Exchange Families**: Switch entire product family easily
4. ✅ **Open/Closed Principle**: Easy to add new product families
5. ✅ **Single Responsibility**: Factory encapsulates product creation

### Disadvantages:

1. ❌ **Complexity**: More classes and interfaces
2. ❌ **Rigid**: Hard to add new product types (need to change all factories)
3. ❌ **Overkill**: Too complex for simple scenarios

---

## 9. When to Use

### Use Abstract Factory When:
- ✅ System needs to work with multiple families of products
- ✅ Products must be used together (consistency requirement)
- ✅ You want to hide concrete implementations
- ✅ You need to switch between product families

### Don't Use When:
- ❌ Only one product family exists
- ❌ Products don't need to be compatible
- ❌ Simple creation is sufficient

---

## 10. Exercises

### Exercise 1: Document Reader Factory
Create factories for different document formats (PDF, Word, Excel) that create reader, writer, and formatter objects.

### Exercise 2: Theme Factory
Implement a theme system with Light/Dark themes creating colors, fonts, and icons.

### Exercise 3: Payment Gateway Factory
Create payment factories (PayPal, Stripe, Square) producing payment processor, refund handler, and receipt generator.

---

## Key Takeaways

1. ✅ Abstract Factory creates **families of related objects**
2. ✅ Ensures **product compatibility** within a family
3. ✅ **Isolates client** from concrete classes
4. ✅ Easy to **switch entire product families**
5. ✅ Follows **Open/Closed Principle** for new families
6. ✅ Use when products **must work together**
7. ✅ More complex than **Factory Method**
8. ✅ Common in **cross-platform frameworks**

---

## Next Lesson Preview

In **Lesson 65**, we'll learn about the **Builder Pattern**:
- Constructing complex objects step-by-step
- Separating construction from representation
- Fluent interfaces
- Director class
- Real examples: HTTP request builder, query builder

---

## Summary Checklist

Before moving to Lesson 65, ensure you can:

- [ ] Explain Abstract Factory pattern
- [ ] Distinguish from Factory Method
- [ ] Create product families
- [ ] Implement concrete factories
- [ ] Understand product consistency
- [ ] Use for cross-platform code
- [ ] Identify when to use Abstract Factory

**Excellent!** You've mastered Abstract Factory!

Next: **Builder Pattern** - constructing complex objects step-by-step!
