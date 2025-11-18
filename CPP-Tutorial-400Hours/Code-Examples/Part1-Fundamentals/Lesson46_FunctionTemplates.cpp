/* Lesson 46: Function Templates
 * Compile: cl Lesson46_FunctionTemplates.cpp */
#include <iostream>
using namespace std;

template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

template <typename T>
void swapValues(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void printArray(T arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    cout << "Max(10, 20): " << maximum(10, 20) << endl;
    cout << "Max(3.5, 2.1): " << maximum(3.5, 2.1) << endl;

    int x = 5, y = 10;
    cout << "Before swap: " << x << ", " << y << endl;
    swapValues(x, y);
    cout << "After swap: " << x << ", " << y << endl;

    int arr[] = {1, 2, 3, 4, 5};
    printArray(arr, 5);

    return 0;
}
