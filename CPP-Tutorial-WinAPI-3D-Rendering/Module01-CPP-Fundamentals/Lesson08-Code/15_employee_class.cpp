/*
 * Program: Employee Class
 * Description: Employee management with salary calculations
 * Compilation: g++ -std=c++17 15_employee_class.cpp -o 15_employee_class
 * Execution: ./15_employee_class
 */

#include <iostream>
#include <string>
using namespace std;

class Employee {
private:
    string name;
    int id;
    string department;
    double salary;
    double hoursWorked;
    static double taxRate;

public:
    Employee(const string& n, int empId, const string& dept, double sal)
        : name(n), id(empId), department(dept), salary(sal), hoursWorked(0) {}

    void setName(const string& n) { name = n; }
    void setDepartment(const string& dept) { department = dept; }
    void setSalary(double sal) { if (sal > 0) salary = sal; }

    string getName() const { return name; }
    int getId() const { return id; }
    string getDepartment() const { return department; }
    double getSalary() const { return salary; }

    void addHours(double hours) {
        if (hours > 0) {
            hoursWorked += hours;
        }
    }

    double calculateNetPay() const {
        return salary * (1 - taxRate);
    }

    double calculateMonthlyPay() const {
        return salary / 12.0;
    }

    void displayInfo() const {
        cout << "\n===== Employee Information =====" << endl;
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
        cout << "Department: " << department << endl;
        cout << "Salary: $" << salary << endl;
        cout << "Monthly Pay: $" << calculateMonthlyPay() << endl;
        cout << "Net Annual Pay: $" << calculateNetPay() << endl;
        cout << "Hours Worked: " << hoursWorked << endl;
    }

    void givereRaise(double percentage) {
        if (percentage > 0) {
            double increase = salary * (percentage / 100.0);
            salary += increase;
            cout << "Salary increased by " << percentage << "%" << endl;
            cout << "New salary: $" << salary << endl;
        }
    }
};

double Employee::taxRate = 0.20;  // 20% tax

int main() {
    cout << "=== Employee Class ===" << endl << endl;

    Employee emp("John Smith", 1001, "Engineering", 75000);

    emp.displayInfo();

    cout << "\nAdding work hours..." << endl;
    emp.addHours(40);
    emp.addHours(42);

    emp.displayInfo();

    cout << "\nGiving 10% raise..." << endl;
    emp.giveRaise(10);

    emp.displayInfo();

    return 0;
}
