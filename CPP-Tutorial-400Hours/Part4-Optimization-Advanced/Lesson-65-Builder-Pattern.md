# Lesson 65: Builder Pattern

**Duration**: 6 hours
**Difficulty**: Advanced
**Pattern Type**: Creational
**Part**: 4 - Optimization & Advanced Techniques

## Table of Contents
1. What is the Builder Pattern?
2. Problem: Telescoping Constructor
3. Builder Solution
4. Structure and Components
5. Fluent Interface Builder
6. Director Class
7. Real-World Examples
8. Complete Working Examples
9. Builder vs Factory
10. Exercises

---

## 1. What is the Builder Pattern?

The **Builder Pattern** separates the construction of a complex object from its representation, allowing the same construction process to create different representations.

### Intent:
- Construct complex objects step-by-step
- Separate construction from representation
- Same construction process, different results
- Hide complex construction logic

### Real-World Analogy:

**Building a House**: You don't build a house in one step. You:
1. Build foundation
2. Build walls
3. Install roof
4. Add plumbing
5. Add electrical
6. Interior finishing

Different builders can use the same steps to create different houses (modern, traditional, minimalist).

**Restaurant Order**: Building a custom burger:
1. Choose bun type
2. Choose patty
3. Add cheese (optional)
4. Add toppings
5. Add sauce

Same process, different burgers!

---

## 2. Problem: Telescoping Constructor

### The Problem:

```cpp
class Computer {
private:
    // Required parameters
    string CPU;
    string RAM;

    // Optional parameters
    string GPU;
    string storage;
    string motherboard;
    string powerSupply;
    string coolingSystem;
    string caseType;

public:
    // Constructor with 2 parameters
    Computer(string cpu, string ram)
        : CPU(cpu), RAM(ram) {}

    // Constructor with 3 parameters
    Computer(string cpu, string ram, string gpu)
        : CPU(cpu), RAM(ram), GPU(gpu) {}

    // Constructor with 4 parameters
    Computer(string cpu, string ram, string gpu, string stor)
        : CPU(cpu), RAM(ram), GPU(gpu), storage(stor) {}

    // ... many more constructors!

    // ❌ PROBLEMS:
    // 1. Too many constructors (telescoping constructor anti-pattern)
    // 2. Hard to remember parameter order
    // 3. Can't skip optional parameters in the middle
    // 4. Unreadable: Computer("Intel", "16GB", "", "1TB", "", "750W") // What are these empty strings?
};

// Client code is confusing:
Computer pc("Intel i9", "32GB", "RTX 4090", "2TB SSD", "", "", "", "");
// What are all these empty strings?
```

---

## 3. Builder Solution

### Basic Builder Implementation:

```cpp
#include <iostream>
#include <string>
#include <memory>
using namespace std;

// Product
class Computer {
private:
    // Required
    string CPU;
    string RAM;

    // Optional
    string GPU;
    string storage;
    string motherboard;
    string powerSupply;

    friend class ComputerBuilder;  // Builder can access private members

public:
    void Display() const {
        cout << "Computer Configuration:\n";
        cout << "  CPU: " << CPU << "\n";
        cout << "  RAM: " << RAM << "\n";
        if (!GPU.empty()) cout << "  GPU: " << GPU << "\n";
        if (!storage.empty()) cout << "  Storage: " << storage << "\n";
        if (!motherboard.empty()) cout << "  Motherboard: " << motherboard << "\n";
        if (!powerSupply.empty()) cout << "  Power Supply: " << powerSupply << "\n";
    }
};

// Builder
class ComputerBuilder {
private:
    unique_ptr<Computer> computer;

public:
    ComputerBuilder() {
        computer = make_unique<Computer>();
    }

    ComputerBuilder& SetCPU(const string& cpu) {
        computer->CPU = cpu;
        return *this;  // Return reference for chaining
    }

    ComputerBuilder& SetRAM(const string& ram) {
        computer->RAM = ram;
        return *this;
    }

    ComputerBuilder& SetGPU(const string& gpu) {
        computer->GPU = gpu;
        return *this;
    }

    ComputerBuilder& SetStorage(const string& storage) {
        computer->storage = storage;
        return *this;
    }

    ComputerBuilder& SetMotherboard(const string& mb) {
        computer->motherboard = mb;
        return *this;
    }

    ComputerBuilder& SetPowerSupply(const string& psu) {
        computer->powerSupply = psu;
        return *this;
    }

    unique_ptr<Computer> Build() {
        return move(computer);
    }
};

int main() {
    // ✅ Clear, readable, flexible!
    auto gamingPC = ComputerBuilder()
        .SetCPU("Intel i9-13900K")
        .SetRAM("64GB DDR5")
        .SetGPU("RTX 4090")
        .SetStorage("2TB NVMe SSD")
        .SetPowerSupply("1000W")
        .Build();

    gamingPC->Display();

    cout << "\n";

    // Can easily create different configurations
    auto officePC = ComputerBuilder()
        .SetCPU("Intel i5-13400")
        .SetRAM("16GB DDR4")
        .SetStorage("512GB SSD")
        .Build();

    officePC->Display();

    return 0;
}
```

