# Lesson 53: Stack and Queue Container Adapters

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Container Adapters
2. Stack Container Adapter
3. Queue Container Adapter
4. Priority Queue
5. Custom Comparators for Priority Queue
6. Real-World Applications
7. Implementation Details
8. Exercises and Projects

---

## 1. Introduction to Container Adapters

**Container adapters** are NOT containers themselves. They're interfaces that use an underlying container and restrict access to provide specific functionality.

### The Three Standard Adapters:

```
Stack:
- LIFO (Last In, First Out)
- Default: deque
- Operations: push, pop, top

Queue:
- FIFO (First In, First Out)
- Default: deque
- Operations: push, pop, front, back

Priority Queue:
- Heap-based priority ordering
- Default: vector
- Operations: push, pop, top
```

### Why Use Adapters?

```cpp
/*
Advantages:
✅ Clear intent - name expresses purpose
✅ Restricted interface - prevents misuse
✅ Flexibility - can change underlying container
✅ Type safety - compile-time checks

Example:
Instead of: deque<int> myStack;  // Could use any deque operation
Use:        stack<int> myStack;  // Only stack operations allowed
*/
```

---

## 2. Stack Container Adapter

### What is a Stack?

A **stack** follows the LIFO (Last In, First Out) principle. Think of a stack of plates - you add and remove from the top.

**Visual:**
```
push(3)  →  | 3 | ← top
push(2)  →  | 2 |
push(1)  →  | 1 |
            -----
pop()    ←  returns 3
```

### Creating Stacks:

```cpp
#include <iostream>
#include <stack>
#include <vector>
#include <list>

int main()
{
    // Default stack (uses deque)
    std::stack<int> s1;

    // Stack with vector as underlying container
    std::stack<int, std::vector<int>> s2;

    // Stack with list as underlying container
    std::stack<int, std::list<int>> s3;

    // Initialize from existing container
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::stack<int, std::vector<int>> s4(vec);

    std::cout << "Stack s4 top: " << s4.top() << "\n";
    std::cout << "Stack s4 size: " << s4.size() << "\n";

    return 0;
}
```

### Stack Operations:

```cpp
#include <iostream>
#include <stack>

int main()
{
    std::stack<int> s;

    // push() - add element to top
    s.push(10);
    s.push(20);
    s.push(30);
    s.push(40);
    s.push(50);

    std::cout << "Stack size: " << s.size() << "\n";
    std::cout << "Top element: " << s.top() << "\n";

    // pop() - remove top element (returns void!)
    s.pop();
    std::cout << "After pop, top: " << s.top() << "\n";

    // empty() - check if empty
    std::cout << "Is empty? " << (s.empty() ? "Yes" : "No") << "\n";

    // Print and empty the stack
    std::cout << "Stack contents (top to bottom): ";
    while (!s.empty()) {
        std::cout << s.top() << " ";
        s.pop();
    }
    std::cout << "\n";

    std::cout << "After emptying, is empty? " << (s.empty() ? "Yes" : "No") << "\n";

    return 0;
}
```

**Output:**
```
Stack size: 5
Top element: 50
After pop, top: 40
Is empty? No
Stack contents (top to bottom): 40 30 20 10
After emptying, is empty? Yes
```

### Emplace vs Push:

```cpp
#include <iostream>
#include <stack>
#include <string>

struct Person {
    std::string name;
    int age;

    Person(std::string n, int a) : name(n), age(a) {
        std::cout << "Person created: " << name << "\n";
    }
};

int main()
{
    std::stack<Person> people;

    // push - creates temporary object, then copies
    std::cout << "Using push:\n";
    people.push(Person("Alice", 30));

    // emplace - constructs directly in place (more efficient)
    std::cout << "\nUsing emplace:\n";
    people.emplace("Bob", 25);

    std::cout << "\nTop person: " << people.top().name << "\n";

    return 0;
}
```

### Stack with Custom Class:

