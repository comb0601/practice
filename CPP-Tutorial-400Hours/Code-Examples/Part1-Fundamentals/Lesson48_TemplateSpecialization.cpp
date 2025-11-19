/* Lesson 48: Template Specialization
 * Compile: cl Lesson48_TemplateSpecialization.cpp */
#include <iostream>
#include <cstring>
using namespace std;

template <typename T>
class Storage {
private:
    T value;
public:
    Storage(T v) : value(v) {}
    void print() { cout << "Value: " << value << endl; }
};

template <>
class Storage<char*> {
private:
    char *value;
public:
    Storage(char *v) {
        value = new char[strlen(v) + 1];
        strcpy(value, v);
    }
    ~Storage() { delete[] value; }
    void print() { cout << "String: " << value << endl; }
};

int main() {
    Storage<int> intStorage(42);
    intStorage.print();

    Storage<double> doubleStorage(3.14);
    doubleStorage.print();

    char str[] = "Hello";
    Storage<char*> strStorage(str);
    strStorage.print();

    return 0;
}
