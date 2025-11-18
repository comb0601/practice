/*
 * Program: Temperature Converter
 * Description: Converts between Celsius, Fahrenheit, and Kelvin
 * Compilation: g++ 04_temperature_converter.cpp -o 04_temperature_converter
 * Execution: ./04_temperature_converter
 */

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double temperature;
    int choice;

    cout << "=== Temperature Converter ===" << endl << endl;

    cout << "Select input temperature scale:" << endl;
    cout << "1. Celsius" << endl;
    cout << "2. Fahrenheit" << endl;
    cout << "3. Kelvin" << endl;
    cout << "Enter choice (1-3): ";
    cin >> choice;

    cout << "Enter temperature value: ";
    cin >> temperature;

    cout << fixed << setprecision(2);
    cout << "\n=== Conversion Results ===" << endl;

    if (choice == 1) {
        // From Celsius
        double fahrenheit = (temperature * 9.0 / 5.0) + 32.0;
        double kelvin = temperature + 273.15;

        cout << temperature << "°C = " << fahrenheit << "°F" << endl;
        cout << temperature << "°C = " << kelvin << " K" << endl;
    }
    else if (choice == 2) {
        // From Fahrenheit
        double celsius = (temperature - 32.0) * 5.0 / 9.0;
        double kelvin = celsius + 273.15;

        cout << temperature << "°F = " << celsius << "°C" << endl;
        cout << temperature << "°F = " << kelvin << " K" << endl;
    }
    else if (choice == 3) {
        // From Kelvin
        double celsius = temperature - 273.15;
        double fahrenheit = (celsius * 9.0 / 5.0) + 32.0;

        cout << temperature << " K = " << celsius << "°C" << endl;
        cout << temperature << " K = " << fahrenheit << "°F" << endl;
    }
    else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
