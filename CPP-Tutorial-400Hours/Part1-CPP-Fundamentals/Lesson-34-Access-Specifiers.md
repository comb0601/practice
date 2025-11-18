# Lesson 34: Access Specifiers (public, private, protected)

**Duration**: 7 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Access Control
2. Public Access Specifier
3. Private Access Specifier
4. Protected Access Specifier
5. Default Access Levels
6. Access Control Best Practices
7. Real-World Examples
8. Exercises
9. Practice Projects

---

## 1. Introduction to Access Control

**Access specifiers** control how class members can be accessed from outside the class. They are fundamental to **data hiding** and **encapsulation**.

### The Three Access Specifiers:

1. **public**: Accessible from anywhere
2. **private**: Accessible only within the class
3. **protected**: Accessible within the class and derived classes

### Why Use Access Control?

1. **Data Protection**: Prevent invalid modifications
2. **Encapsulation**: Hide implementation details
3. **Maintainability**: Change internals without affecting users
4. **Security**: Control what can be accessed
5. **Interface Design**: Define clear public API

### Access Levels Diagram:

```
┌─────────────────────────────────────┐
│           Class Members             │
├─────────────────────────────────────┤
│  public:                            │
│    - Accessible everywhere          │
│                                     │
│  protected:                         │
│    - Accessible in class            │
│    - Accessible in derived classes  │
│                                     │
│  private:                           │
│    - Accessible only in class       │
└─────────────────────────────────────┘
```

---

## 2. Public Access Specifier

**Public members** can be accessed from anywhere: inside the class, outside the class, and in derived classes.

### Syntax:
```cpp
class ClassName {
public:
    // Public members here
    int publicVariable;
    void publicFunction();
};
```

### Example 1: Public Members

```cpp
#include <iostream>
#include <string>
using namespace std;

class Student {
public:
    // Public data members (not recommended for important data)
    string name;
    int rollNumber;

    // Public member functions
    void displayInfo() {
        cout << "Name: " << name << endl;
        cout << "Roll Number: " << rollNumber << endl;
    }

    void updateName(string newName) {
        name = newName;
    }
};

int main() {
    Student student;

    // Can access public members directly
    student.name = "Alice";
    student.rollNumber = 101;

    student.displayInfo();

    // Can modify public members directly
    student.rollNumber = 102;  // Direct access
    student.updateName("Alice Johnson");

    student.displayInfo();

    return 0;
}
```

**Output**:
```
Name: Alice
Roll Number: 101
Name: Alice Johnson
Roll Number: 102
```

### When to Use Public:
- Interface functions (methods users should call)
- Constants that should be accessible
- Utility functions

---

## 3. Private Access Specifier

**Private members** can only be accessed from within the class itself. This is the cornerstone of encapsulation.

### Syntax:
```cpp
class ClassName {
private:
    // Private members here
    int privateVariable;
    void privateFunction();
};
```

### Example 1: Basic Private Members

```cpp
#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    // Private data members - cannot be accessed directly from outside
    string accountNumber;
    double balance;
    string pin;

public:
    // Public constructor
    BankAccount(string accNum, double initialBalance, string pinCode)
        : accountNumber(accNum), balance(initialBalance), pin(pinCode) {
        cout << "Account created: " << accountNumber << endl;
    }

    // Public interface to access private data
    bool withdraw(double amount, string pinCode) {
        if (pinCode != pin) {
            cout << "Invalid PIN!" << endl;
            return false;
        }

        if (amount > balance) {
            cout << "Insufficient funds!" << endl;
            return false;
        }

        balance -= amount;
        cout << "Withdrawn: $" << amount << endl;
        return true;
    }

    bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: $" << amount << endl;
            return true;
        }
        return false;
    }

    double getBalance(string pinCode) const {
        if (pinCode == pin) {
            return balance;
        }
        cout << "Invalid PIN!" << endl;
        return -1;
    }

    void displayInfo(string pinCode) const {
        if (pinCode != pin) {
            cout << "Invalid PIN!" << endl;
            return;
        }

        cout << "Account: " << accountNumber << endl;
        cout << "Balance: $" << balance << endl;
    }
};

int main() {
    BankAccount account("ACC12345", 1000.0, "1234");

    // Cannot access private members
    // account.balance = 5000;  // ERROR: balance is private
    // account.pin = "0000";    // ERROR: pin is private

    // Must use public interface
    account.deposit(500);
    account.withdraw(200, "1234");

    double bal = account.getBalance("1234");
    cout << "Current balance: $" << bal << endl;

    // Wrong PIN
    account.withdraw(100, "0000");

    return 0;
}
```