```cpp
#include <iostream>
#include <stack>
#include <string>

class Book {
private:
    std::string title;
    std::string author;
    int pages;

public:
    Book(std::string t, std::string a, int p)
        : title(t), author(a), pages(p) {}

    void display() const {
        std::cout << "\"" << title << "\" by " << author
                  << " (" << pages << " pages)\n";
    }

    std::string getTitle() const { return title; }
};

int main()
{
    std::stack<Book> bookStack;

    // Add books to stack
    bookStack.emplace("1984", "George Orwell", 328);
    bookStack.emplace("To Kill a Mockingbird", "Harper Lee", 324);
    bookStack.emplace("The Great Gatsby", "F. Scott Fitzgerald", 180);

    std::cout << "Books on stack (top to bottom):\n";
    std::cout << "================================\n";

    while (!bookStack.empty()) {
        bookStack.top().display();
        bookStack.pop();
    }

    return 0;
}
```

---

## 3. Queue Container Adapter

### What is a Queue?

A **queue** follows the FIFO (First In, First Out) principle. Like a line at a store - first come, first served.

**Visual:**
```
push(1)  →  | 1 | ← back
push(2)  →  | 2 |
push(3)  →  | 3 |
            -----
            front
pop()    ←  returns 1
```

### Creating Queues:

```cpp
#include <iostream>
#include <queue>
#include <list>

int main()
{
    // Default queue (uses deque)
    std::queue<int> q1;

    // Queue with list as underlying container
    std::queue<int, std::list<int>> q2;

    // Note: Can't use vector as underlying container for queue
    // because vector doesn't have efficient pop_front()

    q1.push(10);
    q1.push(20);
    q1.push(30);

    std::cout << "Front: " << q1.front() << "\n";
    std::cout << "Back: " << q1.back() << "\n";
    std::cout << "Size: " << q1.size() << "\n";

    return 0;
}
```

### Queue Operations:

```cpp
#include <iostream>
#include <queue>

int main()
{
    std::queue<int> q;

    // push() - add element to back
    q.push(10);
    q.push(20);
    q.push(30);
    q.push(40);
    q.push(50);

    std::cout << "Queue size: " << q.size() << "\n";
    std::cout << "Front element: " << q.front() << "\n";
    std::cout << "Back element: " << q.back() << "\n";

    // pop() - remove front element
    q.pop();
    std::cout << "After pop, front: " << q.front() << "\n";

    // empty() - check if empty
    std::cout << "Is empty? " << (q.empty() ? "Yes" : "No") << "\n";

    // Process entire queue
    std::cout << "Queue contents (front to back): ";
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << "\n";

    return 0;
}
```

**Output:**
```
Queue size: 5
Front element: 10
Back element: 50
After pop, front: 20
Is empty? No
Queue contents (front to back): 20 30 40 50
```

### Queue for Task Processing:

```cpp
#include <iostream>
#include <queue>
#include <string>

struct Task {
    std::string name;
    int estimatedTime;  // in minutes

    Task(std::string n, int t) : name(n), estimatedTime(t) {}
};

class TaskProcessor {
private:
    std::queue<Task> tasks;
    int totalTimeProcessed = 0;

public:
    void addTask(const std::string& name, int time) {
        tasks.emplace(name, time);
        std::cout << "Added task: " << name << " (" << time << " min)\n";
    }

    void processNext() {
        if (tasks.empty()) {
            std::cout << "No tasks to process\n";
            return;
        }

        Task current = tasks.front();
        tasks.pop();

        std::cout << "Processing: " << current.name
                  << " (Est. time: " << current.estimatedTime << " min)\n";
        totalTimeProcessed += current.estimatedTime;
    }

    void processAll() {
        std::cout << "\nProcessing all tasks:\n";
        std::cout << "====================\n";
        while (!tasks.empty()) {
            processNext();
        }
        std::cout << "\nTotal time: " << totalTimeProcessed << " minutes\n";
    }

    int remainingTasks() const {
        return tasks.size();
    }
};

int main()
{
    TaskProcessor processor;

    processor.addTask("Write report", 30);
    processor.addTask("Code review", 45);
    processor.addTask("Team meeting", 60);
    processor.addTask("Email responses", 15);

    std::cout << "\nRemaining tasks: " << processor.remainingTasks() << "\n";

    processor.processNext();
    std::cout << "Remaining tasks: " << processor.remainingTasks() << "\n";

    processor.processAll();

    return 0;
}
```

