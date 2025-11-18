# Lesson 81: Software Architecture Principles

**Duration**: 8 hours
**Difficulty**: Advanced
**Prerequisites**: Lessons 1-80, Strong understanding of OOP, Design Patterns, and C++ Advanced Features

## Table of Contents
1. Introduction to Software Architecture
2. SOLID Principles in Depth
3. Architectural Patterns
4. Design Patterns in Real Systems
5. Layered Architecture
6. Domain-Driven Design (DDD)
7. Component-Based Architecture
8. Scalability Principles
9. Performance Architecture
10. Real-World Example: Multi-Tier Application
11. Exercises and Projects

---

## 1. Introduction to Software Architecture

### What is Software Architecture?

Software architecture is the fundamental organization of a software system, including its components, their relationships, and the principles governing their design and evolution.

### Key Responsibilities of Architecture:

1. **Structural Organization**: How components are arranged
2. **Communication Patterns**: How components interact
3. **Quality Attributes**: Performance, scalability, maintainability
4. **Technology Selection**: Frameworks, libraries, tools
5. **Evolution Strategy**: How system grows and adapts

### Architecture vs Design:

```
┌─────────────────────────────────────┐
│         ARCHITECTURE                │
│  (High-level decisions)             │
│  ┌───────────────────────────────┐  │
│  │       DESIGN                  │  │
│  │  (Component-level decisions)  │  │
│  │  ┌─────────────────────────┐  │  │
│  │  │   IMPLEMENTATION        │  │  │
│  │  │  (Code-level decisions) │  │  │
│  │  └─────────────────────────┘  │  │
│  └───────────────────────────────┘  │
└─────────────────────────────────────┘
```

### Architectural Goals:

```cpp
// Good Architecture Characteristics
namespace ArchitectureGoals {
    struct QualityAttributes {
        // Performance
        bool lowLatency;
        bool highThroughput;
        bool efficientResourceUse;

        // Scalability
        bool horizontalScaling;
        bool verticalScaling;
        bool elasticity;

        // Maintainability
        bool modularity;
        bool testability;
        bool readability;

        // Reliability
        bool availability;
        bool faultTolerance;
        bool recoverability;

        // Security
        bool authentication;
        bool authorization;
        bool dataProtection;
    };
}
```

---

## 2. SOLID Principles in Depth

### Single Responsibility Principle (SRP)

**Definition**: A class should have only one reason to change.

#### Bad Example:
```cpp
// Violates SRP - Multiple responsibilities
class Employee {
private:
    std::string name;
    double salary;

public:
    // Business logic
    void calculatePay() { /* ... */ }

    // Persistence
    void save() { /* ... */ }

    // Reporting
    void generateReport() { /* ... */ }

    // Email notification
    void sendEmail() { /* ... */ }
};
```

#### Good Example:
```cpp
// Single Responsibility - Business Entity
class Employee {
private:
    std::string name;
    double salary;
    std::string department;

public:
    Employee(const std::string& n, double s, const std::string& dept)
        : name(n), salary(s), department(dept) {}

    const std::string& getName() const { return name; }
    double getSalary() const { return salary; }
    const std::string& getDepartment() const { return department; }

    void setSalary(double newSalary) { salary = newSalary; }
};

// Single Responsibility - Payroll Calculation
class PayrollCalculator {
public:
    double calculatePay(const Employee& employee) {
        // Complex payroll logic
        double basePay = employee.getSalary();
        double taxes = calculateTaxes(basePay);
        double benefits = calculateBenefits(employee);
        return basePay - taxes + benefits;
    }

private:
    double calculateTaxes(double amount) { /* ... */ return amount * 0.25; }
    double calculateBenefits(const Employee& emp) { /* ... */ return 500.0; }
};

// Single Responsibility - Data Persistence
class EmployeeRepository {
private:
    std::unordered_map<std::string, Employee> employees;

public:
    void save(const Employee& employee) {
        employees[employee.getName()] = employee;
        // Database operations
    }

    Employee* find(const std::string& name) {
        auto it = employees.find(name);
        return (it != employees.end()) ? &it->second : nullptr;
    }

    void remove(const std::string& name) {
        employees.erase(name);
    }
};

// Single Responsibility - Report Generation
class EmployeeReportGenerator {
public:
    std::string generateReport(const Employee& employee) {
        std::ostringstream oss;
        oss << "Employee Report\n";
        oss << "================\n";
        oss << "Name: " << employee.getName() << "\n";
        oss << "Salary: $" << employee.getSalary() << "\n";
        oss << "Department: " << employee.getDepartment() << "\n";
        return oss.str();
    }
};

// Single Responsibility - Notification
class NotificationService {
public:
    void sendEmail(const std::string& recipient, const std::string& message) {
        // Email sending logic
        std::cout << "Sending email to: " << recipient << "\n";
        std::cout << "Message: " << message << "\n";
    }
};
```

