/* Lesson 49: Variadic Templates (C++11)
 * Compile: cl /std:c++17 Lesson49_VariadicTemplates.cpp */
#include <iostream>
using namespace std;

template<typename T>
T sum(T value) {
    return value;
}

template<typename T, typename... Args>
T sum(T first, Args... args) {
    return first + sum(args...);
}

template<typename T>
void print(T value) {
    cout << value << endl;
}

template<typename T, typename... Args>
void print(T first, Args... args) {
    cout << first << " ";
    print(args...);
}

int main() {
    cout << "Sum: " << sum(1, 2, 3, 4, 5) << endl;
    cout << "Print: ";
    print(1, 2.5, "Hello", 'A');
    return 0;
}
