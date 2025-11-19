/*
 * Lesson 12: Templates - Class Template Basics
 * Compilation: g++ -std=c++17 02_class_template_basics.cpp -o class_template
 */

#include <iostream>
#include <stdexcept>

template<typename T>
class Stack {
private:
    T* elements;
    size_t capacity;
    size_t top_index;

public:
    Stack(size_t cap = 10) : capacity(cap), top_index(0) {
        elements = new T[capacity];
    }

    ~Stack() {
        delete[] elements;
    }

    void push(const T& element) {
        if (top_index >= capacity) {
            throw std::overflow_error("Stack overflow");
        }
        elements[top_index++] = element;
    }

    T pop() {
        if (top_index == 0) {
            throw std::underflow_error("Stack underflow");
        }
        return elements[--top_index];
    }

    const T& top() const {
        if (top_index == 0) {
            throw std::underflow_error("Stack is empty");
        }
        return elements[top_index - 1];
    }

    bool empty() const {
        return top_index == 0;
    }

    size_t size() const {
        return top_index;
    }
};

int main() {
    std::cout << "=== Class Template Basics ===" << std::endl << std::endl;

    Stack<int> intStack(5);
    std::cout << "Pushing integers: 10, 20, 30" << std::endl;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);

    std::cout << "Top element: " << intStack.top() << std::endl;
    std::cout << "Stack size: " << intStack.size() << std::endl;

    std::cout << "\nPopping elements: ";
    while (!intStack.empty()) {
        std::cout << intStack.pop() << " ";
    }
    std::cout << std::endl;

    Stack<std::string> stringStack(3);
    std::cout << "\nPushing strings: Hello, World, C++" << std::endl;
    stringStack.push("Hello");
    stringStack.push("World");
    stringStack.push("C++");

    std::cout << "Popping strings: ";
    while (!stringStack.empty()) {
        std::cout << stringStack.pop() << " ";
    }
    std::cout << std::endl;

    return 0;
}