**Output:**
```
Computer Configuration:
  CPU: Intel i9-13900K
  RAM: 64GB DDR5
  GPU: RTX 4090
  Storage: 2TB NVMe SSD
  Power Supply: 1000W

Computer Configuration:
  CPU: Intel i5-13400
  RAM: 16GB DDR4
  Storage: 512GB SSD
```

---

## 4. Structure and Components

### UML Structure:

```
┌────────────────┐
│    Director    │
├────────────────┤
│ + Construct()  │
└────────┬───────┘
         │ uses
         ▼
┌─────────────────────┐
│   Builder          │ ← Interface
├─────────────────────┤
│ + BuildPartA()      │
│ + BuildPartB()      │
│ + GetResult()       │
└─────────────────────┘
         △
         │ implements
         │
┌─────────────────────┐
│ ConcreteBuilder     │
├─────────────────────┤
│ - product           │
│ + BuildPartA()      │
│ + BuildPartB()      │
│ + GetResult()       │
└─────────────────────┘
         │ creates
         ▼
┌─────────────────────┐
│      Product        │
└─────────────────────┘
```

### Components:

**1. Product** - Complex object being built
```cpp
class Pizza {
    string dough;
    string sauce;
    string topping;
    // ... many more attributes
};
```

**2. Builder** - Abstract interface
```cpp
class PizzaBuilder {
public:
    virtual void BuildDough() = 0;
    virtual void BuildSauce() = 0;
    virtual void BuildTopping() = 0;
    virtual Pizza* GetPizza() = 0;
};
```

**3. ConcreteBuilder** - Implements building steps
```cpp
class MargheritaBuilder : public PizzaBuilder {
    Pizza* pizza;
public:
    void BuildDough() override { pizza->dough = "thin crust"; }
    void BuildSauce() override { pizza->sauce = "tomato"; }
    void BuildTopping() override { pizza->topping = "mozzarella"; }
    Pizza* GetPizza() override { return pizza; }
};
```

**4. Director** (Optional) - Constructs using builder
```cpp
class Waiter {
public:
    void ConstructPizza(PizzaBuilder& builder) {
        builder.BuildDough();
        builder.BuildSauce();
        builder.BuildTopping();
    }
};
```

---

## 5. Fluent Interface Builder

### Fluent Builder (Modern C++ Style):

```cpp
#include <iostream>
#include <string>
#include <memory>
using namespace std;

class HttpRequest {
private:
    string url;
    string method;
    string body;
    string contentType;
    int timeout;

public:
    HttpRequest() : method("GET"), timeout(30) {}

    void Send() const {
        cout << "Sending HTTP Request:\n";
        cout << "  Method: " << method << "\n";
        cout << "  URL: " << url << "\n";
        if (!body.empty()) {
            cout << "  Body: " << body << "\n";
            cout << "  Content-Type: " << contentType << "\n";
        }
        cout << "  Timeout: " << timeout << "s\n";
    }

    // Nested Builder class
    class Builder {
    private:
        unique_ptr<HttpRequest> request;

    public:
        Builder() : request(make_unique<HttpRequest>()) {}

        Builder& Url(const string& url) {
            request->url = url;
            return *this;
        }

        Builder& Method(const string& method) {
            request->method = method;
            return *this;
        }

        Builder& Body(const string& body) {
            request->body = body;
            return *this;
        }

        Builder& ContentType(const string& type) {
            request->contentType = type;
            return *this;
        }

        Builder& Timeout(int seconds) {
            request->timeout = seconds;
            return *this;
        }

        unique_ptr<HttpRequest> Build() {
            return move(request);
        }
    };
};

int main() {
    // Fluent interface - very readable!
    auto getRequest = HttpRequest::Builder()
        .Url("https://api.example.com/users")
        .Method("GET")
        .Timeout(60)
        .Build();

    getRequest->Send();

    cout << "\n";

    auto postRequest = HttpRequest::Builder()
        .Url("https://api.example.com/users")
        .Method("POST")
        .Body("{\"name\":\"John\",\"age\":30}")
        .ContentType("application/json")
        .Timeout(45)
        .Build();

    postRequest->Send();

    return 0;
}
```

