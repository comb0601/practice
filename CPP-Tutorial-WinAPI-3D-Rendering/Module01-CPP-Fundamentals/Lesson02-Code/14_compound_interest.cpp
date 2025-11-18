/*
 * Program: Compound Interest Calculator
 * Description: Calculates compound interest for investments
 * Compilation: g++ 14_compound_interest.cpp -o 14_compound_interest
 * Execution: ./14_compound_interest
 */

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    double principal, rate, time;
    int compoundFrequency;

    cout << "=== Compound Interest Calculator ===" << endl << endl;

    // Input
    cout << "Enter principal amount ($): ";
    cin >> principal;

    cout << "Enter annual interest rate (%): ";
    cin >> rate;

    cout << "Enter time period (years): ";
    cin >> time;

    cout << "\nSelect compounding frequency:" << endl;
    cout << "1. Annually (1 time per year)" << endl;
    cout << "2. Semi-annually (2 times per year)" << endl;
    cout << "3. Quarterly (4 times per year)" << endl;
    cout << "4. Monthly (12 times per year)" << endl;
    cout << "5. Daily (365 times per year)" << endl;
    cout << "Enter choice (1-5): ";
    cin >> compoundFrequency;

    // Set n (number of times interest is compounded per year)
    int n;
    string frequencyName;
    switch (compoundFrequency) {
        case 1: n = 1; frequencyName = "Annually"; break;
        case 2: n = 2; frequencyName = "Semi-annually"; break;
        case 3: n = 4; frequencyName = "Quarterly"; break;
        case 4: n = 12; frequencyName = "Monthly"; break;
        case 5: n = 365; frequencyName = "Daily"; break;
        default:
            cout << "Invalid choice!" << endl;
            return 1;
    }

    // Calculate compound interest
    // Formula: A = P(1 + r/n)^(nt)
    double rateDecimal = rate / 100.0;
    double amount = principal * pow((1 + rateDecimal / n), n * time);
    double interest = amount - principal;

    // Display results
    cout << fixed << setprecision(2);
    cout << "\n=== Calculation Results ===" << endl;
    cout << "Principal: $" << principal << endl;
    cout << "Interest Rate: " << rate << "%" << endl;
    cout << "Time Period: " << time << " years" << endl;
    cout << "Compounding: " << frequencyName << " (" << n << " times/year)" << endl;
    cout << "\nFinal Amount: $" << amount << endl;
    cout << "Total Interest: $" << interest << endl;
    cout << "Total Return: " << ((interest / principal) * 100) << "%" << endl;

    // Compare with simple interest
    double simpleInterest = principal * (rateDecimal * time);
    double simpleAmount = principal + simpleInterest;

    cout << "\n=== Comparison with Simple Interest ===" << endl;
    cout << "Simple Interest Amount: $" << simpleAmount << endl;
    cout << "Compound Interest Advantage: $" << (amount - simpleAmount) << endl;

    return 0;
}