**Output**:
```
Account created: ACC12345
Deposited: $500
Withdrawn: $200
Current balance: $1300
Invalid PIN!
```

### Example 2: Private Helper Functions

```cpp
#include <iostream>
#include <string>
using namespace std;

class PasswordValidator {
private:
    string password;

    // Private helper functions
    bool hasUpperCase() const {
        for (char c : password) {
            if (isupper(c)) return true;
        }
        return false;
    }

    bool hasLowerCase() const {
        for (char c : password) {
            if (islower(c)) return true;
        }
        return false;
    }

    bool hasDigit() const {
        for (char c : password) {
            if (isdigit(c)) return true;
        }
        return false;
    }

    bool hasSpecialChar() const {
        string special = "!@#$%^&*()";
        for (char c : password) {
            if (special.find(c) != string::npos) return true;
        }
        return false;
    }

public:
    void setPassword(const string& pwd) {
        password = pwd;
    }

    // Public function uses private helpers
    bool isStrong() const {
        if (password.length() < 8) return false;
        if (!hasUpperCase()) return false;
        if (!hasLowerCase()) return false;
        if (!hasDigit()) return false;
        if (!hasSpecialChar()) return false;
        return true;
    }

    void validate() const {
        cout << "Password: " << password << endl;
        cout << "Length: " << password.length() << " - "
             << (password.length() >= 8 ? "OK" : "Too short") << endl;
        cout << "Has uppercase: " << (hasUpperCase() ? "Yes" : "No") << endl;
        cout << "Has lowercase: " << (hasLowerCase() ? "Yes" : "No") << endl;
        cout << "Has digit: " << (hasDigit() ? "Yes" : "No") << endl;
        cout << "Has special: " << (hasSpecialChar() ? "Yes" : "No") << endl;
        cout << "Is strong: " << (isStrong() ? "Yes" : "No") << endl;
    }
};

int main() {
    PasswordValidator validator;

    cout << "=== Testing Password 1 ===" << endl;
    validator.setPassword("weak");
    validator.validate();

    cout << "\n=== Testing Password 2 ===" << endl;
    validator.setPassword("Strong123!");
    validator.validate();

    return 0;
}
```

---

## 4. Protected Access Specifier

**Protected members** are accessible within the class and in derived (child) classes, but not from outside.

### Syntax:
```cpp
class ClassName {
protected:
    // Protected members here
    int protectedVariable;
    void protectedFunction();
};
```

### Example 1: Protected in Inheritance

```cpp
#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    // Protected members - accessible in derived classes
    string name;
    int age;

private:
    // Private member - NOT accessible in derived classes
    string ssn;

public:
    Person(string n, int a, string s)
        : name(n), age(a), ssn(s) {}

    void displayBasicInfo() const {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        // Can access private member in same class
        cout << "SSN: " << ssn << endl;
    }
};

class Employee : public Person {
private:
    int employeeID;
    double salary;

public:
    Employee(string n, int a, string s, int id, double sal)
        : Person(n, a, s), employeeID(id), salary(sal) {}

    void displayEmployeeInfo() {
        // Can access protected members from base class
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;

        // Cannot access private members from base class
        // cout << ssn << endl;  // ERROR: ssn is private

        cout << "Employee ID: " << employeeID << endl;
        cout << "Salary: $" << salary << endl;
    }

    void celebrateBirthday() {
        // Can modify protected members
        age++;
        cout << name << " is now " << age << " years old!" << endl;
    }
};

int main() {
    Employee emp("Alice Johnson", 25, "123-45-6789", 1001, 60000);

    emp.displayBasicInfo();
    cout << "\n--- Employee Details ---" << endl;
    emp.displayEmployeeInfo();

    // Cannot access protected members from outside
    // emp.name = "Bob";  // ERROR: name is protected
    // emp.age = 30;      // ERROR: age is protected

    emp.celebrateBirthday();

    return 0;
}
```

**Output**:
```
Name: Alice Johnson
Age: 25
SSN: 123-45-6789

--- Employee Details ---
Name: Alice Johnson
Age: 25
Employee ID: 1001
Salary: $60000
Alice Johnson is now 26 years old!
```

