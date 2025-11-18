# C++ Quick Reference Guide

## Essential C++ Syntax Cheat Sheet

### Data Types

```cpp
// Integer types
char c = 'A';              // 1 byte
short s = 100;             // 2 bytes
int i = 1000;              // 4 bytes
long l = 100000L;          // 4-8 bytes
long long ll = 1000000LL;  // 8 bytes

// Unsigned variants
unsigned int ui = 100;
unsigned long ul = 1000UL;

// Floating-point
float f = 3.14f;           // 4 bytes
double d = 3.14159;        // 8 bytes
long double ld = 3.14159L; // 8-16 bytes

// Boolean
bool flag = true;          // 1 byte

// String
std::string str = "Hello";
```

### Variables and Constants

```cpp
// Declaration
int x;
int y = 10;
int z{20};  // Uniform initialization (C++11)

// Constants
const int MAX = 100;
constexpr int SIZE = 50;  // Compile-time constant

// Auto type deduction
auto value = 42;        // int
auto pi = 3.14;         // double
```

### Operators

```cpp
// Arithmetic
+ - * / %

// Comparison
== != < > <= >=

// Logical
&& || !

// Assignment
= += -= *= /= %=

// Increment/Decrement
++ --

// Ternary
condition ? true_value : false_value

// Bitwise
& | ^ ~ << >>
```

### Control Flow

```cpp
// If-else
if (condition) {
    // code
} else if (other_condition) {
    // code
} else {
    // code
}

// Switch
switch (variable) {
    case value1:
        // code
        break;
    case value2:
        // code
        break;
    default:
        // code
}

// Ternary operator
result = (x > y) ? x : y;
```

### Loops

```cpp
// For loop
for (int i = 0; i < 10; i++) {
    // code
}

// Range-based for (C++11)
for (auto& element : container) {
    // code
}

// While loop
while (condition) {
    // code
}

// Do-while loop
do {
    // code
} while (condition);

// Loop control
break;      // Exit loop
continue;   // Skip to next iteration
```

### Functions

```cpp
// Function declaration
return_type function_name(parameter_type param);

// Function definition
int add(int a, int b) {
    return a + b;
}

// Function with default parameters
void print(int x = 0, int y = 0);

// Function overloading
void display(int x);
void display(double x);
void display(std::string x);

// Lambda expression (C++11)
auto lambda = [](int x) { return x * 2; };
auto result = lambda(5);  // 10
```

### Arrays

```cpp
// C-style array
int arr[5] = {1, 2, 3, 4, 5};

// Access elements
int first = arr[0];

// Array size
int size = sizeof(arr) / sizeof(arr[0]);

// 2D array
int matrix[3][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};
```

### Pointers and References

```cpp
// Pointer
int* ptr = &variable;
int value = *ptr;  // Dereference

// Reference
int& ref = variable;

// Dynamic allocation
int* p = new int;
int* arr = new int[10];

// Deallocation
delete p;
delete[] arr;

// Smart pointers (C++11)
std::unique_ptr<int> up = std::make_unique<int>(42);
std::shared_ptr<int> sp = std::make_shared<int>(42);
```

### Structures and Classes

```cpp
// Structure
struct Point {
    int x;
    int y;
};

// Class
class Rectangle {
private:
    int width;
    int height;

public:
    // Constructor
    Rectangle(int w, int h) : width(w), height(h) {}

    // Member function
    int area() const {
        return width * height;
    }

    // Getter
    int getWidth() const { return width; }

    // Setter
    void setWidth(int w) { width = w; }
};

// Usage
Rectangle rect(10, 20);
int area = rect.area();
```

### STL Containers

```cpp
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>

// Vector (dynamic array)
std::vector<int> vec = {1, 2, 3, 4, 5};
vec.push_back(6);
vec.pop_back();
int size = vec.size();

// Map (key-value pairs)
std::map<std::string, int> ages;
ages["Alice"] = 25;
ages["Bob"] = 30;

// Set (unique elements)
std::set<int> numbers = {1, 2, 3, 4, 5};
numbers.insert(6);

// Queue
std::queue<int> q;
q.push(1);
int front = q.front();
q.pop();

// Stack
std::stack<int> s;
s.push(1);
int top = s.top();
s.pop();
```

### STL Algorithms

```cpp
#include <algorithm>

std::vector<int> vec = {5, 2, 8, 1, 9};

// Sort
std::sort(vec.begin(), vec.end());

// Find
auto it = std::find(vec.begin(), vec.end(), 8);

// Count
int count = std::count(vec.begin(), vec.end(), 2);

// For each
std::for_each(vec.begin(), vec.end(), [](int x) {
    std::cout << x << " ";
});

// Transform
std::transform(vec.begin(), vec.end(), vec.begin(), [](int x) {
    return x * 2;
});
```

### File I/O

