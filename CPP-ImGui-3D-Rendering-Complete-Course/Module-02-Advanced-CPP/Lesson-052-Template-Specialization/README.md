# Lesson 52: Template Specialization

**Duration:** 2 hours

## Learning Objectives
- Understand full template specialization
- Learn partial template specialization
- Master function template specialization
- Implement type-specific optimizations
- Handle special cases in generic code

## Introduction

Template specialization allows you to provide custom implementations for specific types or sets of types. This is essential when the generic implementation doesn't work well for certain types or when you need type-specific optimizations.

## Full Class Template Specialization

### Basic Specialization

```cpp
#include <iostream>
#include <cstring>

// Primary template
template <typename T>
class Storage {
private:
    T value;

public:
    Storage(T v) : value(v) {}

    void print() const {
        std::cout << "Generic: " << value << std::endl;
    }

    T getValue() const { return value; }
};

// Full specialization for char*
template <>
class Storage<char*> {
private:
    char* value;

public:
    Storage(char* v) {
        value = new char[strlen(v) + 1];
        strcpy(value, v);
    }

    ~Storage() {
        delete[] value;
    }

    Storage(const Storage& other) {
        value = new char[strlen(other.value) + 1];
        strcpy(value, other.value);
    }

    void print() const {
        std::cout << "Specialized for char*: " << value << std::endl;
    }

    char* getValue() const { return value; }
};

int main() {
    Storage<int> intStorage(42);
    intStorage.print();

    Storage<double> doubleStorage(3.14);
    doubleStorage.print();

    char str[] = "Hello";
    Storage<char*> strStorage(str);
    strStorage.print();

    return 0;
}
```

**Output:**
```
Generic: 42
Generic: 3.14
Specialized for char*: Hello
```

### Specialization for bool

```cpp
#include <iostream>
#include <vector>

// Primary template
template <typename T>
class Array {
private:
    std::vector<T> data;

public:
    void add(T value) {
        data.push_back(value);
    }

    T get(size_t index) const {
        return data[index];
    }

    size_t size() const {
        return data.size();
    }

    void print() const {
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

// Specialization for bool (bit-packed storage)
template <>
class Array<bool> {
private:
    std::vector<unsigned char> bytes;
    size_t count;

    static const size_t BITS_PER_BYTE = 8;

public:
    Array() : count(0) {}

    void add(bool value) {
        if (count % BITS_PER_BYTE == 0) {
            bytes.push_back(0);
        }

        if (value) {
            size_t byteIndex = count / BITS_PER_BYTE;
            size_t bitIndex = count % BITS_PER_BYTE;
            bytes[byteIndex] |= (1 << bitIndex);
        }

        ++count;
    }

    bool get(size_t index) const {
        if (index >= count) {
            throw std::out_of_range("Index out of bounds");
        }

        size_t byteIndex = index / BITS_PER_BYTE;
        size_t bitIndex = index % BITS_PER_BYTE;
        return (bytes[byteIndex] & (1 << bitIndex)) != 0;
    }

    size_t size() const {
        return count;
    }

    void print() const {
        for (size_t i = 0; i < count; ++i) {
            std::cout << (get(i) ? "true" : "false") << " ";
        }
        std::cout << std::endl;
        std::cout << "Memory: " << bytes.size() << " bytes for "
                  << count << " bools" << std::endl;
    }
};

int main() {
    Array<int> intArray;
    intArray.add(10);
    intArray.add(20);
    intArray.add(30);
    std::cout << "Int array: ";
    intArray.print();

    Array<bool> boolArray;
    for (int i = 0; i < 20; ++i) {
        boolArray.add(i % 2 == 0);
    }
    std::cout << "\nBool array: ";
    boolArray.print();

    return 0;
}
```

## Partial Template Specialization

### Pointer Specialization

```cpp
#include <iostream>

// Primary template
template <typename T>
class SmartPointer {
private:
    T value;

public:
    SmartPointer(T v) : value(v) {}

    void print() const {
        std::cout << "Value: " << value << std::endl;
    }
};

// Partial specialization for pointer types
template <typename T>
class SmartPointer<T*> {
private:
    T* ptr;
    bool owner;

public:
    SmartPointer(T* p, bool own = true) : ptr(p), owner(own) {}

    ~SmartPointer() {
        if (owner && ptr) {
            delete ptr;
        }
    }

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    void print() const {
        if (ptr) {
            std::cout << "Pointer to: " << *ptr << std::endl;
        } else {
            std::cout << "Null pointer" << std::endl;
        }
    }

    T* get() const { return ptr; }
};

int main() {
    SmartPointer<int> regular(42);
    regular.print();

    SmartPointer<int*> smart(new int(100));
    smart.print();
    std::cout << "Value via operator*: " << *smart << std::endl;

    return 0;
}
```

