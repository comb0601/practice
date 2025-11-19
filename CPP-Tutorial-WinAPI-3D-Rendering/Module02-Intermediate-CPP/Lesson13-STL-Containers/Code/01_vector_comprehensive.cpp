/*
 * Lesson 13: STL Containers - Comprehensive Vector Example
 *
 * This example provides a detailed exploration of std::vector,
 * the most commonly used STL container.
 *
 * Compilation: g++ -std=c++17 01_vector_comprehensive.cpp -o vector_demo
 * Execution: ./vector_demo
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

// Helper function to print vector
template<typename T>
void printVector(const std::vector<T>& vec, const std::string& label = "Vector") {
    std::cout << label << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// Helper function to show vector stats
template<typename T>
void showStats(const std::vector<T>& vec, const std::string& name) {
    std::cout << "\n" << name << " Statistics:" << std::endl;
    std::cout << "  Size: " << vec.size() << std::endl;
    std::cout << "  Capacity: " << vec.capacity() << std::endl;
    std::cout << "  Empty: " << (vec.empty() ? "yes" : "no") << std::endl;
    if (!vec.empty()) {
        std::cout << "  First element: " << vec.front() << std::endl;
        std::cout << "  Last element: " << vec.back() << std::endl;
    }
}

class Student {
public:
    std::string name;
    int age;
    double gpa;

    Student(const std::string& n, int a, double g)
        : name(n), age(a), gpa(g) {}

    void display() const {
        std::cout << std::left << std::setw(15) << name
                  << " Age: " << std::setw(3) << age
                  << " GPA: " << std::fixed << std::setprecision(2) << gpa;
    }
};

int main() {
    std::cout << "=== Comprehensive std::vector Tutorial ===" << std::endl;

    // 1. Construction and Initialization
    std::cout << "\n1. CONSTRUCTION AND INITIALIZATION" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<int> v1;                          // Empty vector
    std::vector<int> v2(5);                       // 5 elements, default initialized to 0
    std::vector<int> v3(5, 100);                  // 5 elements, all initialized to 100
    std::vector<int> v4{1, 2, 3, 4, 5};          // Initializer list
    std::vector<int> v5(v4.begin(), v4.end());   // Copy from another vector

    printVector(v2, "v2 (5 default elements)");
    printVector(v3, "v3 (5 elements = 100)");
    printVector(v4, "v4 (initializer list)");

    // 2. Adding Elements
    std::cout << "\n2. ADDING ELEMENTS" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<int> numbers;
    std::cout << "Using push_back:" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        numbers.push_back(i * 10);
        printVector(numbers, "  After push_back(" + std::to_string(i * 10) + ")");
    }

    std::cout << "\nUsing emplace_back (more efficient):" << std::endl;
    numbers.emplace_back(60);
    printVector(numbers, "  After emplace_back(60)");

    std::cout << "\nUsing insert:" << std::endl;
    numbers.insert(numbers.begin() + 2, 25);  // Insert at position 2
    printVector(numbers, "  After insert at pos 2");

    // 3. Accessing Elements
    std::cout << "\n3. ACCESSING ELEMENTS" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<int> data{10, 20, 30, 40, 50};
    printVector(data, "Original data");

    std::cout << "data[2] = " << data[2] << std::endl;
    std::cout << "data.at(2) = " << data.at(2) << std::endl;
    std::cout << "data.front() = " << data.front() << std::endl;
    std::cout << "data.back() = " << data.back() << std::endl;

    // Using iterators
    std::cout << "\nIterating with iterators:" << std::endl;
    for (auto it = data.begin(); it != data.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Range-based for loop (C++11)
    std::cout << "Range-based for loop: ";
    for (const auto& value : data) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // 4. Modifying Elements
    std::cout << "\n4. MODIFYING ELEMENTS" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<int> modify{1, 2, 3, 4, 5};
    printVector(modify, "Before modification");

    modify[0] = 100;
    modify.at(1) = 200;
    printVector(modify, "After modify[0]=100, at(1)=200");

    // Modify using algorithm
    std::transform(modify.begin(), modify.end(), modify.begin(),
                   [](int n) { return n * 2; });
    printVector(modify, "After doubling all elements");

    // 5. Removing Elements
    std::cout << "\n5. REMOVING ELEMENTS" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<int> remove{10, 20, 30, 40, 50};
    printVector(remove, "Original");

    remove.pop_back();
    printVector(remove, "After pop_back()");

    remove.erase(remove.begin() + 1);
    printVector(remove, "After erase(begin()+1)");

    remove.erase(remove.begin(), remove.begin() + 2);
    printVector(remove, "After erase(begin(), begin()+2)");

    // 6. Capacity Management
    std::cout << "\n6. CAPACITY MANAGEMENT" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<int> capacity;
    showStats(capacity, "Empty vector");

    capacity.reserve(100);
    showStats(capacity, "After reserve(100)");

    for (int i = 0; i < 10; ++i) {
        capacity.push_back(i);
    }
    showStats(capacity, "After adding 10 elements");

    capacity.shrink_to_fit();
    showStats(capacity, "After shrink_to_fit()");

    // 7. Searching and Sorting
    std::cout << "\n7. SEARCHING AND SORTING" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<int> search{50, 20, 60, 10, 30, 40};
    printVector(search, "Original");

    // Find element
    auto it = std::find(search.begin(), search.end(), 30);
    if (it != search.end()) {
        std::cout << "Found 30 at position: " << (it - search.begin()) << std::endl;
    }

    // Sort
    std::sort(search.begin(), search.end());
    printVector(search, "After sorting");

    // Binary search (requires sorted container)
    bool found = std::binary_search(search.begin(), search.end(), 30);
    std::cout << "Binary search for 30: " << (found ? "found" : "not found") << std::endl;

    // 8. Advanced Operations
    std::cout << "\n8. ADVANCED OPERATIONS" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<int> nums{1, 2, 3, 4, 5};
    printVector(nums, "Original");

    // Sum
    int sum = std::accumulate(nums.begin(), nums.end(), 0);
    std::cout << "Sum: " << sum << std::endl;

    // Count
    int count = std::count_if(nums.begin(), nums.end(),
                              [](int n) { return n % 2 == 0; });
    std::cout << "Even numbers: " << count << std::endl;

    // Reverse
    std::reverse(nums.begin(), nums.end());
    printVector(nums, "After reverse");

    // Rotate
    std::rotate(nums.begin(), nums.begin() + 2, nums.end());
    printVector(nums, "After rotate by 2");

    // 9. Vector of Custom Objects
    std::cout << "\n9. VECTOR OF CUSTOM OBJECTS" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<Student> students;
    students.emplace_back("Alice", 20, 3.8);
    students.emplace_back("Bob", 22, 3.5);
    students.emplace_back("Charlie", 21, 3.9);
    students.emplace_back("Diana", 19, 4.0);

    std::cout << "Students:" << std::endl;
    for (const auto& student : students) {
        std::cout << "  ";
        student.display();
        std::cout << std::endl;
    }

    // Sort by GPA
    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  return a.gpa > b.gpa;
              });

    std::cout << "\nSorted by GPA (descending):" << std::endl;
    for (const auto& student : students) {
        std::cout << "  ";
        student.display();
        std::cout << std::endl;
    }

    // 10. 2D Vector (Matrix)
    std::cout << "\n10. 2D VECTOR (MATRIX)" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::cout << "3x3 Matrix:" << std::endl;
    for (const auto& row : matrix) {
        std::cout << "  ";
        for (const auto& val : row) {
            std::cout << std::setw(3) << val << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nAccessing matrix[1][2] = " << matrix[1][2] << std::endl;

    return 0;
}