### Example 2: Protected Methods

```cpp
#include <iostream>
using namespace std;

class Shape {
protected:
    // Protected helper method
    void printBorder(int width) const {
        for (int i = 0; i < width; i++) {
            cout << "=";
        }
        cout << endl;
    }

public:
    virtual void draw() const = 0;  // Pure virtual function
};

class Rectangle : public Shape {
private:
    int width;
    int height;

public:
    Rectangle(int w, int h) : width(w), height(h) {}

    void draw() const override {
        printBorder(width * 2);  // Can use protected method
        for (int i = 0; i < height; i++) {
            cout << "*";
            for (int j = 0; j < width * 2 - 2; j++) {
                cout << " ";
            }
            cout << "*" << endl;
        }
        printBorder(width * 2);
    }
};

int main() {
    Rectangle rect(10, 5);
    rect.draw();

    // rect.printBorder(20);  // ERROR: printBorder is protected

    return 0;
}
```

---

## 5. Default Access Levels

### Class vs Struct Default Access:

**Class**: Default is **private**
```cpp
class MyClass {
    int x;  // private by default
};
```

**Struct**: Default is **public**
```cpp
struct MyStruct {
    int x;  // public by default
};
```

### Example: Default Access Comparison

```cpp
#include <iostream>
using namespace std;

class MyClass {
    int privateByDefault;  // private by default

public:
    int publicMember;

    void test() {
        privateByDefault = 10;  // OK: Inside class
    }
};

struct MyStruct {
    int publicByDefault;  // public by default

private:
    int privateMember;

public:
    void test() {
        privateMember = 20;  // OK: Inside struct
    }
};

int main() {
    MyClass obj1;
    // obj1.privateByDefault = 5;  // ERROR: private by default
    obj1.publicMember = 10;        // OK: public

    MyStruct obj2;
    obj2.publicByDefault = 15;     // OK: public by default
    // obj2.privateMember = 20;    // ERROR: explicitly private

    return 0;
}
```

---

## 6. Access Control Best Practices

### Best Practice Guidelines:

1. **Make data members private**
2. **Provide public getters/setters**
3. **Use protected for inheritance hierarchies**
4. **Keep implementation details private**
5. **Minimize public interface**

### Example: Well-Designed Class

```cpp
#include <iostream>
#include <string>
using namespace std;

class Product {
private:
    // All data is private
    int productID;
    string name;
    double price;
    int stock;

    // Private validation
    bool isValidPrice(double p) const {
        return p >= 0;
    }

    bool isValidStock(int s) const {
        return s >= 0;
    }

public:
    // Constructor
    Product(int id, const string& n, double p, int s)
        : productID(id), name(n), price(0), stock(0) {
        setPrice(p);
        setStock(s);
    }

    // Public getters (const)
    int getProductID() const { return productID; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    // Public setters with validation
    void setName(const string& n) {
        if (!n.empty()) {
            name = n;
        }
    }

    bool setPrice(double p) {
        if (isValidPrice(p)) {
            price = p;
            return true;
        }
        cout << "Invalid price!" << endl;
        return false;
    }

    bool setStock(int s) {
        if (isValidStock(s)) {
            stock = s;
            return true;
        }
        cout << "Invalid stock!" << endl;
        return false;
    }

    // Public business logic
    bool purchase(int quantity) {
        if (quantity <= stock) {
            stock -= quantity;
            return true;
        }
        return false;
    }

    void restock(int quantity) {
        if (quantity > 0) {
            stock += quantity;
        }
    }

    void display() const {
        cout << "ID: " << productID << endl;
        cout << "Name: " << name << endl;
        cout << "Price: $" << price << endl;
        cout << "Stock: " << stock << endl;
    }
};

int main() {
    Product product(1001, "Laptop", 999.99, 10);

    product.display();

    // Cannot access private members
    // product.price = -100;  // ERROR: price is private

    // Must use public interface
    product.setPrice(899.99);  // Validated

    if (product.purchase(3)) {
        cout << "\nPurchased 3 units successfully!" << endl;
    }

    product.display();

    return 0;
}
```

---

## 7. Real-World Examples

### Example 1: Secure User Account

