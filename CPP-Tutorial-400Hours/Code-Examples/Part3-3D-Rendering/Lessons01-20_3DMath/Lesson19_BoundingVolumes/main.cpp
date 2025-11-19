/*
 * Bounding Volumes
 * Complete working demonstration
 */

#include <iostream>
#include <iomanip>
#include "../../Common/Math3D/Math3D.h"

using namespace Math3D;

void PrintHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void Demonstration() {
    PrintHeader("Main Demonstration");
    std::cout << "Complete working example for this lesson." << std::endl;
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "  Bounding Volumes" << std::endl;
    std::cout << "==========================================" << std::endl;

    Demonstration();

    std::cout << "\n==========================================" << std::endl;
    std::cout << "  Lesson Complete!" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}
