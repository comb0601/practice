# Lesson 12: Function Parameters and Return Values

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Pass by Value Deep Dive
2. Pass by Reference
3. Const Parameters
4. Multiple Return Strategies
5. Return by Reference
6. Function Parameter Best Practices
7. Default Parameters
8. Exercises and Projects

---

## 1. Pass by Value Deep Dive

### Understanding Pass by Value:

When you pass arguments by value, C++ creates a copy of the variable. Changes inside the function don't affect the original variable.

```cpp
#include <iostream>

void modifyValue(int x)
{
    std::cout << "Inside function - before: " << x << "\n";
    x = 100;  // Only modifies the copy
    std::cout << "Inside function - after: " << x << "\n";
}

int main()
{
    int number = 50;

    std::cout << "Before function call: " << number << "\n";
    modifyValue(number);
    std::cout << "After function call: " << number << "\n";

    return 0;
}
```

**Output**:
```
Before function call: 50
Inside function - before: 50
Inside function - after: 100
After function call: 50
```

### Performance Implications:

```cpp
#include <iostream>
#include <string>
#include <vector>

struct LargeObject
{
    int data[1000];
    std::string name;
    std::vector<double> values;
};

// Inefficient - copies entire object
void processExpensive(LargeObject obj)
{
    std::cout << "Processing large object...\n";
}

// More efficient - we'll learn this in next section
void processEfficient(const LargeObject& obj)
{
    std::cout << "Processing large object efficiently...\n";
}

int main()
{
    LargeObject myObject;

    // This creates a full copy of myObject (expensive!)
    processExpensive(myObject);

    // This passes a reference (cheap!)
    processEfficient(myObject);

    return 0;
}
```

---

## 2. Pass by Reference

### Basic Reference Parameters:

Reference parameters allow functions to modify the original variable without copying it.

```cpp
#include <iostream>

void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

void increment(int& value)
{
    value++;
}

int main()
{
    int x = 10, y = 20;

    std::cout << "Before swap: x = " << x << ", y = " << y << "\n";
    swap(x, y);
    std::cout << "After swap: x = " << x << ", y = " << y << "\n\n";

    int counter = 0;
    std::cout << "Counter: " << counter << "\n";
    increment(counter);
    std::cout << "After increment: " << counter << "\n";

    return 0;
}
```

**Output**:
```
Before swap: x = 10, y = 20
After swap: x = 20, y = 10

Counter: 0
After increment: 1
```

### Multiple Output Parameters:

```cpp
#include <iostream>
#include <string>

void divideWithRemainder(int dividend, int divisor, int& quotient, int& remainder)
{
    quotient = dividend / divisor;
    remainder = dividend % divisor;
}

void calculateCircle(double radius, double& area, double& circumference)
{
    const double PI = 3.14159265359;
    area = PI * radius * radius;
    circumference = 2 * PI * radius;
}

void parseFullName(std::string fullName, std::string& firstName, std::string& lastName)
{
    size_t spacePos = fullName.find(' ');
    if (spacePos != std::string::npos)
    {
        firstName = fullName.substr(0, spacePos);
        lastName = fullName.substr(spacePos + 1);
    }
}

int main()
{
    // Division with remainder
    int quot, rem;
    divideWithRemainder(17, 5, quot, rem);
    std::cout << "17 / 5 = " << quot << " remainder " << rem << "\n\n";

    // Circle calculations
    double area, circumference;
    calculateCircle(5.0, area, circumference);
    std::cout << "Circle with radius 5.0:\n";
    std::cout << "  Area: " << area << "\n";
    std::cout << "  Circumference: " << circumference << "\n\n";

    // Name parsing
    std::string first, last;
    parseFullName("John Doe", first, last);
    std::cout << "First name: " << first << "\n";
    std::cout << "Last name: " << last << "\n";

    return 0;
}
```

---

## 3. Const Parameters

### Const Value Parameters:

```cpp
#include <iostream>

void printValue(const int x)
{
    std::cout << "Value: " << x << "\n";
    // x = 10;  // ERROR: Cannot modify const parameter
}

int main()
{
    int number = 42;
    printValue(number);

    return 0;
}
```

### Const Reference Parameters:

