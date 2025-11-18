# Lesson 13: File I/O Operations

**Duration**: 3 hours
**Prerequisite**: Lesson 12 - Enumerations and Type Definitions

---

## Table of Contents
1. [Introduction to File I/O](#introduction)
2. [Writing to Files](#writing-files)
3. [Reading from Files](#reading-files)
4. [File Opening Modes](#file-modes)
5. [Error Handling](#error-handling)
6. [Binary Files](#binary-files)
7. [File Positioning](#file-positioning)
8. [Practical Exercises](#exercises)
9. [Complete Projects](#projects)

---

## 1. Introduction to File I/O {#introduction}

### What is File I/O?

File Input/Output (I/O) allows programs to read data from files and write data to files, making data persistent beyond program execution.

**Why Use File I/O?**
- Save program state
- Store user data
- Read configuration files
- Log program activity
- Data exchange between programs

### File Stream Classes

```cpp
#include <fstream>

// Three main classes:
std::ofstream  // Output file stream (writing)
std::ifstream  // Input file stream (reading)
std::fstream   // File stream (reading and writing)
```

### Basic Concept

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Writing to a file
    std::ofstream outFile("example.txt");
    outFile << "Hello, File I/O!" << std::endl;
    outFile << "This is a test." << std::endl;
    outFile.close();

    // Reading from a file
    std::ifstream inFile("example.txt");
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
    inFile.close();

    return 0;
}
```

---

## 2. Writing to Files {#writing-files}

### Basic File Writing

```cpp
#include <iostream>
#include <fstream>

int main() {
    // Create output file stream
    std::ofstream outFile("output.txt");

    // Check if file opened successfully
    if (!outFile) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // Write to file
    outFile << "Line 1: Hello, World!" << std::endl;
    outFile << "Line 2: Writing to files is easy!" << std::endl;
    outFile << "Line 3: Number: " << 42 << std::endl;
    outFile << "Line 4: Float: " << 3.14159 << std::endl;

    // Close file
    outFile.close();

    std::cout << "File written successfully!" << std::endl;

    return 0;
}
```

### Writing Different Data Types

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

int main() {
    std::ofstream file("data.txt");

    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    // Write integers
    int age = 25;
    file << "Age: " << age << std::endl;

    // Write floating-point with precision
    double pi = 3.14159265359;
    file << std::fixed << std::setprecision(2);
    file << "Pi (2 decimals): " << pi << std::endl;

    file << std::setprecision(6);
    file << "Pi (6 decimals): " << pi << std::endl;

    // Write strings
    std::string name = "Alice Johnson";
    file << "Name: " << name << std::endl;

    // Write boolean
    bool active = true;
    file << "Active: " << std::boolalpha << active << std::endl;

    file.close();
    std::cout << "Data written successfully!" << std::endl;

    return 0;
}
```

### Appending to Files

```cpp
#include <iostream>
#include <fstream>

int main() {
    // First, write initial content
    std::ofstream file("log.txt");
    file << "=== Log File ===" << std::endl;
    file << "Program started" << std::endl;
    file.close();

    // Append more content (don't overwrite)
    file.open("log.txt", std::ios::app);
    file << "Processing data..." << std::endl;
    file << "Task completed" << std::endl;
    file << "Program ended" << std::endl;
    file.close();

    std::cout << "Log file updated!" << std::endl;

    return 0;
}
```

---

## 3. Reading from Files {#reading-files}

### Reading Line by Line

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("input.txt");

    if (!file.is_open()) {
        std::cerr << "Could not open file!" << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 1;

    while (std::getline(file, line)) {
        std::cout << lineNumber << ": " << line << std::endl;
        lineNumber++;
    }

    file.close();

    return 0;
}
```

### Reading Word by Word

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("words.txt");

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string word;
    int wordCount = 0;

    while (file >> word) {
        std::cout << "Word " << (++wordCount) << ": " << word << std::endl;
    }

    std::cout << "\nTotal words: " << wordCount << std::endl;

    file.close();

    return 0;
}
```

### Reading Structured Data

```cpp
#include <iostream>
#include <fstream>
#include <string>

struct Student {
    int id;
    std::string name;
    double gpa;
};

int main() {
    // First, write structured data
    std::ofstream outFile("students.txt");
    outFile << "101 Alice 3.85" << std::endl;
    outFile << "102 Bob 3.62" << std::endl;
    outFile << "103 Carol 3.91" << std::endl;
    outFile.close();

    // Read structured data
    std::ifstream inFile("students.txt");

    if (!inFile.is_open()) {
        std::cerr << "Could not open file!" << std::endl;
        return 1;
    }

    Student student;
    std::cout << "Student Records:" << std::endl;

    while (inFile >> student.id >> student.name >> student.gpa) {
        std::cout << "ID: " << student.id << ", Name: " << student.name
                  << ", GPA: " << student.gpa << std::endl;
    }

    inFile.close();

    return 0;
}
```

### Reading Entire File

```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    std::ifstream file("document.txt");

    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    // Read entire file into string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    file.close();

    std::cout << "File Contents:" << std::endl;
    std::cout << content << std::endl;

    std::cout << "\nFile size: " << content.length() << " bytes" << std::endl;

    return 0;
}
```

---

## 4. File Opening Modes {#file-modes}

### Available Modes

```cpp
#include <iostream>
#include <fstream>

int main() {
    // Mode flags:
    // std::ios::in     - Open for reading
    // std::ios::out    - Open for writing
    // std::ios::app    - Append to end of file
    // std::ios::ate    - Open and seek to end
    // std::ios::trunc  - Truncate file to zero length
    // std::ios::binary - Binary mode

    // Examples:

    // Write mode (default for ofstream)
    std::ofstream file1("file1.txt", std::ios::out);

    // Append mode
    std::ofstream file2("file2.txt", std::ios::app);

    // Read and write
    std::fstream file3("file3.txt", std::ios::in | std::ios::out);

    // Write binary
    std::ofstream file4("file4.bin", std::ios::out | std::ios::binary);

    // Truncate and write
    std::ofstream file5("file5.txt", std::ios::out | std::ios::trunc);

    // Close all
    file1.close();
    file2.close();
    file3.close();
    file4.close();
    file5.close();

    return 0;
}
```

### Mode Examples

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main() {
    // 1. Overwrite mode (default)
    std::ofstream file1("test.txt");
    file1 << "This will overwrite existing content" << std::endl;
    file1.close();

    // 2. Append mode
    std::ofstream file2("test.txt", std::ios::app);
    file2 << "This will be appended" << std::endl;
    file2.close();

    // 3. Read and write
    std::fstream file3("test.txt", std::ios::in | std::ios::out | std::ios::app);
    file3 << "Another line" << std::endl;

    // Go back to beginning and read
    file3.seekg(0);
    std::string line;
    std::cout << "File contents:" << std::endl;
    while (std::getline(file3, line)) {
        std::cout << line << std::endl;
    }
    file3.close();

    return 0;
}
```

---

## 5. Error Handling {#error-handling}

### Checking File State

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("nonexistent.txt");

    // Method 1: Check with operator!
    if (!file) {
        std::cerr << "Error: Could not open file!" << std::endl;
        return 1;
    }

    // Method 2: Check with is_open()
    if (!file.is_open()) {
        std::cerr << "Error: File is not open!" << std::endl;
        return 1;
    }

    // Method 3: Check state flags
    if (file.fail()) {
        std::cerr << "Error: File operation failed!" << std::endl;
        return 1;
    }

    if (file.bad()) {
        std::cerr << "Error: Critical file error!" << std::endl;
        return 1;
    }

    if (file.eof()) {
        std::cout << "Reached end of file" << std::endl;
    }

    return 0;
}
```

### Robust File Reading

```cpp
#include <iostream>
#include <fstream>
#include <string>

bool readFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        return false;
    }

    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        lineNum++;

        if (file.fail() && !file.eof()) {
            std::cerr << "Error reading line " << lineNum << std::endl;
            file.close();
            return false;
        }

        std::cout << line << std::endl;
    }

    if (file.bad()) {
        std::cerr << "Critical error during file reading!" << std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}

int main() {
    if (readFile("example.txt")) {
        std::cout << "File read successfully!" << std::endl;
    } else {
        std::cout << "Failed to read file!" << std::endl;
    }

    return 0;
}
```

### Exception Handling with Files

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

void processFile(const std::string& filename) {
    std::ifstream file;

    // Enable exceptions
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filename);

        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }

        file.close();

    } catch (const std::ifstream::failure& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        if (file.is_open()) {
            file.close();
        }
        throw;
    }
}

int main() {
    try {
        processFile("test.txt");
    } catch (const std::exception& e) {
        std::cerr << "Error processing file: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

---

## 6. Binary Files {#binary-files}

### Writing Binary Data

```cpp
#include <iostream>
#include <fstream>

struct Data {
    int id;
    double value;
    char flag;
};

int main() {
    std::ofstream file("data.bin", std::ios::binary);

    if (!file) {
        std::cerr << "Cannot create binary file!" << std::endl;
        return 1;
    }

    Data data1 = {101, 3.14159, 'A'};
    Data data2 = {102, 2.71828, 'B'};

    // Write binary data
    file.write(reinterpret_cast<char*>(&data1), sizeof(Data));
    file.write(reinterpret_cast<char*>(&data2), sizeof(Data));

    file.close();

    std::cout << "Binary data written!" << std::endl;

    return 0;
}
```

### Reading Binary Data

```cpp
#include <iostream>
#include <fstream>

struct Data {
    int id;
    double value;
    char flag;
};

int main() {
    std::ifstream file("data.bin", std::ios::binary);

    if (!file) {
        std::cerr << "Cannot open binary file!" << std::endl;
        return 1;
    }

    Data data;

    while (file.read(reinterpret_cast<char*>(&data), sizeof(Data))) {
        std::cout << "ID: " << data.id << ", Value: " << data.value
                  << ", Flag: " << data.flag << std::endl;
    }

    file.close();

    return 0;
}
```

### Binary vs Text Files

```cpp
#include <iostream>
#include <fstream>

int main() {
    int numbers[] = {100, 200, 300, 400, 500};
    int size = 5;

    // Write as text
    std::ofstream textFile("numbers_text.txt");
    for (int i = 0; i < size; i++) {
        textFile << numbers[i] << " ";
    }
    textFile.close();

    // Write as binary
    std::ofstream binFile("numbers_binary.bin", std::ios::binary);
    binFile.write(reinterpret_cast<char*>(numbers), size * sizeof(int));
    binFile.close();

    // Compare file sizes
    std::ifstream tf("numbers_text.txt", std::ios::ate);
    std::ifstream bf("numbers_binary.bin", std::ios::ate);

    std::cout << "Text file size: " << tf.tellg() << " bytes" << std::endl;
    std::cout << "Binary file size: " << bf.tellg() << " bytes" << std::endl;

    tf.close();
    bf.close();

    return 0;
}
```

---

## 7. File Positioning {#file-positioning}

### Seek and Tell

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Create a file
    std::ofstream outFile("positions.txt");
    outFile << "0123456789" << std::endl;
    outFile << "ABCDEFGHIJ" << std::endl;
    outFile.close();

    // Read with positioning
    std::ifstream inFile("positions.txt");

    // Get current position
    std::streampos pos = inFile.tellg();
    std::cout << "Starting position: " << pos << std::endl;

    // Read first line
    std::string line;
    std::getline(inFile, line);
    std::cout << "First line: " << line << std::endl;

    // Get position after first line
    pos = inFile.tellg();
    std::cout << "Position after first line: " << pos << std::endl;

    // Go back to beginning
    inFile.seekg(0, std::ios::beg);
    std::getline(inFile, line);
    std::cout << "Back to start: " << line << std::endl;

    // Jump to position 5
    inFile.seekg(5, std::ios::beg);
    char ch;
    inFile.get(ch);
    std::cout << "Character at position 5: " << ch << std::endl;

    // Go to end
    inFile.seekg(0, std::ios::end);
    std::cout << "File size: " << inFile.tellg() << " bytes" << std::endl;

    inFile.close();

    return 0;
}
```

### Random Access

```cpp
#include <iostream>
#include <fstream>

struct Record {
    int id;
    char name[20];
    double value;
};

int main() {
    // Write records
    std::ofstream outFile("records.bin", std::ios::binary);

    Record records[] = {
        {1, "Alice", 100.5},
        {2, "Bob", 200.7},
        {3, "Carol", 300.3}
    };

    for (const auto& rec : records) {
        outFile.write(reinterpret_cast<const char*>(&rec), sizeof(Record));
    }
    outFile.close();

    // Random access read
    std::fstream file("records.bin", std::ios::in | std::ios::binary);

    // Read second record directly
    file.seekg(1 * sizeof(Record));
    Record rec;
    file.read(reinterpret_cast<char*>(&rec), sizeof(Record));

    std::cout << "Record 2: ID=" << rec.id << ", Name=" << rec.name
              << ", Value=" << rec.value << std::endl;

    // Read first record
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&rec), sizeof(Record));

    std::cout << "Record 1: ID=" << rec.id << ", Name=" << rec.name
              << ", Value=" << rec.value << std::endl;

    file.close();

    return 0;
}
```

---

## 8. Practical Exercises {#exercises}

### Exercise 1: File Copy Program

```cpp
#include <iostream>
#include <fstream>
#include <string>

bool copyFile(const std::string& source, const std::string& destination) {
    std::ifstream src(source, std::ios::binary);
    if (!src) {
        std::cerr << "Error: Cannot open source file!" << std::endl;
        return false;
    }

    std::ofstream dst(destination, std::ios::binary);
    if (!dst) {
        std::cerr << "Error: Cannot create destination file!" << std::endl;
        src.close();
        return false;
    }

    // Copy data
    dst << src.rdbuf();

    src.close();
    dst.close();

    return true;
}

int main() {
    std::string source, destination;

    std::cout << "Enter source file: ";
    std::getline(std::cin, source);

    std::cout << "Enter destination file: ";
    std::getline(std::cin, destination);

    if (copyFile(source, destination)) {
        std::cout << "File copied successfully!" << std::endl;
    } else {
        std::cout << "File copy failed!" << std::endl;
    }

    return 0;
}
```

### Exercise 2: Word Counter

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>

std::string toLower(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

int main() {
    std::string filename;
    std::cout << "Enter filename: ";
    std::getline(std::cin, filename);

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file!" << std::endl;
        return 1;
    }

    std::map<std::string, int> wordCount;
    std::string word;

    while (file >> word) {
        // Remove punctuation
        std::string cleanWord;
        for (char c : word) {
            if (std::isalnum(c)) {
                cleanWord += c;
            }
        }

        if (!cleanWord.empty()) {
            wordCount[toLower(cleanWord)]++;
        }
    }

    file.close();

    // Display results
    std::cout << "\nWord frequencies:" << std::endl;
    for (const auto& pair : wordCount) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "\nTotal unique words: " << wordCount.size() << std::endl;

    return 0;
}
```

---

## 9. Complete Projects {#projects}

### Project 1: Contact Book Manager

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Contact {
    std::string name;
    std::string phone;
    std::string email;
};

class ContactBook {
private:
    std::vector<Contact> contacts;
    std::string filename;

public:
    ContactBook(const std::string& file) : filename(file) {
        loadFromFile();
    }

    ~ContactBook() {
        saveToFile();
    }

    void addContact(const Contact& contact) {
        contacts.push_back(contact);
        std::cout << "Contact added!" << std::endl;
    }

    void displayAll() const {
        if (contacts.empty()) {
            std::cout << "No contacts found." << std::endl;
            return;
        }

        std::cout << "\n=== Contact Book ===" << std::endl;
        for (size_t i = 0; i < contacts.size(); i++) {
            std::cout << (i + 1) << ". " << contacts[i].name << std::endl;
            std::cout << "   Phone: " << contacts[i].phone << std::endl;
            std::cout << "   Email: " << contacts[i].email << std::endl;
            std::cout << std::endl;
        }
    }

    void searchByName(const std::string& name) const {
        bool found = false;

        for (const auto& contact : contacts) {
            if (contact.name.find(name) != std::string::npos) {
                std::cout << "Name: " << contact.name << std::endl;
                std::cout << "Phone: " << contact.phone << std::endl;
                std::cout << "Email: " << contact.email << std::endl;
                std::cout << std::endl;
                found = true;
            }
        }

        if (!found) {
            std::cout << "No contacts found matching '" << name << "'" << std::endl;
        }
    }

    void deleteContact(const std::string& name) {
        for (auto it = contacts.begin(); it != contacts.end(); ++it) {
            if (it->name == name) {
                contacts.erase(it);
                std::cout << "Contact deleted!" << std::endl;
                return;
            }
        }
        std::cout << "Contact not found!" << std::endl;
    }

private:
    void loadFromFile() {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "No existing contact file. Starting fresh." << std::endl;
            return;
        }

        Contact contact;
        while (std::getline(file, contact.name)) {
            std::getline(file, contact.phone);
            std::getline(file, contact.email);
            contacts.push_back(contact);
        }

        file.close();
        std::cout << "Loaded " << contacts.size() << " contacts." << std::endl;
    }

    void saveToFile() const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error: Cannot save contacts!" << std::endl;
            return;
        }

        for (const auto& contact : contacts) {
            file << contact.name << std::endl;
            file << contact.phone << std::endl;
            file << contact.email << std::endl;
        }

        file.close();
    }
};

void displayMenu() {
    std::cout << "\n=== Contact Book Menu ===" << std::endl;
    std::cout << "1. Add Contact" << std::endl;
    std::cout << "2. Display All Contacts" << std::endl;
    std::cout << "3. Search Contact" << std::endl;
    std::cout << "4. Delete Contact" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << "Choice: ";
}

int main() {
    ContactBook book("contacts.txt");
    int choice;
    std::string name, phone, email;

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                std::cout << "Enter name: ";
                std::getline(std::cin, name);
                std::cout << "Enter phone: ";
                std::getline(std::cin, phone);
                std::cout << "Enter email: ";
                std::getline(std::cin, email);
                book.addContact({name, phone, email});
                break;

            case 2:
                book.displayAll();
                break;

            case 3:
                std::cout << "Enter name to search: ";
                std::getline(std::cin, name);
                book.searchByName(name);
                break;

            case 4:
                std::cout << "Enter exact name to delete: ";
                std::getline(std::cin, name);
                book.deleteContact(name);
                break;

            case 5:
                std::cout << "Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice!" << std::endl;
        }

    } while (choice != 5);

    return 0;
}
```

### Project 2: Grade Manager System

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

struct Student {
    int id;
    std::string name;
    std::vector<double> grades;

    double getAverage() const {
        if (grades.empty()) return 0.0;

        double sum = 0.0;
        for (double grade : grades) {
            sum += grade;
        }
        return sum / grades.size();
    }

    char getLetterGrade() const {
        double avg = getAverage();
        if (avg >= 90) return 'A';
        if (avg >= 80) return 'B';
        if (avg >= 70) return 'C';
        if (avg >= 60) return 'D';
        return 'F';
    }
};

class GradeManager {
private:
    std::vector<Student> students;
    std::string filename;

public:
    GradeManager(const std::string& file) : filename(file) {
        loadData();
    }

    ~GradeManager() {
        saveData();
    }

    void addStudent(int id, const std::string& name) {
        students.push_back({id, name, {}});
        std::cout << "Student added!" << std::endl;
    }

    void addGrade(int id, double grade) {
        for (auto& student : students) {
            if (student.id == id) {
                student.grades.push_back(grade);
                std::cout << "Grade added!" << std::endl;
                return;
            }
        }
        std::cout << "Student not found!" << std::endl;
    }

    void displayAll() const {
        if (students.empty()) {
            std::cout << "No students found." << std::endl;
            return;
        }

        std::cout << "\n=== Grade Report ===" << std::endl;
        std::cout << std::setw(5) << "ID" << " | ";
        std::cout << std::setw(20) << std::left << "Name" << " | ";
        std::cout << "Grades" << std::setw(20) << "" << " | ";
        std::cout << "Average" << " | Grade" << std::endl;
        std::cout << std::string(70, '-') << std::endl;

        for (const auto& student : students) {
            std::cout << std::setw(5) << student.id << " | ";
            std::cout << std::setw(20) << std::left << student.name << " | ";

            for (double grade : student.grades) {
                std::cout << std::fixed << std::setprecision(1) << grade << " ";
            }
            std::cout << std::setw(20 - student.grades.size() * 5) << "" << " | ";

            std::cout << std::fixed << std::setprecision(2)
                      << std::setw(7) << student.getAverage() << " | ";
            std::cout << student.getLetterGrade() << std::endl;
        }
    }

    void displayStudent(int id) const {
        for (const auto& student : students) {
            if (student.id == id) {
                std::cout << "\nStudent: " << student.name << std::endl;
                std::cout << "ID: " << student.id << std::endl;
                std::cout << "Grades: ";
                for (double grade : student.grades) {
                    std::cout << grade << " ";
                }
                std::cout << "\nAverage: " << std::fixed << std::setprecision(2)
                          << student.getAverage() << std::endl;
                std::cout << "Letter Grade: " << student.getLetterGrade() << std::endl;
                return;
            }
        }
        std::cout << "Student not found!" << std::endl;
    }

private:
    void loadData() {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "No existing data file. Starting fresh." << std::endl;
            return;
        }

        int id, numGrades;
        std::string name;

        while (file >> id) {
            file.ignore();
            std::getline(file, name);
            file >> numGrades;

            Student student = {id, name, {}};
            for (int i = 0; i < numGrades; i++) {
                double grade;
                file >> grade;
                student.grades.push_back(grade);
            }

            students.push_back(student);
        }

        file.close();
        std::cout << "Loaded " << students.size() << " students." << std::endl;
    }

    void saveData() const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error: Cannot save data!" << std::endl;
            return;
        }

        for (const auto& student : students) {
            file << student.id << std::endl;
            file << student.name << std::endl;
            file << student.grades.size() << std::endl;
            for (double grade : student.grades) {
                file << grade << " ";
            }
            file << std::endl;
        }

        file.close();
    }
};

int main() {
    GradeManager manager("grades.txt");
    int choice, id;
    std::string name;
    double grade;

    do {
        std::cout << "\n=== Grade Manager ===" << std::endl;
        std::cout << "1. Add Student" << std::endl;
        std::cout << "2. Add Grade" << std::endl;
        std::cout << "3. Display All Students" << std::endl;
        std::cout << "4. Display Student" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                std::cout << "Enter student ID: ";
                std::cin >> id;
                std::cin.ignore();
                std::cout << "Enter student name: ";
                std::getline(std::cin, name);
                manager.addStudent(id, name);
                break;

            case 2:
                std::cout << "Enter student ID: ";
                std::cin >> id;
                std::cout << "Enter grade: ";
                std::cin >> grade;
                manager.addGrade(id, grade);
                break;

            case 3:
                manager.displayAll();
                break;

            case 4:
                std::cout << "Enter student ID: ";
                std::cin >> id;
                manager.displayStudent(id);
                break;

            case 5:
                std::cout << "Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice!" << std::endl;
        }

    } while (choice != 5);

    return 0;
}
```

