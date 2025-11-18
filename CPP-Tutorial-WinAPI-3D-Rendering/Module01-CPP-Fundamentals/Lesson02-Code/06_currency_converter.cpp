/*
 * Program: Currency Converter
 * Description: Converts between USD, EUR, GBP, and JPY
 * Compilation: g++ 06_currency_converter.cpp -o 06_currency_converter
 * Execution: ./06_currency_converter
 */

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    // Exchange rates (as of sample date - USD base)
    const double USD_TO_EUR = 0.92;
    const double USD_TO_GBP = 0.79;
    const double USD_TO_JPY = 149.50;

    double amount;
    int fromCurrency, toCurrency;

    cout << "=== Currency Converter ===" << endl << endl;

    cout << "Select source currency:" << endl;
    cout << "1. USD (US Dollar)" << endl;
    cout << "2. EUR (Euro)" << endl;
    cout << "3. GBP (British Pound)" << endl;
    cout << "4. JPY (Japanese Yen)" << endl;
    cout << "Enter choice (1-4): ";
    cin >> fromCurrency;

    cout << "Enter amount: ";
    cin >> amount;

    cout << "\nSelect target currency:" << endl;
    cout << "1. USD (US Dollar)" << endl;
    cout << "2. EUR (Euro)" << endl;
    cout << "3. GBP (British Pound)" << endl;
    cout << "4. JPY (Japanese Yen)" << endl;
    cout << "Enter choice (1-4): ";
    cin >> toCurrency;

    // First convert to USD
    double amountInUSD = amount;
    if (fromCurrency == 2) amountInUSD = amount / USD_TO_EUR;
    else if (fromCurrency == 3) amountInUSD = amount / USD_TO_GBP;
    else if (fromCurrency == 4) amountInUSD = amount / USD_TO_JPY;

    // Then convert to target currency
    double result = amountInUSD;
    if (toCurrency == 2) result = amountInUSD * USD_TO_EUR;
    else if (toCurrency == 3) result = amountInUSD * USD_TO_GBP;
    else if (toCurrency == 4) result = amountInUSD * USD_TO_JPY;

    cout << fixed << setprecision(2);
    cout << "\n=== Conversion Result ===" << endl;

    string currencies[] = {"", "USD", "EUR", "GBP", "JPY"};
    cout << amount << " " << currencies[fromCurrency]
         << " = " << result << " " << currencies[toCurrency] << endl;

    cout << "\nNote: Exchange rates are sample values." << endl;

    return 0;
}
