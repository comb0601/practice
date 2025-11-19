/*
 * Program: Abstract Interface
 * Description: Using abstract classes as interfaces
 * Compilation: g++ -std=c++17 14_abstract_interface.cpp -o 14_abstract_interface
 * Execution: ./14_abstract_interface
 */

#include <iostream>
#include <vector>
using namespace std;

class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() {}
};

class IPrintable {
public:
    virtual void print() const = 0;
    virtual ~IPrintable() {}
};

class Document : public IDrawable, public IPrintable {
private:
    string content;

public:
    Document(const string& c) : content(c) {}

    void draw() const override {
        cout << "Drawing document: " << content << endl;
    }

    void print() const override {
        cout << "Printing document: " << content << endl;
    }
};

class Image : public IDrawable {
private:
    string filename;

public:
    Image(const string& f) : filename(f) {}

    void draw() const override {
        cout << "Drawing image: " << filename << endl;
    }
};

void displayDrawable(const IDrawable& obj) {
    obj.draw();
}

int main() {
    cout << "=== Abstract Interface ===" << endl << endl;

    Document doc("Report.pdf");
    Image img("photo.jpg");

    vector<const IDrawable*> drawables = {&doc, &img};

    cout << "Drawing all objects:" << endl;
    for (auto obj : drawables) {
        obj->draw();
    }

    cout << "\nPrinting document:" << endl;
    doc.print();

    return 0;
}