**Output:**
```
Sending HTTP Request:
  Method: GET
  URL: https://api.example.com/users
  Timeout: 60s

Sending HTTP Request:
  Method: POST
  URL: https://api.example.com/users
  Body: {"name":"John","age":30}
  Content-Type: application/json
  Timeout: 45s
```

---

## 6. Director Class

### Using Director for Standard Configurations:

```cpp
#include <iostream>
#include <string>
#include <memory>
using namespace std;

// Product
class Car {
public:
    string engine;
    int seats;
    string transmission;
    bool GPS;
    bool tripComputer;

    void Display() const {
        cout << "Car Specifications:\n";
        cout << "  Engine: " << engine << "\n";
        cout << "  Seats: " << seats << "\n";
        cout << "  Transmission: " << transmission << "\n";
        cout << "  GPS: " << (GPS ? "Yes" : "No") << "\n";
        cout << "  Trip Computer: " << (tripComputer ? "Yes" : "No") << "\n";
    }
};

// Builder Interface
class CarBuilder {
public:
    virtual void SetSeats(int num) = 0;
    virtual void SetEngine(const string& engine) = 0;
    virtual void SetTransmission(const string& trans) = 0;
    virtual void SetGPS(bool has) = 0;
    virtual void SetTripComputer(bool has) = 0;
    virtual unique_ptr<Car> GetCar() = 0;
    virtual ~CarBuilder() = default;
};

// Concrete Builder
class ModernCarBuilder : public CarBuilder {
private:
    unique_ptr<Car> car;

public:
    ModernCarBuilder() {
        car = make_unique<Car>();
    }

    void SetSeats(int num) override {
        car->seats = num;
    }

    void SetEngine(const string& engine) override {
        car->engine = engine;
    }

    void SetTransmission(const string& trans) override {
        car->transmission = trans;
    }

    void SetGPS(bool has) override {
        car->GPS = has;
    }

    void SetTripComputer(bool has) override {
        car->tripComputer = has;
    }

    unique_ptr<Car> GetCar() override {
        return move(car);
    }
};

// Director - knows how to build standard configurations
class CarDirector {
private:
    CarBuilder* builder;

public:
    void SetBuilder(CarBuilder* b) {
        builder = b;
    }

    void ConstructSportsCar() {
        builder->SetSeats(2);
        builder->SetEngine("V8 Twin-Turbo");
        builder->SetTransmission("7-speed Dual Clutch");
        builder->SetGPS(true);
        builder->SetTripComputer(true);
    }

    void ConstructFamilyCar() {
        builder->SetSeats(5);
        builder->SetEngine("2.0L Inline-4");
        builder->SetTransmission("CVT");
        builder->SetGPS(true);
        builder->SetTripComputer(false);
    }

    void ConstructEconomyCar() {
        builder->SetSeats(4);
        builder->SetEngine("1.5L Inline-3");
        builder->SetTransmission("5-speed Manual");
        builder->SetGPS(false);
        builder->SetTripComputer(false);
    }
};

int main() {
    CarDirector director;
    ModernCarBuilder builder;

    director.SetBuilder(&builder);

    // Build sports car
    cout << "=== Building Sports Car ===\n";
    director.ConstructSportsCar();
    auto sportsCar = builder.GetCar();
    sportsCar->Display();

    // Build family car
    cout << "\n=== Building Family Car ===\n";
    ModernCarBuilder builder2;
    director.SetBuilder(&builder2);
    director.ConstructFamilyCar();
    auto familyCar = builder2.GetCar();
    familyCar->Display();

    // Build economy car
    cout << "\n=== Building Economy Car ===\n";
    ModernCarBuilder builder3;
    director.SetBuilder(&builder3);
    director.ConstructEconomyCar();
    auto economyCar = builder3.GetCar();
    economyCar->Display();

    return 0;
}
```

