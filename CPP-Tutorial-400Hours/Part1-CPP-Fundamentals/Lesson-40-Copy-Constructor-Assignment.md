# Lesson 40: Copy Constructor and Assignment Operator

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Understanding Copying
2. Shallow vs Deep Copy
3. Copy Constructor
4. Copy Assignment Operator
5. Rule of Three
6. Move Semantics Introduction
7. Rule of Five
8. Exercises
9. Practice Projects

---

## 1. Understanding Copying

When objects are copied, C++ provides default behavior. However, for classes managing resources, you need custom implementations.

### When Copying Happens:
```cpp
MyClass obj1;
MyClass obj2 = obj1;      // Copy constructor
MyClass obj3(obj1);       // Copy constructor
MyClass obj4; obj4 = obj1; // Copy assignment operator
```

### Default Behavior:
C++ provides default copy constructor and assignment operator that perform **memberwise copy**.

---

## 2. Shallow vs Deep Copy

### Shallow Copy (Default)
```cpp
class ShallowExample {
private:
    int* data;

public:
    ShallowExample(int value) {
        data = new int(value);
    }

    // Using default copy - DANGEROUS!
    // Both objects will point to same memory
};

int main() {
    ShallowExample obj1(10);
    ShallowExample obj2 = obj1;  // Shallow copy - both point to same data!
    // PROBLEM: When destructors run, double delete!
}
```

### Deep Copy (Custom Implementation)
```cpp
class DeepExample {
private:
    int* data;

public:
    DeepExample(int value) {
        data = new int(value);
    }

    // Deep copy constructor
    DeepExample(const DeepExample& other) {
        data = new int(*other.data);  // Create new memory
    }

    ~DeepExample() {
        delete data;
    }
};
```

### Visual Comparison:

```
Shallow Copy:
obj1.data ──┐
            ├──> [42]
obj2.data ──┘

Deep Copy:
obj1.data ──> [42]
obj2.data ──> [42]  (different memory)
```

---

## 3. Copy Constructor

**Copy constructor** creates a new object as a copy of an existing object.

### Syntax:
```cpp
class ClassName {
public:
    ClassName(const ClassName& other);  // Copy constructor
};
```

### Example 1: Basic Copy Constructor

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    char* data;
    int length;

public:
    // Constructor
    String(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        cout << "Constructor: Created string \"" << data << "\"" << endl;
    }

    // Copy constructor
    String(const String& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        cout << "Copy Constructor: Copied \"" << data << "\"" << endl;
    }

    // Destructor
    ~String() {
        cout << "Destructor: Deleting \"" << data << "\"" << endl;
        delete[] data;
    }

    void display() const {
        cout << data;
    }
};

int main() {
    String s1("Hello");
    String s2 = s1;      // Copy constructor
    String s3(s1);       // Copy constructor

    cout << "s1: "; s1.display(); cout << endl;
    cout << "s2: "; s2.display(); cout << endl;
    cout << "s3: "; s3.display(); cout << endl;

    return 0;
}
```

**Output**:
```
Constructor: Created string "Hello"
Copy Constructor: Copied "Hello"
Copy Constructor: Copied "Hello"
s1: Hello
s2: Hello
s3: Hello
Destructor: Deleting "Hello"
Destructor: Deleting "Hello"
Destructor: Deleting "Hello"
```

### Example 2: Dynamic Array Copy

```cpp
#include <iostream>
using namespace std;

class IntArray {
private:
    int* data;
    int size;

public:
    IntArray(int s) : size(s) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
        cout << "Array of size " << size << " created" << endl;
    }

    // Copy constructor - Deep copy
    IntArray(const IntArray& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "Array copied (size " << size << ")" << endl;
    }

    ~IntArray() {
        delete[] data;
        cout << "Array of size " << size << " destroyed" << endl;
    }

    void set(int index, int value) {
        if (index >= 0 && index < size) {
            data[index] = value;
        }
    }

    int get(int index) const {
        if (index >= 0 && index < size) {
            return data[index];
        }
        return 0;
    }

    void display() const {
        cout << "[";
        for (int i = 0; i < size; i++) {
            cout << data[i];
            if (i < size - 1) cout << ", ";
        }
        cout << "]";
    }
};

