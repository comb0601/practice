/* Lesson 65: Binary File I/O
 * Compile: cl Lesson65_BinaryFileIO.cpp */
#include <iostream>
#include <fstream>
using namespace std;

struct Student {
    char name[50];
    int age;
    double gpa;
};

int main() {
    Student s1 = {"Alice", 20, 3.8};

    // Write binary
    ofstream outFile("student.dat", ios::binary);
    outFile.write(reinterpret_cast<char*>(&s1), sizeof(Student));
    outFile.close();

    // Read binary
    Student s2;
    ifstream inFile("student.dat", ios::binary);
    inFile.read(reinterpret_cast<char*>(&s2), sizeof(Student));
    inFile.close();

    cout << "Name: " << s2.name << endl;
    cout << "Age: " << s2.age << endl;
    cout << "GPA: " << s2.gpa << endl;

    return 0;
}
