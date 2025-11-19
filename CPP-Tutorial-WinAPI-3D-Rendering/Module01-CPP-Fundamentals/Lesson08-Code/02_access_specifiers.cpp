/*
 * Program: Access Specifiers
 * Description: Public, private, and protected access control
 * Compilation: g++ -std=c++17 02_access_specifiers.cpp -o 02_access_specifiers
 * Execution: ./02_access_specifiers
 */

#include <iostream>
using namespace std;

class BankAccount {
private:  // Private members - only accessible within class
    double balance;
    string accountNumber;

public:  // Public members - accessible from anywhere
    string ownerName;

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited $" << amount << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrew $" << amount << endl;
        } else {
            cout << "Insufficient funds!" << endl;
        }
    }

    double getBalance() {
        return balance;
    }

    void setAccountNumber(string accNum) {
        accountNumber = accNum;
    }

    string getAccountNumber() {
        return accountNumber;
    }

    void displayInfo() {
        cout << "Account: " << accountNumber << endl;
        cout << "Owner: " << ownerName << endl;
        cout << "Balance: $" << balance << endl;
    }
};

class Student {
private:
    int id;
    double gpa;

public:
    string name;

    // Getters
    int getId() { return id; }
    double getGpa() { return gpa; }

    // Setters
    void setId(int newId) { id = newId; }

    void setGpa(double newGpa) {
        if (newGpa >= 0.0 && newGpa <= 4.0) {
            gpa = newGpa;
        } else {
            cout << "Invalid GPA!" << endl;
        }
    }
};

int main() {
    cout << "=== Access Specifiers ===" << endl << endl;

    // BankAccount
    cout << "1. Bank Account:" << endl;
    BankAccount account;

    // account.balance = 1000000;  // ERROR - balance is private!

    account.ownerName = "John Doe";  // OK - public
    account.setAccountNumber("123-456-789");
    account.deposit(1000);
    account.deposit(500);
    account.withdraw(300);

    account.displayInfo();
    cout << endl;

    // Student
    cout << "2. Student:" << endl;
    Student student;

    student.name = "Alice";  // OK - public
    student.setId(12345);
    student.setGpa(3.8);

    cout << "Name: " << student.name << endl;
    cout << "ID: " << student.getId() << endl;
    cout << "GPA: " << student.getGpa() << endl;
    cout << endl;

    // Validation
    cout << "3. Validation:" << endl;
    student.setGpa(5.0);  // Invalid!
    cout << "GPA after invalid set: " << student.getGpa() << endl;

    student.setGpa(3.9);  // Valid
    cout << "GPA after valid set: " << student.getGpa() << endl;

    return 0;
}
