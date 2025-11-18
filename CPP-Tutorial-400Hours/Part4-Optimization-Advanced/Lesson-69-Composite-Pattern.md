# Lesson 69: Composite Pattern

**Duration**: 5 hours
**Difficulty**: Advanced
**Pattern Type**: Structural
**Part**: 4 - Optimization & Advanced Techniques

## Table of Contents
1. What is the Composite Pattern?
2. Problem and Solution
3. Structure and Components
4. Tree Structures
5. Real-World Examples
6. Complete Working Examples
7. Safety vs Transparency
8. Advantages and Disadvantages
9. When to Use
10. Exercises

---

## 1. What is the Composite Pattern?

The **Composite Pattern** composes objects into **tree structures** to represent part-whole hierarchies. It lets clients treat **individual objects and compositions uniformly**.

### Intent:
- Represent part-whole hierarchies
- Treat individual objects and compositions uniformly
- Create tree structures
- Recursive composition

### Real-World Analogy:

**File System**: Folders can contain files AND other folders. Both files and folders can be displayed, moved, deleted - treated uniformly.

**Organization Chart**: An employee can be an individual contributor OR a manager with subordinates. Both can receive tasks.

**Graphics**: A drawing can contain simple shapes AND grouped shapes. Both can be drawn and moved.

---

## 2. Problem and Solution

### The Problem:

```cpp
// ❌ Without Composite - Different handling for individuals vs groups
void DrawCircle(Circle* c) { c->Draw(); }
void DrawGroup(vector<Shape*> shapes) {
    for (auto shape : shapes) {
        // How to handle if shape is also a group?
    }
}
```

### The Solution:

```cpp
#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// Component interface
class Graphic {
public:
    virtual void Draw() = 0;
    virtual ~Graphic() = default;
};

// Leaf - Individual object
class Circle : public Graphic {
public:
    void Draw() override {
        cout << "Drawing Circle\n";
    }
};

// Composite - Container object
class GraphicGroup : public Graphic {
private:
    vector<unique_ptr<Graphic>> children;

public:
    void Add(unique_ptr<Graphic> graphic) {
        children.push_back(move(graphic));
    }

    void Draw() override {
        cout << "Drawing Group:\n";
        for (auto& child : children) {
            child->Draw();
        }
    }
};

int main() {
    // Create individual objects
    auto circle1 = make_unique<Circle>();
    auto circle2 = make_unique<Circle>();

    // Create composite
    auto group = make_unique<GraphicGroup>();
    group->Add(move(circle1));
    group->Add(move(circle2));

    // Treat uniformly
    group->Draw();  // Draws all children

    return 0;
}
```

---

## 3. Structure and Components

### UML Structure:

```
┌─────────────────┐
│    Component    │ ← Interface
├─────────────────┤
│ + Operation()   │
└─────────────────┘
        △
        │
   ┌────┴────┐
   │         │
┌──────┐  ┌─────────────┐
│ Leaf │  │  Composite  │
├──────┤  ├─────────────┤
│      │  │ - children  │
└──────┘  │ + Add()     │
          │ + Remove()  │
          │ + GetChild()│
          └─────────────┘
```

---

## 4. Real-World Examples

### Example 1: File System

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

// Component
class FileSystemNode {
protected:
    string name;

public:
    FileSystemNode(const string& n) : name(n) {}
    virtual void Display(int indent = 0) = 0;
    virtual int GetSize() = 0;
    virtual ~FileSystemNode() = default;
};

// Leaf - File
class File : public FileSystemNode {
private:
    int size;

public:
    File(const string& name, int fileSize)
        : FileSystemNode(name), size(fileSize) {}

    void Display(int indent = 0) override {
        cout << string(indent, ' ') << "- " << name
             << " (" << size << " KB)\n";
    }

    int GetSize() override { return size; }
};

// Composite - Folder
class Folder : public FileSystemNode {
private:
    vector<unique_ptr<FileSystemNode>> children;

public:
    Folder(const string& name) : FileSystemNode(name) {}

    void Add(unique_ptr<FileSystemNode> node) {
        children.push_back(move(node));
    }

    void Display(int indent = 0) override {
        cout << string(indent, ' ') << "+ " << name << "/\n";
        for (auto& child : children) {
            child->Display(indent + 2);
        }
    }

    int GetSize() override {
        int totalSize = 0;
        for (auto& child : children) {
            totalSize += child->GetSize();
        }
        return totalSize;
    }
};

