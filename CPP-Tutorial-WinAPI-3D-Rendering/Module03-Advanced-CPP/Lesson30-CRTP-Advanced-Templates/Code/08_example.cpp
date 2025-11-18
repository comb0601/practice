#include <iostream>
#include <type_traits>

// CRTP Example
template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

class Derived : public Base<Derived> {
public:
    void implementation() {
        std::cout << "Derived implementation\n";
    }
};

// SFINAE Example
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
process(T value) {
    std::cout << "Processing integer: " << value << "\n";
}

int main() {
    Derived d;
    d.interface();
    
    process(42);
    
    return 0;
}