---

## 4. Priority Queue

### What is a Priority Queue?

A **priority queue** is a heap-based structure where elements are ordered by priority. The highest priority element is always at the front.

**Default:** Max heap (largest element has highest priority)

### Creating Priority Queues:

```cpp
#include <iostream>
#include <queue>
#include <vector>

int main()
{
    // Default priority queue (max heap)
    std::priority_queue<int> pq1;

    // Min heap (using greater comparator)
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq2;

    // Max heap with explicit parameters
    std::priority_queue<int, std::vector<int>, std::less<int>> pq3;

    // Add elements
    pq1.push(30);
    pq1.push(10);
    pq1.push(50);
    pq1.push(20);
    pq1.push(40);

    std::cout << "Max heap (top = largest):\n";
    while (!pq1.empty()) {
        std::cout << pq1.top() << " ";
        pq1.pop();
    }
    std::cout << "\n";

    // Min heap
    pq2.push(30);
    pq2.push(10);
    pq2.push(50);
    pq2.push(20);
    pq2.push(40);

    std::cout << "Min heap (top = smallest):\n";
    while (!pq2.empty()) {
        std::cout << pq2.top() << " ";
        pq2.pop();
    }
    std::cout << "\n";

    return 0;
}
```

**Output:**
```
Max heap (top = largest):
50 40 30 20 10
Min heap (top = smallest):
10 20 30 40 50
```

### Priority Queue Operations:

```cpp
#include <iostream>
#include <queue>

int main()
{
    std::priority_queue<int> pq;

    // push() - add element
    pq.push(30);
    pq.push(100);
    pq.push(25);
    pq.push(40);

    std::cout << "Priority queue size: " << pq.size() << "\n";
    std::cout << "Top element (highest priority): " << pq.top() << "\n";

    // pop() - remove highest priority element
    pq.pop();
    std::cout << "After pop, top: " << pq.top() << "\n";

    // emplace() - construct in place
    pq.emplace(75);
    std::cout << "After emplace(75), top: " << pq.top() << "\n";

    // empty() - check if empty
    std::cout << "Is empty? " << (pq.empty() ? "Yes" : "No") << "\n";

    return 0;
}
```

---

## 5. Custom Comparators for Priority Queue

### Struct with Operator Overloading:

```cpp
#include <iostream>
#include <queue>
#include <string>

struct Task {
    std::string name;
    int priority;  // Lower number = higher priority

    Task(std::string n, int p) : name(n), priority(p) {}

    // Operator for max heap behavior (reverse for min heap)
    bool operator<(const Task& other) const {
        return priority > other.priority;  // Reverse: lower number = higher priority
    }
};

int main()
{
    std::priority_queue<Task> tasks;

    tasks.emplace("Low priority task", 3);
    tasks.emplace("Critical bug fix", 1);
    tasks.emplace("Medium task", 2);
    tasks.emplace("Documentation", 4);
    tasks.emplace("Security patch", 1);

    std::cout << "Tasks by priority:\n";
    std::cout << "==================\n";
    while (!tasks.empty()) {
        Task t = tasks.top();
        tasks.pop();
        std::cout << "Priority " << t.priority << ": " << t.name << "\n";
    }

    return 0;
}
```

### Custom Comparator Function:

```cpp
#include <iostream>
#include <queue>
#include <string>

struct Student {
    std::string name;
    double gpa;

    Student(std::string n, double g) : name(n), gpa(g) {}
};

// Comparator for min heap (lowest GPA first)
struct CompareGPA {
    bool operator()(const Student& a, const Student& b) const {
        return a.gpa > b.gpa;  // Reverse for min heap
    }
};

int main()
{
    std::priority_queue<Student, std::vector<Student>, CompareGPA> students;

    students.emplace("Alice", 3.8);
    students.emplace("Bob", 3.2);
    students.emplace("Charlie", 3.9);
    students.emplace("David", 2.8);
    students.emplace("Eve", 3.5);

    std::cout << "Students by GPA (lowest first):\n";
    std::cout << "================================\n";
    while (!students.empty()) {
        Student s = students.top();
        students.pop();
        std::cout << s.name << ": " << s.gpa << "\n";
    }

    return 0;
}
```