### Multiple Type Parameters

```cpp
#include <iostream>
#include <vector>

// Primary template
template <typename T, typename Container>
class Adapter {
private:
    Container container;

public:
    void add(T item) {
        container.push_back(item);
    }

    void print() const {
        std::cout << "Generic adapter: ";
        for (const auto& item : container) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

// Partial specialization for vector container
template <typename T>
class Adapter<T, std::vector<T>> {
private:
    std::vector<T> container;

public:
    void add(T item) {
        container.push_back(item);
    }

    void print() const {
        std::cout << "Vector adapter (optimized): ";
        for (const auto& item : container) {
            std::cout << item << " ";
        }
        std::cout << " [size: " << container.size()
                  << ", capacity: " << container.capacity() << "]"
                  << std::endl;
    }

    void reserve(size_t n) {
        container.reserve(n);
    }
};

int main() {
    Adapter<int, std::vector<int>> adapter;
    adapter.reserve(10);  // Available in specialized version
    adapter.add(1);
    adapter.add(2);
    adapter.add(3);
    adapter.print();

    return 0;
}
```

## Function Template Specialization

```cpp
#include <iostream>
#include <cstring>

// Primary template
template <typename T>
bool isEqual(T a, T b) {
    std::cout << "Generic comparison" << std::endl;
    return a == b;
}

// Specialization for C-strings
template <>
bool isEqual<const char*>(const char* a, const char* b) {
    std::cout << "C-string comparison" << std::endl;
    return strcmp(a, b) == 0;
}

// Specialization for float (epsilon comparison)
template <>
bool isEqual<float>(float a, float b) {
    std::cout << "Float comparison with epsilon" << std::endl;
    const float epsilon = 0.0001f;
    return std::abs(a - b) < epsilon;
}

int main() {
    std::cout << "42 == 42: " << isEqual(42, 42) << std::endl;

    const char* s1 = "hello";
    const char* s2 = "hello";
    std::cout << "\"hello\" == \"hello\": " << isEqual(s1, s2) << std::endl;

    float f1 = 3.14159f;
    float f2 = 3.14160f;
    std::cout << "3.14159 == 3.14160: " << isEqual(f1, f2) << std::endl;

    return 0;
}
```

## Practical Example 1: Type Traits

```cpp
#include <iostream>

// Primary template - false by default
template <typename T>
struct IsPointer {
    static const bool value = false;
};

// Specialization for pointer types
template <typename T>
struct IsPointer<T*> {
    static const bool value = true;
};

// Primary template for checking if type is integral
template <typename T>
struct IsIntegral {
    static const bool value = false;
};

// Specializations for integral types
template <>
struct IsIntegral<int> {
    static const bool value = true;
};

template <>
struct IsIntegral<long> {
    static const bool value = true;
};

template <>
struct IsIntegral<short> {
    static const bool value = true;
};

template <>
struct IsIntegral<char> {
    static const bool value = true;
};

// Template function that uses type traits
template <typename T>
void processValue(T value) {
    std::cout << "Value: " << value << std::endl;
    std::cout << "Is pointer: " << (IsPointer<T>::value ? "Yes" : "No") << std::endl;
    std::cout << "Is integral: " << (IsIntegral<T>::value ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
}

int main() {
    int x = 42;
    int* px = &x;
    double d = 3.14;

    processValue(x);
    processValue(px);
    processValue(d);

    return 0;
}
```

## Practical Example 2: Serializer