### Open/Closed Principle (OCP)

**Definition**: Software entities should be open for extension but closed for modification.

```cpp
#include <memory>
#include <vector>
#include <cmath>

// Bad Example - Must modify class to add new shapes
class AreaCalculatorBad {
public:
    double calculateArea(const std::string& shapeType, double param1, double param2 = 0) {
        if (shapeType == "circle") {
            return 3.14159 * param1 * param1;
        } else if (shapeType == "rectangle") {
            return param1 * param2;
        } else if (shapeType == "triangle") {
            return 0.5 * param1 * param2;
        }
        // Need to modify this method to add new shapes!
        return 0;
    }
};

// Good Example - Open for extension, closed for modification
class Shape {
public:
    virtual ~Shape() = default;
    virtual double calculateArea() const = 0;
    virtual std::string getName() const = 0;
};

class Circle : public Shape {
private:
    double radius;

public:
    explicit Circle(double r) : radius(r) {}

    double calculateArea() const override {
        return 3.14159265359 * radius * radius;
    }

    std::string getName() const override { return "Circle"; }
};

class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double calculateArea() const override {
        return width * height;
    }

    std::string getName() const override { return "Rectangle"; }
};

class Triangle : public Shape {
private:
    double base;
    double height;

public:
    Triangle(double b, double h) : base(b), height(h) {}

    double calculateArea() const override {
        return 0.5 * base * height;
    }

    std::string getName() const override { return "Triangle"; }
};

// Can add new shapes without modifying this class
class AreaCalculator {
public:
    double calculateTotalArea(const std::vector<std::unique_ptr<Shape>>& shapes) const {
        double total = 0;
        for (const auto& shape : shapes) {
            total += shape->calculateArea();
        }
        return total;
    }

    void printAreas(const std::vector<std::unique_ptr<Shape>>& shapes) const {
        for (const auto& shape : shapes) {
            std::cout << shape->getName() << " area: "
                      << shape->calculateArea() << "\n";
        }
    }
};
```

### Liskov Substitution Principle (LSP)

**Definition**: Objects of a superclass should be replaceable with objects of its subclasses without breaking the application.

```cpp
#include <stdexcept>
#include <memory>

// Bad Example - Violates LSP
class RectangleBad {
protected:
    int width;
    int height;

public:
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getArea() const { return width * height; }
};

class SquareBad : public RectangleBad {
public:
    void setWidth(int w) override {
        width = w;
        height = w; // Violates expectations!
    }

    void setHeight(int h) override {
        width = h;
        height = h; // Violates expectations!
    }
};

// Good Example - Follows LSP
class Shape2D {
public:
    virtual ~Shape2D() = default;
    virtual double getArea() const = 0;
    virtual double getPerimeter() const = 0;
};

class Rectangle2D : public Shape2D {
protected:
    double width;
    double height;

public:
    Rectangle2D(double w, double h) : width(w), height(h) {
        if (w <= 0 || h <= 0) {
            throw std::invalid_argument("Dimensions must be positive");
        }
    }

    void setDimensions(double w, double h) {
        if (w <= 0 || h <= 0) {
            throw std::invalid_argument("Dimensions must be positive");
        }
        width = w;
        height = h;
    }

    double getArea() const override {
        return width * height;
    }

    double getPerimeter() const override {
        return 2 * (width + height);
    }

    double getWidth() const { return width; }
    double getHeight() const { return height; }
};

class Square2D : public Shape2D {
private:
    double side;

public:
    explicit Square2D(double s) : side(s) {
        if (s <= 0) {
            throw std::invalid_argument("Side must be positive");
        }
    }

    void setSide(double s) {
        if (s <= 0) {
            throw std::invalid_argument("Side must be positive");
        }
        side = s;
    }

    double getArea() const override {
        return side * side;
    }

    double getPerimeter() const override {
        return 4 * side;
    }

    double getSide() const { return side; }
};
```

### Interface Segregation Principle (ISP)

**Definition**: Clients should not be forced to depend on interfaces they don't use.