### Lambda Comparator:

```cpp
#include <iostream>
#include <queue>
#include <vector>
#include <string>

struct Event {
    std::string name;
    int timestamp;

    Event(std::string n, int t) : name(n), timestamp(t) {}
};

int main()
{
    // Lambda comparator for min heap (earliest timestamp first)
    auto compare = [](const Event& a, const Event& b) {
        return a.timestamp > b.timestamp;
    };

    std::priority_queue<Event, std::vector<Event>, decltype(compare)> events(compare);

    events.emplace("Meeting", 1400);
    events.emplace("Breakfast", 800);
    events.emplace("Lunch", 1200);
    events.emplace("Dinner", 1900);
    events.emplace("Exercise", 600);

    std::cout << "Events in chronological order:\n";
    std::cout << "==============================\n";
    while (!events.empty()) {
        Event e = events.top();
        events.pop();
        std::cout << e.timestamp << ": " << e.name << "\n";
    }

    return 0;
}
```

---

## 6. Real-World Applications

### Application 1: Expression Evaluation

```cpp
#include <iostream>
#include <stack>
#include <string>
#include <cctype>

// Evaluate postfix expression
int evaluatePostfix(const std::string& expression)
{
    std::stack<int> s;

    for (char ch : expression) {
        if (std::isspace(ch)) continue;

        if (std::isdigit(ch)) {
            s.push(ch - '0');
        }
        else {
            int operand2 = s.top(); s.pop();
            int operand1 = s.top(); s.pop();

            switch (ch) {
                case '+': s.push(operand1 + operand2); break;
                case '-': s.push(operand1 - operand2); break;
                case '*': s.push(operand1 * operand2); break;
                case '/': s.push(operand1 / operand2); break;
            }
        }
    }

    return s.top();
}

int main()
{
    std::string expression = "23*54*+9-";  // (2*3) + (5*4) - 9 = 17

    std::cout << "Expression: " << expression << "\n";
    std::cout << "Result: " << evaluatePostfix(expression) << "\n";

    return 0;
}
```

### Application 2: Balanced Parentheses

```cpp
#include <iostream>
#include <stack>
#include <string>

bool isBalanced(const std::string& str)
{
    std::stack<char> s;

    for (char ch : str) {
        if (ch == '(' || ch == '[' || ch == '{') {
            s.push(ch);
        }
        else if (ch == ')' || ch == ']' || ch == '}') {
            if (s.empty()) return false;

            char top = s.top();
            s.pop();

            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                return false;
            }
        }
    }

    return s.empty();
}

int main()
{
    std::string tests[] = {
        "{[()]}",
        "{[(])}",
        "((()))",
        "((())",
        "{[]}()"
    };

    for (const auto& test : tests) {
        std::cout << test << " is "
                  << (isBalanced(test) ? "balanced" : "NOT balanced")
                  << "\n";
    }

    return 0;
}
```

### Application 3: Process Scheduling

```cpp
#include <iostream>
#include <queue>
#include <string>

struct Process {
    std::string name;
    int priority;
    int cpuTime;

    Process(std::string n, int p, int t)
        : name(n), priority(p), cpuTime(t) {}

    bool operator<(const Process& other) const {
        return priority < other.priority;  // Higher number = higher priority
    }
};

class ProcessScheduler {
private:
    std::priority_queue<Process> readyQueue;
    int totalCpuTime = 0;

public:
    void addProcess(const std::string& name, int priority, int cpuTime) {
        readyQueue.emplace(name, priority, cpuTime);
        std::cout << "Added process: " << name
                  << " (Priority: " << priority
                  << ", CPU time: " << cpuTime << "ms)\n";
    }

    void execute() {
        std::cout << "\nExecuting processes by priority:\n";
        std::cout << "=================================\n";

        while (!readyQueue.empty()) {
            Process p = readyQueue.top();
            readyQueue.pop();

            std::cout << "Executing: " << p.name
                      << " (Priority: " << p.priority
                      << ", Time: " << p.cpuTime << "ms)\n";
            totalCpuTime += p.cpuTime;
        }

        std::cout << "\nTotal CPU time: " << totalCpuTime << "ms\n";
    }
};

int main()
{
    ProcessScheduler scheduler;

    scheduler.addProcess("Background Update", 1, 200);
    scheduler.addProcess("User Input", 9, 50);
    scheduler.addProcess("File Download", 3, 150);
    scheduler.addProcess("System Critical", 10, 100);
    scheduler.addProcess("Idle Task", 0, 300);

    scheduler.execute();

    return 0;
}
```

