/*
 * Lesson 11: Operator Overloading
 * File: 05_increment_decrement_operators.cpp
 *
 * Demonstrates increment (++) and decrement (--) operator overloading
 * in both prefix and postfix forms.
 */

#include <iostream>

// Counter class demonstrating increment/decrement
class Counter {
private:
    int count;

public:
    // Constructor
    Counter(int c = 0) : count(c) {}

    // Getter
    int getCount() const { return count; }

    // Prefix increment: ++counter
    // Returns reference to allow chaining
    Counter& operator++() {
        ++count;
        std::cout << "Prefix ++: count is now " << count << "\n";
        return *this;
    }

    // Postfix increment: counter++
    // int parameter distinguishes from prefix (dummy parameter)
    // Returns copy of original value
    Counter operator++(int) {
        Counter temp(*this);  // Save current state
        ++count;              // Increment
        std::cout << "Postfix ++: count was " << temp.count
                  << ", now " << count << "\n";
        return temp;          // Return old value
    }

    // Prefix decrement: --counter
    Counter& operator--() {
        --count;
        std::cout << "Prefix --: count is now " << count << "\n";
        return *this;
    }

    // Postfix decrement: counter--
    Counter operator--(int) {
        Counter temp(*this);
        --count;
        std::cout << "Postfix --: count was " << temp.count
                  << ", now " << count << "\n";
        return temp;
    }

    // Output operator
    friend std::ostream& operator<<(std::ostream& os, const Counter& c) {
        os << "Counter(" << c.count << ")";
        return os;
    }
};

// Iterator class demonstrating pointer-like behavior
class Iterator {
private:
    int* ptr;

public:
    // Constructor
    Iterator(int* p = nullptr) : ptr(p) {}

    // Dereference operator
    int& operator*() {
        return *ptr;
    }

    // Prefix increment
    Iterator& operator++() {
        ++ptr;
        return *this;
    }

    // Postfix increment
    Iterator operator++(int) {
        Iterator temp(*this);
        ++ptr;
        return temp;
    }

    // Prefix decrement
    Iterator& operator--() {
        --ptr;
        return *this;
    }

    // Postfix decrement
    Iterator operator--(int) {
        Iterator temp(*this);
        --ptr;
        return temp;
    }

    // Comparison
    bool operator==(const Iterator& other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const Iterator& other) const {
        return ptr != other.ptr;
    }

    // Get pointer for display
    int* getPtr() const { return ptr; }
};

// Date class with increment/decrement
class Date {
private:
    int day, month, year;