### Project 3: Log File Analyzer

```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

struct LogEntry {
    std::string timestamp;
    std::string level;
    std::string message;
};

class LogAnalyzer {
private:
    std::vector<LogEntry> entries;

public:
    bool loadLog(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Cannot open log file!" << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Parse format: [timestamp] LEVEL: message
            if (line.length() < 10) continue;

            LogEntry entry;

            size_t endTime = line.find(']');
            if (endTime != std::string::npos) {
                entry.timestamp = line.substr(1, endTime - 1);

                size_t levelEnd = line.find(':', endTime);
                if (levelEnd != std::string::npos) {
                    entry.level = line.substr(endTime + 2, levelEnd - endTime - 2);
                    entry.message = line.substr(levelEnd + 2);
                    entries.push_back(entry);
                }
            }
        }

        file.close();
        return true;
    }

    void displayStatistics() const {
        if (entries.empty()) {
            std::cout << "No log entries!" << std::endl;
            return;
        }

        std::map<std::string, int> levelCount;

        for (const auto& entry : entries) {
            levelCount[entry.level]++;
        }

        std::cout << "\n=== Log Statistics ===" << std::endl;
        std::cout << "Total entries: " << entries.size() << std::endl;
        std::cout << "\nBy level:" << std::endl;

        for (const auto& pair : levelCount) {
            std::cout << "  " << pair.first << ": " << pair.second << std::endl;
        }
    }

    void filterByLevel(const std::string& level) const {
        std::cout << "\n=== Entries with level: " << level << " ===" << std::endl;

        int count = 0;
        for (const auto& entry : entries) {
            if (entry.level == level) {
                std::cout << "[" << entry.timestamp << "] "
                          << entry.level << ": " << entry.message << std::endl;
                count++;
            }
        }

        std::cout << "\nFound " << count << " entries." << std::endl;
    }

    void searchKeyword(const std::string& keyword) const {
        std::cout << "\n=== Entries containing '" << keyword << "' ===" << std::endl;

        int count = 0;
        for (const auto& entry : entries) {
            if (entry.message.find(keyword) != std::string::npos) {
                std::cout << "[" << entry.timestamp << "] "
                          << entry.level << ": " << entry.message << std::endl;
                count++;
            }
        }

        std::cout << "\nFound " << count << " entries." << std::endl;
    }

    void exportFiltered(const std::string& level, const std::string& outFile) const {
        std::ofstream file(outFile);
        if (!file) {
            std::cerr << "Cannot create output file!" << std::endl;
            return;
        }

        for (const auto& entry : entries) {
            if (entry.level == level) {
                file << "[" << entry.timestamp << "] "
                     << entry.level << ": " << entry.message << std::endl;
            }
        }

        file.close();
        std::cout << "Filtered log exported to " << outFile << std::endl;
    }
};

int main() {
    // Create sample log file
    std::ofstream sampleLog("app.log");
    sampleLog << "[2024-01-15 10:30:15] INFO: Application started" << std::endl;
    sampleLog << "[2024-01-15 10:30:20] DEBUG: Loading configuration" << std::endl;
    sampleLog << "[2024-01-15 10:30:25] INFO: Configuration loaded successfully" << std::endl;
    sampleLog << "[2024-01-15 10:30:30] WARNING: Cache memory at 80%" << std::endl;
    sampleLog << "[2024-01-15 10:30:35] ERROR: Failed to connect to database" << std::endl;
    sampleLog << "[2024-01-15 10:30:40] INFO: Retrying connection..." << std::endl;
    sampleLog << "[2024-01-15 10:30:45] INFO: Connection established" << std::endl;
    sampleLog << "[2024-01-15 10:30:50] ERROR: Invalid user credentials" << std::endl;
    sampleLog.close();

    LogAnalyzer analyzer;

    if (!analyzer.loadLog("app.log")) {
        return 1;
    }

    int choice;
    std::string level, keyword, outFile;

    do {
        std::cout << "\n=== Log Analyzer ===" << std::endl;
        std::cout << "1. Display Statistics" << std::endl;
        std::cout << "2. Filter by Level" << std::endl;
        std::cout << "3. Search Keyword" << std::endl;
        std::cout << "4. Export Filtered Log" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                analyzer.displayStatistics();
                break;

            case 2:
                std::cout << "Enter level (INFO/DEBUG/WARNING/ERROR): ";
                std::getline(std::cin, level);
                analyzer.filterByLevel(level);
                break;

            case 3:
                std::cout << "Enter keyword: ";
                std::getline(std::cin, keyword);
                analyzer.searchKeyword(keyword);
                break;

            case 4:
                std::cout << "Enter level to export: ";
                std::getline(std::cin, level);
                std::cout << "Enter output filename: ";
                std::getline(std::cin, outFile);
                analyzer.exportFiltered(level, outFile);
                break;

            case 5:
                std::cout << "Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice!" << std::endl;
        }

    } while (choice != 5);

    return 0;
}
```

