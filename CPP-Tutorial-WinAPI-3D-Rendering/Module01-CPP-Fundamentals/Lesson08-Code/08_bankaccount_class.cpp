/*
 * Program: BankAccount Class
 * Description: Bank account with deposits, withdrawals, and transactions
 * Compilation: g++ -std=c++17 08_bankaccount_class.cpp -o 08_bankaccount_class
 * Execution: ./08_bankaccount_class
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BankAccount {
private:
    string accountNumber;
    string ownerName;
    double balance;
    vector<string> transactions;

    void logTransaction(const string& transaction) {
        transactions.push_back(transaction);
    }

public:
    BankAccount(const string& accNum, const string& owner)
        : accountNumber(accNum), ownerName(owner), balance(0.0) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            logTransaction("Deposit: +" + to_string(amount));
            cout << "Deposited $" << amount << endl;
        }
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            logTransaction("Withdrawal: -" + to_string(amount));
            cout << "Withdrew $" << amount << endl;
            return true;
        }
        cout << "Insufficient funds!" << endl;
        return false;
    }

    bool transfer(BankAccount& toAccount, double amount) {
        if (withdraw(amount)) {
            toAccount.deposit(amount);
            logTransaction("Transfer to " + toAccount.accountNumber + ": -" + to_string(amount));
            return true;
        }
        return false;
    }

    double getBalance() const { return balance; }

    void displayInfo() const {
        cout << "\n===== Account Information =====" << endl;
        cout << "Account: " << accountNumber << endl;
        cout << "Owner: " << ownerName << endl;
        cout << "Balance: $" << balance << endl;
    }

    void displayTransactions() const {
        cout << "\nRecent Transactions:" << endl;
        for (const auto& t : transactions) {
            cout << "  " << t << endl;
        }
    }
};

int main() {
    cout << "=== BankAccount Class ===" << endl << endl;

    BankAccount account1("123-456", "John Doe");
    BankAccount account2("789-012", "Jane Smith");

    account1.deposit(1000);
    account1.deposit(500);
    account1.withdraw(200);

    account1.displayInfo();
    account1.displayTransactions();

    cout << "\nTransfer $300 from account1 to account2:" << endl;
    account1.transfer(account2, 300);

    account1.displayInfo();
    account2.displayInfo();

    return 0;
}