    // Helper function to check if year is leap year
    bool isLeapYear(int y) const {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    // Get days in month
    int daysInMonth(int m, int y) const {
        const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (m == 2 && isLeapYear(y))
            return 29;
        return days[m];
    }

public:
    // Constructor
    Date(int d = 1, int m = 1, int y = 2000) : day(d), month(m), year(y) {}

    // Prefix increment (next day)
    Date& operator++() {
        ++day;
        if (day > daysInMonth(month, year)) {
            day = 1;
            ++month;
            if (month > 12) {
                month = 1;
                ++year;
            }
        }
        return *this;
    }

    // Postfix increment
    Date operator++(int) {
        Date temp(*this);
        ++(*this);
        return temp;
    }

    // Prefix decrement (previous day)
    Date& operator--() {
        --day;
        if (day < 1) {
            --month;
            if (month < 1) {
                month = 12;
                --year;
            }
            day = daysInMonth(month, year);
        }
        return *this;
    }

    // Postfix decrement
    Date operator--(int) {
        Date temp(*this);
        --(*this);
        return temp;
    }

    // Output operator
    friend std::ostream& operator<<(std::ostream& os, const Date& d) {
        os << d.day << "/" << d.month << "/" << d.year;
        return os;
    }
};

int main() {
    std::cout << "=== Increment/Decrement Operators Demo ===\n\n";

    // Counter class demo
    std::cout << "--- Counter Class ---\n";
    Counter c1(5);
    std::cout << "Initial: " << c1 << "\n\n";

    std::cout << "Prefix increment (++c1):\n";
    ++c1;
    std::cout << "After: " << c1 << "\n\n";

    std::cout << "Postfix increment (c1++):\n";
    Counter c2 = c1++;
    std::cout << "c1 is now: " << c1 << "\n";
    std::cout << "c2 (returned value): " << c2 << "\n\n";

    std::cout << "Prefix decrement (--c1):\n";
    --c1;
    std::cout << "After: " << c1 << "\n\n";

    std::cout << "Postfix decrement (c1--):\n";
    Counter c3 = c1--;
    std::cout << "c1 is now: " << c1 << "\n";
    std::cout << "c3 (returned value): " << c3 << "\n\n";

    // Chaining prefix operators
    std::cout << "Chaining prefix operators: ++++c1\n";
    ++++c1;
    std::cout << "After: " << c1 << "\n\n";

    // Demonstrating difference in return values
    std::cout << "--- Demonstrating Return Value Differences ---\n";
    Counter c4(10);
    std::cout << "Initial c4: " << c4 << "\n";
    std::cout << "Value of ++c4: " << ++c4 << "\n";
    std::cout << "c4 after ++c4: " << c4 << "\n\n";

    Counter c5(10);
    std::cout << "Initial c5: " << c5 << "\n";
    std::cout << "Value of c5++: " << c5++ << "\n";
    std::cout << "c5 after c5++: " << c5 << "\n\n";

    // Iterator class demo
    std::cout << "--- Iterator Class ---\n";
    int arr[] = {10, 20, 30, 40, 50};
    Iterator it(arr);

    std::cout << "Array: ";
    for (int i = 0; i < 5; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n\n";

    std::cout << "Current value: " << *it << "\n";
    ++it;
    std::cout << "After ++it: " << *it << "\n";
    ++it;
    std::cout << "After ++it: " << *it << "\n";
    --it;
    std::cout << "After --it: " << *it << "\n\n";

    // Using iterator in loop
    std::cout << "Iterating forward:\n";
    it = Iterator(arr);
    for (int i = 0; i < 5; i++) {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << "\n\n";

    // Date class demo
    std::cout << "--- Date Class ---\n";
    Date d1(28, 2, 2024);  // Leap year
    std::cout << "Initial date: " << d1 << "\n";
    ++d1;
    std::cout << "After ++d1 (leap year): " << d1 << "\n";
    ++d1;
    std::cout << "After ++d1 (month change): " << d1 << "\n\n";

    Date d2(31, 12, 2023);
    std::cout << "Year-end date: " << d2 << "\n";
    ++d2;
    std::cout << "After ++d2 (year change): " << d2 << "\n\n";

    Date d3(1, 1, 2024);
    std::cout << "Year-start date: " << d3 << "\n";
    --d3;
    std::cout << "After --d3 (year change): " << d3 << "\n\n";

    // Counting days
    std::cout << "Counting forward from 27/2/2024 (5 days):\n";
    Date d4(27, 2, 2024);
    for (int i = 0; i < 5; i++) {
        std::cout << d4 << "\n";
        ++d4;
    }
    std::cout << "\n";

    // Performance note
    std::cout << "--- Performance Note ---\n";
    std::cout << "Prefix operators (++x) are generally more efficient\n";
    std::cout << "because they don't need to create a temporary copy.\n";
    std::cout << "Postfix operators (x++) create a copy of the original value.\n";
    std::cout << "For complex objects, prefer prefix when the return value\n";
    std::cout << "is not needed.\n";

    return 0;
}

/*
 * EXPECTED OUTPUT:
 * =================================
 * === Increment/Decrement Operators Demo ===
 *
 * --- Counter Class ---
 * Initial: Counter(5)
 *
 * Prefix increment (++c1):
 * Prefix ++: count is now 6
 * After: Counter(6)
 *
 * Postfix increment (c1++):
 * Postfix ++: count was 6, now 7
 * c1 is now: Counter(7)
 * c2 (returned value): Counter(6)
 *
 * Prefix decrement (--c1):
 * Prefix --: count is now 6
 * After: Counter(6)
 *
 * Postfix decrement (c1--):
 * Postfix --: count was 6, now 5
 * c1 is now: Counter(5)
 * c3 (returned value): Counter(6)
 *
 * Chaining prefix operators: ++++c1
 * Prefix ++: count is now 6
 * Prefix ++: count is now 7
 * After: Counter(7)
 *
 * --- Demonstrating Return Value Differences ---
 * Initial c4: Counter(10)
 * Prefix ++: count is now 11
 * Value of ++c4: Counter(11)
 * c4 after ++c4: Counter(11)
 *
 * Initial c5: Counter(10)
 * Postfix ++: count was 10, now 11
 * Value of c5++: Counter(10)
 * c5 after c5++: Counter(11)
 *
 * --- Iterator Class ---
 * Array: 10 20 30 40 50
 *
 * Current value: 10
 * After ++it: 20
 * After ++it: 30
 * After --it: 20
 *
 * Iterating forward:
 * 10 20 30 40 50
 *
 * --- Date Class ---
 * Initial date: 28/2/2024
 * After ++d1 (leap year): 29/2/2024
 * After ++d1 (month change): 1/3/2024
 *
 * Year-end date: 31/12/2023
 * After ++d2 (year change): 1/1/2024
 *
 * Year-start date: 1/1/2024
 * After --d3 (year change): 31/12/2023
 *
 * Counting forward from 27/2/2024 (5 days):
 * 27/2/2024
 * 28/2/2024
 * 29/2/2024
 * 1/3/2024
 * 2/3/2024
 *
 * --- Performance Note ---
 * Prefix operators (++x) are generally more efficient
 * because they don't need to create a temporary copy.
 * Postfix operators (x++) create a copy of the original value.
 * For complex objects, prefer prefix when the return value
 * is not needed.
 */
