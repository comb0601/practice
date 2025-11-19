/*
 * Lesson 12: Templates - Template Specialization
 * Compilation: g++ -std=c++17 03_template_specialization.cpp -o specialization
 */

#include <iostream>
#include <cstring>

// Primary template
template<typename T>
class Storage {
private:
    T data;
public:
    Storage(T d) : data(d) {}
    void print() const {
        std::cout << "Generic: " << data << std::endl;
    }
    T getData() const { return data; }
};

// Full specialization for char*
template<>
class Storage<char*> {
private:
    char* data;
public:
    Storage(char* d) {
        data = new char[strlen(d) + 1];
        strcpy(data, d);
    }
    ~Storage() {
        delete[] data;
    }
    void print() const {
        std::cout << "Specialized for char*: " << data << std::endl;
    }
    char* getData() const { return data; }
};

// Partial specialization for pointers
template<typename T>
class Storage<T*> {
private:
    T* data;
public:
    Storage(T* d) : data(d) {}
    void print() const {
        std::cout << "Specialized for pointer: " << *data << std::endl;
    }
    T* getData() const { return data; }
};

int main() {
    std::cout << "=== Template Specialization ===" << std::endl << std::endl;

    Storage<int> intStorage(42);
    intStorage.print();

    char str[] = "Hello, Templates!";
    Storage<char*> charStorage(str);
    charStorage.print();

    int value = 100;
    Storage<int*> ptrStorage(&value);
    ptrStorage.print();

    return 0;
}
