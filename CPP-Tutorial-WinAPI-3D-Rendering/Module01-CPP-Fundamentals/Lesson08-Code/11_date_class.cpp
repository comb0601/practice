/*
 * Program: Date Class
 * Description: Date class with validation and formatting
 * Compilation: g++ -std=c++17 11_date_class.cpp -o 11_date_class
 * Execution: ./11_date_class
 */

#include <iostream>
#include <string>
using namespace std;

class Date {
private:
    int day, month, year;

    bool isLeapYear(int y) const {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    int daysInMonth(int m, int y) const {
        int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (m == 2 && isLeapYear(y)) return 29;
        return days[m];
    }

    bool isValid() const {
        if (year < 1 || month < 1 || month > 12 || day < 1) return false;
        return day <= daysInMonth(month, year);
    }

public:
    Date(int d, int m, int y) : day(d), month(m), year(y) {
        if (!isValid()) {
            day = month = 1;
            year = 2000;
        }
    }

    void display() const {
        cout << day << "/" << month << "/" << year;
    }

    void displayLong() const {
        string months[] = {"", "January", "February", "March", "April", "May", "June",
                          "July", "August", "September", "October", "November", "December"};
        cout << months[month] << " " << day << ", " << year;
    }

    void nextDay() {
        day++;
        if (day > daysInMonth(month, year)) {
            day = 1;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }
};

int main() {
    cout << "=== Date Class ===" << endl << endl;

    Date date(25, 12, 2024);

    cout << "Short format: ";
    date.display();
    cout << endl;

    cout << "Long format: ";
    date.displayLong();
    cout << endl;

    cout << "\nNext day: ";
    date.nextDay();
    date.display();
    cout << endl;

    return 0;
}
