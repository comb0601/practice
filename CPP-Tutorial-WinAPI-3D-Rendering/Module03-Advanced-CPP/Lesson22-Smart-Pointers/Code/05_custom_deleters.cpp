/*
 * Lesson 22: Smart Pointers - Custom Deleters
 * Compile: g++ -std=c++17 -Wall 05_custom_deleters.cpp -o 05_custom_deleters
 */
#include <iostream>
#include <memory>
#include <cstdio>

int main() {
    std::cout << "=== Custom Deleters ===\n\n";
    
    // 1. FILE* with custom deleter
    std::cout << "1. FILE HANDLE DELETER:\n";
    {
        auto fileDeleter = [](FILE* f) {
            if (f) {
                fclose(f);
                std::cout << "File closed\n";
            }
        };
        
        std::unique_ptr<FILE, decltype(fileDeleter)> file(
            fopen("test.txt", "w"),
            fileDeleter
        );
        
        if (file) {
            fputs("Hello, Smart Pointers!\n", file.get());
        }
    }  // File automatically closed
    
    // 2. shared_ptr with custom deleter
    std::cout << "\n2. SHARED_PTR CUSTOM DELETER:\n";
    {
        std::shared_ptr<FILE> file(
            fopen("test2.txt", "w"),
            [](FILE* f) {
                if (f) {
                    fclose(f);
                    std::cout << "Shared file closed\n";
                }
            }
        );
        
        if (file) {
            fputs("Using shared_ptr\n", file.get());
        }
    }
    
    // 3. Array custom deleter
    std::cout << "\n3. ARRAY CUSTOM DELETER:\n";
    {
        auto arrayDeleter = [](int* arr) {
            std::cout << "Deleting array\n";
            delete[] arr;
        };
        
        std::unique_ptr<int, decltype(arrayDeleter)> arr(
            new int[10],
            arrayDeleter
        );
        
        arr.get()[0] = 42;
    }
    
    return 0;
}