```cpp
// Bad Example - Fat Interface
class WorkerBad {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
    virtual void sleep() = 0;
    virtual void getMaintenance() = 0; // Not all workers need this!
};

// Good Example - Segregated Interfaces
class Workable {
public:
    virtual ~Workable() = default;
    virtual void work() = 0;
};

class Eatable {
public:
    virtual ~Eatable() = default;
    virtual void eat() = 0;
};

class Sleepable {
public:
    virtual ~Sleepable() = default;
    virtual void sleep() = 0;
};

class Maintainable {
public:
    virtual ~Maintainable() = default;
    virtual void performMaintenance() = 0;
};

// Human worker implements only relevant interfaces
class HumanWorker : public Workable, public Eatable, public Sleepable {
private:
    std::string name;

public:
    explicit HumanWorker(const std::string& n) : name(n) {}

    void work() override {
        std::cout << name << " is working...\n";
    }

    void eat() override {
        std::cout << name << " is eating lunch...\n";
    }

    void sleep() override {
        std::cout << name << " is sleeping...\n";
    }
};

// Robot worker implements only relevant interfaces
class RobotWorker : public Workable, public Maintainable {
private:
    std::string id;

public:
    explicit RobotWorker(const std::string& robotId) : id(robotId) {}

    void work() override {
        std::cout << "Robot " << id << " is working...\n";
    }

    void performMaintenance() override {
        std::cout << "Robot " << id << " is undergoing maintenance...\n";
    }
};
```

### Dependency Inversion Principle (DIP)

**Definition**: High-level modules should not depend on low-level modules. Both should depend on abstractions.

```cpp
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>

// Bad Example - High-level module depends on low-level module
class MySQLDatabaseBad {
public:
    void save(const std::string& data) {
        std::cout << "Saving to MySQL: " << data << "\n";
    }
};

class UserServiceBad {
private:
    MySQLDatabaseBad database; // Tight coupling!

public:
    void saveUser(const std::string& userData) {
        database.save(userData);
    }
};

// Good Example - Both depend on abstraction
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void save(const std::string& data) = 0;
    virtual std::string load(const std::string& key) = 0;
};

class MySQLDatabase : public IDatabase {
public:
    void save(const std::string& data) override {
        std::cout << "Saving to MySQL: " << data << "\n";
        // Actual MySQL implementation
    }

    std::string load(const std::string& key) override {
        return "Data from MySQL for key: " + key;
    }
};

class PostgreSQLDatabase : public IDatabase {
public:
    void save(const std::string& data) override {
        std::cout << "Saving to PostgreSQL: " << data << "\n";
        // Actual PostgreSQL implementation
    }

    std::string load(const std::string& key) override {
        return "Data from PostgreSQL for key: " + key;
    }
};

class FileDatabase : public IDatabase {
private:
    std::string filename;

public:
    explicit FileDatabase(const std::string& file) : filename(file) {}

    void save(const std::string& data) override {
        std::ofstream ofs(filename, std::ios::app);
        ofs << data << "\n";
    }

    std::string load(const std::string& key) override {
        std::ifstream ifs(filename);
        std::string line;
        while (std::getline(ifs, line)) {
            if (line.find(key) != std::string::npos) {
                return line;
            }
        }
        return "";
    }
};

// High-level module depends on abstraction
class UserService {
private:
    std::unique_ptr<IDatabase> database;

public:
    explicit UserService(std::unique_ptr<IDatabase> db)
        : database(std::move(db)) {}

    void saveUser(const std::string& userData) {
        database->save(userData);
    }

    std::string loadUser(const std::string& userId) {
        return database->load(userId);
    }
};
```

---

## 3. Architectural Patterns

### Model-View-Controller (MVC)

```cpp
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

// MODEL - Business logic and data
class UserModel {
private:
    std::string username;
    std::string email;
    int age;

public:
    UserModel() : age(0) {}
    UserModel(const std::string& user, const std::string& mail, int a)
        : username(user), email(mail), age(a) {}

    // Getters
    const std::string& getUsername() const { return username; }
    const std::string& getEmail() const { return email; }
    int getAge() const { return age; }

    // Setters with validation
    bool setUsername(const std::string& user) {
        if (user.length() >= 3) {
            username = user;
            return true;
        }
        return false;
    }

    bool setEmail(const std::string& mail) {
        if (mail.find('@') != std::string::npos) {
            email = mail;
            return true;
        }
        return false;
    }

    bool setAge(int a) {
        if (a >= 0 && a <= 150) {
            age = a;
            return true;
        }
        return false;
    }

    // Business logic
    bool isAdult() const {
        return age >= 18;
    }
};

// VIEW - Presentation layer
class UserView {
public:
    void displayUser(const UserModel& user) const {
        std::cout << "\n=== User Information ===\n";
        std::cout << "Username: " << user.getUsername() << "\n";
        std::cout << "Email: " << user.getEmail() << "\n";
        std::cout << "Age: " << user.getAge() << "\n";
        std::cout << "Status: " << (user.isAdult() ? "Adult" : "Minor") << "\n";
    }

    void displayError(const std::string& error) const {
        std::cout << "ERROR: " << error << "\n";
    }

    void displaySuccess(const std::string& message) const {
        std::cout << "SUCCESS: " << message << "\n";
    }
};

// CONTROLLER - Mediator between Model and View
class UserController {
private:
    UserModel& model;
    UserView& view;

public:
    UserController(UserModel& m, UserView& v) : model(m), view(v) {}

    void updateUsername(const std::string& username) {
        if (model.setUsername(username)) {
            view.displaySuccess("Username updated successfully");
        } else {
            view.displayError("Username must be at least 3 characters");
        }
    }

    void updateEmail(const std::string& email) {
        if (model.setEmail(email)) {
            view.displaySuccess("Email updated successfully");
        } else {
            view.displayError("Invalid email format");
        }
    }

    void updateAge(int age) {
        if (model.setAge(age)) {
            view.displaySuccess("Age updated successfully");
        } else {
            view.displayError("Age must be between 0 and 150");
        }
    }

    void displayUserInfo() {
        view.displayUser(model);
    }
};
```

