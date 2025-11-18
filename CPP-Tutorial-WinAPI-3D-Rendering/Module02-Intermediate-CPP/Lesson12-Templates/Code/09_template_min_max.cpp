/**
 * Template Min/Max Functions
 * Comprehensive min/max implementations with various features
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Basic min/max templates
template <typename T>
T min(T a, T b) {
    return (a < b) ? a : b;
}

template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// Three-argument versions
template <typename T>
T min(T a, T b, T c) {
    return min(min(a, b), c);
}

template <typename T>
T max(T a, T b, T c) {
    return max(max(a, b), c);
}

// Variadic min/max
template <typename T>
T min_variadic(T value) {
    return value;
}

template <typename T, typename... Args>
T min_variadic(T first, Args... rest) {
    T min_rest = min_variadic(rest...);
    return (first < min_rest) ? first : min_rest;
}

template <typename T>
T max_variadic(T value) {
    return value;
}

template <typename T, typename... Args>
T max_variadic(T first, Args... rest) {
    T max_rest = max_variadic(rest...);
    return (first > max_rest) ? first : max_rest;
}

// Array min/max
template <typename T, size_t N>
T minArray(const T (&arr)[N]) {
    T minimum = arr[0];
    for (size_t i = 1; i < N; i++) {
        if (arr[i] < minimum) {
            minimum = arr[i];
        }
    }
    return minimum;
}

template <typename T, size_t N>
T maxArray(const T (&arr)[N]) {
    T maximum = arr[0];
    for (size_t i = 1; i < N; i++) {
        if (arr[i] > maximum) {
            maximum = arr[i];
        }
    }
    return maximum;
}

// Vector min/max
template <typename T>
T minVector(const vector<T>& vec) {
    if (vec.empty()) {
        throw runtime_error("Empty vector");
    }
    T minimum = vec[0];
    for (const T& val : vec) {
        if (val < minimum) {
            minimum = val;
        }
    }
    return minimum;
}

template <typename T>
T maxVector(const vector<T>& vec) {
    if (vec.empty()) {
        throw runtime_error("Empty vector");
    }
    T maximum = vec[0];
    for (const T& val : vec) {
        if (val > maximum) {
            maximum = val;
        }
    }
    return maximum;
}

// Min/Max pair
template <typename T>
struct MinMaxPair {
    T minimum;
    T maximum;
};

template <typename T>
MinMaxPair<T> minmax(T a, T b) {
    if (a < b) {
        return {a, b};
    } else {
        return {b, a};
    }
}

template <typename T>
MinMaxPair<T> minmaxArray(const T* arr, size_t size) {
    MinMaxPair<T> result = {arr[0], arr[0]};
    for (size_t i = 1; i < size; i++) {
        if (arr[i] < result.minimum) {
            result.minimum = arr[i];
        }
        if (arr[i] > result.maximum) {
            result.maximum = arr[i];
        }
    }
    return result;
}

// Clamp function
template <typename T>
T clamp(T value, T low, T high) {
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

int main() {
    cout << "=== Template Min/Max Functions ===" << endl << endl;

    // Basic min/max
    cout << "Basic Min/Max:" << endl;
    cout << "min(5, 3) = " << min(5, 3) << endl;
    cout << "max(5, 3) = " << max(5, 3) << endl;
    cout << "min(3.14, 2.71) = " << min(3.14, 2.71) << endl;
    cout << "max(3.14, 2.71) = " << max(3.14, 2.71) << endl;
    cout << endl;

    // Three arguments
    cout << "Three Arguments:" << endl;
    cout << "min(5, 3, 8) = " << min(5, 3, 8) << endl;
    cout << "max(5, 3, 8) = " << max(5, 3, 8) << endl;
    cout << endl;

    // Variadic
    cout << "Variadic Min/Max:" << endl;
    cout << "min_variadic(5, 3, 8, 1, 9, 2) = " << min_variadic(5, 3, 8, 1, 9, 2) << endl;
    cout << "max_variadic(5, 3, 8, 1, 9, 2) = " << max_variadic(5, 3, 8, 1, 9, 2) << endl;
    cout << "min_variadic(3.14, 2.71, 1.41, 5.67, 0.99) = "
         << min_variadic(3.14, 2.71, 1.41, 5.67, 0.99) << endl;
    cout << endl;

    // Array min/max
    cout << "Array Min/Max:" << endl;
    int intArray[] = {5, 2, 8, 1, 9, 3, 7};
    cout << "Array: {5, 2, 8, 1, 9, 3, 7}" << endl;
    cout << "minArray = " << minArray(intArray) << endl;
    cout << "maxArray = " << maxArray(intArray) << endl;
    cout << endl;

    double doubleArray[] = {3.14, 2.71, 1.41, 5.67, 0.99};
    cout << "Array: {3.14, 2.71, 1.41, 5.67, 0.99}" << endl;
    cout << "minArray = " << minArray(doubleArray) << endl;
    cout << "maxArray = " << maxArray(doubleArray) << endl;
    cout << endl;

    // Vector min/max
    cout << "Vector Min/Max:" << endl;
    vector<int> vec = {10, 5, 20, 3, 15, 8};
    cout << "Vector: {10, 5, 20, 3, 15, 8}" << endl;
    cout << "minVector = " << minVector(vec) << endl;
    cout << "maxVector = " << maxVector(vec) << endl;
    cout << endl;

    vector<string> strVec = {"apple", "zebra", "mango", "banana"};
    cout << "String Vector: {apple, zebra, mango, banana}" << endl;
    cout << "minVector = " << minVector(strVec) << endl;
    cout << "maxVector = " << maxVector(strVec) << endl;
    cout << endl;

    // MinMax pair
    cout << "MinMax Pair:" << endl;
    auto pair1 = minmax(10, 5);
    cout << "minmax(10, 5): min = " << pair1.minimum << ", max = " << pair1.maximum << endl;

    int arr[] = {5, 2, 8, 1, 9, 3, 7};
    auto pair2 = minmaxArray(arr, 7);
    cout << "minmaxArray({5,2,8,1,9,3,7}): min = " << pair2.minimum
         << ", max = " << pair2.maximum << endl;
    cout << endl;

    // Clamp function
    cout << "Clamp Function:" << endl;
    cout << "clamp(5, 0, 10) = " << clamp(5, 0, 10) << endl;
    cout << "clamp(-5, 0, 10) = " << clamp(-5, 0, 10) << endl;
    cout << "clamp(15, 0, 10) = " << clamp(15, 0, 10) << endl;
    cout << "clamp(3.14, 0.0, 5.0) = " << clamp(3.14, 0.0, 5.0) << endl;

    return 0;
}
