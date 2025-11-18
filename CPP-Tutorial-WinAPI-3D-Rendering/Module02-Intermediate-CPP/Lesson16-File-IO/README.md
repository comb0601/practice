# Lesson 16: File I/O in C++

**Estimated Time**: 4 hours | **Difficulty**: Intermediate

## Table of Contents
1. [Introduction](#intro) 2. [File Streams](#streams) 3. [Reading Files](#read)
4. [Writing Files](#write) 5. [Binary Files](#binary) 6. [File Positioning](#position)
7. [Error Handling](#errors) 8. [Practical Applications](#practical)

## 1. Introduction {#intro}
C++ provides file I/O through stream classes: `ifstream` (input), `ofstream` (output), `fstream` (both).

## 2. File Streams {#streams}
```cpp
#include <fstream>

std::ifstream input("file.txt");
std::ofstream output("file.txt");
std::fstream file("file.txt", std::ios::in | std::ios::out);
```

**Open modes**:
- `ios::in`: Read
- `ios::out`: Write
- `ios::app`: Append
- `ios::binary`: Binary mode
- `ios::trunc`: Truncate

## 3. Reading Files {#read}
```cpp
std::ifstream file("input.txt");
std::string line;
while (std::getline(file, line)) {
    // process line
}
```

**Reading word by word**:
```cpp
std::string word;
while (file >> word) {
    // process word
}
```

## 4. Writing Files {#write}
```cpp
std::ofstream file("output.txt");
file << "Hello, World!" << std::endl;
file << 42 << " " << 3.14 << std::endl;
```

## 5. Binary Files {#binary}
```cpp
std::ofstream file("data.bin", std::ios::binary);
int data[] = {1, 2, 3, 4, 5};
file.write(reinterpret_cast<char*>(data), sizeof(data));
```

**Reading binary**:
```cpp
std::ifstream file("data.bin", std::ios::binary);
int data[5];
file.read(reinterpret_cast<char*>(data), sizeof(data));
```

## 6. File Positioning {#position}
```cpp
file.seekg(0, std::ios::beg);  // Beginning
file.seekg(0, std::ios::end);  // End
file.seekg(10, std::ios::cur); // Current + 10
auto pos = file.tellg();       // Get position
```

## 7. Error Handling {#errors}
```cpp
if (!file) {
    std::cerr << "Error opening file" << std::endl;
}
if (file.fail()) { /* I/O error */ }
if (file.eof()) { /* End of file */ }
if (file.bad()) { /* Fatal error */ }
```

## 8. Practical Applications {#practical}
- CSV parsing
- Configuration files
- Data serialization
- Log files
- Binary data storage

**Best Practices**:
1. Always check if file opened successfully
2. Close files explicitly or use RAII
3. Use binary mode for non-text data
4. Handle exceptions appropriately
5. Consider file locking for concurrent access

**Next**: Lesson 17 - Namespaces & Preprocessor