```cpp
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class UserAccount {
private:
    string username;
    string passwordHash;  // Never store plain password
    bool isLocked;
    int failedAttempts;
    const int MAX_ATTEMPTS = 3;

    // Private helper: simulate password hashing
    string hashPassword(const string& password) const {
        // In real application, use proper hashing (bcrypt, etc.)
        string hashed = "";
        for (char c : password) {
            hashed += char(c + 3);  // Simple Caesar cipher (NOT secure!)
        }
        return hashed;
    }

    bool verifyPassword(const string& password) const {
        return hashPassword(password) == passwordHash;
    }

protected:
    // Protected for potential derived classes
    void logActivity(const string& activity) const {
        time_t now = time(0);
        cout << "[" << ctime(&now) << "] " << activity << endl;
    }

public:
    UserAccount(const string& user, const string& pass)
        : username(user), isLocked(false), failedAttempts(0) {
        passwordHash = hashPassword(pass);
        logActivity("Account created: " + username);
    }

    bool login(const string& password) {
        if (isLocked) {
            cout << "Account is locked!" << endl;
            return false;
        }

        if (verifyPassword(password)) {
            failedAttempts = 0;
            logActivity("Successful login: " + username);
            return true;
        } else {
            failedAttempts++;
            logActivity("Failed login attempt: " + username);

            if (failedAttempts >= MAX_ATTEMPTS) {
                isLocked = true;
                logActivity("Account locked: " + username);
            }
            return false;
        }
    }

    bool changePassword(const string& oldPass, const string& newPass) {
        if (!verifyPassword(oldPass)) {
            cout << "Invalid current password!" << endl;
            return false;
        }

        passwordHash = hashPassword(newPass);
        logActivity("Password changed: " + username);
        return true;
    }

    string getUsername() const { return username; }
    bool getIsLocked() const { return isLocked; }
};

int main() {
    UserAccount account("alice123", "SecurePass123");

    // Try correct password
    if (account.login("SecurePass123")) {
        cout << "Login successful!\n" << endl;
    }

    // Try wrong passwords
    cout << "Attempting wrong passwords..." << endl;
    account.login("wrong1");
    account.login("wrong2");
    account.login("wrong3");

    cout << "\nAccount status: "
         << (account.getIsLocked() ? "LOCKED" : "ACTIVE") << endl;

    return 0;
}
```

### Example 2: Temperature Sensor with Calibration

```cpp
#include <iostream>
using namespace std;

class TemperatureSensor {
private:
    double rawReading;
    double calibrationOffset;
    const double MIN_TEMP = -50.0;
    const double MAX_TEMP = 150.0;

    bool isValidReading(double temp) const {
        return temp >= MIN_TEMP && temp <= MAX_TEMP;
    }

protected:
    // Protected: Can be used by specialized sensor classes
    double applyCalibration(double raw) const {
        return raw + calibrationOffset;
    }

public:
    TemperatureSensor() : rawReading(0), calibrationOffset(0) {}

    bool takeReading(double raw) {
        if (isValidReading(raw)) {
            rawReading = raw;
            return true;
        }
        cout << "Invalid reading!" << endl;
        return false;
    }

    void calibrate(double offset) {
        calibrationOffset = offset;
        cout << "Sensor calibrated with offset: " << offset << endl;
    }

    double getCelsius() const {
        return applyCalibration(rawReading);
    }

    double getFahrenheit() const {
        return getCelsius() * 9.0 / 5.0 + 32.0;
    }

    double getKelvin() const {
        return getCelsius() + 273.15;
    }

    void displayAll() const {
        cout << "Temperature Readings:" << endl;
        cout << "  Celsius: " << getCelsius() << "°C" << endl;
        cout << "  Fahrenheit: " << getFahrenheit() << "°F" << endl;
        cout << "  Kelvin: " << getKelvin() << "K" << endl;
    }
};

int main() {
    TemperatureSensor sensor;

    sensor.takeReading(25.0);
    sensor.displayAll();

    cout << "\nCalibrating sensor..." << endl;
    sensor.calibrate(-2.0);  // Sensor reads 2 degrees high

    sensor.takeReading(25.0);
    sensor.displayAll();

    return 0;
}
```

---

## 8. Exercises

### Exercise 1: Secure Wallet
Create a wallet class with private balance and transaction history.

```cpp
class Wallet {
private:
    double balance;
    string currency;
    // Add transaction history array

public:
    // Constructor
    // Getters
    // addMoney()
    // spendMoney()
    // displayBalance()
};
```