int main() {
    // Create file system structure
    auto root = make_unique<Folder>("root");

    auto documents = make_unique<Folder>("Documents");
    documents->Add(make_unique<File>("resume.pdf", 500));
    documents->Add(make_unique<File>("cover_letter.doc", 100));

    auto photos = make_unique<Folder>("Photos");
    photos->Add(make_unique<File>("vacation1.jpg", 2000));
    photos->Add(make_unique<File>("vacation2.jpg", 1800));

    auto work = make_unique<Folder>("Work");
    work->Add(make_unique<File>("project.ppt", 3000));
    work->Add(make_unique<File>("notes.txt", 50));

    documents->Add(move(work));

    root->Add(move(documents));
    root->Add(move(photos));
    root->Add(make_unique<File>("readme.txt", 10));

    // Display entire tree
    root->Display();

    cout << "\nTotal size: " << root->GetSize() << " KB\n";

    return 0;
}
```

**Output:**
```
+ root/
  + Documents/
    - resume.pdf (500 KB)
    - cover_letter.doc (100 KB)
    + Work/
      - project.ppt (3000 KB)
      - notes.txt (50 KB)
  + Photos/
    - vacation1.jpg (2000 KB)
    - vacation2.jpg (1800 KB)
  - readme.txt (10 KB)

Total size: 7460 KB
```

### Example 2: Organization Chart

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

// Component
class Employee {
protected:
    string name;
    string position;
    double salary;

public:
    Employee(const string& n, const string& pos, double sal)
        : name(n), position(pos), salary(sal) {}

    virtual void ShowDetails(int indent = 0) = 0;
    virtual double GetTotalSalary() = 0;
    virtual int GetTeamSize() = 0;
    virtual ~Employee() = default;
};

// Leaf - Individual Contributor
class IndividualContributor : public Employee {
public:
    IndividualContributor(const string& n, const string& pos, double sal)
        : Employee(n, pos, sal) {}

    void ShowDetails(int indent = 0) override {
        cout << string(indent, ' ') << "- " << name
             << " (" << position << ") $" << salary << "\n";
    }

    double GetTotalSalary() override { return salary; }
    int GetTeamSize() override { return 1; }
};

// Composite - Manager
class Manager : public Employee {
private:
    vector<unique_ptr<Employee>> team;

public:
    Manager(const string& n, const string& pos, double sal)
        : Employee(n, pos, sal) {}

    void AddTeamMember(unique_ptr<Employee> member) {
        team.push_back(move(member));
    }

    void ShowDetails(int indent = 0) override {
        cout << string(indent, ' ') << "+ " << name
             << " (" << position << ") $" << salary
             << " [Team: " << GetTeamSize() << "]\n";
        for (auto& member : team) {
            member->ShowDetails(indent + 2);
        }
    }

    double GetTotalSalary() override {
        double total = salary;
        for (auto& member : team) {
            total += member->GetTotalSalary();
        }
        return total;
    }

    int GetTeamSize() override {
        int count = 1;  // Self
        for (auto& member : team) {
            count += member->GetTeamSize();
        }
        return count;
    }
};

int main() {
    // CEO
    auto ceo = make_unique<Manager>("Alice Johnson", "CEO", 250000);

    // CTO and team
    auto cto = make_unique<Manager>("Bob Smith", "CTO", 180000);
    cto->AddTeamMember(make_unique<Manager>("Carol White", "Engineering Manager", 140000));

    auto engManager = make_unique<Manager>("David Brown", "Senior Engineer", 120000);
    engManager->AddTeamMember(make_unique<IndividualContributor>("Eve Davis", "Software Engineer", 90000));
    engManager->AddTeamMember(make_unique<IndividualContributor>("Frank Miller", "Software Engineer", 85000));

    dynamic_cast<Manager*>(cto.get())->AddTeamMember(move(engManager));

    // CFO
    auto cfo = make_unique<Manager>("Grace Lee", "CFO", 180000);
    cfo->AddTeamMember(make_unique<IndividualContributor>("Henry Wilson", "Accountant", 70000));

    // Add to CEO
    ceo->AddTeamMember(move(cto));
    ceo->AddTeamMember(move(cfo));

    // Display org chart
    cout << "=== Organization Chart ===\n";
    ceo->ShowDetails();

    cout << "\nTotal Payroll: $" << ceo->GetTotalSalary() << "\n";
    cout << "Total Employees: " << ceo->GetTeamSize() << "\n";

    return 0;
}
```

---

## 5. Complete Working Examples

### Complete Example: Menu System

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