```cpp
#include <fstream>

// Write to file
std::ofstream outFile("output.txt");
outFile << "Hello, File!" << std::endl;
outFile.close();

// Read from file
std::ifstream inFile("input.txt");
std::string line;
while (std::getline(inFile, line)) {
    std::cout << line << std::endl;
}
inFile.close();

// Both read and write
std::fstream file("data.txt", std::ios::in | std::ios::out);
```

### Exception Handling

```cpp
try {
    // Code that might throw
    if (error_condition) {
        throw std::runtime_error("Error message");
    }
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
} catch (...) {
    std::cerr << "Unknown error" << std::endl;
}
```

### Templates

```cpp
// Function template
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// Class template
template<typename T>
class Container {
private:
    T value;

public:
    Container(T v) : value(v) {}
    T getValue() const { return value; }
};

// Usage
int max_int = maximum(5, 10);
Container<double> c(3.14);
```

### Modern C++ Features

```cpp
// Auto (C++11)
auto x = 42;
auto str = "Hello";

// Range-based for (C++11)
for (const auto& item : container) {
    // code
}

// Lambda (C++11)
auto lambda = [capture](params) -> return_type {
    // code
};

// Smart pointers (C++11)
std::unique_ptr<int> up = std::make_unique<int>(42);
std::shared_ptr<int> sp = std::make_shared<int>(42);

// Move semantics (C++11)
std::vector<int> v1 = {1, 2, 3};
std::vector<int> v2 = std::move(v1);  // v1 is now empty

// Nullptr (C++11)
int* ptr = nullptr;

// Structured bindings (C++17)
auto [x, y] = std::make_pair(1, 2);

// If with initializer (C++17)
if (auto it = map.find(key); it != map.end()) {
    // use it
}
```

---

## Qt Quick Reference

### Basic Qt Application

```cpp
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QPushButton button("Click Me!");
    button.show();

    return app.exec();
}
```

### Signals and Slots

```cpp
// Define signal and slot
class MyClass : public QObject {
    Q_OBJECT

signals:
    void mySignal(int value);

public slots:
    void mySlot(int value) {
        qDebug() << "Received:" << value;
    }
};

// Connect signal to slot
connect(sender, &MyClass::mySignal,
        receiver, &MyClass::mySlot);

// Emit signal
emit mySignal(42);
```

### Common Qt Widgets

```cpp
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QSlider>
#include <QProgressBar>
```

### Qt Layouts

```cpp
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

QVBoxLayout *layout = new QVBoxLayout;
layout->addWidget(widget1);
layout->addWidget(widget2);
window->setLayout(layout);
```

---

## OpenGL Quick Reference

### Basic OpenGL Setup

```cpp
// Vertex shader
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment shader
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";
```

### VBO/VAO Setup

```cpp
unsigned int VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

### Drawing

```cpp
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

---

## Compilation Commands

### Basic Compilation

```bash
# Simple compilation
g++ main.cpp -o program

# With C++20 standard
g++ -std=c++20 main.cpp -o program

# With warnings
g++ -Wall -Wextra main.cpp -o program

# With optimization
g++ -O2 main.cpp -o program

# With debugging symbols
g++ -g main.cpp -o program
```

### Qt Compilation

```bash
# Generate Makefile
qmake project.pro

# Or use CMake
cmake .
make
```

### OpenGL Compilation

```bash
# Linux
g++ main.cpp -lGL -lGLU -lglut -o program

# With GLEW
g++ main.cpp -lGL -lGLEW -lglfw -o program
```

---

## Common Patterns

### RAII Pattern

```cpp
class Resource {
public:
    Resource() { /* acquire */ }
    ~Resource() { /* release */ }
};
```

### Singleton Pattern

```cpp
class Singleton {
private:
    Singleton() {}

public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    void operator=(const Singleton&) = delete;
};
```

### Observer Pattern

```cpp
class Observer {
public:
    virtual void update() = 0;
};

class Subject {
private:
    std::vector<Observer*> observers;

public:
    void attach(Observer* obs) {
        observers.push_back(obs);
    }

    void notify() {
        for (auto* obs : observers) {
            obs->update();
        }
    }
};
```

---

## Debugging Tips

```cpp
// Print debugging
std::cout << "Debug: value = " << value << std::endl;

// Assert
#include <cassert>
assert(condition && "Error message");

// Static assert (compile-time)
static_assert(sizeof(int) == 4, "Int must be 4 bytes");
```

---

## Performance Tips

1. **Use references for large objects**
   ```cpp
   void process(const std::vector<int>& data);  // Good
   void process(std::vector<int> data);         // Bad (copy)
   ```

2. **Reserve vector capacity**
   ```cpp
   std::vector<int> vec;
   vec.reserve(1000);  // Avoid reallocations
   ```

3. **Use move semantics**
   ```cpp
   std::vector<int> v1 = getData();
   std::vector<int> v2 = std::move(v1);  // No copy
   ```

4. **Prefer ++i over i++**
   ```cpp
   for (int i = 0; i < n; ++i) {  // Slightly faster
   ```

---

**For more details, see the individual lessons!**
