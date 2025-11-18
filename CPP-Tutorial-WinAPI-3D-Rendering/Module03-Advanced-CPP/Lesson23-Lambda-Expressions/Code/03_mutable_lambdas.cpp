#include <iostream>

int main() {
    int x = 10;
    
    // Without mutable (ERROR if we try to modify x)
    // auto lambda1 = [x]() { x++; };  // Error!
    
    // With mutable
    auto lambda2 = [x]() mutable {
        x++;
        std::cout << "Inside lambda: " << x << "\n";
    };
    
    lambda2();
    lambda2();
    std::cout << "Outside lambda: " << x << "\n";  // Original unchanged
    
    // Mutable with reference
    auto lambda3 = [&x]() {  // No mutable needed with reference
        x++;
        std::cout << "With reference: " << x << "\n";
    };
    lambda3();
    std::cout << "After reference lambda: " << x << "\n";
    
    return 0;
}