Best practice for large objects you don't want to modify.

```cpp
#include <iostream>
#include <string>
#include <vector>

void printString(const std::string& str)
{
    std::cout << "String: " << str << "\n";
    // str = "new value";  // ERROR: Cannot modify const reference
}

void displayVector(const std::vector<int>& vec)
{
    std::cout << "Vector contents: ";
    for (int val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << "\n";
    // vec.push_back(10);  // ERROR: Cannot modify const reference
}

double calculateAverage(const std::vector<double>& numbers)
{
    if (numbers.empty())
        return 0.0;

    double sum = 0.0;
    for (double num : numbers)
    {
        sum += num;
    }

    return sum / numbers.size();
}

int main()
{
    std::string message = "Hello, World!";
    printString(message);

    std::vector<int> numbers = {1, 2, 3, 4, 5};
    displayVector(numbers);

    std::vector<double> scores = {85.5, 92.0, 78.5, 95.0};
    double avg = calculateAverage(scores);
    std::cout << "Average: " << avg << "\n";

    return 0;
}
```

### Parameter Guidelines:

```cpp
#include <iostream>
#include <string>
#include <vector>

// Small types: pass by value
int square(int x)
{
    return x * x;
}

// Large types (read-only): pass by const reference
void printVector(const std::vector<int>& vec)
{
    for (int val : vec)
        std::cout << val << " ";
    std::cout << "\n";
}

// Large types (modifiable): pass by reference
void sortVector(std::vector<int>& vec)
{
    // Sorting implementation...
}

// Output parameters: pass by reference
void getMinMax(const std::vector<int>& vec, int& min, int& max)
{
    if (vec.empty())
        return;

    min = max = vec[0];
    for (int val : vec)
    {
        if (val < min) min = val;
        if (val > max) max = val;
    }
}

int main()
{
    int result = square(5);
    std::cout << "Square: " << result << "\n";

    std::vector<int> data = {3, 7, 1, 9, 2};
    printVector(data);

    int minimum, maximum;
    getMinMax(data, minimum, maximum);
    std::cout << "Min: " << minimum << ", Max: " << maximum << "\n";

    return 0;
}
```

---

## 4. Multiple Return Strategies

### Using Structs to Return Multiple Values:

```cpp
#include <iostream>
#include <string>
#include <cmath>

struct QuadraticResult
{
    bool hasRoots;
    double root1;
    double root2;
};

QuadraticResult solveQuadratic(double a, double b, double c)
{
    QuadraticResult result;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        result.hasRoots = false;
        result.root1 = 0;
        result.root2 = 0;
    }
    else
    {
        result.hasRoots = true;
        result.root1 = (-b + std::sqrt(discriminant)) / (2 * a);
        result.root2 = (-b - std::sqrt(discriminant)) / (2 * a);
    }

    return result;
}

struct Statistics
{
    double min;
    double max;
    double average;
    double sum;
};

Statistics calculateStats(double arr[], int size)
{
    Statistics stats;

    if (size == 0)
    {
        stats.min = stats.max = stats.average = stats.sum = 0;
        return stats;
    }

    stats.min = stats.max = stats.sum = arr[0];

    for (int i = 1; i < size; i++)
    {
        stats.sum += arr[i];
        if (arr[i] < stats.min) stats.min = arr[i];
        if (arr[i] > stats.max) stats.max = arr[i];
    }

    stats.average = stats.sum / size;

    return stats;
}

int main()
{
    // Quadratic equation
    QuadraticResult result = solveQuadratic(1, -5, 6);

    if (result.hasRoots)
    {
        std::cout << "Roots: " << result.root1 << ", " << result.root2 << "\n";
    }
    else
    {
        std::cout << "No real roots\n";
    }

    // Statistics
    double numbers[] = {10.5, 23.7, 15.2, 8.9, 30.1};
    Statistics stats = calculateStats(numbers, 5);

    std::cout << "\nStatistics:\n";
    std::cout << "  Min: " << stats.min << "\n";
    std::cout << "  Max: " << stats.max << "\n";
    std::cout << "  Average: " << stats.average << "\n";
    std::cout << "  Sum: " << stats.sum << "\n";

    return 0;
}
```

