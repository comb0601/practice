# Lesson 65: Binary File I/O

**Duration**: 5 hours
**Difficulty**: Intermediate

Binary files store data in raw binary format, more efficient than text for structured data.

## Reading/Writing Binary Data:

```cpp
#include <fstream>
#include <iostream>

struct Record {
    int id;
    char name[50];
    double salary;
};

int main()
{
    // Write binary
    Record r1 = {1, "John", 50000.0};
    std::ofstream out("data.bin", std::ios::binary);
    out.write(reinterpret_cast<char*>(&r1), sizeof(Record));
    out.close();

    // Read binary
    Record r2;
    std::ifstream in("data.bin", std::ios::binary);
    in.read(reinterpret_cast<char*>(&r2), sizeof(Record));
    in.close();

    std::cout << "ID: " << r2.id << ", Name: " << r2.name
              << ", Salary: " << r2.salary << "\n";

    return 0;
}
```

## File Positioning:

```cpp
#include <fstream>
#include <iostream>

int main()
{
    std::fstream file("data.bin", std::ios::in | std::ios::out | std::ios::binary);

    // Seek to position
    file.seekg(0, std::ios::end);  // Go to end
    std::streamsize size = file.tellg();  // Get position
    file.seekg(0, std::ios::beg);  // Go to beginning

    std::cout << "File size: " << size << " bytes\n";

    return 0;
}
```

**Key Takeaways:**
- Binary mode with std::ios::binary
- read() and write() for binary I/O
- seekg() and tellg() for positioning
- More efficient than text for structured data