int main() {
    IntArray arr1(5);
    arr1.set(0, 10);
    arr1.set(1, 20);
    arr1.set(2, 30);

    cout << "arr1: "; arr1.display(); cout << endl;

    IntArray arr2 = arr1;  // Copy constructor
    cout << "arr2: "; arr2.display(); cout << endl;

    // Modify arr2
    arr2.set(0, 999);
    cout << "\nAfter modifying arr2:" << endl;
    cout << "arr1: "; arr1.display(); cout << endl;
    cout << "arr2: "; arr2.display(); cout << endl;

    return 0;
}
```

---

## 4. Copy Assignment Operator

**Copy assignment operator** copies an existing object to an already-existing object.

### Syntax:
```cpp
class ClassName {
public:
    ClassName& operator=(const ClassName& other);
};
```

### Example 1: Basic Assignment Operator

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    char* data;

public:
    String(const char* str = "") {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    // Copy constructor
    String(const String& other) {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
    }

    // Copy assignment operator
    String& operator=(const String& other) {
        cout << "Copy assignment operator called" << endl;

        // 1. Check for self-assignment
        if (this == &other) {
            return *this;
        }

        // 2. Free existing resource
        delete[] data;

        // 3. Allocate new resource and copy
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);

        // 4. Return *this
        return *this;
    }

    ~String() {
        delete[] data;
    }

    void display() const {
        cout << data;
    }
};

int main() {
    String s1("Hello");
    String s2("World");

    cout << "Before assignment:" << endl;
    cout << "s1: "; s1.display(); cout << endl;
    cout << "s2: "; s2.display(); cout << endl;

    s2 = s1;  // Assignment operator

    cout << "\nAfter s2 = s1:" << endl;
    cout << "s1: "; s1.display(); cout << endl;
    cout << "s2: "; s2.display(); cout << endl;

    // Chaining
    String s3("Test");
    s3 = s2 = s1;

    return 0;
}
```

### Example 2: Complete Implementation

```cpp
#include <iostream>
using namespace std;

class Resource {
private:
    int* data;
    int size;

public:
    Resource(int s = 0) : size(s) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
        cout << "Resource(" << size << ") constructed" << endl;
    }

    // Copy constructor
    Resource(const Resource& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "Resource copied" << endl;
    }

    // Copy assignment operator
    Resource& operator=(const Resource& other) {
        cout << "Copy assignment" << endl;

        if (this != &other) {  // Self-assignment check
            // Free old resource
            delete[] data;

            // Copy size
            size = other.size;

            // Allocate new resource
            data = new int[size];

            // Copy data
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }

        return *this;
    }

    ~Resource() {
        delete[] data;
        cout << "Resource destroyed" << endl;
    }

    void set(int index, int value) {
        if (index >= 0 && index < size) {
            data[index] = value;
        }
    }

    int get(int index) const {
        return (index >= 0 && index < size) ? data[index] : 0;
    }

    void display() const {
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
    }
};

int main() {
    Resource r1(5);
    r1.set(0, 10);
    r1.set(1, 20);

    Resource r2(3);
    r2 = r1;  // Assignment

    cout << "r1: "; r1.display(); cout << endl;
    cout << "r2: "; r2.display(); cout << endl;

    // Self-assignment
    r1 = r1;  // Should handle gracefully

    return 0;
}
```

---

## 5. Rule of Three

**Rule of Three**: If you need to explicitly define one of the following, you should define all three:

1. Destructor
2. Copy constructor
3. Copy assignment operator

### Example: Complete Rule of Three

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Buffer {
private:
    char* data;
    int capacity;

public:
    // 1. Constructor
    Buffer(int cap = 100) : capacity(cap) {
        data = new char[capacity];
        data[0] = '\0';
        cout << "Buffer created (capacity: " << capacity << ")" << endl;
    }

    // 2. Destructor
    ~Buffer() {
        delete[] data;
        cout << "Buffer destroyed" << endl;
    }

    // 3. Copy constructor
    Buffer(const Buffer& other) : capacity(other.capacity) {
        data = new char[capacity];
        strcpy(data, other.data);
        cout << "Buffer copied" << endl;
    }

    // 4. Copy assignment operator
    Buffer& operator=(const Buffer& other) {
        cout << "Buffer assigned" << endl;

        if (this != &other) {
            delete[] data;

            capacity = other.capacity;
            data = new char[capacity];
            strcpy(data, other.data);
        }

        return *this;
    }

    void append(const char* str) {
        strcat(data, str);
    }

    const char* getData() const {
        return data;
    }
};

