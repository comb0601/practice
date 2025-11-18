#include <iostream>

int main() {
    int x = 10, y = 20;
    
    // Capture by value
    auto byValue = [x, y]() {
        std::cout << "Captured by value: " << x << ", " << y << "\n";
    };
    byValue();
    
    // Capture by reference
    auto byRef = [&x, &y]() {
        x += 10;
        y += 10;
        std::cout << "Modified via reference: " << x << ", " << y << "\n";
    };
    byRef();
    std::cout << "After lambda: x=" << x << ", y=" << y << "\n";
    
    // Capture all by value
    auto captureAll = [=]() {
        std::cout << "Capture all by value: " << x << ", " << y << "\n";
    };
    captureAll();
    
    // Mixed capture
    int z = 30;
    auto mixed = [=, &z]() {
        z += 10;
        std::cout << "Mixed capture: x=" << x << ", z=" << z << "\n";
    };
    mixed();
    
    return 0;
}