---

## 7. Implementation Details

### Building Your Own Stack:

```cpp
#include <iostream>
#include <deque>

template<typename T, typename Container = std::deque<T>>
class MyStack {
private:
    Container c;

public:
    void push(const T& value) {
        c.push_back(value);
    }

    void pop() {
        if (!c.empty()) {
            c.pop_back();
        }
    }

    T& top() {
        return c.back();
    }

    const T& top() const {
        return c.back();
    }

    bool empty() const {
        return c.empty();
    }

    size_t size() const {
        return c.size();
    }
};

int main()
{
    MyStack<int> s;

    s.push(10);
    s.push(20);
    s.push(30);

    std::cout << "Custom stack size: " << s.size() << "\n";
    std::cout << "Top: " << s.top() << "\n";

    s.pop();
    std::cout << "After pop, top: " << s.top() << "\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Reverse a String

```cpp
#include <iostream>
#include <stack>
#include <string>

std::string reverseString(const std::string& str)
{
    std::stack<char> s;

    // Push all characters
    for (char ch : str) {
        s.push(ch);
    }

    // Pop to build reversed string
    std::string reversed;
    while (!s.empty()) {
        reversed += s.top();
        s.pop();
    }

    return reversed;
}

int main()
{
    std::string original = "Hello, World!";
    std::string reversed = reverseString(original);

    std::cout << "Original: " << original << "\n";
    std::cout << "Reversed: " << reversed << "\n";

    return 0;
}
```

### Exercise 2: Bank Queue Simulation

```cpp
#include <iostream>
#include <queue>
#include <string>

struct Customer {
    std::string name;
    int accountNumber;
    std::string serviceType;

    Customer(std::string n, int acc, std::string service)
        : name(n), accountNumber(acc), serviceType(service) {}
};

class BankQueue {
private:
    std::queue<Customer> regularQueue;
    std::queue<Customer> premiumQueue;

public:
    void addCustomer(const std::string& name, int account,
                     const std::string& service, bool isPremium) {
        Customer c(name, account, service);
        if (isPremium) {
            premiumQueue.push(c);
            std::cout << "Added PREMIUM customer: " << name << "\n";
        } else {
            regularQueue.push(c);
            std::cout << "Added regular customer: " << name << "\n";
        }
    }

    void serveNext() {
        if (!premiumQueue.empty()) {
            Customer c = premiumQueue.front();
            premiumQueue.pop();
            std::cout << "Serving PREMIUM: " << c.name
                      << " (" << c.serviceType << ")\n";
        }
        else if (!regularQueue.empty()) {
            Customer c = regularQueue.front();
            regularQueue.pop();
            std::cout << "Serving regular: " << c.name
                      << " (" << c.serviceType << ")\n";
        }
        else {
            std::cout << "No customers waiting\n";
        }
    }

    void displayStatus() const {
        std::cout << "\nQueue Status:\n";
        std::cout << "Premium customers: " << premiumQueue.size() << "\n";
        std::cout << "Regular customers: " << regularQueue.size() << "\n\n";
    }
};

int main()
{
    BankQueue bank;

    bank.addCustomer("Alice", 1001, "Deposit", false);
    bank.addCustomer("Bob", 2002, "Withdrawal", true);
    bank.addCustomer("Charlie", 3003, "Transfer", false);
    bank.addCustomer("David", 4004, "Loan", true);

    bank.displayStatus();

    for (int i = 0; i < 4; i++) {
        bank.serveNext();
    }

    bank.displayStatus();

    return 0;
}
```

### Project: Call Center System

```cpp
#include <iostream>
#include <queue>
#include <string>