### Exercise 2: Grade Book
Create a gradebook with private grades array.

```cpp
class GradeBook {
private:
    double grades[10];
    int count;

    double calculateAverage() const;

public:
    void addGrade(double grade);
    double getHighest() const;
    double getLowest() const;
    char getLetterGrade() const;
};
```

### Exercise 3: Inventory Item
Create an inventory item with proper access control.

```cpp
class InventoryItem {
private:
    int itemID;
    string name;
    int quantity;
    double pricePerUnit;

public:
    // Add appropriate constructors and methods
    // Include validation in setters
};
```

---

## 9. Practice Projects

### Project 1: Complete Library System

```cpp
#include <iostream>
#include <string>
using namespace std;

class LibraryBook {
private:
    const int bookID;
    string title;
    string author;
    string ISBN;
    bool isAvailable;
    string borrowerName;
    int timesIssued;

    bool isValidISBN(const string& isbn) const {
        return isbn.length() >= 10;
    }

protected:
    void logTransaction(const string& transaction) const {
        cout << "[Book " << bookID << "] " << transaction << endl;
    }

public:
    LibraryBook(int id, const string& t, const string& a, const string& isbn)
        : bookID(id), title(t), author(a), ISBN(""), isAvailable(true),
          borrowerName(""), timesIssued(0) {
        if (isValidISBN(isbn)) {
            ISBN = isbn;
        }
        logTransaction("Book added to system");
    }

    // Getters
    int getBookID() const { return bookID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getAvailability() const { return isAvailable; }
    int getTimesIssued() const { return timesIssued; }

    // Business logic
    bool borrowBook(const string& borrower) {
        if (!isAvailable) {
            cout << "Book is already borrowed!" << endl;
            return false;
        }

        isAvailable = false;
        borrowerName = borrower;
        timesIssued++;
        logTransaction("Borrowed by: " + borrower);
        return true;
    }

    bool returnBook() {
        if (isAvailable) {
            cout << "Book was not borrowed!" << endl;
            return false;
        }

        logTransaction("Returned by: " + borrowerName);
        isAvailable = true;
        borrowerName = "";
        return true;
    }

    void displayInfo() const {
        cout << "\n=== Book Information ===" << endl;
        cout << "ID: " << bookID << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << ISBN << endl;
        cout << "Status: " << (isAvailable ? "Available" : "Borrowed") << endl;
        if (!isAvailable) {
            cout << "Borrowed by: " << borrowerName << endl;
        }
        cout << "Times issued: " << timesIssued << endl;
    }
};

int main() {
    LibraryBook book(1, "C++ Programming", "Bjarne Stroustrup", "978-0321563842");

    book.displayInfo();

    book.borrowBook("Alice");
    book.displayInfo();

    book.returnBook();
    book.displayInfo();

    return 0;
}
```

---

## Key Takeaways

1. **private**: Data members should usually be private
2. **public**: Interface methods should be public
3. **protected**: Use for inheritance hierarchies
4. **Encapsulation**: Hide implementation details
5. **Validation**: Implement in private/protected helpers
6. **Class**: Default is private
7. **Struct**: Default is public
8. **Getters/Setters**: Provide controlled access

---

## Common Mistakes

### Mistake 1: Public Data Members
```cpp
class Bad {
public:
    double balance;  // Bad: Direct access, no validation
};
```

### Mistake 2: Accessing Private Members
```cpp
MyClass obj;
obj.privateMember = 10;  // ERROR: Can't access private
```

### Mistake 3: Wrong Access Level
```cpp
class Base {
private:
    int value;  // Should be protected if used by derived
};

class Derived : public Base {
    void test() {
        value = 10;  // ERROR: Can't access private from base
    }
};
```

---

## Summary Checklist

Before moving to Lesson 35, ensure you can:
- [ ] Use public, private, and protected correctly
- [ ] Understand default access levels
- [ ] Implement proper encapsulation
- [ ] Create getters and setters
- [ ] Validate data in setters
- [ ] Use protected for inheritance
- [ ] Design clean public interfaces

---

## Next Lesson Preview

In **Lesson 35: Encapsulation**, we'll learn:
- Principles of encapsulation
- Designing class interfaces
- Information hiding techniques
- Real-world encapsulation patterns

Excellent! You now understand access control in C++!
