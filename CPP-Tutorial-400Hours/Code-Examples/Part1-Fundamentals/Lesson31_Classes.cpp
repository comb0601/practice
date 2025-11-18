/*
 * Lesson 31: Classes - Object-Oriented Programming Basics
 * Compile: cl Lesson31_Classes.cpp
 */

#include <iostream>
#include <string>
using namespace std;

class Rectangle {
public:
    double width;
    double height;

    double area() {
        return width * height;
    }

    double perimeter() {
        return 2 * (width + height);
    }

    void display() {
        cout << "Rectangle: " << width << " x " << height << endl;
        cout << "Area: " << area() << endl;
        cout << "Perimeter: " << perimeter() << endl;
    }
};

class Student {
public:
    string name;
    int age;
    double gpa;

    void displayInfo() {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "GPA: " << gpa << endl;
    }
};

class BankAccount {
public:
    string accountNumber;
    string holderName;
    double balance;

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: $" << amount << endl;
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            cout << "Withdrawn: $" << amount << endl;
        } else {
            cout << "Insufficient funds!" << endl;
        }
    }

    void displayBalance() {
        cout << "Balance: $" << balance << endl;
    }
};

int main() {
    cout << "=== Classes Demo ===" << endl << endl;

    // RECTANGLE CLASS
    cout << "=== RECTANGLE ===" << endl;
    Rectangle rect;
    rect.width = 5.0;
    rect.height = 3.0;
    rect.display();
    cout << endl;

    // STUDENT CLASS
    cout << "=== STUDENT ===" << endl;
    Student student;
    student.name = "Alice";
    student.age = 20;
    student.gpa = 3.8;
    student.displayInfo();
    cout << endl;

    // BANK ACCOUNT CLASS
    cout << "=== BANK ACCOUNT ===" << endl;
    BankAccount account;
    account.accountNumber = "12345";
    account.holderName = "John Doe";
    account.balance = 1000.0;

    account.displayBalance();
    account.deposit(500);
    account.displayBalance();
    account.withdraw(200);
    account.displayBalance();

    return 0;
}