// Component
class MenuComponent {
protected:
    string name;

public:
    MenuComponent(const string& n) : name(n) {}

    virtual void Display(int indent = 0) = 0;
    virtual void Select() = 0;
    virtual ~MenuComponent() = default;
};

// Leaf - Menu Item
class MenuItem : public MenuComponent {
private:
    string action;

public:
    MenuItem(const string& n, const string& act)
        : MenuComponent(n), action(act) {}

    void Display(int indent = 0) override {
        cout << string(indent, ' ') << "  " << name << "\n";
    }

    void Select() override {
        cout << "Executing: " << action << "\n";
    }
};

// Composite - Menu
class Menu : public MenuComponent {
private:
    vector<unique_ptr<MenuComponent>> items;

public:
    Menu(const string& n) : MenuComponent(n) {}

    void Add(unique_ptr<MenuComponent> item) {
        items.push_back(move(item));
    }

    void Display(int indent = 0) override {
        cout << string(indent, ' ') << "> " << name << "\n";
        for (auto& item : items) {
            item->Display(indent + 2);
        }
    }

    void Select() override {
        cout << "Opening menu: " << name << "\n";
        Display(2);
    }
};

int main() {
    // Create menu structure
    auto mainMenu = make_unique<Menu>("Main Menu");

    // File menu
    auto fileMenu = make_unique<Menu>("File");
    fileMenu->Add(make_unique<MenuItem>("New", "CreateNewFile()"));
    fileMenu->Add(make_unique<MenuItem>("Open", "OpenFile()"));
    fileMenu->Add(make_unique<MenuItem>("Save", "SaveFile()"));
    fileMenu->Add(make_unique<MenuItem>("Exit", "ExitProgram()"));

    // Edit menu
    auto editMenu = make_unique<Menu>("Edit");
    editMenu->Add(make_unique<MenuItem>("Copy", "CopyText()"));
    editMenu->Add(make_unique<MenuItem>("Paste", "PasteText()"));

    // Add to main menu
    mainMenu->Add(move(fileMenu));
    mainMenu->Add(move(editMenu));

    // Display
    cout << "=== Application Menu ===\n";
    mainMenu->Display();

    return 0;
}
```

---

## 6. Advantages and Disadvantages

### Advantages:

1. ✅ **Uniform Treatment**: Treat simple and complex objects uniformly
2. ✅ **Hierarchies**: Easy to create complex tree structures
3. ✅ **Open/Closed**: Easy to add new component types
4. ✅ **Recursive Composition**: Natural tree traversal

### Disadvantages:

1. ❌ **Overly General**: Sometimes TOO general
2. ❌ **Type Safety**: Hard to restrict component types
3. ❌ **Complexity**: Can make design overly complex

---

## 7. When to Use

### Use Composite When:
- ✅ Representing part-whole hierarchies
- ✅ Want to treat objects uniformly
- ✅ Working with tree structures
- ✅ Need recursive composition

### Don't Use When:
- ❌ No tree structure needed
- ❌ Objects shouldn't be treated uniformly
- ❌ Simple list is sufficient

---

## 8. Exercises

### Exercise 1: UI Component Hierarchy
Create composite for UI components (Panel, Button, TextBox) where panels can contain other components.

### Exercise 2: Math Expression Tree
Implement expression tree with numbers and operators that can be evaluated recursively.

### Exercise 3: Shopping Cart
Build shopping cart with individual items and bundles (composite of items).

---

## Key Takeaways

1. ✅ Composite creates **tree structures**
2. ✅ **Uniform treatment** of individuals and groups
3. ✅ **Part-whole** hierarchies
4. ✅ **Recursive composition** is natural
5. ✅ Follows **Open/Closed** principle
6. ✅ Common in **file systems, UI, org charts**
7. ✅ **Leaf** and **Composite** share interface
8. ✅ Great for **hierarchical data**

---

## Next Lesson Preview

In **Lesson 70**, we'll learn about the **Decorator Pattern**:
- Adding responsibilities dynamically
- Flexible alternative to subclassing
- Wrapping objects with features
- Coffee shop example, stream decorators

---

## Summary Checklist

Before moving to Lesson 70, ensure you can:

- [ ] Explain the Composite pattern
- [ ] Create tree structures
- [ ] Implement uniform treatment
- [ ] Build part-whole hierarchies
- [ ] Identify when to use Composite
- [ ] Handle recursive composition
- [ ] Apply to real-world hierarchies

**Excellent work!** Composite pattern mastered!

Next: **Decorator Pattern** - dynamic responsibilities!
