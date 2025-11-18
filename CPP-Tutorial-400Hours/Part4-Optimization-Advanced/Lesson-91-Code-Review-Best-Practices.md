# Lesson 91: Code Review Best Practices

**Duration**: 6 hours
**Difficulty**: Advanced
**Prerequisites**: Lessons 1-90, Professional Development Experience

## Table of Contents
1. Code Review Principles
2. Review Checklist
3. Common Issues
4. Automated Tools
5. Best Practices

---

## 1. Code Review Principles

### Why Code Reviews Matter

- **Quality Assurance**: Catch bugs before production
- **Knowledge Sharing**: Team learns from each other
- **Maintainability**: Ensure code is readable
- **Standards**: Enforce coding guidelines
- **Mentorship**: Help junior developers grow

### The Four-Eyes Principle

Every line of production code should be reviewed by at least one other developer.

### Review Types

1. **Pre-commit Reviews**: Before code merges
2. **Post-commit Reviews**: After merge (less common)
3. **Pair Programming**: Real-time review
4. **Tool-assisted**: Automated analysis

---

## 2. Code Review Checklist

### Functionality
```
□ Code does what it's supposed to do
□ Edge cases are handled
□ Error handling is appropriate
□ No obvious bugs
□ Logic is correct
```

### Design & Architecture
```
□ Follows SOLID principles
□ Appropriate design patterns used
□ No code duplication
□ Proper separation of concerns
□ Classes have single responsibility
□ Interfaces are well-defined
```

### Performance
```
□ No obvious performance issues
□ Efficient algorithms used
□ No unnecessary allocations
□ Proper resource management
□ No memory leaks
```

### Security
```
□ No SQL injection vulnerabilities
□ Input validation present
□ No hardcoded credentials
□ Proper authentication/authorization
□ No buffer overflows
```

### Code Quality
```
□ Clear, meaningful names
□ Proper indentation
□ Consistent style
□ Appropriate comments
□ Self-documenting code
```

---

## 3. Common Code Smells

### Memory Management Issues

❌ **Bad**: Memory Leak
```cpp
void processData() {
    int* data = new int[1000];
    // Use data...
    // Forgot to delete[]!
}
```

✅ **Good**: Smart Pointers
```cpp
void processData() {
    auto data = std::make_unique<int[]>(1000);
    // Automatically deleted
}
```

### Exception Safety

❌ **Bad**: Not Exception Safe
```cpp
void transfer(Account& from, Account& to, double amount) {
    from.withdraw(amount);  // What if this throws?
    to.deposit(amount);     // Never executes!
}
```

✅ **Good**: RAII and Transactions
```cpp
void transfer(Account& from, Account& to, double amount) {
    Transaction txn;
    txn.addOperation([&]() { from.withdraw(amount); });
    txn.addOperation([&]() { to.deposit(amount); });
    txn.commit(); // All or nothing
}
```

### Threading Issues

❌ **Bad**: Data Race
```cpp
class Counter {
    int value = 0;
public:
    void increment() {
        value++; // Not thread-safe!
    }
};
```

✅ **Good**: Thread-Safe
```cpp
class Counter {
    std::atomic<int> value{0};
public:
    void increment() {
        value.fetch_add(1, std::memory_order_relaxed);
    }
};
```

---

## 4. Review Comments Best Practices

### Be Constructive, Not Critical

❌ **Bad Comment**:
```
This code is terrible. Why would you write it this way?
```

✅ **Good Comment**:
```
Consider using a hash map here for O(1) lookup instead of O(n)
linear search. This will improve performance for large datasets.

Example:
std::unordered_map<std::string, User> userMap;
```

### Explain the "Why"

❌ **Bad**:
```
Use const here.
```

✅ **Good**:
```
Consider making this parameter const& to avoid unnecessary copies
and to indicate that the function doesn't modify the input.

const User& user
```

### Ask Questions

✅ **Good Approach**:
```
I'm not sure I understand the logic here. Could you explain why
we're checking for nullptr before and after the allocation?

Is this handling a specific edge case?
```

---

## 5. Automated Code Review Tools

### Static Analysis Tools

#### 1. **Clang-Tidy**
```bash
clang-tidy my_file.cpp -- -std=c++17 -I/usr/include
```

Common checks:
- `modernize-*`: Use modern C++ features
- `readability-*`: Code readability
- `performance-*`: Performance issues
- `bugprone-*`: Common bugs

#### 2. **Cppcheck**
```bash
cppcheck --enable=all --inconclusive my_file.cpp
```

Finds:
- Memory leaks
- Null pointer dereferences
- Uninitialized variables
- Dead code

#### 3. **Valgrind** (Memory Analysis)
```bash
valgrind --leak-check=full ./my_program
```

#### 4. **Address Sanitizer**
```bash
g++ -fsanitize=address -g my_file.cpp -o my_program
./my_program
```

### Code Coverage Tools

```bash
g++ -fprofile-arcs -ftest-coverage my_file.cpp -o my_program
./my_program
gcov my_file.cpp
```

---

## 6. Example Review: Before and After

### Before Review

```cpp
// reviewer_before.cpp
#include <iostream>
#include <vector>

class DataProcessor {
public:
    void process(std::vector<int> data) {
        int* temp = new int[data.size()];

        for(int i=0;i<data.size();i++) {
            temp[i]=data[i]*2;
        }

        for(int i=0;i<data.size();i++) {
            std::cout<<temp[i]<<std::endl;
        }

        delete[] temp;
    }
};

int main() {
    DataProcessor dp;
    std::vector<int> v = {1,2,3,4,5};
    dp.process(v);
}
```

### After Review (Improvements)