### Repository Pattern

```cpp
#include <map>
#include <optional>
#include <memory>

// Entity
class Product {
private:
    int id;
    std::string name;
    double price;
    int quantity;

public:
    Product(int productId, const std::string& productName, double productPrice, int qty)
        : id(productId), name(productName), price(productPrice), quantity(qty) {}

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int qty) { quantity = qty; }
};

// Repository Interface
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual void add(const Product& product) = 0;
    virtual std::optional<Product> findById(int id) = 0;
    virtual std::vector<Product> findAll() = 0;
    virtual void update(const Product& product) = 0;
    virtual void remove(int id) = 0;
};

// In-Memory Repository Implementation
class InMemoryProductRepository : public IRepository {
private:
    std::map<int, Product> products;
    int nextId = 1;

public:
    void add(const Product& product) override {
        products.emplace(product.getId(), product);
    }

    std::optional<Product> findById(int id) override {
        auto it = products.find(id);
        if (it != products.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::vector<Product> findAll() override {
        std::vector<Product> result;
        for (const auto& pair : products) {
            result.push_back(pair.second);
        }
        return result;
    }

    void update(const Product& product) override {
        auto it = products.find(product.getId());
        if (it != products.end()) {
            it->second = product;
        }
    }

    void remove(int id) override {
        products.erase(id);
    }
};

// Service Layer using Repository
class ProductService {
private:
    std::unique_ptr<IRepository> repository;

public:
    explicit ProductService(std::unique_ptr<IRepository> repo)
        : repository(std::move(repo)) {}

    void addProduct(const Product& product) {
        repository->add(product);
    }

    std::optional<Product> getProduct(int id) {
        return repository->findById(id);
    }

    std::vector<Product> getAllProducts() {
        return repository->findAll();
    }

    bool updateProductPrice(int id, double newPrice) {
        auto product = repository->findById(id);
        if (product.has_value()) {
            Product updated = product.value();
            updated.setPrice(newPrice);
            repository->update(updated);
            return true;
        }
        return false;
    }
};
```

---

## 4. Layered Architecture

### Complete Multi-Layer Application

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>

// ============================================
// LAYER 1: DATA ACCESS LAYER (DAL)
// ============================================

class Order {
private:
    int id;
    std::string customerName;
    double totalAmount;
    std::string status;

public:
    Order(int orderId, const std::string& customer, double amount)
        : id(orderId), customerName(customer), totalAmount(amount), status("Pending") {}

    int getId() const { return id; }
    const std::string& getCustomerName() const { return customerName; }
    double getTotalAmount() const { return totalAmount; }
    const std::string& getStatus() const { return status; }
    void setStatus(const std::string& newStatus) { status = newStatus; }
};

class OrderRepository {
private:
    std::map<int, Order> orders;
    int nextId = 1;

public:
    int create(const Order& order) {
        int id = nextId++;
        orders.emplace(id, Order(id, order.getCustomerName(), order.getTotalAmount()));
        return id;
    }

