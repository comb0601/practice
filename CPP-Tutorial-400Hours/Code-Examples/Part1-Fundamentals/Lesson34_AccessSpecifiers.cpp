/*
 * Lesson 34: Access Specifiers - public, private, protected
 * Compile: cl Lesson34_AccessSpecifiers.cpp
 */

#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    string accountNumber;
    double balance;
    string pin;

protected:
    string accountType;

public:
    string holderName;

    BankAccount(string holder, string accNum, string p) {
        holderName = holder;
        accountNumber = accNum;
        balance = 0.0;
        pin = p;
        accountType = "Savings";
    }

    // Public methods to access private data
    double getBalance() const {
        return balance;
    }

    bool verifyPin(string inputPin) const {
        return pin == inputPin;
    }

    void deposit(double amount, string inputPin) {
        if (!verifyPin(inputPin)) {
            cout << "Invalid PIN!" << endl;
            return;
        }
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: $" << amount << endl;
        }
    }

    void withdraw(double amount, string inputPin) {
        if (!verifyPin(inputPin)) {
            cout << "Invalid PIN!" << endl;
            return;
        }
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawn: $" << amount << endl;
        } else {
            cout << "Invalid amount or insufficient funds!" << endl;
        }
    }

    void displayInfo() const {
        cout << "Account Holder: " << holderName << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Balance: $" << balance << endl;
    }
};

class Employee {
private:
    int id;
    double salary;

public:
    string name;
    string department;

    Employee(int empId, string empName, string dept, double sal) {
        id = empId;
        name = empName;
        department = dept;
        salary = sal;
    }

    // Getter
    double getSalary() const {
        return salary;
    }

    // Setter with validation
    void setSalary(double newSalary) {
        if (newSalary > 0) {
            salary = newSalary;
            cout << "Salary updated to: $" << salary << endl;
        } else {
            cout << "Invalid salary amount!" << endl;
        }
    }

    void displayInfo() const {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Department: " << department << endl;
        cout << "Salary: $" << salary << endl;
    }
};

int main() {
    cout << "=== Access Specifiers Demo ===" << endl << endl;

    // BANK ACCOUNT
    cout << "=== BANK ACCOUNT ===" << endl;
    BankAccount account("John Doe", "ACC001", "1234");

    // Public member accessible
    cout << "Holder: " << account.holderName << endl;

    // Private members NOT accessible
    // cout << account.balance; // Error!
    // cout << account.pin; // Error!

    // Access through public methods
    account.deposit(1000, "1234");
    account.displayInfo();
    cout << endl;

    account.withdraw(200, "1234");
    account.displayInfo();
    cout << endl;

    account.withdraw(200, "wrong");  // Wrong PIN
    cout << endl;

    // EMPLOYEE
    cout << "=== EMPLOYEE ===" << endl;
    Employee emp(101, "Alice Smith", "IT", 75000);

    // Public members accessible
    cout << "Name: " << emp.name << endl;
    cout << "Department: " << emp.department << endl;

    // Private member accessed through getter
    cout << "Salary: $" << emp.getSalary() << endl;

    // Update through setter
    emp.setSalary(80000);
    cout << endl;

    emp.displayInfo();

    return 0;
}
