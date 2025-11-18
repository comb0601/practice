/*
 * Lesson 22: Smart Pointers - unique_ptr Basics
 * Compile: g++ -std=c++17 -Wall 01_unique_ptr_basics.cpp -o 01_unique_ptr_basics
 */
#include <iostream>
#include <memory>

class Widget {
    int id;
public:
    Widget(int i) : id(i) { std::cout << "Widget " << id << " created\n"; }
    ~Widget() { std::cout << "Widget " << id << " destroyed\n"; }
    void use() { std::cout << "Using Widget " << id << "\n"; }
};

int main() {
    std::cout << "=== unique_ptr Basics ===\n\n";
    
    // 1. Creating unique_ptr
    std::cout << "1. CREATING UNIQUE_PTR:\n";
    auto ptr1 = std::make_unique<Widget>(1);
    ptr1->use();
    
    // 2. Ownership transfer (move)
    std::cout << "\n2. OWNERSHIP TRANSFER:\n";
    auto ptr2 = std::move(ptr1);  // ptr1 becomes nullptr
    if (!ptr1) std::cout << "ptr1 is nullptr\n";
    ptr2->use();
    
    // 3. Reset
    std::cout << "\n3. RESET:\n";
    ptr2.reset(new Widget(3));  // Deletes Widget 2, creates Widget 3
    
    // 4. Release
    std::cout << "\n4. RELEASE:\n";
    Widget* raw = ptr2.release();  // ptr2 becomes nullptr, we own raw
    delete raw;  // Manual deletion required
    
    // 5. Array unique_ptr
    std::cout << "\n5. ARRAY UNIQUE_PTR:\n";
    auto arr = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) arr[i] = i * 10;
    
    std::cout << "\nDestructing...\n";
    return 0;
}
