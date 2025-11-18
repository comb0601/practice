# Lesson 99: Code Maintenance and Refactoring

**Duration**: 6 hours
**Difficulty**: Advanced
**Prerequisites**: Lessons 1-98

## Table of Contents
1. Refactoring Techniques
2. Code Smells
3. Legacy Code
4. Technical Debt
5. Continuous Improvement

---

## 1. Refactoring Example: Extract Method

```cpp
// BEFORE: Long method
void processOrder(Order& order) {
    // Validate
    if (order.items.empty()) {
        throw std::runtime_error("Empty order");
    }
    if (order.customer.empty()) {
        throw std::runtime_error("No customer");
    }

    // Calculate total
    double total = 0;
    for (const auto& item : order.items) {
        total += item.price * item.quantity;
    }

    // Apply discount
    if (total > 100) {
        total *= 0.9;
    }

    // Save
    database.save(order);
}

// AFTER: Extracted methods
void processOrder(Order& order) {
    validateOrder(order);
    double total = calculateTotal(order);
    total = applyDiscount(total);
    saveOrder(order);
}

void validateOrder(const Order& order) {
    if (order.items.empty()) {
        throw std::runtime_error("Empty order");
    }
    if (order.customer.empty()) {
        throw std::runtime_error("No customer");
    }
}

double calculateTotal(const Order& order) {
    double total = 0;
    for (const auto& item : order.items) {
        total += item.price * item.quantity;
    }
    return total;
}

double applyDiscount(double total) {
    return total > 100 ? total * 0.9 : total;
}

void saveOrder(const Order& order) {
    database.save(order);
}
```

---

## 2. Replace Conditional with Polymorphism

```cpp
// BEFORE: Type checking
class Shape {
public:
    enum Type { CIRCLE, RECTANGLE, TRIANGLE };
    Type type;
};

double calculateArea(const Shape& shape) {
    if (shape.type == Shape::CIRCLE) {
        return 3.14 * shape.radius * shape.radius;
    } else if (shape.type == Shape::RECTANGLE) {
        return shape.width * shape.height;
    } else if (shape.type == Shape::TRIANGLE) {
        return 0.5 * shape.base * shape.height;
    }
    return 0;
}

// AFTER: Polymorphism
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape {
    double radius;
public:
    double area() const override {
        return 3.14 * radius * radius;
    }
};

class Rectangle : public Shape {
    double width, height;
public:
    double area() const override {
        return width * height;
    }
};
```

---

## 3. Dependency Injection

```cpp
// BEFORE: Tight coupling
class UserService {
    MySQLDatabase database; // Tightly coupled!

public:
    void saveUser(const User& user) {
        database.save(user);
    }
};

// AFTER: Dependency injection
class UserService {
    IDatabase& database; // Abstraction!

public:
    explicit UserService(IDatabase& db) : database(db) {}

    void saveUser(const User& user) {
        database.save(user);
    }
};

// Now can use any database implementation
MySQLDatabase mysql;
UserService service(mysql);

// Or switch to PostgreSQL
PostgreSQLDatabase postgres;
UserService service2(postgres);
```

---

## Summary

✅ **Extract Method**: Simplify complex code
✅ **Polymorphism**: Replace type checking
✅ **Dependency Injection**: Reduce coupling
✅ **Continuous Refactoring**: Improve gradually