int main() {
    Buffer b1(200);
    b1.append("Hello");

    Buffer b2 = b1;       // Copy constructor
    Buffer b3(100);
    b3 = b1;              // Assignment operator

    cout << "b1: " << b1.getData() << endl;
    cout << "b2: " << b2.getData() << endl;
    cout << "b3: " << b3.getData() << endl;

    return 0;
}
```

---

## 6. Move Semantics Introduction

**Move semantics** (C++11) allows transfer of resources instead of copying.

### Example: Basic Move

```cpp
#include <iostream>
using namespace std;

class MovableResource {
private:
    int* data;
    int size;

public:
    MovableResource(int s) : size(s) {
        data = new int[size];
        cout << "Resource created (size: " << size << ")" << endl;
    }

    ~MovableResource() {
        delete[] data;
        cout << "Resource destroyed" << endl;
    }

    // Copy constructor
    MovableResource(const MovableResource& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "Resource copied" << endl;
    }

    // Move constructor
    MovableResource(MovableResource&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "Resource moved" << endl;
    }

    // Copy assignment
    MovableResource& operator=(const MovableResource& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        cout << "Resource copy-assigned" << endl;
        return *this;
    }

    // Move assignment
    MovableResource& operator=(MovableResource&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        cout << "Resource move-assigned" << endl;
        return *this;
    }
};

MovableResource createResource(int size) {
    return MovableResource(size);  // Move, not copy
}

int main() {
    MovableResource r1 = createResource(100);  // Move

    MovableResource r2 = move(r1);  // Explicit move

    return 0;
}
```

---

## 7. Rule of Five

**Rule of Five** (C++11): If you define any of the five special member functions, consider defining all:

1. Destructor
2. Copy constructor
3. Copy assignment operator
4. Move constructor
5. Move assignment operator

### Example: Complete Rule of Five

```cpp
#include <iostream>
#include <algorithm>
using namespace std;

class DynamicArray {
private:
    int* data;
    int size;

public:
    // 1. Constructor
    DynamicArray(int s = 0) : size(s) {
        data = size > 0 ? new int[size] : nullptr;
        cout << "DynamicArray(" << size << ") constructed" << endl;
    }

    // 2. Destructor
    ~DynamicArray() {
        delete[] data;
        cout << "DynamicArray destroyed" << endl;
    }

    // 3. Copy constructor
    DynamicArray(const DynamicArray& other) : size(other.size) {
        data = size > 0 ? new int[size] : nullptr;
        copy(other.data, other.data + size, data);
        cout << "Copy constructor" << endl;
    }

    // 4. Copy assignment
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = size > 0 ? new int[size] : nullptr;
            copy(other.data, other.data + size, data);
        }
        cout << "Copy assignment" << endl;
        return *this;
    }

    // 5. Move constructor
    DynamicArray(DynamicArray&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "Move constructor" << endl;
    }

    // 6. Move assignment
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        cout << "Move assignment" << endl;
        return *this;
    }

    int getSize() const { return size; }
};

int main() {
    DynamicArray arr1(10);

    DynamicArray arr2 = arr1;          // Copy constructor
    DynamicArray arr3 = move(arr1);    // Move constructor

    DynamicArray arr4(5);
    arr4 = arr2;                       // Copy assignment
    arr4 = DynamicArray(20);           // Move assignment

    return 0;
}
```

---

## 8. Exercises

### Exercise 1: Linked List Node
```cpp
class Node {
private:
    int data;
    Node* next;

public:
    Node(int val);
    Node(const Node& other);  // Deep copy entire chain
    Node& operator=(const Node& other);
    ~Node();
};
```

### Exercise 2: Matrix Class
```cpp
class Matrix {
private:
    double** data;
    int rows, cols;

public:
    Matrix(int r, int c);
    ~Matrix();
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;
};
```

### Exercise 3: Smart String
```cpp
class SmartString {
private:
    char* buffer;
    int length;
    int capacity;

public:
    // Implement Rule of Five
};
```

---

## 9. Practice Projects

### Project: Complete String Class

```cpp
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

