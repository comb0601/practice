/*
 * Lesson 22: Smart Pointers - shared_ptr Basics
 * Compile: g++ -std=c++17 -Wall 02_shared_ptr_basics.cpp -o 02_shared_ptr_basics
 */
#include <iostream>
#include <memory>

class Resource {
    int id;
public:
    Resource(int i) : id(i) { std::cout << "Resource " << id << " created\n"; }
    ~Resource() { std::cout << "Resource " << id << " destroyed\n"; }
    int getId() const { return id; }
};

int main() {
    std::cout << "=== shared_ptr Basics ===\n\n";
    
    // 1. Creating shared_ptr
    std::cout << "1. CREATING SHARED_PTR:\n";
    auto ptr1 = std::make_shared<Resource>(1);
    std::cout << "Count: " << ptr1.use_count() << "\n";
    
    // 2. Shared ownership
    std::cout << "\n2. SHARED OWNERSHIP:\n";
    {
        auto ptr2 = ptr1;  // Share ownership
        auto ptr3 = ptr1;
        std::cout << "Count: " << ptr1.use_count() << "\n";  // 3
        std::cout << "ptr2 ID: " << ptr2->getId() << "\n";
    }  // ptr2 and ptr3 destroyed
    std::cout << "Count after scope: " << ptr1.use_count() << "\n";  // 1
    
    // 3. Converting unique_ptr to shared_ptr
    std::cout << "\n3. UNIQUE TO SHARED CONVERSION:\n";
    auto uptr = std::make_unique<Resource>(2);
    std::shared_ptr<Resource> sptr = std::move(uptr);  // Transfer ownership
    std::cout << "shared_ptr count: " << sptr.use_count() << "\n";
    
    // 4. Reset
    std::cout << "\n4. RESET:\n";
    sptr.reset(new Resource(3));  // Deletes Resource 2
    
    std::cout << "\nDestructing...\n";
    return 0;
}
