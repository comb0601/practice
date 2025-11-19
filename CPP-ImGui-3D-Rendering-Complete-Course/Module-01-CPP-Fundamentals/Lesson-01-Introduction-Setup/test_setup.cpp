#include <iostream>
#include <string>

int main() {
    std::cout << "=== C++ Setup Test ===" << std::endl;
    std::cout << "If you can see this, your C++ environment is working!" << std::endl;
    std::cout << std::endl;

    std::cout << "C++ version: " << __cplusplus << std::endl;
    std::cout << "Compiler: " <<
#ifdef __GNUC__
        "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__
#elif defined(_MSC_VER)
        "MSVC " << _MSC_VER
#elif defined(__clang__)
        "Clang " << __clang_major__ << "." << __clang_minor__
#else
        "Unknown"
#endif
    << std::endl;

    std::cout << std::endl;
    std::cout << "Type your name: ";
    std::string name;
    std::getline(std::cin, name);

    std::cout << "Hello, " << name << "!" << std::endl;
    std::cout << "You're ready to start learning C++!" << std::endl;

    return 0;
}