struct Call {
    std::string callerName;
    std::string issue;
    int priority;  // 1=Critical, 2=High, 3=Medium, 4=Low
    int waitTime = 0;

    Call(std::string name, std::string iss, int pri)
        : callerName(name), issue(iss), priority(pri) {}

    bool operator<(const Call& other) const {
        // Higher priority (lower number) comes first
        // If same priority, older call (higher wait time) comes first
        if (priority != other.priority) {
            return priority > other.priority;
        }
        return waitTime < other.waitTime;
    }
};

class CallCenter {
private:
    std::priority_queue<Call> callQueue;
    int callsHandled = 0;

public:
    void receiveCall(const std::string& name, const std::string& issue, int priority) {
        callQueue.emplace(name, issue, priority);
        std::cout << "Call received from " << name
                  << " (Priority: " << priority << ")\n";
    }

    void handleCall() {
        if (callQueue.empty()) {
            std::cout << "No calls in queue\n";
            return;
        }

        Call c = callQueue.top();
        callQueue.pop();
        callsHandled++;

        std::cout << "\nHandling call #" << callsHandled << ":\n";
        std::cout << "  Caller: " << c.callerName << "\n";
        std::cout << "  Issue: " << c.issue << "\n";
        std::cout << "  Priority: " << c.priority << "\n";
        std::cout << "  Wait time: " << c.waitTime << " min\n";
    }

    void simulateWait() {
        // Increase wait time for all calls
        std::priority_queue<Call> temp;
        while (!callQueue.empty()) {
            Call c = callQueue.top();
            callQueue.pop();
            c.waitTime += 5;
            temp.push(c);
        }
        callQueue = temp;
        std::cout << "[Time advanced 5 minutes]\n";
    }

    void displayStats() const {
        std::cout << "\n=== Call Center Stats ===\n";
        std::cout << "Calls in queue: " << callQueue.size() << "\n";
        std::cout << "Calls handled: " << callsHandled << "\n";
    }
};

int main()
{
    CallCenter center;

    center.receiveCall("John", "Internet not working", 3);
    center.receiveCall("Sarah", "No service", 1);
    center.receiveCall("Mike", "Billing question", 4);
    center.receiveCall("Emma", "Service outage", 1);
    center.receiveCall("Tom", "Slow speed", 2);

    center.displayStats();

    std::cout << "\n=== Handling Calls ===\n";
    for (int i = 0; i < 3; i++) {
        center.handleCall();
    }

    center.simulateWait();
    center.displayStats();

    std::cout << "\n=== Continuing ===\n";
    while (true) {
        if (center.displayStats(), true) break;
        center.handleCall();
    }

    return 0;
}
```

---

## Key Takeaways

1. ✅ Stack is LIFO (Last In, First Out) - like a stack of plates
2. ✅ Queue is FIFO (First In, First Out) - like a line at a store
3. ✅ Priority queue orders elements by priority (heap-based)
4. ✅ Container adapters restrict interface for specific purposes
5. ✅ Stack: push, pop, top, empty, size
6. ✅ Queue: push, pop, front, back, empty, size
7. ✅ Priority queue: push, pop, top, empty, size
8. ✅ Can customize underlying container and comparator
9. ✅ pop() returns void - must use top()/front() to get value
10. ✅ Use adapters for clarity and type safety

---

## Summary Checklist

Before moving to Lesson 54, ensure you can:
- [ ] Create and use stacks for LIFO operations
- [ ] Create and use queues for FIFO operations
- [ ] Work with priority queues and custom comparators
- [ ] Choose appropriate underlying containers
- [ ] Implement custom comparators with operators, functors, and lambdas
- [ ] Apply stacks for parentheses matching and expression evaluation
- [ ] Apply queues for task scheduling and processing
- [ ] Apply priority queues for prioritized scheduling
- [ ] Understand when to use each adapter
- [ ] Build real-world applications with container adapters

---

## Next Lesson Preview

In **Lesson 54**, we'll learn about:
- Set and multiset containers
- Ordered associative containers
- Automatic sorting
- Set operations (union, intersection, difference)
- When to use sets vs vectors

**Excellent work mastering container adapters!**
