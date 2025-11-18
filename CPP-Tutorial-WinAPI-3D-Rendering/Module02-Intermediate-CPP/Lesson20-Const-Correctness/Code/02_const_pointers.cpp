#include <iostream>
using namespace std;
int main() {
    int x = 10, y = 20;
    const int* ptr1 = &x;        // Pointer to const int
    // *ptr1 = 20;               // Error
    ptr1 = &y;                   // OK
    int* const ptr2 = &x;        // Const pointer to int
    *ptr2 = 30;                  // OK
    // ptr2 = &y;                // Error
    const int* const ptr3 = &x;  // Const pointer to const int
    // *ptr3 = 40;               // Error
    // ptr3 = &y;                // Error
    return 0;
}