class String {
private:
    char* data;
    int length;
    int capacity;

public:
    // Constructor
    String(const char* str = "") {
        length = strlen(str);
        capacity = length + 1;
        data = new char[capacity];
        strcpy(data, str);
        cout << "String(\"" << data << "\") constructed" << endl;
    }

    // Destructor
    ~String() {
        cout << "String(\"" << (data ? data : "null") << "\") destroyed" << endl;
        delete[] data;
    }

    // Copy constructor
    String(const String& other) {
        length = other.length;
        capacity = other.capacity;
        data = new char[capacity];
        strcpy(data, other.data);
        cout << "String copied" << endl;
    }

    // Copy assignment
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            capacity = other.capacity;
            data = new char[capacity];
            strcpy(data, other.data);
        }
        cout << "String copy-assigned" << endl;
        return *this;
    }

    // Move constructor
    String(String&& other) noexcept
        : data(other.data), length(other.length), capacity(other.capacity) {
        other.data = nullptr;
        other.length = 0;
        other.capacity = 0;
        cout << "String moved" << endl;
    }

    // Move assignment
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            length = other.length;
            capacity = other.capacity;
            other.data = nullptr;
            other.length = 0;
            other.capacity = 0;
        }
        cout << "String move-assigned" << endl;
        return *this;
    }

    // Concatenation
    String& operator+=(const String& other) {
        int newLength = length + other.length;
        if (newLength + 1 > capacity) {
            int newCapacity = (newLength + 1) * 2;
            char* newData = new char[newCapacity];
            strcpy(newData, data);
            strcat(newData, other.data);
            delete[] data;
            data = newData;
            capacity = newCapacity;
        } else {
            strcat(data, other.data);
        }
        length = newLength;
        return *this;
    }

    String operator+(const String& other) const {
        String result = *this;
        result += other;
        return result;
    }

    const char* c_str() const { return data; }
    int size() const { return length; }

    friend ostream& operator<<(ostream& out, const String& str) {
        out << (str.data ? str.data : "");
        return out;
    }
};

String createString(const char* str) {
    return String(str);  // Will use move, not copy
}

int main() {
    cout << "=== Creating strings ===" << endl;
    String s1("Hello");
    String s2(" World");

    cout << "\n=== Copy operations ===" << endl;
    String s3 = s1;       // Copy constructor
    String s4("Test");
    s4 = s2;              // Copy assignment

    cout << "\n=== Move operations ===" << endl;
    String s5 = createString("Temporary");  // Move
    String s6 = move(s1);                   // Move constructor

    cout << "\n=== String operations ===" << endl;
    cout << "s2: " << s2 << endl;
    cout << "s3: " << s3 << endl;

    String s7 = s3 + s2;
    cout << "s3 + s2: " << s7 << endl;

    cout << "\n=== Destruction ===" << endl;
    return 0;
}
```

---

## Key Takeaways

1. **Rule of Three**: Destructor, copy constructor, copy assignment
2. **Rule of Five**: Add move constructor and move assignment
3. **Self-assignment**: Always check in assignment operator
4. **Deep copy**: Required for dynamically allocated resources
5. **Return *this**: From assignment operators for chaining
6. **noexcept**: Mark move operations as noexcept
7. **Move semantics**: Transfer resources instead of copying
8. **nullptr**: Set moved-from pointers to nullptr

---

## Summary Checklist

Before moving to Lesson 41, ensure you can:
- [ ] Understand shallow vs deep copy
- [ ] Implement copy constructor
- [ ] Implement copy assignment operator
- [ ] Handle self-assignment
- [ ] Apply Rule of Three
- [ ] Understand move semantics
- [ ] Implement move constructor
- [ ] Apply Rule of Five

---

## Next Lesson Preview

In **Lesson 41: Inheritance Basics**, we'll learn:
- What inheritance is
- Base and derived classes
- Protected members in inheritance
- Constructor and destructor order
- Types of inheritance relationships

Excellent! You now understand copy and move semantics!