---

## 7. Real-World Examples

### Example 1: SQL Query Builder

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class SQLQuery {
private:
    string table;
    vector<string> columns;
    vector<string> whereClauses;
    vector<string> orderBy;
    int limitValue;

    friend class QueryBuilder;

public:
    SQLQuery() : limitValue(-1) {}

    string ToString() const {
        ostringstream sql;

        // SELECT
        sql << "SELECT ";
        if (columns.empty()) {
            sql << "*";
        } else {
            for (size_t i = 0; i < columns.size(); ++i) {
                if (i > 0) sql << ", ";
                sql << columns[i];
            }
        }

        // FROM
        sql << " FROM " << table;

        // WHERE
        if (!whereClauses.empty()) {
            sql << " WHERE ";
            for (size_t i = 0; i < whereClauses.size(); ++i) {
                if (i > 0) sql << " AND ";
                sql << whereClauses[i];
            }
        }

        // ORDER BY
        if (!orderBy.empty()) {
            sql << " ORDER BY ";
            for (size_t i = 0; i < orderBy.size(); ++i) {
                if (i > 0) sql << ", ";
                sql << orderBy[i];
            }
        }

        // LIMIT
        if (limitValue > 0) {
            sql << " LIMIT " << limitValue;
        }

        return sql.str();
    }
};

class QueryBuilder {
private:
    SQLQuery query;

public:
    QueryBuilder& From(const string& tableName) {
        query.table = tableName;
        return *this;
    }

    QueryBuilder& Select(const vector<string>& cols) {
        query.columns = cols;
        return *this;
    }

    QueryBuilder& Where(const string& condition) {
        query.whereClauses.push_back(condition);
        return *this;
    }

    QueryBuilder& OrderBy(const string& column, const string& direction = "ASC") {
        query.orderBy.push_back(column + " " + direction);
        return *this;
    }

    QueryBuilder& Limit(int limit) {
        query.limitValue = limit;
        return *this;
    }

    SQLQuery Build() {
        return query;
    }
};

int main() {
    // Simple query
    auto query1 = QueryBuilder()
        .From("users")
        .Build();

    cout << query1.ToString() << "\n\n";

    // Complex query
    auto query2 = QueryBuilder()
        .From("employees")
        .Select({"name", "salary", "department"})
        .Where("salary > 50000")
        .Where("department = 'Engineering'")
        .OrderBy("salary", "DESC")
        .Limit(10)
        .Build();

    cout << query2.ToString() << "\n\n";

    // Another example
    auto query3 = QueryBuilder()
        .From("products")
        .Select({"product_name", "price", "stock"})
        .Where("stock > 0")
        .Where("price < 100")
        .OrderBy("price", "ASC")
        .Build();

    cout << query3.ToString() << "\n";

    return 0;
}
```

**Output:**
```
SELECT * FROM users

SELECT name, salary, department FROM employees WHERE salary > 50000 AND department = 'Engineering' ORDER BY salary DESC LIMIT 10

SELECT product_name, price, stock FROM products WHERE stock > 0 AND price < 100 ORDER BY price ASC
```

### Example 2: Email Builder

```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Email {
private:
    string from;
    vector<string> to;
    vector<string> cc;
    vector<string> bcc;
    string subject;
    string body;
    bool isHTML;
    vector<string> attachments;

    friend class EmailBuilder;

public:
    Email() : isHTML(false) {}

    void Send() const {
        cout << "=== Sending Email ===\n";
        cout << "From: " << from << "\n";

        cout << "To: ";
        for (size_t i = 0; i < to.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << to[i];
        }
        cout << "\n";

        if (!cc.empty()) {
            cout << "CC: ";
            for (size_t i = 0; i < cc.size(); ++i) {
                if (i > 0) cout << ", ";
                cout << cc[i];
            }
            cout << "\n";
        }

        if (!bcc.empty()) {
            cout << "BCC: ";
            for (size_t i = 0; i < bcc.size(); ++i) {
                if (i > 0) cout << ", ";
                cout << bcc[i];
            }
            cout << "\n";
        }

        cout << "Subject: " << subject << "\n";
        cout << "Format: " << (isHTML ? "HTML" : "Plain Text") << "\n";
        cout << "Body:\n" << body << "\n";

        if (!attachments.empty()) {
            cout << "Attachments: ";
            for (size_t i = 0; i < attachments.size(); ++i) {
                if (i > 0) cout << ", ";
                cout << attachments[i];
            }
            cout << "\n";
        }

        cout << "Email sent successfully!\n";
    }
};

