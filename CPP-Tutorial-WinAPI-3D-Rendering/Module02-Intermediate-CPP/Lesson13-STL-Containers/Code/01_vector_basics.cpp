/**
 * Vector Basics
 * Comprehensive vector operations and usage
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void printVector(const vector<int>& vec, const string& label) {
    cout << label << ": [";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int main() {
    cout << "=== Vector Basics ===" << endl << endl;

    // Construction
    cout << "Construction:" << endl;
    vector<int> v1;                      // Empty
    vector<int> v2(5);                   // 5 elements, default-initialized
    vector<int> v3(5, 10);               // 5 elements, all 10
    vector<int> v4 = {1, 2, 3, 4, 5};    // Initializer list
    vector<int> v5(v4);                  // Copy constructor

    printVector(v2, "v2 (5 default)");
    printVector(v3, "v3 (5 x 10)");
    printVector(v4, "v4 (init list)");
    cout << endl;

    // Adding elements
    cout << "Adding Elements:" << endl;
    vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    printVector(vec, "After push_back");

    vec.insert(vec.begin(), 5);          // Insert at beginning
    printVector(vec, "After insert at begin");

    vec.insert(vec.begin() + 2, 15);     // Insert at position
    printVector(vec, "After insert at position 2");
    cout << endl;

    // Accessing elements
    cout << "Accessing Elements:" << endl;
    cout << "First element: " << vec.front() << endl;
    cout << "Last element: " << vec.back() << endl;
    cout << "Element at index 2: " << vec[2] << endl;
    cout << "Element at(3): " << vec.at(3) << endl;
    cout << endl;

    // Modifying elements
    cout << "Modifying Elements:" << endl;
    vec[0] = 100;
    vec.at(1) = 200;
    printVector(vec, "After modification");
    cout << endl;

    // Removing elements
    cout << "Removing Elements:" << endl;
    vec.pop_back();
    printVector(vec, "After pop_back");

    vec.erase(vec.begin() + 1);
    printVector(vec, "After erase at position 1");
    cout << endl;

    // Size and capacity
    cout << "Size and Capacity:" << endl;
    cout << "Size: " << vec.size() << endl;
    cout << "Capacity: " << vec.capacity() << endl;
    cout << "Empty: " << (vec.empty() ? "yes" : "no") << endl;

    vec.reserve(20);
    cout << "After reserve(20) - Capacity: " << vec.capacity() << endl;
    cout << endl;

    // Iteration
    cout << "Iteration Methods:" << endl;
    cout << "Range-based for: ";
    for (int val : vec) {
        cout << val << " ";
    }
    cout << endl;

    cout << "Iterator: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse iterator: ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        cout << *it << " ";
    }
    cout << endl << endl;

    // Finding elements
    cout << "Finding Elements:" << endl;
    vector<int> numbers = {10, 20, 30, 40, 50};
    auto it = find(numbers.begin(), numbers.end(), 30);
    if (it != numbers.end()) {
        cout << "Found 30 at index: " << distance(numbers.begin(), it) << endl;
    }
    cout << endl;

    // Sorting
    cout << "Sorting:" << endl;
    vector<int> unsorted = {5, 2, 8, 1, 9, 3};
    printVector(unsorted, "Before sort");
    sort(unsorted.begin(), unsorted.end());
    printVector(unsorted, "After sort");
    cout << endl;

    // Clear
    cout << "Clear:" << endl;
    printVector(vec, "Before clear");
    vec.clear();
    cout << "After clear - Size: " << vec.size()
         << ", Capacity: " << vec.capacity() << endl;

    return 0;
}
