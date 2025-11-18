/**
 * Template Linked List
 * Generic linked list implementation using templates
 */

#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    size_t count;

public:
    // Constructor
    LinkedList() : head(nullptr), count(0) {}

    // Destructor
    ~LinkedList() {
        clear();
    }

    // Copy constructor
    LinkedList(const LinkedList& other) : head(nullptr), count(0) {
        Node* current = other.head;
        while (current != nullptr) {
            pushBack(current->data);
            current = current->next;
        }
    }

    // Assignment operator
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current != nullptr) {
                pushBack(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // Add to front
    void pushFront(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        count++;
    }

    // Add to back
    void pushBack(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        count++;
    }

    // Remove from front
    void popFront() {
        if (head == nullptr) {
            throw runtime_error("List is empty");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
    }

    // Get front element
    T& front() {
        if (head == nullptr) {
            throw runtime_error("List is empty");
        }
        return head->data;
    }

    const T& front() const {
        if (head == nullptr) {
            throw runtime_error("List is empty");
        }
        return head->data;
    }

    // Check if empty
    bool empty() const {
        return head == nullptr;
    }

    // Get size
    size_t size() const {
        return count;
    }

    // Clear all elements
    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }

    // Find element
    bool contains(const T& value) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Remove element
    bool remove(const T& value) {
        if (head == nullptr) {
            return false;
        }

        if (head->data == value) {
            popFront();
            return true;
        }

        Node* current = head;
        while (current->next != nullptr) {
            if (current->next->data == value) {
                Node* temp = current->next;
                current->next = current->next->next;
                delete temp;
                count--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Print list
    void print() const {
        if (head == nullptr) {
            cout << "[]" << endl;
            return;
        }

        cout << "[";
        Node* current = head;
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << "]" << endl;
    }

    // Reverse list
    void reverse() {
        Node* prev = nullptr;
        Node* current = head;
        Node* next = nullptr;

        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        head = prev;
    }
};

int main() {
    cout << "=== Template Linked List ===" << endl << endl;

    // Integer list
    cout << "Integer Linked List:" << endl;
    LinkedList<int> intList;
    intList.pushBack(10);
    intList.pushBack(20);
    intList.pushBack(30);
    intList.pushFront(5);
    intList.print();
    cout << "Size: " << intList.size() << endl;
    cout << "Front: " << intList.front() << endl;
    cout << endl;

    // String list
    cout << "String Linked List:" << endl;
    LinkedList<string> strList;
    strList.pushBack("apple");
    strList.pushBack("banana");
    strList.pushBack("cherry");
    strList.print();
    cout << "Contains 'banana': " << (strList.contains("banana") ? "Yes" : "No") << endl;
    cout << "Contains 'grape': " << (strList.contains("grape") ? "Yes" : "No") << endl;
    cout << endl;

    // Remove operation
    cout << "Remove Operations:" << endl;
    LinkedList<int> numList;
    numList.pushBack(1);
    numList.pushBack(2);
    numList.pushBack(3);
    numList.pushBack(4);
    numList.pushBack(5);
    cout << "Before: ";
    numList.print();

    numList.remove(3);
    cout << "After removing 3: ";
    numList.print();

    numList.popFront();
    cout << "After popFront: ";
    numList.print();
    cout << endl;

    // Reverse operation
    cout << "Reverse Operation:" << endl;
    LinkedList<int> revList;
    revList.pushBack(1);
    revList.pushBack(2);
    revList.pushBack(3);
    revList.pushBack(4);
    revList.pushBack(5);
    cout << "Before reverse: ";
    revList.print();
    revList.reverse();
    cout << "After reverse: ";
    revList.print();
    cout << endl;

    // Copy constructor
    cout << "Copy Constructor:" << endl;
    LinkedList<string> original;
    original.pushBack("one");
    original.pushBack("two");
    original.pushBack("three");
    cout << "Original: ";
    original.print();

    LinkedList<string> copy = original;
    cout << "Copy: ";
    copy.print();

    copy.pushBack("four");
    cout << "After modifying copy:" << endl;
    cout << "Original: ";
    original.print();
    cout << "Copy: ";
    copy.print();

    return 0;
}
