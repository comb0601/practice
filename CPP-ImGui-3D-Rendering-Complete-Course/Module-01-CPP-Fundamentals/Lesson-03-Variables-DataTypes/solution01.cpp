#include <iostream>
#include <string>

/**
 * Solution 01: Variable Declaration and Initialization
 */

int main() {
    int age = 25;
    double height = 1.75;
    char initial = 'J';
    bool likesProgramming = true;
    std::string favoriteColor = "Blue";

    std::cout << "=== Personal Information ===" << std::endl;
    std::cout << "Age: " << age << " years" << std::endl;
    std::cout << "Height: " << height << " meters" << std::endl;
    std::cout << "Initial: " << initial << std::endl;
    std::cout << "Likes Programming: " << std::boolalpha << likesProgramming << std::endl;
    std::cout << "Favorite Color: " << favoriteColor << std::endl;

    return 0;
}