```cpp
#include <iostream>
#include <sstream>
#include <vector>

// Primary template
template <typename T>
class Serializer {
public:
    static std::string serialize(const T& obj) {
        std::ostringstream oss;
        oss << obj;
        return oss.str();
    }

    static T deserialize(const std::string& str) {
        std::istringstream iss(str);
        T obj;
        iss >> obj;
        return obj;
    }
};

// Specialization for std::vector
template <typename T>
class Serializer<std::vector<T>> {
public:
    static std::string serialize(const std::vector<T>& vec) {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            oss << Serializer<T>::serialize(vec[i]);
            if (i < vec.size() - 1) {
                oss << ",";
            }
        }
        oss << "]";
        return oss.str();
    }

    static std::vector<T> deserialize(const std::string& str) {
        std::vector<T> result;
        // Simplified deserialization
        std::string content = str.substr(1, str.length() - 2);
        std::istringstream iss(content);
        std::string item;
        while (std::getline(iss, item, ',')) {
            result.push_back(Serializer<T>::deserialize(item));
        }
        return result;
    }
};

int main() {
    // Serialize int
    int num = 42;
    std::string serialized = Serializer<int>::serialize(num);
    std::cout << "Serialized int: " << serialized << std::endl;

    // Serialize vector
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::string vecSerialized = Serializer<std::vector<int>>::serialize(vec);
    std::cout << "Serialized vector: " << vecSerialized << std::endl;

    // Deserialize
    std::vector<int> vecDeserialized =
        Serializer<std::vector<int>>::deserialize(vecSerialized);
    std::cout << "Deserialized vector: ";
    for (int val : vecDeserialized) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## Practical Example 3: Custom Allocator

```cpp
#include <iostream>
#include <cstring>

// Primary template
template <typename T>
class CustomAllocator {
public:
    static T* allocate(size_t n) {
        std::cout << "Generic allocation of " << n << " elements" << std::endl;
        return new T[n];
    }

    static void deallocate(T* ptr) {
        std::cout << "Generic deallocation" << std::endl;
        delete[] ptr;
    }

    static void construct(T* ptr, const T& value) {
        new (ptr) T(value);
    }
};

// Specialization for char to use C-style allocation
template <>
class CustomAllocator<char> {
public:
    static char* allocate(size_t n) {
        std::cout << "C-style allocation of " << n << " chars" << std::endl;
        return static_cast<char*>(malloc(n));
    }

    static void deallocate(char* ptr) {
        std::cout << "C-style deallocation" << std::endl;
        free(ptr);
    }

    static void construct(char* ptr, const char& value) {
        *ptr = value;
    }
};

int main() {
    // Generic allocation
    int* intArray = CustomAllocator<int>::allocate(10);
    CustomAllocator<int>::construct(&intArray[0], 42);
    std::cout << "First int: " << intArray[0] << std::endl;
    CustomAllocator<int>::deallocate(intArray);

    std::cout << std::endl;

    // Specialized allocation
    char* charArray = CustomAllocator<char>::allocate(100);
    strcpy(charArray, "Hello, specialization!");
    std::cout << "String: " << charArray << std::endl;
    CustomAllocator<char>::deallocate(charArray);

    return 0;
}
```

## Practical Example 4: Print Formatter

```cpp
#include <iostream>
#include <vector>
#include <map>

// Primary template
template <typename T>
class Printer {
public:
    static void print(const T& value) {
        std::cout << value;
    }
};

// Specialization for bool
template <>
class Printer<bool> {
public:
    static void print(const bool& value) {
        std::cout << (value ? "true" : "false");
    }
};

// Specialization for vectors
template <typename T>
class Printer<std::vector<T>> {
public:
    static void print(const std::vector<T>& vec) {
        std::cout << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            Printer<T>::print(vec[i]);
            if (i < vec.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]";
    }
};

// Helper function
template <typename T>
void print(const T& value) {
    Printer<T>::print(value);
    std::cout << std::endl;
}

int main() {
    print(42);
    print(3.14);
    print(true);
    print(false);

    std::vector<int> vec = {1, 2, 3, 4, 5};
    print(vec);

    std::vector<bool> boolVec = {true, false, true};
    print(boolVec);

    return 0;
}
```

## Exercises

### Exercise 1: Specialized Comparison
**Problem:** Create a template class `Comparator` with specialization for floating-point types that uses epsilon comparison.

**Solution:**
```cpp
#include <iostream>
#include <cmath>

template <typename T>
class Comparator {
public:
    static bool areEqual(T a, T b) {
        return a == b;
    }
};

