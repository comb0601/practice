/*
 * Program: Constructor Examples
 * Description: Practical constructor examples
 * Compilation: g++ -std=c++17 14_constructor_examples.cpp -o 14_constructor_examples
 * Execution: ./14_constructor_examples
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Product {
private:
    string name;
    double price;
    int quantity;

public:
    Product() : name("Unknown"), price(0.0), quantity(0) {}

    Product(string n, double p) : name(n), price(p), quantity(1) {}

    Product(string n, double p, int q) : name(n), price(p), quantity(q) {}

    void display() const {
        cout << name << ": $" << price << " x " << quantity
             << " = $" << (price * quantity) << endl;
    }
};

class ShoppingCart {
private:
    vector<Product> items;
    string customerName;

public:
    ShoppingCart(const string& name) : customerName(name) {
        cout << "Cart created for " << customerName << endl;
    }

    void addItem(const Product& p) {
        items.push_back(p);
    }

    double getTotal() const {
        double total = 0;
        for (const auto& item : items) {
            // Would need getPrice() and getQuantity() methods
        }
        return total;
    }

    void display() const {
        cout << "\nShopping Cart for " << customerName << ":" << endl;
        for (const auto& item : items) {
            item.display();
        }
    }
};

int main() {
    cout << "=== Constructor Examples ===" << endl << endl;

    Product p1;
    Product p2("Laptop", 999.99);
    Product p3("Mouse", 29.99, 2);

    p1.display();
    p2.display();
    p3.display();

    cout << endl;

    ShoppingCart cart("John Doe");
    cart.addItem(p2);
    cart.addItem(p3);
    cart.display();

    return 0;
}
