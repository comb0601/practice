# Lesson 10: Loop Applications and Patterns

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Searching Algorithms
2. Sorting Algorithms
3. Number Theory Applications
4. String Processing with Loops
5. Data Validation Patterns
6. Statistical Calculations
7. Game Development Patterns
8. Exercises and Projects

---

## 1. Searching Algorithms

### Linear Search:

```cpp
#include <iostream>

int linearSearch(int arr[], int size, int target)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == target)
        {
            return i;  // Return index where found
        }
    }
    return -1;  // Not found
}

int main()
{
    int numbers[] = {64, 34, 25, 12, 22, 11, 90};
    int size = 7;
    int target = 22;

    int result = linearSearch(numbers, size, target);

    if (result != -1)
    {
        std::cout << "Found " << target << " at index " << result << "\n";
    }
    else
    {
        std::cout << target << " not found\n";
    }

    return 0;
}
```

### Binary Search (Array must be sorted):

```cpp
#include <iostream>

int binarySearch(int arr[], int size, int target)
{
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        std::cout << "Checking index " << mid << " (value: " << arr[mid] << ")\n";

        if (arr[mid] == target)
        {
            return mid;
        }
        else if (arr[mid] < target)
        {
            left = mid + 1;  // Search right half
        }
        else
        {
            right = mid - 1;  // Search left half
        }
    }

    return -1;  // Not found
}

int main()
{
    int numbers[] = {11, 12, 22, 25, 34, 64, 90};  // Must be sorted!
    int size = 7;
    int target = 25;

    std::cout << "Binary search for " << target << ":\n";
    int result = binarySearch(numbers, size, target);

    if (result != -1)
    {
        std::cout << "\nFound " << target << " at index " << result << "\n";
    }
    else
    {
        std::cout << "\n" << target << " not found\n";
    }

    return 0;
}
```

### Find All Occurrences:

```cpp
#include <iostream>

int main()
{
    int numbers[] = {5, 2, 5, 8, 5, 1, 5, 3};
    int size = 8;
    int target = 5;

    std::cout << "Finding all occurrences of " << target << ":\n";
    bool found = false;

    for (int i = 0; i < size; i++)
    {
        if (numbers[i] == target)
        {
            std::cout << "Found at index " << i << "\n";
            found = true;
        }
    }

    if (!found)
    {
        std::cout << target << " not found\n";
    }

    return 0;
}
```

---

## 2. Sorting Algorithms

### Bubble Sort:

```cpp
#include <iostream>

void bubbleSort(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        bool swapped = false;

        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }

        // Optimization: if no swaps, array is sorted
        if (!swapped)
        {
            std::cout << "Early termination at pass " << (i + 1) << "\n";
            break;
        }
    }
}

void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int main()
{
    int numbers[] = {64, 34, 25, 12, 22, 11, 90};
    int size = 7;

    std::cout << "Original array: ";
    printArray(numbers, size);

    bubbleSort(numbers, size);

    std::cout << "Sorted array: ";
    printArray(numbers, size);

    return 0;
}
```

### Selection Sort:

```cpp
#include <iostream>

void selectionSort(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int minIndex = i;

        // Find minimum in remaining array
        for (int j = i + 1; j < size; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }

        // Swap if needed
        if (minIndex != i)
        {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }

        std::cout << "Pass " << (i + 1) << ": ";
        for (int k = 0; k < size; k++)
            std::cout << arr[k] << " ";
        std::cout << "\n";
    }
}

int main()
{
    int numbers[] = {64, 25, 12, 22, 11};
    int size = 5;

    std::cout << "Selection Sort:\n";
    selectionSort(numbers, size);

    return 0;
}
```

### Insertion Sort:

```cpp
#include <iostream>

void insertionSort(int arr[], int size)
{
    for (int i = 1; i < size; i++)
    {
        int key = arr[i];
        int j = i - 1;

        // Move elements greater than key one position ahead
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

int main()
{
    int numbers[] = {12, 11, 13, 5, 6};
    int size = 5;

    std::cout << "Before sorting: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    insertionSort(numbers, size);

    std::cout << "After sorting: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";

    return 0;
}
```

---

## 3. Number Theory Applications

### Prime Number Generator:

