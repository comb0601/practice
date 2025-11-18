#include <iostream>
#include <iomanip>  // For std::fixed and std::setprecision

/**
 * Solution 04: Shopping Total
 *
 * Calculates shopping total with tax from three item prices.
 */

int main() {
    double item1, item2, item3;
    double subtotal, tax, total;

    std::cout << "Enter price of item 1: $";
    std::cin >> item1;

    std::cout << "Enter price of item 2: $";
    std::cin >> item2;

    std::cout << "Enter price of item 3: $";
    std::cin >> item3;

    subtotal = item1 + item2 + item3;
    tax = subtotal * 0.10;  // 10% tax
    total = subtotal + tax;

    // Format output to 2 decimal places
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "\n=== Receipt ===" << std::endl;
    std::cout << "Subtotal:  $" << subtotal << std::endl;
    std::cout << "Tax (10%): $" << tax << std::endl;
    std::cout << "Total:     $" << total << std::endl;

    return 0;
}

/*
 * EXAMPLE OUTPUT:
 *
 * Enter price of item 1: $10.50
 * Enter price of item 2: $25.00
 * Enter price of item 3: $5.75
 *
 * === Receipt ===
 * Subtotal:  $41.25
 * Tax (10%): $4.13
 * Total:     $45.38
 *
 * KEY POINTS:
 * - Using double for monetary values
 * - std::fixed and std::setprecision(2) formats to 2 decimal places
 * - Tax is calculated as a percentage (0.10 = 10%)
 * - Output is formatted like a receipt
 *
 * ENHANCEMENT IDEAS:
 * - Support variable number of items
 * - Allow different tax rates
 * - Add item names and quantities
 * - Calculate change if given amount paid
 * - Support discounts or coupons
 */
