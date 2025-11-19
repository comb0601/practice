/*
 * Program: Book Class
 * Description: Book class for library management
 * Compilation: g++ -std=c++17 13_book_class.cpp -o 13_book_class
 * Execution: ./13_book_class
 */

#include <iostream>
#include <string>
using namespace std;

class Book {
private:
    string title;
    string author;
    string isbn;
    int pages;
    bool isAvailable;

public:
    Book(const string& t, const string& a, const string& i, int p)
        : title(t), author(a), isbn(i), pages(p), isAvailable(true) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getIsbn() const { return isbn; }
    int getPages() const { return pages; }
    bool checkAvailability() const { return isAvailable; }

    void checkOut() {
        if (isAvailable) {
            isAvailable = false;
            cout << "Book checked out successfully" << endl;
        } else {
            cout << "Book is not available" << endl;
        }
    }

    void returnBook() {
        isAvailable = true;
        cout << "Book returned successfully" << endl;
    }

    void display() const {
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << isbn << endl;
        cout << "Pages: " << pages << endl;
        cout << "Status: " << (isAvailable ? "Available" : "Checked Out") << endl;
    }
};

int main() {
    cout << "=== Book Class ===" << endl << endl;

    Book book("C++ Primer", "Stanley Lippman", "978-0321714114", 976);

    book.display();

    cout << "\nChecking out book..." << endl;
    book.checkOut();

    cout << "\nStatus after checkout:" << endl;
    book.display();

    cout << "\nReturning book..." << endl;
    book.returnBook();

    cout << "\nFinal status:" << endl;
    book.display();

    return 0;
}
