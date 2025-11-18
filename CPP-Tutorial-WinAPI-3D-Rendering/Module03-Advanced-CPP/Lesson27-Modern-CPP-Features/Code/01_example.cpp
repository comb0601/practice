#include <iostream>
#include <vector>
int main() {
    // auto keyword
    auto x = 42;
    auto str = std::string("Hello");
    
    // Range-based for
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    
    // nullptr
    int* ptr = nullptr;
    
    return 0;
}