class EmailBuilder {
private:
    Email email;

public:
    EmailBuilder& From(const string& sender) {
        email.from = sender;
        return *this;
    }

    EmailBuilder& To(const string& recipient) {
        email.to.push_back(recipient);
        return *this;
    }

    EmailBuilder& CC(const string& recipient) {
        email.cc.push_back(recipient);
        return *this;
    }

    EmailBuilder& BCC(const string& recipient) {
        email.bcc.push_back(recipient);
        return *this;
    }

    EmailBuilder& Subject(const string& subj) {
        email.subject = subj;
        return *this;
    }

    EmailBuilder& Body(const string& content) {
        email.body = content;
        return *this;
    }

    EmailBuilder& AsHTML() {
        email.isHTML = true;
        return *this;
    }

    EmailBuilder& Attachment(const string& filePath) {
        email.attachments.push_back(filePath);
        return *this;
    }

    Email Build() {
        return email;
    }
};

int main() {
    auto email = EmailBuilder()
        .From("sender@example.com")
        .To("recipient1@example.com")
        .To("recipient2@example.com")
        .CC("manager@example.com")
        .Subject("Monthly Report")
        .Body("<h1>Sales Report</h1><p>Great progress this month!</p>")
        .AsHTML()
        .Attachment("report.pdf")
        .Attachment("charts.xlsx")
        .Build();

    email.Send();

    return 0;
}
```

---

## 8. Complete Working Examples

### Complete Example: Game Character Builder

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// Product
class GameCharacter {
private:
    // Basic attributes
    string name;
    string characterClass;
    int level;

    // Stats
    int strength;
    int intelligence;
    int agility;

    // Equipment
    string weapon;
    string armor;
    vector<string> skills;

    friend class CharacterBuilder;

public:
    void Display() const {
        cout << "\n=== Character Profile ===\n";
        cout << "Name: " << name << "\n";
        cout << "Class: " << characterClass << "\n";
        cout << "Level: " << level << "\n";
        cout << "\nStats:\n";
        cout << "  Strength: " << strength << "\n";
        cout << "  Intelligence: " << intelligence << "\n";
        cout << "  Agility: " << agility << "\n";
        cout << "\nEquipment:\n";
        cout << "  Weapon: " << weapon << "\n";
        cout << "  Armor: " << armor << "\n";

        if (!skills.empty()) {
            cout << "\nSkills:\n";
            for (const auto& skill : skills) {
                cout << "  - " << skill << "\n";
            }
        }
        cout << "=======================\n";
    }
};

// Builder
class CharacterBuilder {
private:
    unique_ptr<GameCharacter> character;

public:
    CharacterBuilder() {
        character = make_unique<GameCharacter>();
        // Default values
        character->level = 1;
        character->strength = 10;
        character->intelligence = 10;
        character->agility = 10;
    }

    CharacterBuilder& Name(const string& name) {
        character->name = name;
        return *this;
    }

    CharacterBuilder& Class(const string& cls) {
        character->characterClass = cls;
        return *this;
    }

    CharacterBuilder& Level(int lvl) {
        character->level = lvl;
        return *this;
    }

    CharacterBuilder& Strength(int str) {
        character->strength = str;
        return *this;
    }

    CharacterBuilder& Intelligence(int intel) {
        character->intelligence = intel;
        return *this;
    }

    CharacterBuilder& Agility(int agi) {
        character->agility = agi;
        return *this;
    }

    CharacterBuilder& Weapon(const string& wpn) {
        character->weapon = wpn;
        return *this;
    }

    CharacterBuilder& Armor(const string& arm) {
        character->armor = arm;
        return *this;
    }

    CharacterBuilder& AddSkill(const string& skill) {
        character->skills.push_back(skill);
        return *this;
    }

    unique_ptr<GameCharacter> Build() {
        return move(character);
    }
};

// Director - Provides preset configurations
class CharacterDirector {
public:
    static unique_ptr<GameCharacter> CreateWarrior(const string& name) {
        return CharacterBuilder()
            .Name(name)
            .Class("Warrior")
            .Level(1)
            .Strength(18)
            .Intelligence(8)
            .Agility(12)
            .Weapon("Iron Sword")
            .Armor("Chain Mail")
            .AddSkill("Power Strike")
            .AddSkill("Shield Bash")
            .Build();
    }

    static unique_ptr<GameCharacter> CreateMage(const string& name) {
        return CharacterBuilder()
            .Name(name)
            .Class("Mage")
            .Level(1)
            .Strength(6)
            .Intelligence(18)
            .Agility(10)
            .Weapon("Wooden Staff")
            .Armor("Cloth Robe")
            .AddSkill("Fireball")
            .AddSkill("Ice Shard")
            .AddSkill("Teleport")
            .Build();
    }

    static unique_ptr<GameCharacter> CreateRogue(const string& name) {
        return CharacterBuilder()
            .Name(name)
            .Class("Rogue")
            .Level(1)
            .Strength(10)
            .Intelligence(12)
            .Agility(18)
            .Weapon("Twin Daggers")
            .Armor("Leather Armor")
            .AddSkill("Backstab")
            .AddSkill("Stealth")
            .AddSkill("Poison Strike")
            .Build();
    }
};

int main() {
    // Using director for preset characters
    cout << "=== Creating Preset Characters ===\n";

    auto warrior = CharacterDirector::CreateWarrior("Conan");
    warrior->Display();

    auto mage = CharacterDirector::CreateMage("Gandalf");
    mage->Display();

    auto rogue = CharacterDirector::CreateRogue("Ezio");
    rogue->Display();

    // Custom character using builder directly
    cout << "\n=== Creating Custom Character ===\n";

    auto custom = CharacterBuilder()
        .Name("Custom Hero")
        .Class("Paladin")
        .Level(5)
        .Strength(15)
        .Intelligence(14)
        .Agility(12)
        .Weapon("Holy Sword")
        .Armor("Plate Armor")
        .AddSkill("Divine Shield")
        .AddSkill("Healing Light")
        .AddSkill("Smite")
        .Build();

    custom->Display();

    return 0;
}
```

