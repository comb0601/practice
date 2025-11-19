/*
 * Lesson 11: Operator Overloading - Arrow Operator
 *
 * This example demonstrates overloading the arrow operator (->)
 * to create smart pointer-like classes.
 *
 * Compilation: g++ -std=c++17 08_arrow_operator.cpp -o arrow
 * Execution: ./arrow
 */

#include <iostream>
#include <string>

class Resource {
private:
    std::string name;
    int value;

public:
    Resource(const std::string& n, int v) : name(n), value(v) {
        std::cout << "Resource created: " << name << std::endl;
    }

    ~Resource() {
        std::cout << "Resource destroyed: " << name << std::endl;
    }

    void display() const {
        std::cout << "Resource: " << name << ", Value: " << value << std::endl;
    }

    void setValue(int v) {
        value = v;
    }

    int getValue() const {
        return value;
    }

    const std::string& getName() const {
        return name;
    }
};

// Simple smart pointer class
template<typename T>
class SmartPtr {
private:
    T* ptr;

public:
    // Constructor
    explicit SmartPtr(T* p = nullptr) : ptr(p) {
        std::cout << "SmartPtr created" << std::endl;
    }

    // Destructor - automatically deletes the resource
    ~SmartPtr() {
        std::cout << "SmartPtr destroyed, deleting resource" << std::endl;
        delete ptr;
    }

    // Disable copy (for simplicity)
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;

    // Overload -> operator
    T* operator->() const {
        return ptr;
    }

    // Overload * operator (dereference)
    T& operator*() const {
        return *ptr;
    }

    // Check if pointer is valid
    bool isValid() const {
        return ptr != nullptr;
    }

    // Get raw pointer
    T* get() const {
        return ptr;
    }
};

// Another example - wrapper with logging
class LoggedResource {
private:
    Resource* resource;
    mutable int accessCount;

public:
    LoggedResource(Resource* res) : resource(res), accessCount(0) {}

    ~LoggedResource() {
        std::cout << "Total accesses: " << accessCount << std::endl;
        delete resource;
    }

    // Arrow operator with logging
    Resource* operator->() const {
        accessCount++;
        std::cout << "[Access #" << accessCount << "] ";
        return resource;
    }
};

int main() {
    std::cout << "=== Arrow Operator Demo ===" << std::endl << std::endl;

    std::cout << "1. Basic Smart Pointer:" << std::endl;
    {
        SmartPtr<Resource> ptr(new Resource("MyResource", 42));

        // Using arrow operator to access Resource methods
        ptr->display();
        ptr->setValue(100);
        std::cout << "New value: " << ptr->getValue() << std::endl;

        // Using dereference operator
        Resource& ref = *ptr;
        ref.display();

    } // ptr goes out of scope, Resource is automatically deleted

    std::cout << "\n2. Logged Resource Access:" << std::endl;
    {
        LoggedResource logged(new Resource("LoggedResource", 55));

        logged->display();
        logged->setValue(200);
        logged->display();
        std::cout << "Value: " << logged->getValue() << std::endl;

    } // Shows total access count

    std::cout << "\n3. Multiple Smart Pointers:" << std::endl;
    {
        SmartPtr<Resource> ptr1(new Resource("Resource1", 10));
        SmartPtr<Resource> ptr2(new Resource("Resource2", 20));
        SmartPtr<Resource> ptr3(new Resource("Resource3", 30));

        ptr1->display();
        ptr2->display();
        ptr3->display();
    }

    std::cout << "\nProgram ending..." << std::endl;
    return 0;
}
