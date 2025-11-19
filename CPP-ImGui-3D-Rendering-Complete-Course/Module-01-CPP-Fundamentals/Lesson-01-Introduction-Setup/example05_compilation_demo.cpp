#include <iostream>

/**
 * Example 05: Understanding the Compilation Process
 *
 * This program helps you understand what happens during compilation.
 *
 * To see each compilation stage:
 *
 * 1. Preprocessing:
 *    g++ -E example05_compilation_demo.cpp -o example05.i
 *    (See how #include and #define are expanded)
 *
 * 2. Compilation to Assembly:
 *    g++ -S example05_compilation_demo.cpp -o example05.s
 *    (See the assembly code generated)
 *
 * 3. Assembly to Object Code:
 *    g++ -c example05_compilation_demo.cpp -o example05.o
 *    (Creates binary object file)
 *
 * 4. Linking:
 *    g++ example05.o -o example05
 *    (Creates final executable)
 *
 * Or do it all in one step:
 *    g++ example05_compilation_demo.cpp -o example05
 */

// Preprocessor directive - this will be expanded
#define MESSAGE "Hello from preprocessor!"
#define SQUARE(x) ((x) * (x))

int main() {
    std::cout << "=== Compilation Process Demo ===" << std::endl;
    std::cout << std::endl;

    // This MESSAGE will be replaced by the preprocessor
    std::cout << MESSAGE << std::endl;

    // This macro will be expanded
    int num = 5;
    std::cout << "5 squared = " << SQUARE(num) << std::endl;

    std::cout << std::endl;
    std::cout << "Compilation stages:" << std::endl;
    std::cout << "1. Preprocessing: Expands #include and #define" << std::endl;
    std::cout << "2. Compilation: Converts to assembly language" << std::endl;
    std::cout << "3. Assembly: Creates object code (.o/.obj)" << std::endl;
    std::cout << "4. Linking: Creates final executable" << std::endl;

    return 0;
}

/*
 * TRY THIS:
 *
 * 1. Compile with different optimization levels:
 *    g++ -O0 example05_compilation_demo.cpp -o example05_O0  (no optimization)
 *    g++ -O2 example05_compilation_demo.cpp -o example05_O2  (optimize)
 *    g++ -O3 example05_compilation_demo.cpp -o example05_O3  (aggressive optimize)
 *
 * 2. Compare the assembly output:
 *    g++ -S -O0 example05_compilation_demo.cpp -o example05_O0.s
 *    g++ -S -O3 example05_compilation_demo.cpp -o example05_O3.s
 *    diff example05_O0.s example05_O3.s
 *
 * 3. Enable warnings:
 *    g++ -Wall -Wextra example05_compilation_demo.cpp -o example05
 */
