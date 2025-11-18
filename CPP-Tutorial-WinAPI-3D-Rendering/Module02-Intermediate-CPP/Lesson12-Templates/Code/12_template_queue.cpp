/**
 * Template Queue
 * Generic queue implementation using templates
 */

#include <iostream>
#include <deque>
#include <stdexcept>
#include <string>
using namespace std;

template <typename T>
class Queue {
private:
    deque<T> elements;

public:
    // Enqueue - add to back
    void enqueue(const T& value) {
        elements.push_back(value);
    }

    // Dequeue - remove from front
    void dequeue() {
        if (empty()) {
            throw runtime_error("Queue is empty - cannot dequeue");
        }
        elements.pop_front();
    }

    // Get front element
    T& front() {
        if (empty()) {
            throw runtime_error("Queue is empty - no front element");
        }
        return elements.front();
    }

    const T& front() const {
        if (empty()) {
            throw runtime_error("Queue is empty - no front element");
        }
        return elements.front();
    }

    // Get back element
    T& back() {
        if (empty()) {
            throw runtime_error("Queue is empty - no back element");
        }
        return elements.back();
    }

    const T& back() const {
        if (empty()) {
            throw runtime_error("Queue is empty - no back element");
        }
        return elements.back();
    }

    // Check if empty
    bool empty() const {
        return elements.empty();
    }

    // Get size
    size_t size() const {
        return elements.size();
    }

    // Clear all elements
    void clear() {
        elements.clear();
    }

    // Print queue
    void print() const {
        if (empty()) {
            cout << "Queue is empty" << endl;
            return;
        }
        cout << "Queue (front to back): ";
        for (const T& elem : elements) {
            cout << elem << " ";
        }
        cout << endl;
    }
};

// Priority Queue (simple implementation)
template <typename T>
class PriorityQueue {
private:
    struct Element {
        T data;
        int priority;

        Element(const T& d, int p) : data(d), priority(p) {}

        bool operator<(const Element& other) const {
            return priority < other.priority;
        }
    };

    deque<Element> elements;

public:
    // Enqueue with priority
    void enqueue(const T& value, int priority) {
        Element newElem(value, priority);

        // Insert in sorted order (higher priority first)
        auto it = elements.begin();
        while (it != elements.end() && it->priority >= priority) {
            ++it;
        }
        elements.insert(it, newElem);
    }

    // Dequeue highest priority
    void dequeue() {
        if (empty()) {
            throw runtime_error("Priority queue is empty");
        }
        elements.pop_front();
    }

    // Get highest priority element
    T& front() {
        if (empty()) {
            throw runtime_error("Priority queue is empty");
        }
        return elements.front().data;
    }

    bool empty() const {
        return elements.empty();
    }

    size_t size() const {
        return elements.size();
    }

    void print() const {
        if (empty()) {
            cout << "Priority Queue is empty" << endl;
            return;
        }
        cout << "Priority Queue:" << endl;
        for (const auto& elem : elements) {
            cout << "  [" << elem.priority << "] " << elem.data << endl;
        }
    }
};

int main() {
    cout << "=== Template Queue ===" << endl << endl;

    // Integer queue
    cout << "Integer Queue:" << endl;
    Queue<int> intQueue;
    intQueue.enqueue(10);
    intQueue.enqueue(20);
    intQueue.enqueue(30);
    intQueue.enqueue(40);
    intQueue.print();
    cout << "Front: " << intQueue.front() << endl;
    cout << "Back: " << intQueue.back() << endl;
    cout << "Size: " << intQueue.size() << endl;

    intQueue.dequeue();
    cout << "After dequeue: ";
    intQueue.print();
    cout << endl;

    // String queue
    cout << "String Queue:" << endl;
    Queue<string> strQueue;
    strQueue.enqueue("First");
    strQueue.enqueue("Second");
    strQueue.enqueue("Third");
    strQueue.enqueue("Fourth");
    strQueue.print();

    while (!strQueue.empty()) {
        cout << "Dequeuing: " << strQueue.front() << endl;
        strQueue.dequeue();
    }
    strQueue.print();
    cout << endl;

    // Simulate customer service queue
    cout << "Customer Service Queue:" << endl;
    Queue<string> customers;
    customers.enqueue("Alice");
    customers.enqueue("Bob");
    customers.enqueue("Charlie");
    customers.enqueue("Diana");

    cout << "Customers waiting: ";
    customers.print();

    cout << "\nServing customers:" << endl;
    while (!customers.empty()) {
        cout << "  Now serving: " << customers.front() << endl;
        customers.dequeue();
        if (!customers.empty()) {
            cout << "  Remaining: ";
            customers.print();
        }
    }
    cout << endl;

    // Priority Queue
    cout << "Priority Queue Example:" << endl;
    PriorityQueue<string> tasks;
    tasks.enqueue("Low priority task", 1);
    tasks.enqueue("High priority task", 5);
    tasks.enqueue("Medium priority task", 3);
    tasks.enqueue("Urgent task", 10);
    tasks.enqueue("Another medium task", 3);

    cout << "Tasks by priority:" << endl;
    tasks.print();

    cout << "\nProcessing tasks:" << endl;
    while (!tasks.empty()) {
        cout << "Processing: " << tasks.front() << endl;
        tasks.dequeue();
    }
    cout << endl;

    // Number queue simulation
    cout << "Number Processing Queue:" << endl;
    Queue<double> numbers;
    numbers.enqueue(3.14);
    numbers.enqueue(2.71);
    numbers.enqueue(1.41);
    numbers.enqueue(1.73);
    numbers.print();

    double sum = 0;
    while (!numbers.empty()) {
        sum += numbers.front();
        numbers.dequeue();
    }
    cout << "Sum of all numbers: " << sum << endl;

    return 0;
}
