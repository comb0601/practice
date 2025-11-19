/* Lesson 47: Class Templates
 * Compile: cl Lesson47_ClassTemplates.cpp */
#include <iostream>
using namespace std;

template <typename T>
class Pair {
private:
    T first, second;
public:
    Pair(T f, T s) : first(f), second(s) {}
    T getFirst() { return first; }
    T getSecond() { return second; }
    void display() {
        cout << "(" << first << ", " << second << ")" << endl;
    }
};

template <typename T, int SIZE>
class Array {
private:
    T data[SIZE];
public:
    void fill(T value) {
        for (int i = 0; i < SIZE; i++) {
            data[i] = value;
        }
    }
    T& operator[](int index) { return data[index]; }
    int size() { return SIZE; }
};

int main() {
    Pair<int> p1(10, 20);
    p1.display();

    Pair<double> p2(3.14, 2.71);
    p2.display();

    Array<int, 5> arr;
    arr.fill(7);
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