```cpp
#include <iostream>

bool isPrime(int n)
{
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

int main()
{
    int limit = 100;

    std::cout << "Prime numbers up to " << limit << ":\n";

    int count = 0;
    for (int i = 2; i <= limit; i++)
    {
        if (isPrime(i))
        {
            std::cout << i << " ";
            count++;

            if (count % 10 == 0)
                std::cout << "\n";
        }
    }

    std::cout << "\n\nTotal primes: " << count << "\n";

    return 0;
}
```

### Fibonacci Sequence:

```cpp
#include <iostream>

int main()
{
    int n = 15;

    std::cout << "First " << n << " Fibonacci numbers:\n";

    long long a = 0, b = 1;

    for (int i = 0; i < n; i++)
    {
        std::cout << a << " ";

        long long next = a + b;
        a = b;
        b = next;
    }

    std::cout << "\n";

    return 0;
}
```

### GCD (Greatest Common Divisor):

```cpp
#include <iostream>

int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main()
{
    int num1 = 48;
    int num2 = 18;

    std::cout << "GCD of " << num1 << " and " << num2 << " = "
              << gcd(num1, num2) << "\n";

    return 0;
}
```

### Perfect Number Finder:

```cpp
#include <iostream>

bool isPerfect(int n)
{
    int sum = 0;

    for (int i = 1; i <= n / 2; i++)
    {
        if (n % i == 0)
        {
            sum += i;
        }
    }

    return sum == n;
}

int main()
{
    std::cout << "Perfect numbers up to 10000:\n";

    for (int i = 2; i <= 10000; i++)
    {
        if (isPerfect(i))
        {
            std::cout << i << " (";

            // Show divisors
            for (int j = 1; j <= i / 2; j++)
            {
                if (i % j == 0)
                {
                    std::cout << j;
                    if (j < i / 2)
                        std::cout << " + ";
                }
            }

            std::cout << ")\n";
        }
    }

    return 0;
}
```

---

## 4. String Processing with Loops

### Character Counting:

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string text = "Hello, World! How are you?";

    int vowels = 0, consonants = 0, digits = 0, spaces = 0, others = 0;

    for (char c : text)
    {
        c = tolower(c);

        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        {
            vowels++;
        }
        else if (c >= 'a' && c <= 'z')
        {
            consonants++;
        }
        else if (c >= '0' && c <= '9')
        {
            digits++;
        }
        else if (c == ' ')
        {
            spaces++;
        }
        else
        {
            others++;
        }
    }

    std::cout << "Text: \"" << text << "\"\n\n";
    std::cout << "Vowels: " << vowels << "\n";
    std::cout << "Consonants: " << consonants << "\n";
    std::cout << "Digits: " << digits << "\n";
    std::cout << "Spaces: " << spaces << "\n";
    std::cout << "Others: " << others << "\n";

    return 0;
}
```

### Palindrome Checker:

```cpp
#include <iostream>
#include <string>

