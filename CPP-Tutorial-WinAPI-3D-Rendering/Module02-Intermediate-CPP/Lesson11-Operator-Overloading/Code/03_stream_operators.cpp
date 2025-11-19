/*
 * Lesson 11: Operator Overloading - Stream Operators
 *
 * This example demonstrates overloading << and >> operators
 * for input/output operations with custom classes.
 *
 * Compilation: g++ -std=c++17 03_stream_operators.cpp -o stream_op
 * Execution: ./stream_op
 */

#include <iostream>
#include <string>

class Book {
private:
    std::string title;
    std::string author;
    int year;
    double price;

public:
    Book(const std::string& t = "", const std::string& a = "", int y = 0, double p = 0.0)
        : title(t), author(a), year(y), price(p) {}

    // Friend function to overload << operator
    friend std::ostream& operator<<(std::ostream& os, const Book& book);

    // Friend function to overload >> operator
    friend std::istream& operator>>(std::istream& is, Book& book);
};

// Overload << for output
std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << "Title: " << book.title << "\n"
       << "Author: " << book.author << "\n"
       << "Year: " << book.year << "\n"
       << "Price: $" << book.price;
    return os;
}

// Overload >> for input
std::istream& operator>>(std::istream& is, Book& book) {
    std::cout << "Enter book title: ";
    std::getline(is, book.title);

    std::cout << "Enter author name: ";
    std::getline(is, book.author);

    std::cout << "Enter publication year: ";
    is >> book.year;

    std::cout << "Enter price: $";
    is >> book.price;

    // Clear newline from buffer
    is.ignore();

    return is;
}

int main() {
    std::cout << "=== Stream Operators Demo ===" << std::endl << std::endl;

    // Create a book using constructor
    Book book1("The C++ Programming Language", "Bjarne Stroustrup", 2013, 59.99);

    std::cout << "Book 1 Details:" << std::endl;
    std::cout << book1 << std::endl << std::endl;

    // Create a book using input operator
    Book book2;
    std::cout << "Enter details for Book 2:" << std::endl;
    std::cin >> book2;

    std::cout << "\nBook 2 Details:" << std::endl;
    std::cout << book2 << std::endl;

    return 0;
}
