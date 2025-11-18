/*
 * Lesson 11: Operator Overloading
 * File: 04_stream_operators.cpp
 *
 * Demonstrates stream operator overloading (<< and >>)
 * for input and output of custom objects.
 */

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

// Student class with stream operators
class Student {
private:
    std::string name;
    int id;
    double gpa;

public:
    // Constructor
    Student(const std::string& n = "", int i = 0, double g = 0.0)
        : name(n), id(i), gpa(g) {}

    // Getters
    std::string getName() const { return name; }
    int getId() const { return id; }
    double getGpa() const { return gpa; }

    // Output stream operator (friend function)
    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        os << "Student{name: \"" << s.name << "\", id: " << s.id
           << ", GPA: " << std::fixed << std::setprecision(2) << s.gpa << "}";
        return os;
    }

    // Input stream operator (friend function)
    friend std::istream& operator>>(std::istream& is, Student& s) {
        std::cout << "Enter student name: ";
        std::getline(is >> std::ws, s.name);  // std::ws to skip leading whitespace
        std::cout << "Enter student ID: ";
        is >> s.id;
        std::cout << "Enter GPA: ";
        is >> s.gpa;
        return is;
    }
};

// Book class with formatted output
class Book {
private:
    std::string title;
    std::string author;
    int year;
    double price;

public:
    // Constructor
    Book(const std::string& t = "", const std::string& a = "", int y = 0, double p = 0.0)
        : title(t), author(a), year(y), price(p) {}

    // Formatted output operator
    friend std::ostream& operator<<(std::ostream& os, const Book& b) {
        os << std::left << std::setw(30) << b.title
           << std::setw(20) << b.author
           << std::setw(6) << b.year
           << "$" << std::fixed << std::setprecision(2) << b.price;
        return os;
    }

    // Simple input operator
    friend std::istream& operator>>(std::istream& is, Book& b) {
        std::cout << "Enter book title: ";
        std::getline(is >> std::ws, b.title);
        std::cout << "Enter author: ";
        std::getline(is >> std::ws, b.author);
        std::cout << "Enter year: ";
        is >> b.year;
        std::cout << "Enter price: ";
        is >> b.price;
        return is;
    }

    // Static method to print header
    static void printHeader() {
        std::cout << std::left << std::setw(30) << "Title"
                  << std::setw(20) << "Author"
                  << std::setw(6) << "Year"
                  << "Price\n";
        std::cout << std::string(60, '-') << "\n";
    }
};

// Date class with multiple output formats
class Date {
private:
    int day, month, year;

public:
    // Constructor
    Date(int d = 1, int m = 1, int y = 2000) : day(d), month(m), year(y) {}

    // Default output format: DD/MM/YYYY
    friend std::ostream& operator<<(std::ostream& os, const Date& d) {
        os << std::setfill('0') << std::setw(2) << d.day << "/"
           << std::setw(2) << d.month << "/"
           << std::setw(4) << d.year << std::setfill(' ');
        return os;
    }

    // Input operator
    friend std::istream& operator>>(std::istream& is, Date& d) {
        char separator;
        is >> d.day >> separator >> d.month >> separator >> d.year;
        return is;
    }

    // Alternative format: YYYY-MM-DD
    std::string toISO() const {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(4) << year << "-"
            << std::setw(2) << month << "-"
            << std::setw(2) << day;
        return oss.str();
    }

    // Alternative format: Month DD, YYYY
    std::string toLongFormat() const {
        const char* months[] = {"", "January", "February", "March", "April", "May",
                                "June", "July", "August", "September", "October",
                                "November", "December"};
        std::ostringstream oss;
        oss << months[month] << " " << day << ", " << year;
        return oss.str();
    }
};