### Using std::pair and std::tuple:

```cpp
#include <iostream>
#include <utility>  // For std::pair
#include <tuple>    // For std::tuple
#include <string>

// Return pair
std::pair<int, int> divmod(int dividend, int divisor)
{
    return std::make_pair(dividend / divisor, dividend % divisor);
}

// Return tuple
std::tuple<std::string, int, double> getStudentInfo()
{
    return std::make_tuple("Alice", 20, 3.8);
}

// Modern C++ (C++17) - structured bindings
std::pair<double, double> getMinMax(double arr[], int size)
{
    double min = arr[0];
    double max = arr[0];

    for (int i = 1; i < size; i++)
    {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    return {min, max};  // Modern syntax
}

int main()
{
    // Using pair
    auto result = divmod(17, 5);
    std::cout << "17 / 5 = " << result.first << " remainder " << result.second << "\n";

    // Using tuple
    auto [name, age, gpa] = getStudentInfo();  // C++17 structured binding
    std::cout << "\nStudent Info:\n";
    std::cout << "  Name: " << name << "\n";
    std::cout << "  Age: " << age << "\n";
    std::cout << "  GPA: " << gpa << "\n";

    // Min/Max
    double numbers[] = {3.5, 1.2, 9.8, 4.6, 7.3};
    auto [minimum, maximum] = getMinMax(numbers, 5);
    std::cout << "\nMin: " << minimum << ", Max: " << maximum << "\n";

    return 0;
}
```

---

## 5. Return by Reference

### Returning References (Use with Caution):

```cpp
#include <iostream>
#include <vector>

// Safe: returning reference to element of container that persists
int& getElement(std::vector<int>& vec, int index)
{
    return vec[index];
}

// DANGEROUS: returning reference to local variable
int& dangerousFunction()
{
    int local = 42;
    return local;  // ❌ ERROR: local is destroyed after function returns
}

// Safe: returning reference to global or static
int globalValue = 100;

int& getGlobal()
{
    return globalValue;
}

int main()
{
    std::vector<int> numbers = {10, 20, 30, 40, 50};

    // Can modify through reference
    std::cout << "Before: " << numbers[2] << "\n";
    getElement(numbers, 2) = 999;
    std::cout << "After: " << numbers[2] << "\n\n";

    // Modify global
    std::cout << "Global before: " << globalValue << "\n";
    getGlobal() = 500;
    std::cout << "Global after: " << globalValue << "\n";

    return 0;
}
```

---

## 6. Function Parameter Best Practices

### Complete Example with Best Practices:

```cpp
#include <iostream>
#include <string>
#include <vector>

// 1. Small types: pass by value
int calculateSquare(int number)
{
    return number * number;
}

// 2. Large objects (read-only): pass by const reference
void displayMessage(const std::string& message)
{
    std::cout << message << "\n";
}

// 3. Large objects (modifiable): pass by non-const reference
void appendItem(std::vector<int>& vec, int item)
{
    vec.push_back(item);
}

// 4. Multiple outputs: use reference parameters
bool findMinMax(const std::vector<int>& vec, int& outMin, int& outMax)
{
    if (vec.empty())
        return false;

    outMin = outMax = vec[0];
    for (int val : vec)
    {
        if (val < outMin) outMin = val;
        if (val > outMax) outMax = val;
    }

    return true;
}

// 5. Use meaningful parameter names
double calculateRectangleArea(double width, double height)
{
    return width * height;
}

// 6. Consider returning structs for multiple values
struct Circle
{
    double radius;
    double area;
    double circumference;
};

Circle createCircle(double radius)
{
    const double PI = 3.14159265359;
    Circle c;
    c.radius = radius;
    c.area = PI * radius * radius;
    c.circumference = 2 * PI * radius;
    return c;
}

int main()
{
    // Test all functions
    std::cout << "Square of 5: " << calculateSquare(5) << "\n\n";

    displayMessage("Hello, World!");

    std::vector<int> numbers = {1, 2, 3};
    appendItem(numbers, 4);
    std::cout << "\nVector after append: ";
    for (int n : numbers)
        std::cout << n << " ";
    std::cout << "\n\n";

    int min, max;
    if (findMinMax(numbers, min, max))
    {
        std::cout << "Min: " << min << ", Max: " << max << "\n\n";
    }

    Circle c = createCircle(5.0);
    std::cout << "Circle:\n";
    std::cout << "  Radius: " << c.radius << "\n";
    std::cout << "  Area: " << c.area << "\n";
    std::cout << "  Circumference: " << c.circumference << "\n";

    return 0;
}
```

