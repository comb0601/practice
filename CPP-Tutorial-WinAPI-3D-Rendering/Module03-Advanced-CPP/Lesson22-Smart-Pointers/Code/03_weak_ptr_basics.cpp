/*
 * Lesson 22: Smart Pointers - weak_ptr Basics
 * Compile: g++ -std=c++17 -Wall 03_weak_ptr_basics.cpp -o 03_weak_ptr_basics
 */
#include <iostream>
#include <memory>

class Observer {
public:
    Observer() { std::cout << "Observer created\n"; }
    ~Observer() { std::cout << "Observer destroyed\n"; }
    void notify() { std::cout << "Notification received\n"; }
};

int main() {
    std::cout << "=== weak_ptr Basics ===\n\n";
    
    // 1. Creating weak_ptr
    std::cout << "1. CREATING WEAK_PTR:\n";
    std::weak_ptr<Observer> weak;
    {
        auto shared = std::make_shared<Observer>();
        weak = shared;  // weak observes shared
        
        std::cout << "shared count: " << shared.use_count() << "\n";  // 1
        std::cout << "weak expired: " << weak.expired() << "\n";  // false
        
        // 2. Locking weak_ptr
        std::cout << "\n2. LOCKING WEAK_PTR:\n";
        if (auto locked = weak.lock()) {
            locked->notify();
            std::cout << "locked count: " << locked.use_count() << "\n";  // 2
        }
    }  // shared destroyed
    
    // 3. After object deleted
    std::cout << "\n3. AFTER OBJECT DELETED:\n";
    std::cout << "weak expired: " << weak.expired() << "\n";  // true
    if (auto locked = weak.lock()) {
        std::cout << "Object exists\n";
    } else {
        std::cout << "Object has been deleted\n";
    }
    
    return 0;
}
