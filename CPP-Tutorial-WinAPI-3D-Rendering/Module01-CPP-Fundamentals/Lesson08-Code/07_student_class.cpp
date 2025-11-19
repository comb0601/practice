/*
 * Program: Student Class
 * Description: Complete Student class with grades and GPA
 * Compilation: g++ -std=c++17 07_student_class.cpp -o 07_student_class
 * Execution: ./07_student_class
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int id;
    vector<double> grades;

public:
    void setName(const string& n) { name = n; }
    void setId(int i) { id = i; }

    string getName() const { return name; }
    int getId() const { return id; }

    void addGrade(double grade) {
        if (grade >= 0 && grade <= 100) {
            grades.push_back(grade);
        }
    }

    double getAverage() const {
        if (grades.empty()) return 0.0;

        double sum = 0;
        for (double g : grades) {
            sum += g;
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

    void displayInfo() const {
        cout << "Student: " << name << " (ID: " << id << ")" << endl;
        cout << "Grades: ";
        for (double g : grades) {
            cout << g << " ";
        }
        cout << endl;
        cout << "Average: " << getAverage() << endl;
        cout << "Letter Grade: " << getLetterGrade() << endl;
    }
};

int main() {
    cout << "=== Student Class ===" << endl << endl;

    Student student;
    student.setName("Alice Johnson");
    student.setId(12345);

    student.addGrade(95);
    student.addGrade(87);
    student.addGrade(92);
    student.addGrade(88);

    student.displayInfo();

    return 0;
}