---

## 7. Default Parameters

### Basic Default Parameters:

```cpp
#include <iostream>
#include <string>

void greet(std::string name = "Guest")
{
    std::cout << "Hello, " << name << "!\n";
}

void printRectangle(int width, int height = 1, char symbol = '*')
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
}

int main()
{
    greet();              // Uses default "Guest"
    greet("Alice");       // Uses "Alice"

    std::cout << "\n";

    printRectangle(5);              // 5x1 rectangle with *
    std::cout << "\n";

    printRectangle(4, 3);           // 4x3 rectangle with *
    std::cout << "\n";

    printRectangle(6, 2, '#');      // 6x2 rectangle with #

    return 0;
}
```

### Rules for Default Parameters:

```cpp
#include <iostream>

// ✅ CORRECT: Default parameters must be rightmost
void function1(int a, int b = 5, int c = 10)
{
    std::cout << "a=" << a << ", b=" << b << ", c=" << c << "\n";
}

// ❌ INCORRECT: Cannot have non-default after default
// void function2(int a = 1, int b, int c = 3)  // ERROR!

// ✅ Declaration with defaults
void function3(int x, int y = 20, int z = 30);

int main()
{
    function1(1);           // a=1, b=5, c=10
    function1(1, 2);        // a=1, b=2, c=10
    function1(1, 2, 3);     // a=1, b=2, c=3

    return 0;
}

// Definition (don't repeat defaults)
void function3(int x, int y, int z)
{
    std::cout << "x=" << x << ", y=" << y << ", z=" << z << "\n";
}
```

### Practical Default Parameters:

```cpp
#include <iostream>
#include <string>
#include <iomanip>

void displayPrice(double price, std::string currency = "$", int precision = 2)
{
    std::cout << currency << std::fixed << std::setprecision(precision) << price << "\n";
}

void createAccount(std::string username,
                  std::string email = "no-email@example.com",
                  int age = 18,
                  bool isActive = true)
{
    std::cout << "===== ACCOUNT CREATED =====\n";
    std::cout << "Username: " << username << "\n";
    std::cout << "Email: " << email << "\n";
    std::cout << "Age: " << age << "\n";
    std::cout << "Active: " << (isActive ? "Yes" : "No") << "\n";
    std::cout << "===========================\n\n";
}

void printHeader(std::string title, int width = 50, char borderChar = '=')
{
    std::string border(width, borderChar);
    std::cout << border << "\n";

    int padding = (width - title.length()) / 2;
    std::cout << std::string(padding, ' ') << title << "\n";

    std::cout << border << "\n";
}

int main()
{
    // Price display
    displayPrice(29.99);                    // $29.99
    displayPrice(49.5, "€");                // €49.50
    displayPrice(99.999, "£", 3);           // £100.000

    std::cout << "\n";

    // Account creation
    createAccount("john_doe");
    createAccount("jane_smith", "jane@example.com");
    createAccount("bob", "bob@example.com", 25);
    createAccount("alice", "alice@example.com", 30, false);

    // Headers
    printHeader("Welcome");
    std::cout << "\n";
    printHeader("Important", 60, '*');
    std::cout << "\n";
    printHeader("Notice", 40, '-');

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Array Processing Functions

```cpp
#include <iostream>

void fillArray(int arr[], int size, int value)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = value;
    }
}

