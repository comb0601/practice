/* Lesson 70: Smart Pointers Advanced
 * Compile: cl /std:c++17 Lesson70_SmartPointersAdvanced.cpp */
#include <iostream>
#include <memory>
using namespace std;

class Node {
public:
    int data;
    shared_ptr<Node> next;
    weak_ptr<Node> prev;  // Use weak_ptr to break cycles

    Node(int d) : data(d) {
        cout << "Node " << data << " created" << endl;
    }

    ~Node() {
        cout << "Node " << data << " destroyed" << endl;
    }
};

int main() {
    auto node1 = make_shared<Node>(1);
    auto node2 = make_shared<Node>(2);

    node1->next = node2;
    node2->prev = node1;

    cout << "Use count node1: " << node1.use_count() << endl;
    cout << "Use count node2: " << node2.use_count() << endl;

    return 0;
}
