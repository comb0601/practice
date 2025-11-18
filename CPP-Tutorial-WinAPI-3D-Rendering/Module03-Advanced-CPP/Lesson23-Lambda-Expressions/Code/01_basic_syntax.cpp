#include <iostream>
#include <algorithm>
#include <vector>

int main() {
    // Basic lambda
    auto hello = []() { std::cout << "Hello Lambda!\n"; };
    hello();
    
    // Lambda with parameters
    auto add = [](int a, int b) { return a + b; };
    std::cout << "5 + 3 = " << add(5, 3) << "\n";
    
    // Lambda with explicit return type
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0.0;
        return a / b;
    };
    std::cout << "10 / 3 = " << divide(10, 3) << "\n";
    
    // Immediate invocation
    int result = [](int x) { return x * x; }(5);
    std::cout << "5^2 = " << result << "\n";
    
    // With STL
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::for_each(vec.begin(), vec.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "\n";
    
    return 0;
}