void printArray(const int arr[], int size, std::string label = "Array")
{
    std::cout << label << ": ";
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

void reverseArray(int arr[], int size)
{
    for (int i = 0; i < size / 2; i++)
    {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

int sumArray(const int arr[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return sum;
}

void getArrayStats(const int arr[], int size,
                  int& outMin, int& outMax, double& outAvg)
{
    if (size == 0)
    {
        outMin = outMax = 0;
        outAvg = 0.0;
        return;
    }

    outMin = outMax = arr[0];
    int sum = arr[0];

    for (int i = 1; i < size; i++)
    {
        sum += arr[i];
        if (arr[i] < outMin) outMin = arr[i];
        if (arr[i] > outMax) outMax = arr[i];
    }

    outAvg = static_cast<double>(sum) / size;
}

int main()
{
    const int SIZE = 10;
    int numbers[SIZE] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 10};

    printArray(numbers, SIZE, "Original");

    std::cout << "Sum: " << sumArray(numbers, SIZE) << "\n";

    int min, max;
    double avg;
    getArrayStats(numbers, SIZE, min, max, avg);
    std::cout << "Min: " << min << ", Max: " << max << ", Avg: " << avg << "\n\n";

    reverseArray(numbers, SIZE);
    printArray(numbers, SIZE, "Reversed");

    fillArray(numbers, SIZE, 99);
    printArray(numbers, SIZE, "Filled");

    return 0;
}
```

### Exercise 2: String Manipulation Functions

```cpp
#include <iostream>
#include <string>
#include <cctype>

void toUpperCase(std::string& str)
{
    for (char& c : str)
    {
        c = std::toupper(c);
    }
}

void toLowerCase(std::string& str)
{
    for (char& c : str)
    {
        c = std::tolower(c);
    }
}

std::string getUpperCase(const std::string& str)
{
    std::string result = str;
    toUpperCase(result);
    return result;
}

bool isPalindrome(const std::string& str)
{
    int left = 0;
    int right = str.length() - 1;

    while (left < right)
    {
        if (std::tolower(str[left]) != std::tolower(str[right]))
            return false;
        left++;
        right--;
    }

    return true;
}

int countVowels(const std::string& str)
{
    int count = 0;
    for (char c : str)
    {
        char lower = std::tolower(c);
        if (lower == 'a' || lower == 'e' || lower == 'i' ||
            lower == 'o' || lower == 'u')
        {
            count++;
        }
    }
    return count;
}

void analyzeString(const std::string& str,
                  int& letters, int& digits, int& spaces, int& others)
{
    letters = digits = spaces = others = 0;

    for (char c : str)
    {
        if (std::isalpha(c))
            letters++;
        else if (std::isdigit(c))
            digits++;
        else if (std::isspace(c))
            spaces++;
        else
            others++;
    }
}

int main()
{
    std::string text = "Hello World 123!";

    std::cout << "Original: " << text << "\n";
    std::cout << "Uppercase: " << getUpperCase(text) << "\n";

    std::string copy = text;
    toLowerCase(copy);
    std::cout << "Lowercase: " << copy << "\n\n";

    // Palindrome check
    std::string words[] = {"racecar", "hello", "madam", "world"};
    std::cout << "Palindrome Check:\n";
    for (const std::string& word : words)
    {
        std::cout << "  " << word << ": "
                  << (isPalindrome(word) ? "Yes" : "No") << "\n";
    }

    std::cout << "\nVowels in '" << text << "': " << countVowels(text) << "\n\n";

    // String analysis
    int letters, digits, spaces, others;
    analyzeString(text, letters, digits, spaces, others);

    std::cout << "String Analysis:\n";
    std::cout << "  Letters: " << letters << "\n";
    std::cout << "  Digits: " << digits << "\n";
    std::cout << "  Spaces: " << spaces << "\n";
    std::cout << "  Others: " << others << "\n";

    return 0;
}
```

### Exercise 3: Mathematical Functions

```cpp
#include <iostream>
#include <cmath>

struct Point
{
    double x;
    double y;
};

double distance(const Point& p1, const Point& p2)
{
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

Point midpoint(const Point& p1, const Point& p2)
{
    Point mid;
    mid.x = (p1.x + p2.x) / 2.0;
    mid.y = (p1.y + p2.y) / 2.0;
    return mid;
}

double gcd(double a, double b)
{
    a = std::abs(a);
    b = std::abs(b);

    while (b != 0)
    {
        double temp = b;
        b = std::fmod(a, b);
        a = temp;
    }

    return a;
}

double lcm(double a, double b)
{
    return std::abs(a * b) / gcd(a, b);
}

bool isPowerOfTwo(int n)
{
    if (n <= 0)
        return false;
    return (n & (n - 1)) == 0;
}

int fibonacci(int n, bool useLoop = true)
{
    if (!useLoop)
    {
        // Recursive version (we'll cover recursion in next lesson)
        if (n <= 1)
            return n;
        return fibonacci(n - 1, false) + fibonacci(n - 2, false);
    }

    // Iterative version
    if (n <= 1)
        return n;

    int prev = 0, curr = 1;
    for (int i = 2; i <= n; i++)
    {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}

int main()
{
    // Point operations
    Point p1 = {0, 0};
    Point p2 = {3, 4};

    std::cout << "Point 1: (" << p1.x << ", " << p1.y << ")\n";
    std::cout << "Point 2: (" << p2.x << ", " << p2.y << ")\n";
    std::cout << "Distance: " << distance(p1, p2) << "\n";

    Point mid = midpoint(p1, p2);
    std::cout << "Midpoint: (" << mid.x << ", " << mid.y << ")\n\n";

    // GCD and LCM
    std::cout << "GCD(48, 18) = " << gcd(48, 18) << "\n";
    std::cout << "LCM(12, 18) = " << lcm(12, 18) << "\n\n";

    // Power of two
    std::cout << "Power of Two Check:\n";
    for (int i = 1; i <= 10; i++)
    {
        std::cout << "  " << i << ": "
                  << (isPowerOfTwo(i) ? "Yes" : "No") << "\n";
    }

    std::cout << "\nFibonacci Sequence (first 10):\n";
    for (int i = 0; i < 10; i++)
    {
        std::cout << fibonacci(i) << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## Practice Project: Student Grade Management System

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

struct Student
{
    std::string name;
    int id;
    std::vector<double> grades;
};

void displayMenu()
{
    std::cout << "\n===== STUDENT GRADE MANAGER =====\n";
    std::cout << "1. Add Student\n";
    std::cout << "2. Add Grade to Student\n";
    std::cout << "3. Display Student Info\n";
    std::cout << "4. Display All Students\n";
    std::cout << "5. Calculate Statistics\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter choice: ";
}

void addStudent(std::vector<Student>& students)
{
    Student newStudent;

    std::cout << "\nEnter student name: ";
    std::cin.ignore();
    std::getline(std::cin, newStudent.name);

    std::cout << "Enter student ID: ";
    std::cin >> newStudent.id;

    students.push_back(newStudent);
    std::cout << "Student added successfully!\n";
}

int findStudentById(const std::vector<Student>& students, int id)
{
    for (size_t i = 0; i < students.size(); i++)
    {
        if (students[i].id == id)
            return i;
    }
    return -1;
}

void addGrade(std::vector<Student>& students)
{
    int id;
    std::cout << "\nEnter student ID: ";
    std::cin >> id;

    int index = findStudentById(students, id);
    if (index == -1)
    {
        std::cout << "Student not found!\n";
        return;
    }

    double grade;
    std::cout << "Enter grade (0-100): ";
    std::cin >> grade;

    if (grade < 0 || grade > 100)
    {
        std::cout << "Invalid grade!\n";
        return;
    }

    students[index].grades.push_back(grade);
    std::cout << "Grade added successfully!\n";
}

double calculateAverage(const std::vector<double>& grades)
{
    if (grades.empty())
        return 0.0;

    double sum = 0.0;
    for (double grade : grades)
    {
        sum += grade;
    }

    return sum / grades.size();
}

char getLetterGrade(double average)
{
    if (average >= 90) return 'A';
    if (average >= 80) return 'B';
    if (average >= 70) return 'C';
    if (average >= 60) return 'D';
    return 'F';
}

void displayStudent(const Student& student)
{
    std::cout << "\n===== STUDENT INFO =====\n";
    std::cout << "Name: " << student.name << "\n";
    std::cout << "ID: " << student.id << "\n";
    std::cout << "Number of grades: " << student.grades.size() << "\n";

    if (!student.grades.empty())
    {
        std::cout << "Grades: ";
        for (double grade : student.grades)
        {
            std::cout << std::fixed << std::setprecision(1) << grade << " ";
        }
        std::cout << "\n";

        double avg = calculateAverage(student.grades);
        std::cout << "Average: " << avg << "\n";
        std::cout << "Letter Grade: " << getLetterGrade(avg) << "\n";
    }
    else
    {
        std::cout << "No grades yet.\n";
    }

    std::cout << "========================\n";
}

void displayStudentInfo(const std::vector<Student>& students)
{
    int id;
    std::cout << "\nEnter student ID: ";
    std::cin >> id;

    int index = findStudentById(students, id);
    if (index == -1)
    {
        std::cout << "Student not found!\n";
        return;
    }

    displayStudent(students[index]);
}

void displayAllStudents(const std::vector<Student>& students)
{
    if (students.empty())
    {
        std::cout << "\nNo students in system.\n";
        return;
    }

    std::cout << "\n===== ALL STUDENTS =====\n";
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(20) << "Name"
              << std::setw(10) << "Grades"
              << std::setw(10) << "Average"
              << "Letter\n";
    std::cout << std::string(60, '-') << "\n";

    for (const Student& student : students)
    {
        double avg = calculateAverage(student.grades);

        std::cout << std::left << std::setw(5) << student.id
                  << std::setw(20) << student.name
                  << std::setw(10) << student.grades.size()
                  << std::setw(10) << std::fixed << std::setprecision(1) << avg
                  << getLetterGrade(avg) << "\n";
    }
}

void calculateStatistics(const std::vector<Student>& students)
{
    if (students.empty())
    {
        std::cout << "\nNo students in system.\n";
        return;
    }

    double totalAvg = 0.0;
    int totalGrades = 0;

    for (const Student& student : students)
    {
        totalAvg += calculateAverage(student.grades);
        totalGrades += student.grades.size();
    }

    std::cout << "\n===== STATISTICS =====\n";
    std::cout << "Total Students: " << students.size() << "\n";
    std::cout << "Total Grades Recorded: " << totalGrades << "\n";
    std::cout << "Class Average: " << std::fixed << std::setprecision(2)
              << (totalAvg / students.size()) << "\n";
}

int main()
{
    std::vector<Student> students;
    int choice;

    do
    {
        displayMenu();
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                addStudent(students);
                break;
            case 2:
                addGrade(students);
                break;
            case 3:
                displayStudentInfo(students);
                break;
            case 4:
                displayAllStudents(students);
                break;
            case 5:
                calculateStatistics(students);
                break;
            case 0:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Not Using Const for Read-Only Parameters
```cpp
// ❌ BAD: Unnecessary copy, no protection
void display(std::string str) {
    std::cout << str;
}

// ✅ GOOD: No copy, protected from modification
void display(const std::string& str) {
    std::cout << str;
}
```

### Mistake 2: Forgetting Reference for Output Parameters
```cpp
// ❌ BAD: Won't modify original
void swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

// ✅ GOOD: Modifies original
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
```

### Mistake 3: Returning Reference to Local Variable
```cpp
// ❌ DANGEROUS: Local variable destroyed
int& bad() {
    int x = 10;
    return x;  // ERROR!
}

// ✅ GOOD: Return by value
int good() {
    int x = 10;
    return x;
}
```

---

## Key Takeaways

1. Pass small types by value, large types by const reference
2. Use references for output parameters
3. Const reference parameters are efficient and safe
4. Multiple return values can use structs, pairs, or tuples
5. Default parameters must be rightmost in parameter list
6. Never return references to local variables
7. Use meaningful parameter names
8. Document complex parameter requirements

---

## Summary Checklist

Before moving to Lesson 13, ensure you can:
- [ ] Understand pass by value vs pass by reference
- [ ] Use const parameters correctly
- [ ] Return multiple values from functions
- [ ] Use default parameters effectively
- [ ] Choose appropriate parameter passing strategies
- [ ] Avoid common parameter mistakes
- [ ] Write functions following best practices

---

## Next Lesson Preview

In **Lesson 13**, we'll explore:
- Function overloading concepts
- Overloading with different parameter types
- Overloading with different parameter counts
- Name mangling and how overloading works
- Common overloading patterns

**Master those parameters!**