```cpp
// reviewer_after.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

/**
 * @brief Processes integer data by applying transformations
 *
 * This class provides thread-safe data processing operations
 * using modern C++ practices.
 */
class DataProcessor {
public:
    /**
     * @brief Doubles each element in the input vector and prints results
     * @param data Input vector (passed by const reference for efficiency)
     *
     * Time Complexity: O(n)
     * Space Complexity: O(n)
     */
    void process(const std::vector<int>& data) {
        // Use algorithm instead of raw loop
        std::vector<int> doubled;
        doubled.reserve(data.size()); // Pre-allocate memory

        // Transform in-place using STL algorithm
        std::transform(data.begin(), data.end(),
                      std::back_inserter(doubled),
                      [](int value) { return value * 2; });

        // Print results using range-based for loop
        for (int value : doubled) {
            std::cout << value << '\n'; // '\n' faster than std::endl
        }
    }
};

int main() {
    DataProcessor processor;
    const std::vector<int> values = {1, 2, 3, 4, 5};
    processor.process(values);

    return 0;
}
```

### Review Comments:

1. **Memory Management**:
   - ❌ Raw pointers with new/delete
   - ✅ STL containers (automatic memory management)

2. **Performance**:
   - ❌ Pass by value (copies entire vector)
   - ✅ Pass by const reference

3. **Code Style**:
   - ❌ Inconsistent spacing, no comments
   - ✅ Proper formatting, documentation

4. **Modern C++**:
   - ❌ C-style loops
   - ✅ STL algorithms, range-based for

5. **Safety**:
   - ❌ Potential memory leak if exception thrown
   - ✅ RAII ensures cleanup

---

## 7. Pull Request Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Unit tests added/updated
- [ ] Integration tests pass
- [ ] Manual testing performed

## Checklist
- [ ] Code follows project style guidelines
- [ ] Self-review performed
- [ ] Comments added for complex logic
- [ ] Documentation updated
- [ ] No compiler warnings
- [ ] All tests pass

## Performance Impact
Describe any performance implications

## Screenshots (if applicable)
Add screenshots for UI changes
```

---

## 8. Metrics to Track

### Review Effectiveness

```cpp
struct ReviewMetrics {
    int totalReviews;
    int bugsFoundInReview;
    int bugsFOundInProduction;
    double averageReviewTime; // minutes
    double codeChurnRate; // % of code changed after review

    double defectRemovalEfficiency() {
        return (double)bugsFoundInReview /
               (bugsFoundInReview + bugsFoundInProduction);
    }
};
```

---

## 9. Cultural Best Practices

### The Reviewer

1. **Be Kind**: Assume good intent
2. **Be Thorough**: But don't nitpick
3. **Be Prompt**: Review within 24 hours
4. **Be Educational**: Explain reasoning
5. **Be Consistent**: Apply same standards

### The Author

1. **Keep Changes Small**: 200-400 lines ideal
2. **Provide Context**: Good PR description
3. **Respond Promptly**: Address feedback quickly
4. **Be Open**: Accept constructive criticism
5. **Learn**: Each review is a learning opportunity

---

## 10. Example Review Session

```cpp
// File: user_service.cpp
// PR #123: Add user authentication

/* Reviewer Comment #1:
 * Line 45: Consider using bcrypt for password hashing instead of SHA-256
 *
 * Reasoning: SHA-256 is too fast for password hashing, making brute-force
 * attacks easier. Bcrypt is specifically designed for passwords with
 * built-in salt and adjustable work factor.
 *
 * Example:
 * #include <bcrypt/BCrypt.hpp>
 * std::string hash = BCrypt::generateHash(password);
 *
 * Severity: High (Security)
 */

/* Author Response:
 * Good catch! I'll update to use bcrypt. I wasn't aware of the
 * performance implications for security.
 *
 * Status: Will fix in next commit
 */

/* Reviewer Comment #2:
 * Line 78: This constant should be in a config file
 *
 * Reasoning: MAX_LOGIN_ATTEMPTS shouldn't be hardcoded. Move to
 * configuration so it can be changed without recompilation.
 *
 * Severity: Medium (Maintainability)
 */

/* Author Response:
 * Agreed. I'll move all security constants to security_config.json
 *
 * Status: Fixed in commit abc123
 */
```

---

## Key Takeaways

1. ✅ **Code reviews improve quality**: Catch bugs early
2. ✅ **Automate what you can**: Use static analysis
3. ✅ **Be respectful**: Focus on code, not person
4. ✅ **Keep reviews small**: Easier to review thoroughly
5. ✅ **Document decisions**: Future you will thank you
6. ✅ **Learn continuously**: Every review is education
7. ✅ **Track metrics**: Measure improvement over time

---

## Exercises

### Exercise 1: Review This Code

```cpp
void processOrders(vector<Order*> orders) {
    for(int i=0; i<orders.size(); i++) {
        if(orders[i]->total > 100)
            orders[i]->discount = 10;
        orders[i]->process();
    }
}
```

**What issues can you find?**

### Exercise 2: Write Better Comments

Review this PR comment and improve it:
```
"This is wrong. Fix it."
```

---

## Additional Resources

- **Books**:
  - "Code Complete" by Steve McConnell
  - "Clean Code" by Robert C. Martin
  - "The Pragmatic Programmer" by Hunt & Thomas

- **Tools**:
  - GitHub Code Review
  - GitLab Merge Requests
  - Crucible
  - Gerrit

- **Style Guides**:
  - Google C++ Style Guide
  - LLVM Coding Standards
  - C++ Core Guidelines

---

**Next Lesson**: Testing Strategies - Unit, Integration, and Performance Testing!
