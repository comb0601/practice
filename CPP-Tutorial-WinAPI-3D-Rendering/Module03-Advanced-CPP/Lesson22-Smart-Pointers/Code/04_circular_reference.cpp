/*
 * Lesson 22: Smart Pointers - Breaking Circular References
 * Compile: g++ -std=c++17 -Wall 04_circular_reference.cpp -o 04_circular_reference
 */
#include <iostream>
#include <memory>

// BAD: Circular reference with shared_ptr (memory leak)
namespace bad {
    struct Node {
        std::shared_ptr<Node> next;
        std::shared_ptr<Node> prev;
        int data;
        
        Node(int d) : data(d) { std::cout << "Node " << data << " created\n"; }
        ~Node() { std::cout << "Node " << data << " destroyed\n"; }
    };
}

// GOOD: Breaking cycle with weak_ptr
namespace good {
    struct Node {
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;  // Break cycle with weak_ptr
        int data;
        
        Node(int d) : data(d) { std::cout << "Node " << data << " created\n"; }
        ~Node() { std::cout << "Node " << data << " destroyed\n"; }
    };
}

int main() {
    std::cout << "=== Circular References ===\n\n";
    
    // 1. Circular reference problem
    std::cout << "1. BAD: CIRCULAR REFERENCE (MEMORY LEAK):\n";
    {
        auto node1 = std::make_shared<bad::Node>(1);
        auto node2 = std::make_shared<bad::Node>(2);
        node1->next = node2;
        node2->prev = node1;  // Circular reference!
        std::cout << "node1 count: " << node1.use_count() << "\n";  // 2
        std::cout << "node2 count: " << node2.use_count() << "\n";  // 2
    }  // Nodes NOT destroyed! Memory leak!
    std::cout << "After scope - nodes still exist (leaked)\n\n";
    
    // 2. Fixed with weak_ptr
    std::cout << "2. GOOD: USING WEAK_PTR:\n";
    {
        auto node1 = std::make_shared<good::Node>(3);
        auto node2 = std::make_shared<good::Node>(4);
        node1->next = node2;
        node2->prev = node1;  // No cycle, prev is weak
        std::cout << "node1 count: " << node1.use_count() << "\n";  // 1
        std::cout << "node2 count: " << node2.use_count() << "\n";  // 2
    }  // Nodes properly destroyed
    std::cout << "After scope - nodes destroyed\n";
    
    return 0;
}