int main() {
    std::cout << "=== Stream Operators Demo ===\n\n";

    // Student class demo
    std::cout << "--- Student Class ---\n";
    Student s1("Alice Johnson", 12345, 3.85);
    Student s2("Bob Smith", 67890, 3.92);

    std::cout << "Student 1: " << s1 << "\n";
    std::cout << "Student 2: " << s2 << "\n\n";

    // Chaining output
    std::cout << "Chained output: " << s1 << " and " << s2 << "\n\n";

    // Interactive input (commented out for automated testing)
    /*
    std::cout << "Enter student information:\n";
    Student s3;
    std::cin >> s3;
    std::cout << "You entered: " << s3 << "\n\n";
    */

    // Book class demo with formatted output
    std::cout << "--- Book Class (Formatted Output) ---\n";
    Book::printHeader();
    Book b1("The C++ Programming Language", "Bjarne Stroustrup", 2013, 69.99);
    Book b2("Effective Modern C++", "Scott Meyers", 2014, 44.99);
    Book b3("Design Patterns", "Gang of Four", 1994, 54.99);

    std::cout << b1 << "\n";
    std::cout << b2 << "\n";
    std::cout << b3 << "\n\n";

    // Date class demo
    std::cout << "--- Date Class (Multiple Formats) ---\n";
    Date d1(15, 3, 2024);
    Date d2(4, 7, 1776);
    Date d3(25, 12, 2023);

    std::cout << "Default format (DD/MM/YYYY):\n";
    std::cout << "  Date 1: " << d1 << "\n";
    std::cout << "  Date 2: " << d2 << "\n";
    std::cout << "  Date 3: " << d3 << "\n\n";

    std::cout << "ISO format (YYYY-MM-DD):\n";
    std::cout << "  Date 1: " << d1.toISO() << "\n";
    std::cout << "  Date 2: " << d2.toISO() << "\n";
    std::cout << "  Date 3: " << d3.toISO() << "\n\n";

    std::cout << "Long format (Month DD, YYYY):\n";
    std::cout << "  Date 1: " << d1.toLongFormat() << "\n";
    std::cout << "  Date 2: " << d2.toLongFormat() << "\n";
    std::cout << "  Date 3: " << d3.toLongFormat() << "\n\n";

    // String stream demonstration
    std::cout << "--- String Stream Usage ---\n";
    std::ostringstream oss;
    oss << "Students: " << s1 << " and " << s2;
    std::string result = oss.str();
    std::cout << result << "\n\n";

    // Parsing from string stream
    std::istringstream dateStream("25/12/2023");
    Date parsedDate;
    dateStream >> parsedDate;
    std::cout << "Parsed date: " << parsedDate << "\n";
    std::cout << "Long format: " << parsedDate.toLongFormat() << "\n\n";

    // Demonstrating stream manipulators
    std::cout << "--- Stream Manipulators with Custom Objects ---\n";
    std::cout << std::left << "Left-aligned: " << s1 << "\n";
    std::cout << std::right << "Right-aligned: " << s1 << "\n";

    return 0;
}

/*
 * EXPECTED OUTPUT:
 * =================================
 * === Stream Operators Demo ===
 *
 * --- Student Class ---
 * Student 1: Student{name: "Alice Johnson", id: 12345, GPA: 3.85}
 * Student 2: Student{name: "Bob Smith", id: 67890, GPA: 3.92}
 *
 * Chained output: Student{name: "Alice Johnson", id: 12345, GPA: 3.85} and Student{name: "Bob Smith", id: 67890, GPA: 3.92}
 *
 * --- Book Class (Formatted Output) ---
 * Title                         Author              Year  Price
 * ------------------------------------------------------------
 * The C++ Programming Language  Bjarne Stroustrup   2013  $69.99
 * Effective Modern C++          Scott Meyers        2014  $44.99
 * Design Patterns               Gang of Four        1994  $54.99
 *
 * --- Date Class (Multiple Formats) ---
 * Default format (DD/MM/YYYY):
 *   Date 1: 15/03/2024
 *   Date 2: 04/07/1776
 *   Date 3: 25/12/2023
 *
 * ISO format (YYYY-MM-DD):
 *   Date 1: 2024-03-15
 *   Date 2: 1776-07-04
 *   Date 3: 2023-12-25
 *
 * Long format (Month DD, YYYY):
 *   Date 1: March 15, 2024
 *   Date 2: July 4, 1776
 *   Date 3: December 25, 2023
 *
 * --- String Stream Usage ---
 * Students: Student{name: "Alice Johnson", id: 12345, GPA: 3.85} and Student{name: "Bob Smith", id: 67890, GPA: 3.92}
 *
 * Parsed date: 25/12/2023
 * Long format: December 25, 2023
 *
 * --- Stream Manipulators with Custom Objects ---
 * Left-aligned: Student{name: "Alice Johnson", id: 12345, GPA: 3.85}
 * Right-aligned: Student{name: "Alice Johnson", id: 12345, GPA: 3.85}
 */
