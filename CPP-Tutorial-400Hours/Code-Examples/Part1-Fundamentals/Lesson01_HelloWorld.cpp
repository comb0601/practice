/*
 * Lesson 1: Hello World - Your First C++ Program
 *
 * This is the classic first program that every programmer writes.
 * It demonstrates the basic structure of a C++ program and outputs
 * text to the console.
 *
 * Compile: cl Lesson01_HelloWorld.cpp (MSVC)
 * Run: Lesson01_HelloWorld.exe
 */

#include <iostream>  // Include the input/output stream library

// The main function - program execution starts here
int main() {
    // Output "Hello, World!" to the console
    std::cout << "Hello, World!" << std::endl;

    // Additional examples of output
    std::cout << "Welcome to C++ Programming!" << std::endl;
    std::cout << "This is your first step in a 400-hour journey." << std::endl;

    // You can also chain multiple outputs
    std::cout << "C++" << " is " << "awesome!" << std::endl;

    // \n is another way to create a new line
    std::cout << "Line 1\nLine 2\nLine 3\n";

    return 0;  // Return 0 to indicate successful program execution
}

/*
 * Expected Output:
 * Hello, World!
 * Welcome to C++ Programming!
 * This is your first step in a 400-hour journey.
 * C++ is awesome!
 * Line 1
 * Line 2
 * Line 3
 */
