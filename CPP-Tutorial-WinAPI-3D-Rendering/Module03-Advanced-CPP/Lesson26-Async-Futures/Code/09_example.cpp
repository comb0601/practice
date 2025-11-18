#include <iostream>
#include <future>
int compute(int x) { return x * x; }
int main() {
    auto fut = std::async(std::launch::async, compute, 10);
    std::cout << "Result: " << fut.get() << "\n";
    return 0;
}
