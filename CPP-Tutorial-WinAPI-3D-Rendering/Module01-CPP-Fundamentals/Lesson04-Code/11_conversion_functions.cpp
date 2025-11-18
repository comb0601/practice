/*
 * Program: Conversion Functions
 * Description: Various conversion and formatting functions
 * Compilation: g++ 11_conversion_functions.cpp -o 11_conversion_functions
 * Execution: ./11_conversion_functions
 */

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// Celsius to Fahrenheit
double celsiusToFahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

// Fahrenheit to Celsius
double fahrenheitToCelsius(double fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

// Kilometers to Miles
double kmToMiles(double km) {
    return km * 0.621371;
}

// Miles to Kilometers
double milesToKm(double miles) {
    return miles / 0.621371;
}

// Decimal to Binary (string)
string decimalToBinary(int decimal) {
    if (decimal == 0) return "0";

    string binary = "";
    while (decimal > 0) {
        binary = to_string(decimal % 2) + binary;
        decimal /= 2;
    }
    return binary;
}

// Binary to Decimal (from string)
int binaryToDecimal(string binary) {
    int decimal = 0;
    int base = 1;

    for (int i = binary.length() - 1; i >= 0; i--) {
        if (binary[i] == '1') {
            decimal += base;
        }
        base *= 2;
    }
    return decimal;
}

// Decimal to Hexadecimal
string decimalToHex(int decimal) {
    if (decimal == 0) return "0";

    string hex = "";
    char hexChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    while (decimal > 0) {
        hex = hexChars[decimal % 16] + hex;
        decimal /= 16;
    }
    return hex;
}

// Integer to String
string intToString(int number) {
    stringstream ss;
    ss << number;
    return ss.str();
}

// String to Integer
int stringToInt(string str) {
    stringstream ss(str);
    int number;
    ss >> number;
    return number;
}

// Seconds to Time Format (HH:MM:SS)
string secondsToTime(int totalSeconds) {
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    string result = "";
    if (hours < 10) result += "0";
    result += to_string(hours) + ":";
    if (minutes < 10) result += "0";
    result += to_string(minutes) + ":";
    if (seconds < 10) result += "0";
    result += to_string(seconds);

    return result;
}

int main() {
    cout << "=== Conversion Functions ===" << endl << endl;

    // Temperature conversion
    cout << "1. Temperature Conversion:" << endl;
    cout << "25°C = " << celsiusToFahrenheit(25) << "°F" << endl;
    cout << "77°F = " << fahrenheitToCelsius(77) << "°C" << endl << endl;

    // Distance conversion
    cout << "2. Distance Conversion:" << endl;
    cout << "10 km = " << kmToMiles(10) << " miles" << endl;
    cout << "5 miles = " << milesToKm(5) << " km" << endl << endl;

    // Number system conversion
    cout << "3. Decimal to Binary:" << endl;
    cout << "42 = " << decimalToBinary(42) << endl;
    cout << "255 = " << decimalToBinary(255) << endl << endl;

    cout << "4. Binary to Decimal:" << endl;
    cout << "1010 = " << binaryToDecimal("1010") << endl;
    cout << "11111111 = " << binaryToDecimal("11111111") << endl << endl;

    cout << "5. Decimal to Hexadecimal:" << endl;
    cout << "255 = 0x" << decimalToHex(255) << endl;
    cout << "4096 = 0x" << decimalToHex(4096) << endl << endl;

    // String conversion
    cout << "6. Integer to String:" << endl;
    string str = intToString(12345);
    cout << "12345 as string: \"" << str << "\"" << endl;
    cout << "String length: " << str.length() << endl << endl;

    cout << "7. String to Integer:" << endl;
    int num = stringToInt("9876");
    cout << "\"9876\" as integer: " << num << endl;
    cout << "Doubled: " << (num * 2) << endl << endl;

    // Time conversion
    cout << "8. Seconds to Time Format:" << endl;
    cout << "3665 seconds = " << secondsToTime(3665) << endl;
    cout << "7384 seconds = " << secondsToTime(7384) << endl;

    return 0;
}
