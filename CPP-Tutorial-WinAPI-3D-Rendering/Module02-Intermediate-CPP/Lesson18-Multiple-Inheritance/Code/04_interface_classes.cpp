#include <iostream>
using namespace std;
class Printable {
public:
    virtual void print() = 0;
};
class Saveable {
public:
    virtual void save() = 0;
};
class Document : public Printable, public Saveable {
public:
    void print() override { cout << "Printing document" << endl; }
    void save() override { cout << "Saving document" << endl; }
};
int main() {
    Document doc;
    doc.print();
    doc.save();
    return 0;
}
