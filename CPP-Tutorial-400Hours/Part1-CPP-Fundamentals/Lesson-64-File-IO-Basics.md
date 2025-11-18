# Lesson 64: File I/O Basics

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to File I/O
2. Writing to Files
3. Reading from Files
4. File Stream States
5. File Operations
6. Exercises and Projects

---

## 1. Introduction to File I/O

File I/O in C++ uses stream classes from `<fstream>`:
- `ifstream` - Input file stream (reading)
- `ofstream` - Output file stream (writing)
- `fstream` - Both input and output

---

## 2. Writing to Files

### Basic File Writing:

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    // Create and open file for writing
    std::ofstream outFile("output.txt");

    if (!outFile) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    // Write to file
    outFile << "Hello, File I/O!\n";
    outFile << "Line 2\n";
    outFile << "Line 3\n";

    // File automatically closed when outFile goes out of scope
    std::cout << "File written successfully\n";

    return 0;
}
```

### Append Mode:

```cpp
#include <fstream>
#include <iostream>

int main()
{
    // Open in append mode
    std::ofstream outFile("output.txt", std::ios::app);

    if (outFile) {
        outFile << "Appended line\n";
    }

    return 0;
}
```

---

## 3. Reading from Files

### Reading Line by Line:

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream inFile("input.txt");

    if (!inFile) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << "\n";
    }

    return 0;
}
```

### Reading Word by Word:

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream inFile("input.txt");

    std::string word;
    while (inFile >> word) {
        std::cout << word << " ";
    }
    std::cout << "\n";

    return 0;
}
```

---

## 4. File Stream States

### Checking Stream State:

```cpp
#include <fstream>
#include <iostream>

int main()
{
    std::ifstream file("test.txt");

    if (!file.is_open()) {
        std::cout << "File not open\n";
        return 1;
    }

    // Check various states
    std::cout << "Good: " << file.good() << "\n";
    std::cout << "EOF: " << file.eof() << "\n";
    std::cout << "Fail: " << file.fail() << "\n";
    std::cout << "Bad: " << file.bad() << "\n";

    return 0;
}
```

---

## 5. File Operations

### Copy File:

```cpp
#include <fstream>
#include <iostream>

bool copyFile(const std::string& source, const std::string& dest) {
    std::ifstream src(source, std::ios::binary);
    std::ofstream dst(dest, std::ios::binary);

    if (!src || !dst) return false;

    dst << src.rdbuf();
    return true;
}

int main()
{
    if (copyFile("source.txt", "dest.txt")) {
        std::cout << "File copied successfully\n";
    } else {
        std::cout << "Copy failed\n";
    }

    return 0;
}
```

---

## 6. Exercises and Projects

### Project: Text File Processor

```cpp
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

class TextProcessor {
public:
    static void wordCount(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Cannot open " << filename << "\n";
            return;
        }

        std::map<std::string, int> words;
        std::string word;

        while (file >> word) {
            words[word]++;
        }

        std::cout << "Word frequencies:\n";
        for (const auto& [w, count] : words) {
            std::cout << w << ": " << count << "\n";
        }
    }

    static void lineCount(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) return;

        int count = 0;
        std::string line;
        while (std::getline(file, line)) {
            count++;
        }

        std::cout << "Lines: " << count << "\n";
    }
};

int main()
{
    TextProcessor::wordCount("input.txt");
    TextProcessor::lineCount("input.txt");
    return 0;
}
```

---

## Key Takeaways

1. ✅ Use ofstream for writing files
2. ✅ Use ifstream for reading files
3. ✅ Check if file opened successfully
4. ✅ Files close automatically with RAII
5. ✅ getline() for line-by-line reading
6. ✅ >> operator for word-by-word reading
7. ✅ Use binary mode for binary files

---

## Next Lesson Preview

In **Lesson 65**, we'll learn about:
- Binary file I/O
- Reading/writing structures
- File positioning
- Random access

**Great work on file I/O!**
