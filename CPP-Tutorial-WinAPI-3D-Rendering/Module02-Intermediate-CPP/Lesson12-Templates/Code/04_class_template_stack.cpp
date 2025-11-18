/**
 * Class Template: Generic Stack
 * Implements a type-safe stack using templates
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
using namespace std;

template <typename T>
class Stack {
private:
    vector<T> elements;

public:
    // Push element onto stack
    void push(const T& element) {
        elements.push_back(element);
    }

    // Pop element from stack
    void pop() {
        if (empty()) {
            throw out_of_range("Stack is empty - cannot pop");
        }
        elements.pop_back();
    }

    // Get top element
    T& top() {
        if (empty()) {
            throw out_of_range("Stack is empty - no top element");
        }
        return elements.back();
    }

    const T& top() const {
        if (empty()) {
            throw out_of_range("Stack is empty - no top element");
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

    // Print stack contents
    void print() const {
        if (empty()) {
            cout << "Stack is empty" << endl;
            return;
        }
        cout << "Stack (top to bottom): ";
        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
    }
};

int main() {
    cout << "=== Generic Stack Template ===" << endl << endl;

    // Integer stack
    cout << "Integer Stack:" << endl;
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    intStack.push(40);
    intStack.print();
    cout << "Top element: " << intStack.top() << endl;
    cout << "Size: " << intStack.size() << endl;

    intStack.pop();
    cout << "After pop: ";
    intStack.print();
    cout << endl;

    // String stack
    cout << "String Stack:" << endl;
    Stack<string> stringStack;
    stringStack.push("First");
    stringStack.push("Second");
    stringStack.push("Third");
    stringStack.print();

    while (!stringStack.empty()) {
        cout << "Popping: " << stringStack.top() << endl;
        stringStack.pop();
    }
    stringStack.print();
    cout << endl;

    // Double stack
    cout << "Double Stack:" << endl;
    Stack<double> doubleStack;
    doubleStack.push(3.14);
    doubleStack.push(2.71);
    doubleStack.push(1.41);
    doubleStack.print();
    cout << "Size: " << doubleStack.size() << endl;
    cout << endl;

    // Character stack - palindrome checker
    cout << "Palindrome Checker using Stack:" << endl;
    string word = "racecar";
    Stack<char> charStack;

    // Push first half
    for (size_t i = 0; i < word.length() / 2; i++) {
        charStack.push(word[i]);
    }

    // Check second half
    bool isPalindrome = true;
    size_t start = (word.length() + 1) / 2;
    for (size_t i = start; i < word.length(); i++) {
        if (charStack.empty() || charStack.top() != word[i]) {
            isPalindrome = false;
            break;
        }
        charStack.pop();
    }

    cout << "\"" << word << "\" is "
         << (isPalindrome ? "a palindrome" : "not a palindrome") << endl;
    cout << endl;

    // Exception handling
    cout << "Exception Handling:" << endl;
    Stack<int> emptyStack;
    try {
        emptyStack.pop();
    } catch (const out_of_range& e) {
        cout << "Caught exception on pop: " << e.what() << endl;
    }

    try {
        int val = emptyStack.top();
        cout << val << endl;
    } catch (const out_of_range& e) {
        cout << "Caught exception on top: " << e.what() << endl;
    }

    return 0;
}