---

## 9. Builder vs Factory

### Comparison:

| Aspect | Builder | Factory |
|--------|---------|---------|
| **Purpose** | Step-by-step construction | One-step creation |
| **Complexity** | Complex objects | Simple/medium objects |
| **Control** | Fine-grained control | Coarse-grained control |
| **Process** | Multi-step process | Single method call |
| **Focus** | HOW to build | WHAT to build |
| **Variations** | Many configurations | Few variations |

### When to Use:

**Use Builder** when:
- Object has many optional parameters
- Object requires step-by-step construction
- Same process creates different representations

**Use Factory** when:
- Object creation is simple
- Client doesn't need construction details
- Creating object families

---

## 10. Exercises

### Exercise 1: Resume Builder
Create a builder for generating resumes with sections: personal info, education, experience, skills.

### Exercise 2: Pizza Builder
Implement a pizza builder with: size, crust type, sauce, cheese, and multiple toppings.

### Exercise 3: Report Builder
Build a report generator with: title, sections, charts, tables, and formatting options.

---

## Key Takeaways

1. ✅ Builder separates **construction from representation**
2. ✅ Solves **telescoping constructor** problem
3. ✅ Provides **step-by-step** object construction
4. ✅ **Fluent interface** improves readability
5. ✅ **Director** encapsulates construction algorithms
6. ✅ Great for objects with **many optional parameters**
7. ✅ Allows **different representations** same process
8. ✅ More **flexible** than constructor

---

## Next Lesson Preview

In **Lesson 66**, we'll learn about the **Prototype Pattern**:
- Cloning objects instead of creating new
- Shallow vs deep copy
- Clone method implementation
- Performance benefits
- Registry of prototypes

---

## Summary Checklist

Before moving to Lesson 66, ensure you can:

- [ ] Explain the Builder pattern
- [ ] Identify telescoping constructor problem
- [ ] Implement fluent interface builder
- [ ] Use director for standard configurations
- [ ] Distinguish Builder from Factory
- [ ] Create complex objects step-by-step
- [ ] Use builder for optional parameters

**Outstanding!** You've mastered the Builder pattern!

Next: **Prototype Pattern** - cloning objects efficiently!
