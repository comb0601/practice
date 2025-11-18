/**
 * Template Smart Pointer
 * Simple unique pointer implementation using templates
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Simple UniquePtr implementation
template <typename T>
class UniquePtr {
private:
    T* ptr;

    // Delete copy constructor and copy assignment
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

public:
    // Constructor
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}

    // Destructor
    ~UniquePtr() {
        delete ptr;
    }

    // Move constructor
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // Move assignment
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Arrow operator
    T* operator->() const {
        return ptr;
    }

    // Get raw pointer
    T* get() const {
        return ptr;
    }

    // Release ownership
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // Reset pointer
    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }

    // Check if valid
    explicit operator bool() const {
        return ptr != nullptr;
    }
};

// Helper function to create UniquePtr
template <typename T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args) {
    return UniquePtr<T>(new T(args...));
}

// Test classes
class Person {
private:
    string name;
    int age;

public:
    Person(string n, int a) : name(n), age(a) {
        cout << "Person created: " << name << endl;
    }

    ~Person() {
        cout << "Person destroyed: " << name << endl;
    }

    void display() const {
        cout << name << " (age " << age << ")" << endl;
    }

    string getName() const { return name; }
    int getAge() const { return age; }
};

class Resource {
private:
    int id;
    vector<int> data;

public:
    Resource(int i, int size) : id(i), data(size, 0) {
        cout << "Resource " << id << " allocated (" << size << " elements)" << endl;
    }

    ~Resource() {
        cout << "Resource " << id << " deallocated" << endl;
    }

    void setData(int index, int value) {
        if (index >= 0 && index < data.size()) {
            data[index] = value;
        }
    }

    int getData(int index) const {
        if (index >= 0 && index < data.size()) {
            return data[index];
        }
        return -1;
    }

    int getId() const { return id; }
};

// Function that takes ownership
template <typename T>
void processAndDelete(UniquePtr<T> ptr) {
    if (ptr) {
        cout << "Processing resource..." << endl;
        // ptr will be destroyed when function exits
    }
}

int main() {
    cout << "=== Template Smart Pointer ===" << endl << endl;

    // Basic usage with int
    cout << "Basic Integer Pointer:" << endl;
    {
        UniquePtr<int> ptr(new int(42));
        cout << "Value: " << *ptr << endl;
        *ptr = 100;
        cout << "New value: " << *ptr << endl;
    }  // ptr destroyed here, memory freed
    cout << endl;

    // Usage with Person class
    cout << "Person Smart Pointer:" << endl;
    {
        UniquePtr<Person> person(new Person("Alice", 30));
        person->display();
        cout << "Name: " << person->getName() << endl;
        cout << "Age: " << person->getAge() << endl;
    }  // person destroyed here
    cout << endl;

    // Using makeUnique helper
    cout << "Using makeUnique:" << endl;
    {
        auto p1 = makeUnique<Person>("Bob", 25);
        p1->display();

        auto p2 = makeUnique<Person>("Charlie", 35);
        p2->display();
    }
    cout << endl;

    // Move semantics
    cout << "Move Semantics:" << endl;
    {
        UniquePtr<Person> original(new Person("Diana", 28));
        cout << "Original valid: " << (original ? "yes" : "no") << endl;

        UniquePtr<Person> moved = move(original);
        cout << "After move:" << endl;
        cout << "  Original valid: " << (original ? "yes" : "no") << endl;
        cout << "  Moved valid: " << (moved ? "yes" : "no") << endl;

        if (moved) {
            moved->display();
        }
    }
    cout << endl;

    // Resource management
    cout << "Resource Management:" << endl;
    {
        UniquePtr<Resource> res(new Resource(1, 5));
        res->setData(0, 100);
        res->setData(1, 200);
        res->setData(2, 300);
        cout << "Data[0] = " << res->getData(0) << endl;
        cout << "Data[1] = " << res->getData(1) << endl;
        cout << "Data[2] = " << res->getData(2) << endl;
    }
    cout << endl;

    // Reset functionality
    cout << "Reset Functionality:" << endl;
    {
        UniquePtr<Person> ptr(new Person("Eve", 22));
        ptr->display();

        cout << "Resetting pointer..." << endl;
        ptr.reset(new Person("Frank", 40));
        ptr->display();

        cout << "Resetting to nullptr..." << endl;
        ptr.reset();
        cout << "Pointer valid: " << (ptr ? "yes" : "no") << endl;
    }
    cout << endl;

    // Release functionality
    cout << "Release Functionality:" << endl;
    {
        UniquePtr<Person> ptr(new Person("Grace", 33));
        Person* raw = ptr.release();
        cout << "Pointer valid: " << (ptr ? "yes" : "no") << endl;
        cout << "Raw pointer: " << raw << endl;
        raw->display();

        cout << "Manually deleting..." << endl;
        delete raw;  // Must manually delete now
    }
    cout << endl;

    // Array of smart pointers
    cout << "Array of Smart Pointers:" << endl;
    {
        vector<UniquePtr<Person>> people;
        people.push_back(makeUnique<Person>("Hannah", 27));
        people.push_back(makeUnique<Person>("Ivan", 31));
        people.push_back(makeUnique<Person>("Jane", 29));

        cout << "People:" << endl;
        for (auto& person : people) {
            cout << "  ";
            person->display();
        }
    }  // All persons destroyed automatically
    cout << endl;

    // Exception safety
    cout << "Exception Safety Demo:" << endl;
    {
        try {
            UniquePtr<Resource> res(new Resource(99, 1000));
            // Even if exception occurs, res will be destroyed
            throw runtime_error("Simulated error");
        } catch (const exception& e) {
            cout << "Caught exception: " << e.what() << endl;
            cout << "(Resource was properly cleaned up)" << endl;
        }
    }

    cout << "\nAll resources cleaned up automatically!" << endl;

    return 0;
}