    std::optional<Order> findById(int id) {
        auto it = orders.find(id);
        if (it != orders.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::vector<Order> findByCustomer(const std::string& customerName) {
        std::vector<Order> result;
        for (const auto& pair : orders) {
            if (pair.second.getCustomerName() == customerName) {
                result.push_back(pair.second);
            }
        }
        return result;
    }

    void update(const Order& order) {
        orders[order.getId()] = order;
    }

    std::vector<Order> findAll() {
        std::vector<Order> result;
        for (const auto& pair : orders) {
            result.push_back(pair.second);
        }
        return result;
    }
};

// ============================================
// LAYER 2: BUSINESS LOGIC LAYER (BLL)
// ============================================

class OrderService {
private:
    OrderRepository& repository;

    bool validateOrder(const Order& order) {
        return !order.getCustomerName().empty() && order.getTotalAmount() > 0;
    }

public:
    explicit OrderService(OrderRepository& repo) : repository(repo) {}

    int createOrder(const std::string& customerName, double amount) {
        if (customerName.empty()) {
            throw std::invalid_argument("Customer name cannot be empty");
        }
        if (amount <= 0) {
            throw std::invalid_argument("Order amount must be positive");
        }

        Order order(0, customerName, amount);
        if (!validateOrder(order)) {
            throw std::runtime_error("Order validation failed");
        }

        return repository.create(order);
    }

    bool processOrder(int orderId) {
        auto order = repository.findById(orderId);
        if (!order.has_value()) {
            return false;
        }

        Order updatedOrder = order.value();

        // Business logic: Apply discount for large orders
        if (updatedOrder.getTotalAmount() > 1000) {
            std::cout << "Applying 10% discount for large order\n";
        }

        updatedOrder.setStatus("Processing");
        repository.update(updatedOrder);
        return true;
    }

    bool completeOrder(int orderId) {
        auto order = repository.findById(orderId);
        if (!order.has_value()) {
            return false;
        }

        Order updatedOrder = order.value();
        if (updatedOrder.getStatus() != "Processing") {
            throw std::runtime_error("Order must be in Processing status");
        }

        updatedOrder.setStatus("Completed");
        repository.update(updatedOrder);
        return true;
    }

    std::vector<Order> getCustomerOrders(const std::string& customerName) {
        return repository.findByCustomer(customerName);
    }

    double calculateTotalRevenue() {
        auto orders = repository.findAll();
        double total = 0;
        for (const auto& order : orders) {
            if (order.getStatus() == "Completed") {
                total += order.getTotalAmount();
            }
        }
        return total;
    }
};

// ============================================
// LAYER 3: PRESENTATION LAYER (UI)
// ============================================

class OrderConsoleUI {
private:
    OrderService& service;

public:
    explicit OrderConsoleUI(OrderService& orderService) : service(orderService) {}

    void run() {
        std::cout << "\n=== Order Management System ===\n";

        try {
            // Create orders
            int order1 = service.createOrder("John Doe", 1500.00);
            std::cout << "Created order " << order1 << " for John Doe\n";

            int order2 = service.createOrder("Jane Smith", 750.00);
            std::cout << "Created order " << order2 << " for Jane Smith\n";

            int order3 = service.createOrder("John Doe", 2000.00);
            std::cout << "Created order " << order3 << " for John Doe\n";

            // Process orders
            service.processOrder(order1);
            service.processOrder(order2);
            service.processOrder(order3);

            // Complete some orders
            service.completeOrder(order1);
            service.completeOrder(order2);

            // Display customer orders
            displayCustomerOrders("John Doe");

            // Display revenue
            std::cout << "\nTotal Revenue: $" << service.calculateTotalRevenue() << "\n";

        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void displayCustomerOrders(const std::string& customerName) {
        std::cout << "\nOrders for " << customerName << ":\n";
        std::cout << "--------------------------------\n";

        auto orders = service.getCustomerOrders(customerName);
        for (const auto& order : orders) {
            std::cout << "Order #" << order.getId() << "\n";
            std::cout << "  Amount: $" << order.getTotalAmount() << "\n";
            std::cout << "  Status: " << order.getStatus() << "\n";
        }
    }
};
```

---

## 5. Component-Based Architecture

```cpp
#include <memory>
#include <vector>
#include <string>
#include <typeindex>
#include <unordered_map>

// Component Base Class
class Component {
public:
    virtual ~Component() = default;
    virtual void initialize() {}
    virtual void update(float deltaTime) {}
    virtual std::string getType() const = 0;
};

// Transform Component
class Transform : public Component {
private:
    float x, y, z;
    float rotationX, rotationY, rotationZ;
    float scaleX, scaleY, scaleZ;

public:
    Transform() : x(0), y(0), z(0),
                  rotationX(0), rotationY(0), rotationZ(0),
                  scaleX(1), scaleY(1), scaleZ(1) {}

    void setPosition(float px, float py, float pz) {
        x = px; y = py; z = pz;
    }

    void translate(float dx, float dy, float dz) {
        x += dx; y += dy; z += dz;
    }

    void setRotation(float rx, float ry, float rz) {
        rotationX = rx; rotationY = ry; rotationZ = rz;
    }

    std::string getType() const override { return "Transform"; }

    void printPosition() const {
        std::cout << "Position: (" << x << ", " << y << ", " << z << ")\n";
    }
};

// Renderer Component
class Renderer : public Component {
private:
    std::string meshName;
    std::string materialName;
    bool visible;

public:
    Renderer(const std::string& mesh, const std::string& material)
        : meshName(mesh), materialName(material), visible(true) {}

    void render() {
        if (visible) {
            std::cout << "Rendering mesh: " << meshName
                      << " with material: " << materialName << "\n";
        }
    }

    void setVisible(bool vis) { visible = vis; }

    std::string getType() const override { return "Renderer"; }
};

// Physics Component
class Physics : public Component {
private:
    float velocityX, velocityY, velocityZ;
    float mass;
    bool useGravity;

public:
    Physics(float m = 1.0f)
        : velocityX(0), velocityY(0), velocityZ(0), mass(m), useGravity(true) {}

    void applyForce(float fx, float fy, float fz) {
        // F = ma, so a = F/m
        velocityX += fx / mass;
        velocityY += fy / mass;
        velocityZ += fz / mass;
    }

    void update(float deltaTime) override {
        if (useGravity) {
            velocityY -= 9.81f * deltaTime; // Gravity
        }
    }

    std::string getType() const override { return "Physics"; }

    void printVelocity() const {
        std::cout << "Velocity: (" << velocityX << ", " << velocityY
                  << ", " << velocityZ << ")\n";
    }
};

// GameObject - Entity with components
class GameObject {
private:
    std::string name;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
    bool active;

public:
    explicit GameObject(const std::string& objectName)
        : name(objectName), active(true) {}

    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value,
                      "T must derive from Component");

        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        components[std::type_index(typeid(T))] = component;
        component->initialize();
        return component.get();
    }

    template<typename T>
    T* getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    bool hasComponent() const {
        return components.find(std::type_index(typeid(T))) != components.end();
    }

    void update(float deltaTime) {
        if (!active) return;

        for (auto& pair : components) {
            pair.second->update(deltaTime);
        }
    }

    const std::string& getName() const { return name; }
    void setActive(bool isActive) { active = isActive; }
    bool isActive() const { return active; }
};

// Scene - Container for game objects
class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;

public:
    GameObject* createGameObject(const std::string& name) {
        auto obj = std::make_unique<GameObject>(name);
        GameObject* ptr = obj.get();
        gameObjects.push_back(std::move(obj));
        return ptr;
    }

    void update(float deltaTime) {
        for (auto& obj : gameObjects) {
            if (obj->isActive()) {
                obj->update(deltaTime);
            }
        }
    }

    GameObject* findGameObject(const std::string& name) {
        for (auto& obj : gameObjects) {
            if (obj->getName() == name) {
                return obj.get();
            }
        }
        return nullptr;
    }
};
```

---

## 6. Complete Architecture Example: E-Commerce System

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>

// ============================================
// DOMAIN LAYER - Core Business Entities
// ============================================

class Money {
private:
    double amount;
    std::string currency;

public:
    Money(double amt = 0.0, const std::string& curr = "USD")
        : amount(amt), currency(curr) {}

    double getAmount() const { return amount; }
    const std::string& getCurrency() const { return currency; }

    Money operator+(const Money& other) const {
        if (currency != other.currency) {
            throw std::runtime_error("Currency mismatch");
        }
        return Money(amount + other.amount, currency);
    }

    Money operator*(double multiplier) const {
        return Money(amount * multiplier, currency);
    }

    bool operator>=(const Money& other) const {
        return amount >= other.amount;
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << currency << " " << std::fixed << std::setprecision(2) << amount;
        return oss.str();
    }
};

class Product {
private:
    std::string id;
    std::string name;
    std::string description;
    Money price;
    int stockQuantity;

public:
    Product(const std::string& productId, const std::string& productName,
            const std::string& desc, Money productPrice, int stock)
        : id(productId), name(productName), description(desc),
          price(productPrice), stockQuantity(stock) {}

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const Money& getPrice() const { return price; }
    int getStockQuantity() const { return stockQuantity; }

    bool isInStock() const { return stockQuantity > 0; }

    bool reduceStock(int quantity) {
        if (quantity <= stockQuantity) {
            stockQuantity -= quantity;
            return true;
        }
        return false;
    }

    void addStock(int quantity) {
        stockQuantity += quantity;
    }
};

class OrderItem {
private:
    std::shared_ptr<Product> product;
    int quantity;
    Money priceAtOrder;

public:
    OrderItem(std::shared_ptr<Product> prod, int qty)
        : product(prod), quantity(qty), priceAtOrder(prod->getPrice()) {}

    std::shared_ptr<Product> getProduct() const { return product; }
    int getQuantity() const { return quantity; }

    Money getTotal() const {
        return priceAtOrder * quantity;
    }
};

class ShoppingCart {
private:
    std::vector<OrderItem> items;
    std::string customerId;

public:
    explicit ShoppingCart(const std::string& customer) : customerId(customer) {}

    void addItem(std::shared_ptr<Product> product, int quantity) {
        if (!product->isInStock()) {
            throw std::runtime_error("Product out of stock");
        }
        if (quantity > product->getStockQuantity()) {
            throw std::runtime_error("Insufficient stock");
        }

        // Check if product already in cart
        for (auto& item : items) {
            if (item.getProduct()->getId() == product->getId()) {
                // Update existing item
                return;
            }
        }

        items.emplace_back(product, quantity);
    }

    void removeItem(const std::string& productId) {
        items.erase(
            std::remove_if(items.begin(), items.end(),
                [&productId](const OrderItem& item) {
                    return item.getProduct()->getId() == productId;
                }),
            items.end()
        );
    }

    Money getTotal() const {
        Money total;
        for (const auto& item : items) {
            total = total + item.getTotal();
        }
        return total;
    }

    const std::vector<OrderItem>& getItems() const { return items; }
    void clear() { items.clear(); }
    bool isEmpty() const { return items.empty(); }
};

// ============================================
// APPLICATION LAYER - Use Cases
// ============================================

class IProductRepository {
public:
    virtual ~IProductRepository() = default;
    virtual void save(std::shared_ptr<Product> product) = 0;
    virtual std::shared_ptr<Product> findById(const std::string& id) = 0;
    virtual std::vector<std::shared_ptr<Product>> findAll() = 0;
};

class InMemoryProductRepository : public IProductRepository {
private:
    std::map<std::string, std::shared_ptr<Product>> products;

public:
    void save(std::shared_ptr<Product> product) override {
        products[product->getId()] = product;
    }

    std::shared_ptr<Product> findById(const std::string& id) override {
        auto it = products.find(id);
        return (it != products.end()) ? it->second : nullptr;
    }

    std::vector<std::shared_ptr<Product>> findAll() override {
        std::vector<std::shared_ptr<Product>> result;
        for (const auto& pair : products) {
            result.push_back(pair.second);
        }
        return result;
    }
};

class CheckoutService {
private:
    IProductRepository& productRepository;

public:
    explicit CheckoutService(IProductRepository& repo)
        : productRepository(repo) {}

    bool processCheckout(ShoppingCart& cart) {
        if (cart.isEmpty()) {
            throw std::runtime_error("Cart is empty");
        }

        // Verify stock availability
        for (const auto& item : cart.getItems()) {
            auto product = item.getProduct();
            if (item.getQuantity() > product->getStockQuantity()) {
                throw std::runtime_error("Insufficient stock for: " + product->getName());
            }
        }

        // Process payment (simplified)
        Money total = cart.getTotal();
        std::cout << "Processing payment of " << total.toString() << "\n";

        // Update stock
        for (const auto& item : cart.getItems()) {
            auto product = item.getProduct();
            product->reduceStock(item.getQuantity());
            productRepository.save(product);
        }

        // Clear cart
        cart.clear();

        std::cout << "Order completed successfully!\n";
        return true;
    }
};

// ============================================
// PRESENTATION LAYER
// ============================================

class ECommerceConsoleUI {
private:
    IProductRepository& productRepository;
    CheckoutService& checkoutService;

public:
    ECommerceConsoleUI(IProductRepository& repo, CheckoutService& checkout)
        : productRepository(repo), checkoutService(checkout) {}

    void run() {
        std::cout << "\n=== E-Commerce System ===\n";

        // Setup products
        setupProducts();

        // Simulate shopping
        ShoppingCart cart("customer123");

        // Add items to cart
        auto laptop = productRepository.findById("PROD001");
        if (laptop) {
            cart.addItem(laptop, 1);
            std::cout << "Added " << laptop->getName() << " to cart\n";
        }

        auto mouse = productRepository.findById("PROD002");
        if (mouse) {
            cart.addItem(mouse, 2);
            std::cout << "Added " << mouse->getName() << " x2 to cart\n";
        }

        // Display cart
        displayCart(cart);

        // Checkout
        try {
            checkoutService.processCheckout(cart);
        } catch (const std::exception& e) {
            std::cout << "Checkout failed: " << e.what() << "\n";
        }

        // Display updated stock
        displayProducts();
    }

private:
    void setupProducts() {
        auto laptop = std::make_shared<Product>(
            "PROD001", "Gaming Laptop", "High-performance gaming laptop",
            Money(1299.99), 10
        );
        productRepository.save(laptop);

        auto mouse = std::make_shared<Product>(
            "PROD002", "Wireless Mouse", "Ergonomic wireless mouse",
            Money(29.99), 50
        );
        productRepository.save(mouse);

        auto keyboard = std::make_shared<Product>(
            "PROD003", "Mechanical Keyboard", "RGB mechanical keyboard",
            Money(149.99), 25
        );
        productRepository.save(keyboard);
    }

    void displayCart(const ShoppingCart& cart) {
        std::cout << "\n=== Shopping Cart ===\n";
        for (const auto& item : cart.getItems()) {
            auto product = item.getProduct();
            std::cout << product->getName() << " x" << item.getQuantity()
                      << " = " << item.getTotal().toString() << "\n";
        }
        std::cout << "Total: " << cart.getTotal().toString() << "\n";
    }

    void displayProducts() {
        std::cout << "\n=== Available Products ===\n";
        auto products = productRepository.findAll();
        for (const auto& product : products) {
            std::cout << product->getName() << " - "
                      << product->getPrice().toString()
                      << " (Stock: " << product->getStockQuantity() << ")\n";
        }
    }
};
```

---

## 7. Main Application Demonstrating All Concepts

```cpp
int main() {
    std::cout << "=== Software Architecture Principles Demonstration ===\n\n";

    // 1. MVC Pattern Demo
    std::cout << "1. MVC Pattern Demo:\n";
    std::cout << "====================\n";
    UserModel userModel;
    UserView userView;
    UserController userController(userModel, userView);

    userController.updateUsername("john_doe");
    userController.updateEmail("john@example.com");
    userController.updateAge(25);
    userController.displayUserInfo();

    // 2. Layered Architecture Demo
    std::cout << "\n\n2. Layered Architecture Demo:\n";
    std::cout << "==============================\n";
    OrderRepository orderRepo;
    OrderService orderService(orderRepo);
    OrderConsoleUI orderUI(orderService);
    orderUI.run();

    // 3. Component-Based Architecture Demo
    std::cout << "\n\n3. Component-Based Architecture Demo:\n";
    std::cout << "=====================================\n";
    Scene scene;

    auto player = scene.createGameObject("Player");
    player->addComponent<Transform>();
    player->addComponent<Renderer>("PlayerMesh", "PlayerMaterial");
    player->addComponent<Physics>(75.0f);

    auto enemy = scene.createGameObject("Enemy");
    enemy->addComponent<Transform>();
    enemy->addComponent<Renderer>("EnemyMesh", "EnemyMaterial");

    std::cout << "Created game objects with components\n";
    std::cout << "Player has Transform: " << player->hasComponent<Transform>() << "\n";
    std::cout << "Player has Physics: " << player->hasComponent<Physics>() << "\n";

    auto transform = player->getComponent<Transform>();
    if (transform) {
        transform->setPosition(10, 20, 30);
        transform->printPosition();
    }

    // 4. E-Commerce System Demo
    std::cout << "\n\n4. E-Commerce System Demo:\n";
    std::cout << "===========================\n";
    InMemoryProductRepository productRepo;
    CheckoutService checkoutService(productRepo);
    ECommerceConsoleUI ecommerceUI(productRepo, checkoutService);
    ecommerceUI.run();

    return 0;
}
```

---

## 8. Key Takeaways

### Architecture Best Practices:

1. **Separation of Concerns**: Each component has a single, well-defined responsibility
2. **Dependency Management**: Use interfaces and dependency injection
3. **Layering**: Organize code into logical layers (Presentation, Business, Data)
4. **Modularity**: Create loosely coupled, highly cohesive components
5. **Testability**: Design for easy unit and integration testing
6. **Scalability**: Plan for growth from the beginning
7. **Maintainability**: Write clean, documented, understandable code

### Common Pitfalls to Avoid:

- ❌ Tight coupling between layers
- ❌ God classes with too many responsibilities
- ❌ Circular dependencies
- ❌ Leaking domain logic into presentation layer
- ❌ Direct database access from UI layer
- ❌ Premature optimization
- ❌ Over-engineering simple problems

---

## 9. Exercises

### Exercise 1: Implement Factory Pattern
Create a notification system with multiple notification types (Email, SMS, Push) using Factory pattern.

### Exercise 2: Add Caching Layer
Extend the repository pattern to include a caching layer that reduces database calls.

### Exercise 3: Implement Observer Pattern
Create an event system where game objects can subscribe to and emit events.

### Exercise 4: Design a Plugin System
Create an architecture that allows loading and unloading plugins at runtime.

---

## Summary

In this lesson, you learned:

- ✅ SOLID principles and their practical applications
- ✅ Common architectural patterns (MVC, Repository, Layered)
- ✅ Component-based architecture design
- ✅ How to structure large-scale applications
- ✅ Dependency management and inversion of control
- ✅ Best practices for maintainable, scalable code

**Next Lesson**: We'll start building real-world projects, beginning with a High-Performance Data Processing Engine!

---

**Estimated Time to Master**: 20-30 hours of practice
**Difficulty Level**: Advanced
**Prerequisites Complete**: ✅ You're ready for real-world project development!
