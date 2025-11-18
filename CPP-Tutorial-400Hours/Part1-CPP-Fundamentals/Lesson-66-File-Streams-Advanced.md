# Lesson 66: File Streams Advanced

**Duration**: 5 hours
**Difficulty**: Intermediate

Advanced file stream operations including formatting, random access, and file management.

## File Open Modes:

```cpp
#include <fstream>
#include <iostream>

int main()
{
    // Open modes
    std::ofstream file1("test.txt", std::ios::out);           // Write (default)
    std::ofstream file2("test.txt", std::ios::app);           // Append
    std::ofstream file3("test.txt", std::ios::out | std::ios::trunc);  // Truncate

    std::ifstream file4("test.txt", std::ios::in);            // Read (default)

    std::fstream file5("test.txt", std::ios::in | std::ios::out);  // Read/Write
    std::fstream file6("test.txt", std::ios::binary);         // Binary mode

    return 0;
}
```

## Formatted Output:

```cpp
#include <fstream>
#include <iomanip>

int main()
{
    std::ofstream file("formatted.txt");

    file << std::fixed << std::setprecision(2);
    file << std::setw(10) << "Name" << std::setw(10) << "Score\n";
    file << std::setw(10) << "Alice" << std::setw(10) << 95.5 << "\n";
    file << std::setw(10) << "Bob" << std::setw(10) << 87.3 << "\n";

    return 0;
}
```

## Random Access:

```cpp
#include <fstream>
#include <iostream>

int main()
{
    std::fstream file("data.txt", std::ios::in | std::ios::out);

    // Write at specific position
    file.seekp(10);  // Position for writing
    file << "HELLO";

    // Read from specific position
    file.seekg(10);  // Position for reading
    char buffer[6];
    file.read(buffer, 5);
    buffer[5] = '\0';
    std::cout << buffer << "\n";

    return 0;
}
```

**Key Takeaways:**
- Multiple open modes can be combined
- Formatting with iomanip
- seekp() for write position, seekg() for read position
- Random access for efficient file operations
