#include <iostream>
#include <string>
using namespace std;
class BankAccount {
    string owner;
    double balance;
    mutable int transactionCount;
public:
    BankAccount(const string& o, double b)
        : owner(o), balance(b), transactionCount(0) {}
    const string& getOwner() const { return owner; }
    double getBalance() const {
        transactionCount++;  // OK: mutable
        return balance;
    }
    void deposit(double amount) {
        balance += amount;
        transactionCount++;
    }
    void withdraw(double amount) {
        if(amount <= balance) {
            balance -= amount;
            transactionCount++;
        }
    }
    void displayInfo() const {
        cout << "Owner: " << owner << endl;
        cout << "Balance: $" << balance << endl;
        cout << "Transactions: " << transactionCount << endl;
    }
};
void printAccountInfo(const BankAccount& account) {
    account.displayInfo();  // OK: const function
    // account.deposit(100);  // Error: non-const function
}
int main() {
    BankAccount account("Alice", 1000.0);
    account.deposit(500);
    account.withdraw(200);
    printAccountInfo(account);
    return 0;
}