template <>
class Comparator<float> {
public:
    static bool areEqual(float a, float b) {
        const float epsilon = 1e-6f;
        return std::fabs(a - b) < epsilon;
    }
};

template <>
class Comparator<double> {
public:
    static bool areEqual(double a, double b) {
        const double epsilon = 1e-10;
        return std::fabs(a - b) < epsilon;
    }
};

int main() {
    std::cout << "5 == 5: " << Comparator<int>::areEqual(5, 5) << std::endl;
    std::cout << "5 == 6: " << Comparator<int>::areEqual(5, 6) << std::endl;

    float f1 = 0.1f + 0.2f;
    float f2 = 0.3f;
    std::cout << "0.1 + 0.2 == 0.3 (float): "
              << Comparator<float>::areEqual(f1, f2) << std::endl;

    double d1 = 0.1 + 0.2;
    double d2 = 0.3;
    std::cout << "0.1 + 0.2 == 0.3 (double): "
              << Comparator<double>::areEqual(d1, d2) << std::endl;

    return 0;
}
```

### Exercise 2: Type Size Reporter
**Problem:** Create a template struct `TypeInfo` that reports the size of a type, with specializations for common types that also print their names.

**Solution:**
```cpp
#include <iostream>

template <typename T>
struct TypeInfo {
    static void report() {
        std::cout << "Unknown type, size: " << sizeof(T) << " bytes" << std::endl;
    }
};

template <>
struct TypeInfo<int> {
    static void report() {
        std::cout << "Type: int, size: " << sizeof(int) << " bytes" << std::endl;
    }
};

template <>
struct TypeInfo<double> {
    static void report() {
        std::cout << "Type: double, size: " << sizeof(double) << " bytes" << std::endl;
    }
};

template <>
struct TypeInfo<char> {
    static void report() {
        std::cout << "Type: char, size: " << sizeof(char) << " bytes" << std::endl;
    }
};

template <typename T>
struct TypeInfo<T*> {
    static void report() {
        std::cout << "Pointer type, size: " << sizeof(T*) << " bytes" << std::endl;
    }
};

int main() {
    TypeInfo<int>::report();
    TypeInfo<double>::report();
    TypeInfo<char>::report();
    TypeInfo<int*>::report();
    TypeInfo<float>::report();

    return 0;
}
```

### Exercise 3: Container Printer
**Problem:** Create a `ContainerPrinter` template with specialization for different container types.

**Solution:**
```cpp
#include <iostream>
#include <vector>
#include <list>
#include <set>

