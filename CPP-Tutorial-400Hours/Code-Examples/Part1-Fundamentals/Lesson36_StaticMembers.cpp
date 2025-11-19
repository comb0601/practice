/*
 * Lesson 36: Static Members - Class-Level Data and Functions
 * Compile: cl Lesson36_StaticMembers.cpp
 */

#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int id;
    static int studentCount;
    static string schoolName;

public:
    Student(string n) : name(n), id(++studentCount) {
        cout << "Student created: " << name << " (ID: " << id << ")" << endl;
    }

    static int getStudentCount() {
        return studentCount;
    }

    static void setSchoolName(string school) {
        schoolName = school;
    }

    static string getSchoolName() {
        return schoolName;
    }

    void display() const {
        cout << "Student: " << name << " (ID: " << id << ")" << endl;
        cout << "School: " << schoolName << endl;
    }
};

int Student::studentCount = 0;
string Student::schoolName = "Unknown School";

class MathUtil {
public:
    static constexpr double PI = 3.14159;

    static double circleArea(double radius) {
        return PI * radius * radius;
    }

    static double rectangleArea(double length, double width) {
        return length * width;
    }

    static int factorial(int n) {
        if (n <= 1) return 1;
        return n * factorial(n - 1);
    }
};

int main() {
    cout << "=== Static Members Demo ===" << endl << endl;

    // STATIC MEMBER VARIABLES
    cout << "Initial student count: " << Student::getStudentCount() << endl;

    Student::setSchoolName("MIT");
    cout << "School: " << Student::getSchoolName() << endl << endl;

    Student s1("Alice");
    Student s2("Bob");
    Student s3("Charlie");

    cout << "\nTotal students: " << Student::getStudentCount() << endl << endl;

    s1.display();
    cout << endl;

    // STATIC UTILITY FUNCTIONS
    cout << "=== STATIC UTILITY FUNCTIONS ===" << endl;
    cout << "PI: " << MathUtil::PI << endl;
    cout << "Circle area (r=5): " << MathUtil::circleArea(5) << endl;
    cout << "Rectangle area (4x6): " << MathUtil::rectangleArea(4, 6) << endl;
    cout << "Factorial(5): " << MathUtil::factorial(5) << endl;

    return 0;
}
