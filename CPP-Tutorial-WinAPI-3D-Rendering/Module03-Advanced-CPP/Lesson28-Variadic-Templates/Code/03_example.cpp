#include <iostream>

template<typename... Args>
void print(Args... args) {
    (std::cout << ... << args) << "\n";
}

template<typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

int main() {
    print("Hello", " ", "World", "!");
    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << "\n";
    return 0;
}