template <typename Container>
class ContainerPrinter {
public:
    static void print(const Container& c) {
        std::cout << "Container: ";
        for (const auto& item : c) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

template <typename T>
class ContainerPrinter<std::vector<T>> {
public:
    static void print(const std::vector<T>& c) {
        std::cout << "Vector[" << c.size() << "]: ";
        for (const auto& item : c) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

template <typename T>
class ContainerPrinter<std::set<T>> {
public:
    static void print(const std::set<T>& c) {
        std::cout << "Set{" << c.size() << "}: ";
        for (const auto& item : c) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    ContainerPrinter<std::vector<int>>::print(vec);

    std::set<int> s = {5, 3, 1, 4, 2};
    ContainerPrinter<std::set<int>>::print(s);

    std::list<double> lst = {1.1, 2.2, 3.3};
    ContainerPrinter<std::list<double>>::print(lst);

    return 0;
}
```

### Exercise 4: Math Operations
**Problem:** Create a `Math` template class with specialized operations for different numeric types.

**Solution:**
```cpp
#include <iostream>
#include <cmath>

template <typename T>
class Math {
public:
    static T square(T value) {
        return value * value;
    }

    static T absolute(T value) {
        return value < 0 ? -value : value;
    }
};

template <>
class Math<int> {
public:
    static int square(int value) {
        std::cout << "[int square] ";
        return value * value;
    }

    static int absolute(int value) {
        std::cout << "[int abs] ";
        return std::abs(value);
    }

    static int factorial(int n) {
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }
};

template <>
class Math<double> {
public:
    static double square(double value) {
        std::cout << "[double square] ";
        return value * value;
    }

    static double absolute(double value) {
        std::cout << "[double abs] ";
        return std::fabs(value);
    }

    static double sqrt(double value) {
        return std::sqrt(value);
    }
};

int main() {
    std::cout << "Int square of 5: " << Math<int>::square(5) << std::endl;
    std::cout << "Int absolute of -10: " << Math<int>::absolute(-10) << std::endl;
    std::cout << "Int factorial of 5: " << Math<int>::factorial(5) << std::endl;

    std::cout << "\nDouble square of 3.5: " << Math<double>::square(3.5) << std::endl;
    std::cout << "Double absolute of -7.5: " << Math<double>::absolute(-7.5) << std::endl;
    std::cout << "Double sqrt of 16: " << Math<double>::sqrt(16.0) << std::endl;

    return 0;
}
```

### Exercise 5: Smart Array
**Problem:** Create a template array class with specialization for bool to use bit packing.

**Solution:**
```cpp
#include <iostream>
#include <stdexcept>

template <typename T, size_t Size>
class SmartArray {
private:
    T data[Size];

public:
    T& operator[](size_t index) {
        if (index >= Size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= Size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }

    size_t size() const { return Size; }

    void fill(const T& value) {
        for (size_t i = 0; i < Size; ++i) {
            data[i] = value;
        }
    }

    void print() const {
        for (size_t i = 0; i < Size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

template <size_t Size>
class SmartArray<bool, Size> {
private:
    static const size_t BITS_PER_BYTE = 8;
    static const size_t BYTE_SIZE = (Size + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
    unsigned char data[BYTE_SIZE];

public:
    SmartArray() {
        for (size_t i = 0; i < BYTE_SIZE; ++i) {
            data[i] = 0;
        }
    }

    class Reference {
    private:
        unsigned char& byte;
        size_t bitIndex;

    public:
        Reference(unsigned char& b, size_t idx) : byte(b), bitIndex(idx) {}

        Reference& operator=(bool value) {
            if (value) {
                byte |= (1 << bitIndex);
            } else {
                byte &= ~(1 << bitIndex);
            }
            return *this;
        }

        operator bool() const {
            return (byte & (1 << bitIndex)) != 0;
        }
    };

    Reference operator[](size_t index) {
        if (index >= Size) throw std::out_of_range("Index out of bounds");
        size_t byteIndex = index / BITS_PER_BYTE;
        size_t bitIndex = index % BITS_PER_BYTE;
        return Reference(data[byteIndex], bitIndex);
    }

    bool operator[](size_t index) const {
        if (index >= Size) throw std::out_of_range("Index out of bounds");
        size_t byteIndex = index / BITS_PER_BYTE;
        size_t bitIndex = index % BITS_PER_BYTE;
        return (data[byteIndex] & (1 << bitIndex)) != 0;
    }

    size_t size() const { return Size; }

    void fill(bool value) {
        unsigned char fillByte = value ? 0xFF : 0x00;
        for (size_t i = 0; i < BYTE_SIZE; ++i) {
            data[i] = fillByte;
        }
    }

    void print() const {
        for (size_t i = 0; i < Size; ++i) {
            std::cout << ((*this)[i] ? "true" : "false") << " ";
        }
        std::cout << "\nMemory usage: " << BYTE_SIZE << " bytes for "
                  << Size << " bools" << std::endl;
    }
};

int main() {
    SmartArray<int, 5> intArray;
    intArray.fill(42);
    std::cout << "Int array: ";
    intArray.print();

    SmartArray<bool, 16> boolArray;
    boolArray[0] = true;
    boolArray[1] = false;
    boolArray[2] = true;
    boolArray[3] = true;
    boolArray.fill(false);
    boolArray[5] = true;
    boolArray[10] = true;
    std::cout << "\nBool array: ";
    boolArray.print();

    return 0;
}
```

## Summary

In this lesson, you learned:
- Full template specialization for specific types
- Partial template specialization for type categories
- Function template specialization
- Type-specific optimizations
- Creating type traits
- Specialized behaviors for different types

## Checklist

- [ ] Understand full template specialization
- [ ] Can write partial specializations
- [ ] Know when to use specialization
- [ ] Can specialize function templates
- [ ] Understand specialization vs overloading
- [ ] Can create type traits
- [ ] Completed all 5 exercises
- [ ] Can optimize with specialization
- [ ] Understand template selection rules
- [ ] Ready to learn variadic templates

## Next Lesson

In Lesson 53, we'll explore **Variadic Templates** and learn how to work with variable numbers of template arguments.