bool isPalindrome(std::string str)
{
    int left = 0;
    int right = str.length() - 1;

    while (left < right)
    {
        if (tolower(str[left]) != tolower(str[right]))
        {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

int main()
{
    std::string words[] = {"racecar", "hello", "madam", "world", "level"};
    int size = 5;

    std::cout << "Palindrome check:\n";

    for (int i = 0; i < size; i++)
    {
        std::cout << words[i] << ": "
                  << (isPalindrome(words[i]) ? "Yes" : "No") << "\n";
    }

    return 0;
}
```

### Word Counter:

```cpp
#include <iostream>
#include <string>

int countWords(std::string str)
{
    int count = 0;
    bool inWord = false;

    for (char c : str)
    {
        if (c == ' ' || c == '\t' || c == '\n')
        {
            inWord = false;
        }
        else if (!inWord)
        {
            inWord = true;
            count++;
        }
    }

    return count;
}

int main()
{
    std::string text = "The quick brown fox jumps over the lazy dog";

    std::cout << "Text: \"" << text << "\"\n";
    std::cout << "Word count: " << countWords(text) << "\n";

    return 0;
}
```

---

## 5. Data Validation Patterns

### Range Validation:

```cpp
#include <iostream>

int getValidInput(int min, int max)
{
    int value;

    while (true)
    {
        std::cout << "Enter a number (" << min << "-" << max << "): ";
        std::cin >> value;

        if (value >= min && value <= max)
        {
            return value;
        }

        std::cout << "Invalid! Must be between " << min << " and " << max << "\n";
    }
}

int main()
{
    std::cout << "Age validator:\n";
    int age = getValidInput(1, 120);

    std::cout << "\nScore validator:\n";
    int score = getValidInput(0, 100);

    std::cout << "\nValid age: " << age << "\n";
    std::cout << "Valid score: " << score << "\n";

    return 0;
}
```

### Email Validation (Basic):

```cpp
#include <iostream>
#include <string>

bool isValidEmail(std::string email)
{
    bool hasAt = false;
    bool hasDot = false;
    int atPosition = -1;

    for (int i = 0; i < email.length(); i++)
    {
        if (email[i] == '@')
        {
            if (hasAt)  // Multiple @ symbols
                return false;
            hasAt = true;
            atPosition = i;
        }
        else if (email[i] == '.' && hasAt)
        {
            if (i > atPosition + 1)  // Dot after @
                hasDot = true;
        }
    }

    return hasAt && hasDot && atPosition > 0 && atPosition < email.length() - 2;
}

int main()
{
    std::string emails[] = {
        "user@example.com",
        "invalid.email",
        "@example.com",
        "user@.com",
        "user@example."
    };

    std::cout << "Email validation:\n";

    for (auto email : emails)
    {
        std::cout << email << ": "
                  << (isValidEmail(email) ? "Valid" : "Invalid") << "\n";
    }

    return 0;
}
```

---

## 6. Statistical Calculations

### Mean, Median, Mode:

```cpp
#include <iostream>
#include <algorithm>

double calculateMean(int arr[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return static_cast<double>(sum) / size;
}

double calculateMedian(int arr[], int size)
{
    int temp[size];
    for (int i = 0; i < size; i++)
        temp[i] = arr[i];

    // Sort array
    std::sort(temp, temp + size);

    if (size % 2 == 0)
    {
        return (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
    }
    else
    {
        return temp[size / 2];
    }
}

int calculateMode(int arr[], int size)
{
    int maxCount = 0;
    int mode = arr[0];

    for (int i = 0; i < size; i++)
    {
        int count = 0;

        for (int j = 0; j < size; j++)
        {
            if (arr[j] == arr[i])
                count++;
        }

        if (count > maxCount)
        {
            maxCount = count;
            mode = arr[i];
        }
    }

    return mode;
}

int main()
{
    int numbers[] = {5, 2, 8, 2, 9, 2, 3, 7};
    int size = 8;

    std::cout << "Numbers: ";
    for (int i = 0; i < size; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n\n";

    std::cout << "Mean: " << calculateMean(numbers, size) << "\n";
    std::cout << "Median: " << calculateMedian(numbers, size) << "\n";
    std::cout << "Mode: " << calculateMode(numbers, size) << "\n";

    return 0;
}
```

### Standard Deviation:

```cpp
#include <iostream>
#include <cmath>

double calculateStdDev(int arr[], int size)
{
    // Calculate mean
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    double mean = sum / size;

    // Calculate variance
    double variance = 0;
    for (int i = 0; i < size; i++)
    {
        variance += pow(arr[i] - mean, 2);
    }
    variance /= size;

    // Standard deviation is square root of variance
    return sqrt(variance);
}

int main()
{
    int scores[] = {85, 90, 78, 92, 88, 76, 95, 89};
    int size = 8;

    std::cout << "Scores: ";
    for (int i = 0; i < size; i++)
        std::cout << scores[i] << " ";
    std::cout << "\n\n";

    std::cout << "Standard Deviation: " << calculateStdDev(scores, size) << "\n";

    return 0;
}
```

---

## 7. Game Development Patterns

### Random Number Generation:

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(0));  // Seed with current time

    std::cout << "Rolling dice 10 times:\n";
    for (int i = 0; i < 10; i++)
    {
        int roll = (rand() % 6) + 1;  // 1-6
        std::cout << "Roll " << (i + 1) << ": " << roll << "\n";
    }

    return 0;
}
```

### Simple Game Loop:

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(0));

    int playerHP = 100;
    int enemyHP = 100;
    int round = 1;

    std::cout << "===== BATTLE GAME =====\n\n";

    while (playerHP > 0 && enemyHP > 0)
    {
        std::cout << "=== Round " << round << " ===\n";
        std::cout << "Your HP: " << playerHP << " | Enemy HP: " << enemyHP << "\n";

        // Player attacks
        int playerDamage = (rand() % 20) + 10;  // 10-29 damage
        enemyHP -= playerDamage;
        std::cout << "You deal " << playerDamage << " damage!\n";

        if (enemyHP <= 0)
        {
            std::cout << "\n🎉 You WIN! 🎉\n";
            break;
        }

        // Enemy attacks
        int enemyDamage = (rand() % 15) + 5;  // 5-19 damage
        playerHP -= enemyDamage;
        std::cout << "Enemy deals " << enemyDamage << " damage!\n";

        if (playerHP <= 0)
        {
            std::cout << "\n💀 You LOSE! 💀\n";
            break;
        }

        std::cout << "\n";
        round++;
    }

    std::cout << "Battle lasted " << round << " rounds\n";

    return 0;
}
```

---

## 8. Exercises and Projects

### Exercise 1: Array Statistics

```cpp
#include <iostream>
#include <climits>

int main()
{
    int numbers[] = {23, 45, 12, 67, 34, 89, 15, 56};
    int size = 8;

    int sum = 0;
    int max = INT_MIN;
    int min = INT_MAX;

    for (int i = 0; i < size; i++)
    {
        sum += numbers[i];

        if (numbers[i] > max)
            max = numbers[i];

        if (numbers[i] < min)
            min = numbers[i];
    }

    double average = static_cast<double>(sum) / size;

    std::cout << "Array Statistics:\n";
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Average: " << average << "\n";
    std::cout << "Maximum: " << max << "\n";
    std::cout << "Minimum: " << min << "\n";
    std::cout << "Range: " << (max - min) << "\n";

    return 0;
}
```

### Exercise 2: Frequency Counter

```cpp
#include <iostream>

int main()
{
    int numbers[] = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5};
    int size = 11;

    std::cout << "Frequency analysis:\n";

    for (int i = 1; i <= 5; i++)  // Numbers 1-5
    {
        int count = 0;

        for (int j = 0; j < size; j++)
        {
            if (numbers[j] == i)
                count++;
        }

        std::cout << i << ": ";
        for (int k = 0; k < count; k++)
            std::cout << "*";
        std::cout << " (" << count << ")\n";
    }

    return 0;
}
```

### Exercise 3: Number Pattern Generator

```cpp
#include <iostream>

int main()
{
    int n = 5;

    std::cout << "Pattern 1: Floyd's Triangle\n";
    int num = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            std::cout << num++ << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nPattern 2: Number Pyramid\n";
    for (int i = 1; i <= n; i++)
    {
        // Spaces
        for (int j = 1; j <= n - i; j++)
            std::cout << " ";

        // Ascending
        for (int j = 1; j <= i; j++)
            std::cout << j;

        // Descending
        for (int j = i - 1; j >= 1; j--)
            std::cout << j;

        std::cout << "\n";
    }

    return 0;
}
```

### Exercise 4: Prime Factorization

```cpp
#include <iostream>

int main()
{
    int number = 315;

    std::cout << "Prime factorization of " << number << ":\n";
    std::cout << number << " = ";

    int n = number;
    bool first = true;

    for (int i = 2; i <= n; i++)
    {
        while (n % i == 0)
        {
            if (!first)
                std::cout << " × ";
            std::cout << i;
            n /= i;
            first = false;
        }
    }

    std::cout << "\n";

    return 0;
}
```

### Exercise 5: Collatz Conjecture

```cpp
#include <iostream>

int main()
{
    int n;

    std::cout << "Enter a positive integer: ";
    std::cin >> n;

    std::cout << "\nCollatz sequence:\n";
    int steps = 0;

    while (n != 1)
    {
        std::cout << n << " -> ";

        if (n % 2 == 0)
        {
            n = n / 2;
        }
        else
        {
            n = 3 * n + 1;
        }

        steps++;

        if (steps % 10 == 0)
            std::cout << "\n";
    }

    std::cout << "1\n";
    std::cout << "\nReached 1 in " << steps << " steps\n";

    return 0;
}
```

---

## Practice Project: Student Grade Management System

```cpp
#include <iostream>
#include <string>
#include <iomanip>

const int MAX_STUDENTS = 50;

int main()
{
    std::string names[MAX_STUDENTS];
    int scores[MAX_STUDENTS];
    int studentCount = 0;

    std::cout << "===== STUDENT GRADE MANAGEMENT =====\n\n";

    // Input students
    std::cout << "How many students? ";
    std::cin >> studentCount;
    std::cin.ignore();  // Clear newline

    for (int i = 0; i < studentCount; i++)
    {
        std::cout << "\nStudent " << (i + 1) << ":\n";
        std::cout << "Name: ";
        std::getline(std::cin, names[i]);

        std::cout << "Score (0-100): ";
        std::cin >> scores[i];
        std::cin.ignore();

        // Validate score
        while (scores[i] < 0 || scores[i] > 100)
        {
            std::cout << "Invalid score! Enter 0-100: ";
            std::cin >> scores[i];
            std::cin.ignore();
        }
    }

    // Calculate statistics
    int sum = 0;
    int highest = scores[0];
    int lowest = scores[0];
    std::string topStudent = names[0];
    std::string bottomStudent = names[0];

    for (int i = 0; i < studentCount; i++)
    {
        sum += scores[i];

        if (scores[i] > highest)
        {
            highest = scores[i];
            topStudent = names[i];
        }

        if (scores[i] < lowest)
        {
            lowest = scores[i];
            bottomStudent = names[i];
        }
    }

    double average = static_cast<double>(sum) / studentCount;

    // Display report
    std::cout << "\n========================================\n";
    std::cout << "          GRADE REPORT                  \n";
    std::cout << "========================================\n\n";

    // Student list with grades
    for (int i = 0; i < studentCount; i++)
    {
        std::cout << std::setw(20) << std::left << names[i];
        std::cout << std::setw(10) << std::right << scores[i];

        // Letter grade
        char grade;
        if (scores[i] >= 90) grade = 'A';
        else if (scores[i] >= 80) grade = 'B';
        else if (scores[i] >= 70) grade = 'C';
        else if (scores[i] >= 60) grade = 'D';
        else grade = 'F';

        std::cout << "    " << grade << "\n";
    }

    // Statistics
    std::cout << "\n========================================\n";
    std::cout << "STATISTICS:\n";
    std::cout << "----------------------------------------\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Class Average: " << average << "\n";
    std::cout << "Highest Score: " << highest << " (" << topStudent << ")\n";
    std::cout << "Lowest Score:  " << lowest << " (" << bottomStudent << ")\n";

    // Grade distribution
    int gradeCount[5] = {0, 0, 0, 0, 0};  // A, B, C, D, F

    for (int i = 0; i < studentCount; i++)
    {
        if (scores[i] >= 90) gradeCount[0]++;
        else if (scores[i] >= 80) gradeCount[1]++;
        else if (scores[i] >= 70) gradeCount[2]++;
        else if (scores[i] >= 60) gradeCount[3]++;
        else gradeCount[4]++;
    }

    std::cout << "\nGRADE DISTRIBUTION:\n";
    char grades[] = {'A', 'B', 'C', 'D', 'F'};
    for (int i = 0; i < 5; i++)
    {
        std::cout << grades[i] << ": " << gradeCount[i];
        std::cout << " (" << (gradeCount[i] * 100.0 / studentCount) << "%)\n";
    }

    std::cout << "========================================\n";

    return 0;
}
```

---

## Common Mistakes

### Mistake 1: Array Bounds
```cpp
❌ for (int i = 0; i <= size; i++)  // Goes one past end!
✅ for (int i = 0; i < size; i++)
```

### Mistake 2: Integer Division
```cpp
❌ double avg = sum / count;  // Integer division!
✅ double avg = static_cast<double>(sum) / count;
```

### Mistake 3: Uninitialized Variables
```cpp
❌ int sum;  // Contains garbage!
   for (int i = 0; i < n; i++)
       sum += arr[i];

✅ int sum = 0;
   for (int i = 0; i < n; i++)
       sum += arr[i];
```

---

## Key Takeaways

1. ✅ Linear search: O(n), checks every element
2. ✅ Binary search: O(log n), requires sorted array
3. ✅ Bubble sort: O(n²), simple but slow
4. ✅ Selection sort: O(n²), minimizes swaps
5. ✅ Loops are essential for data processing
6. ✅ Validation loops ensure data quality
7. ✅ Statistical functions use accumulation patterns
8. ✅ Game loops combine randomness and iteration

---

## Summary Checklist

Before moving to Lesson 11, ensure you can:
- [ ] Implement linear and binary search
- [ ] Write basic sorting algorithms
- [ ] Use loops for number theory problems
- [ ] Process strings with loops
- [ ] Validate user input effectively
- [ ] Calculate statistical measures
- [ ] Create game loops and patterns
- [ ] Combine multiple loop techniques

---

## Next Lesson Preview

In **Lesson 11**, we'll learn about:
- Function basics and syntax
- Parameters and arguments
- Return values
- Function prototypes
- Scope and lifetime

**Congratulations on mastering loops!** 🎓
