/*
 * Program: Time Converter
 * Description: Converts time between seconds, minutes, hours, and days
 * Compilation: g++ 12_time_converter.cpp -o 12_time_converter
 * Execution: ./12_time_converter
 */

#include <iostream>
using namespace std;

int main() {
    int totalSeconds;

    cout << "=== Time Converter ===" << endl << endl;

    cout << "Enter time in seconds: ";
    cin >> totalSeconds;

    if (totalSeconds < 0) {
        cout << "Error: Time cannot be negative!" << endl;
        return 1;
    }

    // Calculate components
    int days = totalSeconds / 86400;  // 86400 seconds in a day
    int remainingSeconds = totalSeconds % 86400;

    int hours = remainingSeconds / 3600;  // 3600 seconds in an hour
    remainingSeconds = remainingSeconds % 3600;

    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;

    // Display results
    cout << "\n=== Conversion Results ===" << endl;
    cout << totalSeconds << " seconds is equal to:" << endl << endl;

    if (days > 0) {
        cout << "  " << days << " day";
        if (days > 1) cout << "s";
        cout << endl;
    }

    if (hours > 0) {
        cout << "  " << hours << " hour";
        if (hours > 1) cout << "s";
        cout << endl;
    }

    if (minutes > 0) {
        cout << "  " << minutes << " minute";
        if (minutes > 1) cout << "s";
        cout << endl;
    }

    if (seconds > 0) {
        cout << "  " << seconds << " second";
        if (seconds > 1) cout << "s";
        cout << endl;
    }

    cout << "\nFormatted: " << days << " days, "
         << hours << " hours, "
         << minutes << " minutes, "
         << seconds << " seconds" << endl;

    // Additional conversions
    cout << "\n=== Other Units ===" << endl;
    double totalMinutes = totalSeconds / 60.0;
    double totalHours = totalSeconds / 3600.0;
    double totalDays = totalSeconds / 86400.0;

    cout << "Total minutes: " << totalMinutes << endl;
    cout << "Total hours: " << totalHours << endl;
    cout << "Total days: " << totalDays << endl;

    return 0;
}