---

## Summary

In this lesson, you learned:

### Key Concepts

1. **File Streams**
   - `ofstream` for writing
   - `ifstream` for reading
   - `fstream` for both

2. **File Operations**
   - Opening and closing files
   - Writing different data types
   - Reading line by line, word by word
   - Appending to files

3. **File Modes**
   - Input/output modes
   - Append mode
   - Binary mode
   - Truncate mode

4. **Error Handling**
   - Checking file state
   - Using fail(), bad(), eof()
   - Exception handling with files

5. **Binary Files**
   - Writing/reading binary data
   - Advantages of binary files
   - Working with structures

6. **File Positioning**
   - seekg() and seekp()
   - tellg() and tellp()
   - Random access to files

### Best Practices

- Always check if file opened successfully
- Close files when done
- Use binary mode for non-text data
- Handle errors appropriately
- Use RAII (constructor/destructor) for automatic file management
- Prefer text files for human-readable data

---

## Next Lesson

**Lesson 14: Exception Handling**

You'll learn about:
- Try-catch blocks
- Throwing exceptions
- Standard exception types
- Custom exceptions
- Exception safety

---

## Quick Reference

```cpp
// Writing
std::ofstream file("output.txt");
file << "Hello" << std::endl;
file.close();

// Reading
std::ifstream file("input.txt");
std::string line;
while (std::getline(file, line)) {
    // process line
}
file.close();

// Append
std::ofstream file("log.txt", std::ios::app);

// Binary
std::ofstream file("data.bin", std::ios::binary);
file.write(reinterpret_cast<char*>(&data), sizeof(data));

// Check state
if (!file.is_open()) { /* error */ }
if (file.fail()) { /* error */ }
```

**Compilation:**
```bash
g++ -std=c++20 -Wall lesson13.cpp -o lesson13
./lesson13
```

---

**End of Lesson 13**
