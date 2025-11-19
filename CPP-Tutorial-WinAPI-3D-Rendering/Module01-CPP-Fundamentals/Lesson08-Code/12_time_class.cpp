/*
 * Program: Time Class
 * Description: Time class with hours, minutes, seconds
 * Compilation: g++ -std=c++17 12_time_class.cpp -o 12_time_class
 * Execution: ./12_time_class
 */

#include <iostream>
#include <iomanip>
using namespace std;

class Time {
private:
    int hours, minutes, seconds;

    void normalize() {
        if (seconds >= 60) {
            minutes += seconds / 60;
            seconds %= 60;
        }
        if (minutes >= 60) {
            hours += minutes / 60;
            minutes %= 60;
        }
        if (hours >= 24) {
            hours %= 24;
        }
    }

public:
    Time(int h = 0, int m = 0, int s = 0)
        : hours(h), minutes(m), seconds(s) {
        normalize();
    }

    void setTime(int h, int m, int s) {
        hours = h;
        minutes = m;
        seconds = s;
        normalize();
    }

    void addSeconds(int s) {
        seconds += s;
        normalize();
    }

    void addMinutes(int m) {
        minutes += m;
        normalize();
    }

    void addHours(int h) {
        hours += h;
        normalize();
    }

    void display24() const {
        cout << setfill('0')
             << setw(2) << hours << ":"
             << setw(2) << minutes << ":"
             << setw(2) << seconds;
    }

    void display12() const {
        int h = hours % 12;
        if (h == 0) h = 12;
        cout << setfill('0')
             << setw(2) << h << ":"
             << setw(2) << minutes << ":"
             << setw(2) << seconds
             << (hours < 12 ? " AM" : " PM");
    }
};

int main() {
    cout << "=== Time Class ===" << endl << endl;

    Time t(14, 30, 45);

    cout << "24-hour format: ";
    t.display24();
    cout << endl;

    cout << "12-hour format: ";
    t.display12();
    cout << endl;

    t.addSeconds(75);
    cout << "\nAfter adding 75 seconds:" << endl;
    cout << "24-hour: ";
    t.display24();
    cout << endl;

    return 0;
}
